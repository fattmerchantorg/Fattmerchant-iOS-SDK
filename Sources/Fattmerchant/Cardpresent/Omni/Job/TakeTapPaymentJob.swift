import Foundation
import os.log

/// Step-by-step tracing for the tap flow, for use while testing.
///
/// Every line carries elapsed seconds from the start of the step being measured, because the
/// question these logs exist to answer is *where the time went* — specifically whether a tap that
/// reports a timeout spent its budget on the tap itself or on the post-approval expiration lookup.
///
/// Goes through the unified log rather than `print`, because `print` writes to stdout, which iOS
/// discards for an app it launched itself. That makes `print` invisible on any build you cannot
/// attach a debugger to — TestFlight included, since App Store signing omits `get-task-allow`.
/// `os_log` reaches Console.app on a connected device and is captured in a sysdiagnose.
///
/// Uses `os_log` rather than the nicer `Logger` API because the podspec still targets iOS 13, and
/// `Logger` needs 14. Interpolations are marked `%{public}` — the unified log redacts dynamic
/// strings to `<private>` otherwise, which would make every line useless.
///
/// Deliberately not `@available`-restricted: `ChipDnaDriver` is not gated to iOS 17.4 and needs it
/// too. Lives in this file rather than its own because new files require an Xcode project change.
///
/// - Important: `isEnabled` starts `true` for testing. Set it to `false` before release, or drive
///   it from the host app — this records card metadata (last four, card type) to the device log.
enum TapLog {
    static var isEnabled = true

    static let subsystem = "com.fattmerchant.sdk"

    private static let log = OSLog(subsystem: subsystem, category: "TapPayment")

    /// Logs a step with the time taken since `start`.
    static func step(_ message: String, since start: Date) {
        guard isEnabled else { return }
        emit(String(format: "+%.2fs %@", Date().timeIntervalSince(start), message))
    }

    /// Logs a step with no timing attached.
    static func note(_ message: String) {
        guard isEnabled else { return }
        emit(message)
    }

    private static func emit(_ message: String) {
        // `.default`, not `.info`: info-level messages are filtered out by default in both
        // Console.app and `log show`, so they would appear to be missing entirely unless the
        // reader knew to opt in. `.default` is also persisted to the log store, which is what
        // lets a tester reproduce the bug and hand over a sysdiagnose afterwards.
        os_log("[StaxTap] %{public}@", log: log, type: .default, message)
    }
}

@available(iOS 17.4, *)
final class SingleShot {
    private let lock = NSLock()
    private var didResume = false

    /// Locked because the driver's completion and the timeout race from different threads, and
    /// resuming the same continuation twice traps. Not a timing change — same behaviour, just
    /// safe against the two arriving together.
    func run(_ block: () -> Void) {
        lock.lock()
        if didResume {
            lock.unlock()
            return
        }
        didResume = true
        lock.unlock()
        block()
    }
}

/// Thread-safe holder for the driver's `TransactionResult`.
///
/// `performTransaction`'s completion can land *after* our timeout has already fired — the
/// tap is approved, but the continuation it resumes has been discarded along with its task
/// group. Capturing the result here as well means a late approval is still recoverable, so
/// we can void the sale instead of leaving it to settle with no Stax record behind it.
@available(iOS 17.4, *)
final class TransactionResultBox: @unchecked Sendable {
    private let lock = NSLock()
    private var value: TransactionResult?

    /// Records the first result to arrive. Later calls are ignored.
    func set(_ result: TransactionResult) {
        lock.lock()
        defer { lock.unlock() }
        if value == nil { value = result }
    }

    var current: TransactionResult? {
        lock.lock()
        defer { lock.unlock() }
        return value
    }
}

@available(iOS 17.4, *)
actor TakeTapPaymentJob: Job {
    fileprivate static let DEFAULT_TAP_CUSTOMER_NAME = "Tap Payment Customer"

    typealias ResultType = StaxTransaction

    private let client: StaxHttpClientProtocol
    private var request: TransactionRequest
    private var customer: StaxCustomer?
    private let signatureProvider: SignatureProviding?
    private weak var transactionUpdateDelegate: TransactionUpdateDelegate?
    private weak var userNotificationDelegate: UserNotificationDelegate?

    fileprivate var result: TransactionResult? = nil

    // ⚠️⚠️ TESTING ONLY — REVERT TO 90s BEFORE MERGING ⚠️⚠️
    //
    // At 5s every tap reports a timeout, which is the point: it forces the catch block down the
    // void path on demand instead of waiting for a real one.
    //
    // This works only because the timeout cannot surface until the driver's completion has run
    // (the task group has to drain the continuation child). So the sale still completes and is
    // captured normally, the box gets the result, and *then* TimedOut propagates — giving the void
    // something real to reverse. A genuinely hard 5s deadline would fire before the card was even
    // presented, leave the box empty, and test nothing.
    //
    // Shipping this value would fail and reverse every single tap payment.
    private let performTimeout: UInt64 = 5 * 1_000_000_000 // 5s  ← TESTING, was 90s
    private let captureTimeout: UInt64 = 45 * 1_000_000_000 // 45s
    private let voidTimeout: UInt64 = 30 * 1_000_000_000 // 30s

    /// Attempts for a void that reports failure. NMI accepts a void until the batch closes, so a
    /// transient failure is worth retrying rather than dropping. Reuses `voidTimeout` per attempt
    /// and does not wait between them, so this adds no timing behaviour of its own.
    private let voidAttempts = 3

    init(
        request: TransactionRequest,
        client: StaxHttpClientProtocol,
        customer: StaxCustomer? = nil,
        signatureProvider: SignatureProviding? = nil,
        transactionUpdateDelegate: TransactionUpdateDelegate? = nil,
        userNotificationDelegate: UserNotificationDelegate? = nil
    ) {
        self.request = request
        self.client = client
        self.customer = customer
        self.signatureProvider = signatureProvider
        self.transactionUpdateDelegate = transactionUpdateDelegate
        self.userNotificationDelegate = userNotificationDelegate
    }

    func start() async -> JobResult<StaxTransaction> {
        let jobStart = Date()

        let drivers = TapPaymentDriverRepository.shared.all()
        guard let driver = drivers.first, drivers.count == 1 else {
            TapLog.note("ABORT: expected exactly one tap driver, found \(drivers.count)")
            return JobResult.failure(OmniGeneralException.uninitialized)
        }

        // Written from `performTransaction`'s completion, which may fire after our timeout has
        // already abandoned the continuation. Read back in the catch block to recover the sale.
        let resultBox = TransactionResultBox()

        do {
            let currentRequest = self.request
            let currentSignatureProvider = self.signatureProvider
            let currentTransactionUpdateDelegate = self.transactionUpdateDelegate
            let currentUserNotificationDelegate = self.userNotificationDelegate

            TapLog.note("--- tap start. amount=\(currentRequest.amount.dollars()) invoiceId=\(currentRequest.invoiceId ?? "new") tokenize=\(currentRequest.tokenize) timeout=\(performTimeout / 1_000_000_000)s")

            let invoiceStart = Date()
            var invoice = try await getOrCreateInvoice(id: currentRequest.invoiceId)
            TapLog.step("1/6 invoice ready. id=\(invoice.id ?? "nil")", since: invoiceStart)

            // The clock that matters: `performTimeout` is measured against this, and it covers the
            // driver's whole round trip — presentment, authorisation, and any post-approval
            // enrichment the driver does before it calls back.
            let tapStart = Date()
            TapLog.note("2/6 calling performTransaction — waiting for driver")

            result = try await performWithTimeout(timeout: performTimeout) { resume in
                driver.performTransaction(
                    with: currentRequest,
                    signatureProvider: currentSignatureProvider,
                    transactionUpdateDelegate: currentTransactionUpdateDelegate,
                    userNotificationDelegate: currentUserNotificationDelegate
                ) { txnResult in
                    TapLog.step("2/6 driver completion fired. success=\(String(describing: txnResult.success)) userRef=\(txnResult.userReference ?? "nil") txnId=\(txnResult.externalId ?? "nil") cardType=\(txnResult.cardType ?? "nil") last4=\(txnResult.maskedPan?.suffix(4).description ?? "nil") cardExp=\(txnResult.cardExpiration ?? "nil") authCode=\(txnResult.authCode ?? "nil")", since: tapStart)
                    resultBox.set(txnResult)
                    resume(.success(txnResult))
                }
            }

            guard let result = result else {
                throw TakeTapPaymentException.couldNotCreateInvoice(detail: "No TransactionResult")
            }

            // If transaction was not successful (cancelled, declined, or error),
            // don't try to create backend records - return the transaction result as-is
            guard result.success == true else {
                TapLog.step("2/6 tap NOT approved — no records, no void needed. message=\(result.message ?? "nil")", since: jobStart)
                // Create a StaxTransaction representing the failed/cancelled transaction
                var failedTransaction = StaxTransaction()
                failedTransaction.success = false
                failedTransaction.message = result.message
                if let pan = result.maskedPan, pan.count >= 4 {
                    failedTransaction.lastFour = String(pan.suffix(4))
                }
                failedTransaction.total = request.amount.dollars()
                failedTransaction.source = result.source
                
                return JobResult.success(failedTransaction)
            }
        
            // Everything past here runs with money already captured at the gateway, so any
            // failure below must reach the void in the catch block.
            let customerStart = Date()
            let customer = try await createCustomer(from: result, or: self.customer)
            TapLog.step("3/6 customer created. id=\(customer.id ?? "nil")", since: customerStart)

            let pmStart = Date()
            let paymentMethod = try await createPaymentMethod(
                from: customer,
                and: result
            )
            TapLog.step("4/6 payment method created. id=\(paymentMethod.id ?? "nil") cardExpiry=\(paymentMethod.cardExpiry ?? "nil")", since: pmStart)

            let invoiceUpdateStart = Date()
            invoice = try await updateInvoice(
                invoice,
                with: customer,
                and: paymentMethod
            )
            TapLog.step("5/6 invoice updated. id=\(invoice.id ?? "nil")", since: invoiceUpdateStart)

            let txnStart = Date()
            let transaction = try await createTransaction(
                driver: driver,
                customer: customer,
                invoice: invoice,
                paymentMethod: paymentMethod
            )
            TapLog.step("6/6 transaction created. id=\(transaction.id ?? "nil")", since: txnStart)

            guard transaction.id != nil else {
                throw TakeTapPaymentException.couldNotCreateTransaction(
                    detail: "Missing transaction id"
                )
            }

            TapLog.step("=== SUCCESS. txnId=\(transaction.id ?? "nil")", since: jobStart)
            return JobResult.success(transaction)
        } catch {
            // The sale is a SALE with AUTO_CONFIRM, so by the time the driver reports anything the
            // funds are already captured at the gateway. Whatever went wrong above, the only way
            // to keep this from settling with no Stax record is to void it here.
            //
            // `result` is nil on the timeout path, because the assignment never completed — which
            // is why the old `if let result = result` skipped the void entirely and left captured
            // sales unrecorded. `resultBox` holds what the driver's completion delivered, so read
            // that as well. No waiting is needed: the timeout cannot surface until the driver's
            // completion has run, so if a result exists the box already has it.
            TapLog.step("!!! CAUGHT: \(error)", since: jobStart)
            TapLog.note("    result=\(result == nil ? "nil" : "set") resultBox=\(resultBox.current == nil ? "empty" : "populated")")

            if let toVoid = result ?? resultBox.current {
                // Which source supplied it is the whole point of the fix — on the timeout path
                // `result` is nil and only the box has it.
                TapLog.note("    recovered from \(result != nil ? "result" : "resultBox") — attempting void")
                await voidTransaction(toVoid, using: driver)
            } else {
                TapLog.note("    no TransactionResult available — nothing to void (no sale was placed)")
            }

            // If error already conforms to OmniException, return it; otherwise wrap
            if let omni = error as? OmniException {
                TapLog.step("=== FAILED (OmniException): \(omni)", since: jobStart)
                return JobResult.failure(omni)
            } else {
                // Use a generic tap error with detail
                TapLog.step("=== FAILED (wrapped): \(error)", since: jobStart)
                return JobResult.failure(
                    TakeTapPaymentException.couldNotCreateTransaction(detail: String(describing: error))
                )
            }
        }
    }

    // MARK: - Defensive continuation wrapper with timeout

    private func performWithTimeout<T>(
        timeout: UInt64,
        _ body: @escaping (@Sendable (_ resume: @escaping (Result<T, Error>) -> Void) -> Void)
    ) async throws -> T {
        try await withThrowingTaskGroup(of: T.self) { group in
            let single = SingleShot()

            // Task 1: the callback-based operation
            group.addTask {
                try await withCheckedThrowingContinuation { (continuation: CheckedContinuation<T, Error>) in
                    body { result in
                        single.run {
                            switch result {
                            case .success(let value):
                                continuation.resume(returning: value)
                            case .failure(let err):
                                continuation.resume(throwing: err)
                            }
                        }
                    }
                }
            }

            // Task 2: the timeout
            group.addTask {
                try await Task.sleep(nanoseconds: timeout)
                throw TakeTapPaymentException.couldNotCreateTransaction(detail: "Operation timed out")
            }

            // Return the first finished task and cancel the other
            defer { group.cancelAll() }
            for try await value in group {
                return value
            }

            // Should be unreachable
            throw TakeTapPaymentException.couldNotCreateTransaction(detail: "Unknown operation error")
        }
    }

    // MARK: - Void

    /// Voids a transaction, verifying the outcome and retrying on failure.
    ///
    /// The previous implementation discarded the driver's `Bool` and swallowed the timeout
    /// with `try?`, so a void that never happened was indistinguishable from one that did —
    /// which is how captured sales reached settlement with nothing recorded against them.
    private func voidTransaction(
        _ transactionResult: TransactionResult,
        using driver: TapDriver
    ) async {
        // Nothing was captured, so there is nothing to reverse.
        guard transactionResult.success == true else {
            TapLog.note("VOID skipped: transaction was not approved, nothing captured")
            return
        }

        // `ChipDnaDriver.void` bails out immediately without a user reference, so surface that
        // rather than logging three indistinguishable failed attempts.
        guard let userReference = transactionResult.userReference, !userReference.isEmpty else {
            TapLog.note("VOID IMPOSSIBLE: approved sale has no USER_REFERENCE. txnId=\(transactionResult.externalId ?? "nil") — this will settle unrecorded")
            return
        }

        TapLog.note("VOID starting. userRef=\(userReference) txnId=\(transactionResult.externalId ?? "nil") amount=\(request.amount.dollars()) maxAttempts=\(voidAttempts)")

        for attempt in 1...voidAttempts {
            let attemptStart = Date()
            let voided: Bool? = try? await performWithTimeout(timeout: voidTimeout) { resume in
                driver.void(
                    transactionResult: transactionResult,
                    completion: { success in resume(.success(success)) }
                )
            }

            if voided == true {
                TapLog.step("VOID SUCCEEDED on attempt \(attempt)/\(voidAttempts). userRef=\(userReference)", since: attemptStart)
                return
            }

            let reason = voided == nil ? "timed out" : "was rejected by the gateway"
            TapLog.step("VOID attempt \(attempt)/\(voidAttempts) \(reason). userRef=\(userReference)", since: attemptStart)
        }

        // Every attempt failed. This is the orphan: an approved, captured sale that will
        // settle with no Stax transaction behind it. Log loudly enough to alert on.
        TapLog.note("VOID FAILED after \(voidAttempts) attempts — captured sale will settle unrecorded. userRef=\(userReference) txnId=\(transactionResult.externalId ?? "nil") amount=\(request.amount.dollars())")
    }

    fileprivate func getOrCreateInvoice(id: String?) async throws -> StaxInvoice
    {
        let invoiceRepository = StaxInvoiceRepositoryImpl(httpClient: client)

        // GET /invoice/:id
        if let id = id {
            guard !id.isEmpty else {
                throw TakeTapPaymentException.invoiceIdCannotBeBlank
            }
            guard let invoice = try? await invoiceRepository.getInvoice(id: id)
            else {
                throw TakeTapPaymentException.invoiceNotFound
            }
            return invoice
        }

        // POST /invoice
        var invoice = StaxInvoice()
        invoice.total = request.amount.dollars()
        invoice.url = "https://fattpay.com/#/bill"
        invoice.meta = request.createInvoiceMeta()

        guard let created = try? await invoiceRepository.createInvoice(invoice)
        else {
            throw TakeTapPaymentException.couldNotCreateInvoice(detail: nil)
        }

        return created
    }

    fileprivate func createCustomer(from result: TransactionResult, or customer: StaxCustomer? = nil) async throws
        -> StaxCustomer
    {
        let customerRepository = StaxCustomerRepositoryImpl(httpClient: client)
        
        if let customerInput = customer {
            return try await customerRepository.createCustomer(customerInput)
        }

        var name: String = TakeTapPaymentJob.DEFAULT_TAP_CUSTOMER_NAME
        var firstName: String? = nil
        var lastName: String? = nil

        // For tap to pay, use cardholder name from card details if available
        if let first = result.cardHolderFirstName, !first.isEmpty {
            firstName = first
        }
        
        if let last = result.cardHolderLastName, !last.isEmpty {
            lastName = last
        }
        
        // Build the full name from available components
        if let first = firstName, let last = lastName {
            name = "\(first) \(last)"
        } else if let first = firstName {
            name = first
        } else if let last = lastName {
            name = last
        }

        let request = StaxCustomer.from(name: name)
        return try await customerRepository.createCustomer(request)
    }

    fileprivate func createPaymentMethod(
        from customer: StaxCustomer,
        and result: TransactionResult
    ) async throws -> StaxPaymentMethod {
        guard let customerId = customer.id, !customerId.isEmpty else {
            throw TakeTapPaymentException.couldNotCreatePaymentMethod(
                detail: "Customer ID is required"
            )
        }

        guard let lastFour = result.maskedPan?.suffix(4), lastFour.count == 4
        else {
            throw TakeTapPaymentException.couldNotCreatePaymentMethod(
                detail: "Could not retrieve masked pan"
            )
        }

        guard let type = result.cardType else {
            throw TakeTapPaymentException.couldNotCreatePaymentMethod(
                detail: "Card type is required"
            )
        }

        var paymentMethod = StaxPaymentMethod.from(customer: customer)
        
        // Card expiration is now enriched from CardDetails callback for tap to pay
        paymentMethod.cardExpiry = result.cardExpiration
        paymentMethod.method = .card
        paymentMethod.cardType = type
        paymentMethod.cardLastFour = String(lastFour)
        
        // Use cardholder name if available from card details, otherwise use customer name
        if let firstName = result.cardHolderFirstName, 
           let lastName = result.cardHolderLastName,
           !firstName.isEmpty, !lastName.isEmpty {
            paymentMethod.personName = "\(firstName) \(lastName)"
        } else if let firstName = result.cardHolderFirstName, !firstName.isEmpty {
            paymentMethod.personName = firstName
        } else if let lastName = result.cardHolderLastName, !lastName.isEmpty {
            paymentMethod.personName = lastName
        } else {
            paymentMethod.personName = customer.name
        }
        
        paymentMethod.tokenize = false

        // If there is a token, tokenize it with the POST /payment-method/token route
        if let token = result.paymentToken, !token.isEmpty {
            paymentMethod.paymentToken = token
            let request = StaxApiRequest<StaxPaymentMethod>(
                path: "/payment-method/token",
                method: .post,
                body: paymentMethod
            )
            return try await client.perform(request)
        }

        paymentMethod.paymentToken = "nmi_1234"
        // If there is no token, use the normal POST /payment-method route
        let request = StaxApiRequest<StaxPaymentMethod>(
            path: "/payment-method/token",
            method: .post,
            body: paymentMethod
        )
        return try await client.perform(request)
    }

    fileprivate func updateInvoice(
        _ invoice: StaxInvoice,
        with customer: StaxCustomer,
        and paymentMethod: StaxPaymentMethod
    ) async throws -> StaxInvoice {
        guard let id = invoice.id else {
            throw TakeTapPaymentException.couldNotUpdateInvoice(
                detail: "Invoice ID is required"
            )
        }

        guard let paymentMethodId = paymentMethod.id else {
            throw TakeTapPaymentException.couldNotUpdateInvoice(
                detail: "Payment Method ID is required"
            )
        }

        guard let customerId = customer.id else {
            throw TakeTapPaymentException.couldNotUpdateInvoice(
                detail: "Customer ID is required"
            )
        }

        var update = invoice.updating()
        update.customerId = customerId
        update.paymentMethodId = paymentMethodId

        let invoiceRepository = StaxInvoiceRepositoryImpl(httpClient: client)
        return try await invoiceRepository.updateInvoice(id: id, update: update)
    }

    fileprivate func createTransaction(
        driver: TapDriver,
        customer: StaxCustomer,
        invoice: StaxInvoice,
        paymentMethod: StaxPaymentMethod
    ) async throws -> StaxTransaction {

        guard let paymentMethodId = paymentMethod.id else {
          throw TakeMobileReaderPaymentException.couldNotUpdateInvoice(detail: "Payment Method ID is required")
        }

        guard let lastFour = paymentMethod.cardLastFour else {
          throw TakeMobileReaderPaymentException.couldNotCreatePaymentMethod(detail: "Could not retrieve masked pan")
        }
        
        guard let customerId = customer.id else {
          throw TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: "Customer id is required")
        }

        guard let invoiceId = invoice.id else {
          throw TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: "Invoice id is required")
        }
        
        guard let result = result else {
          throw TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: "No TransactionResult returned")
        }

        var gatewayResponseJson: JSONCodable = JSONCodable.null
        if let authCode = result.authCode, result.source.lowercased() == "nmi" {
          let gatewayResponse = [
            "gateway_specific_response_fields": [
              "nmi": [
                "authcode": authCode
              ]
            ]
          ]
          gatewayResponseJson = (try? JSONCodable.encode(gatewayResponse)) ?? JSONCodable.null
        }
        
        var transaction = StaxTransaction()
        transaction.paymentMethodId = paymentMethodId
        transaction.total = request.amount.dollars()
        transaction.success = result.success ?? false
        transaction.lastFour = lastFour
        transaction.meta = result.createTransactionMeta()
        transaction.type = .charge
        transaction.method = "card"
        transaction.source = "iOS|CPSDK|\(result.source)|tap_to_pay"
        transaction.channel = "ios"
        transaction.customerId = customerId
        transaction.customer = customer
        transaction.invoiceId = invoiceId
        transaction.response = gatewayResponseJson
        transaction.token = result.externalId
        transaction.message = result.message

        let request = StaxApiRequest<StaxTransaction>(path: "/transaction", method: .post, body: transaction)
        return try await client.perform(request)
    }
}
