import Foundation

@available(iOS 17.4, *)
final class SingleShot {
    private var didResume = false
    func run(_ block: () -> Void) {
        guard !didResume else { return }
        didResume = true
        block()
    }
}

@available(iOS 17.4, *)
actor TakeTapPaymentJob: Job {
    fileprivate static let DEFAULT_TAP_CUSTOMER_NAME = "Tap Payment Customer"

    typealias ResultType = StaxTransaction

    private let client: StaxHttpClientProtocol
    private var request: TransactionRequest
    private let signatureProvider: SignatureProviding?
    private weak var transactionUpdateDelegate: TransactionUpdateDelegate?
    private weak var userNotificationDelegate: UserNotificationDelegate?

    fileprivate var result: TransactionResult? = nil

    private let performTimeout: UInt64 = 90 * 1_000_000_000 // 90s
    private let captureTimeout: UInt64 = 45 * 1_000_000_000 // 45s
    private let voidTimeout: UInt64 = 30 * 1_000_000_000 // 30s

    init(
        request: TransactionRequest,
        client: StaxHttpClientProtocol,
        signatureProvider: SignatureProviding? = nil,
        transactionUpdateDelegate: TransactionUpdateDelegate? = nil,
        userNotificationDelegate: UserNotificationDelegate? = nil
    ) {
        self.request = request
        self.client = client
        self.signatureProvider = signatureProvider
        self.transactionUpdateDelegate = transactionUpdateDelegate
        self.userNotificationDelegate = userNotificationDelegate
    }

    func start() async -> JobResult<StaxTransaction> {
        let drivers = TapPaymentDriverRepository.shared.all()
        guard let driver = drivers.first, drivers.count == 1 else {
            return JobResult.failure(OmniGeneralException.uninitialized)
        }

        do {
            let currentRequest = self.request
            let currentSignatureProvider = self.signatureProvider
            let currentTransactionUpdateDelegate = self.transactionUpdateDelegate
            let currentUserNotificationDelegate = self.userNotificationDelegate

            var invoice = try await getOrCreateInvoice(id: currentRequest.invoiceId)

            result = try await performWithTimeout(timeout: performTimeout) { resume in
                driver.performTransaction(
                    with: currentRequest,
                    signatureProvider: currentSignatureProvider,
                    transactionUpdateDelegate: currentTransactionUpdateDelegate,
                    userNotificationDelegate: currentUserNotificationDelegate
                ) { txnResult in
                    resume(.success(txnResult))
                }
            }

            guard let result = result else {
                throw TakeTapPaymentException.couldNotCreateInvoice(detail: "No TransactionResult")
            }
            
            // If transaction was not successful (cancelled, declined, or error), 
            // don't try to create backend records - return the transaction result as-is
            guard result.success == true else {
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

            let customer = try await createCustomer(from: result)
            let paymentMethod = try await createPaymentMethod(
                from: customer,
                and: result
            )
            invoice = try await updateInvoice(
                invoice,
                with: customer,
                and: paymentMethod
            )

            let transaction = try await createTransaction(
                driver: driver,
                customer: customer,
                invoice: invoice,
                paymentMethod: paymentMethod
            )

            guard transaction.id != nil else {
                throw TakeTapPaymentException.couldNotCreateTransaction(
                    detail: "Missing transaction id"
                )
            }

            return JobResult.success(transaction)
        } catch {
            // Void the transaction (best-effort) and mark the JobResult as a failure
            if let result = result {
                _ = try? await performWithTimeout(timeout: voidTimeout) { resume in
                    driver.void(
                        transactionResult: result,
                        completion: { _ in resume(.success(())) }
                    )
                }
            }

            // If error already conforms to OmniException, return it; otherwise wrap
            if let omni = error as? OmniException {
                return JobResult.failure(omni)
            } else {
                // Use a generic tap error with detail
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

    fileprivate func createCustomer(from result: TransactionResult) async throws
        -> StaxCustomer
    {
        let customerRepository = StaxCustomerRepositoryImpl(httpClient: client)

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
        transaction.invoiceId = invoiceId
        transaction.response = gatewayResponseJson
        transaction.token = result.externalId
        transaction.message = result.message

        // DEBUG: Pretty-print the JSON body we are about to send
        do {
          let encoder = JSONEncoder()
          encoder.outputFormatting = [.prettyPrinted, .sortedKeys]
          // Use a stable date format if any dates are included (not expected for create payload)
          encoder.dateEncodingStrategy = .iso8601
          let data = try encoder.encode(transaction)
          if let jsonString = String(data: data, encoding: .utf8) {
            print("TakeTapPaymentJob.createTransaction JSON body:\n\(jsonString)")
          }
        } catch {
          print("TakeTapPaymentJob.createTransaction JSON encoding failed: \(error)")
        }

        let request = StaxApiRequest<StaxTransaction>(path: "/transaction", method: .post, body: transaction)
        return try await client.perform(request)
    }
}
