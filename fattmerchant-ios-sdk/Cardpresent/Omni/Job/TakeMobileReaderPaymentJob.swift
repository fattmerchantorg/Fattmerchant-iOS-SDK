import Foundation

actor TakeMobileReaderPaymentJob: Job {
  fileprivate static let DEFAULT_NAME = "SWIPE CUSTOMER"
  fileprivate static let DEFAULT_CONTACTLESS_NAME = "Mobile Device Customer"
  
  typealias ResultType = StaxTransaction
  
  private let client: StaxHttpClientProtocol
  private let request: TransactionRequest
  private let signatureProvider: SignatureProviding?
  private weak var transactionUpdateDelegate: TransactionUpdateDelegate?
  private weak var userNotificationDelegate: UserNotificationDelegate?
  
  fileprivate var result: TransactionResult? = nil

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
    let drivers = await MobileReaderDriverRepository.shared.getInitializedDrivers()
    guard let driver = drivers.first, drivers.count == 1 else {
      return JobResult.failure(OmniGeneralException.uninitialized)
    }
    
    do {
      var invoice = try await getOrCreateInvoice(id: request.invoiceId)
      result = await withCheckedContinuation { continuation in
        let req = self.request
        let sig = self.signatureProvider
        let del = self.transactionUpdateDelegate

        driver.performTransaction(with: req, signatureProvider: sig, transactionUpdateDelegate: del) { result in
          continuation.resume(returning: result)
        }
      }

      guard let result = result else {
        throw TakeMobileReaderPaymentException.couldNotCreateInvoice(detail: nil)
      }

      let customer = try await createCustomer(from: result)
      let paymentMethod = try await createPaymentMethod(from: customer, and: result)
      invoice = try await updateInvoice(invoice, with: customer, and: paymentMethod)
      
      let transaction = try await createTransaction(
        driver: driver,
        customer: customer,
        invoice: invoice,
        paymentMethod: paymentMethod
      )
      // Make sure the transaction from Stax has an id. This should be true pretty much all the time
      guard transaction.id != nil else {
        throw TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: nil)
      }

      // If the transaction is a pre-auth, then we don't need to capture it
      if request.preauth {
        return JobResult.success(transaction)
      }

      let isSuccessful = await withCheckedContinuation { continuation in
        driver.capture(transaction) { isSuccessful in
          continuation.resume(returning: isSuccessful)
        }
      }
      
      if isSuccessful {
        return JobResult.success(transaction)
      }
      
      // We couldn't capture the transaction. So mark it failed on Stax.
      var failed = transaction
      failed.success = false
      failed.message = "Error capturing the transaction"

      let request = StaxApiRequest<StaxTransaction>(path: "/transaction", method: .put, body: failed)
      let _ = try await client.perform(request)
      throw TakeMobileReaderPaymentException.couldNotCaptureTransaction
    } catch {
      // Void the transaction in NMI and mark the JobResult as a failure
      guard let result = result else { return JobResult.failure(error as! OmniException) }
      await withCheckedContinuation { continuation in
        driver.void(transactionResult: result, completion: { _ in continuation.resume(returning: ()) })
      }
      
      return JobResult.failure(error as! OmniException)
    }
  }
  
  fileprivate func getOrCreateInvoice(id: String?) async throws -> StaxInvoice {
    let invoiceRepository = StaxInvoiceRepositoryImpl(httpClient: client)
    
    // GET /invoice/:id
    if let id = id {
      guard !id.isEmpty else {
        throw TakeMobileReaderPaymentException.invoiceIdCannotBeBlank
      }
      guard let invoice = try? await invoiceRepository.getInvoice(id: id) else {
        throw TakeMobileReaderPaymentException.invoiceNotFound
      }
      return invoice
    }

    // POST /invoice
    var invoice = StaxInvoice()
    invoice.total = request.amount.dollars()
    invoice.url = "https://fattpay.com/#/bill"
    invoice.meta = request.createInvoiceMeta()

    guard let created = try? await invoiceRepository.createInvoice(invoice) else {
      throw TakeMobileReaderPaymentException.couldNotCreateInvoice(detail: nil)
    }
    
    return created
  }
  
  fileprivate func createCustomer(from result: TransactionResult) async throws -> StaxCustomer {
    let customerRepository = StaxCustomerRepositoryImpl(httpClient: client)
    
    var name: String = TakeMobileReaderPaymentJob.DEFAULT_NAME
    if let first = result.cardHolderFirstName, let last = result.cardHolderLastName {
      name = "\(first) \(last)"
    } else if let source = result.transactionSource, source.lowercased().contains("contactless") {
      name = TakeMobileReaderPaymentJob.DEFAULT_CONTACTLESS_NAME
    }
    
    let request = StaxCustomer.from(name: name)
    return try await customerRepository.createCustomer(request)
  }
  
  fileprivate func createPaymentMethod(from customer: StaxCustomer, and result: TransactionResult) async throws -> StaxPaymentMethod {
    guard let customerId = customer.id, !customerId.isEmpty else {
      throw TakeMobileReaderPaymentException.couldNotCreatePaymentMethod(detail: "Customer ID is required")
    }
    
    guard let lastFour = result.maskedPan?.suffix(4), lastFour.count == 4 else {
      throw TakeMobileReaderPaymentException.couldNotCreatePaymentMethod(detail: "Could not retrieve masked pan")
    }
    
    guard let type = result.cardType else {
      throw TakeMobileReaderPaymentException.couldNotCreatePaymentMethod(detail: "Card type is required")
    }
    
    var paymentMethod = StaxPaymentMethod.from(customer: customer)
    paymentMethod.cardExpiry = result.cardExpiration
    paymentMethod.method = .card
    paymentMethod.cardType = type
    paymentMethod.cardLastFour = String(lastFour)
    paymentMethod.personName = customer.name
    paymentMethod.tokenize = false
    
    // If there is a token, tokenize it with the POST /payment-method/token route
    if let token = result.paymentToken, !token.isEmpty {
      paymentMethod.paymentToken = token
      let request = StaxApiRequest<StaxPaymentMethod>(path: "/payment-method/token", method: .post, body: paymentMethod)
      return try await client.perform(request)
    }
    
    // If there is no token, use the normal POST /payment-method route
    let request = StaxApiRequest<StaxPaymentMethod>(path: "/payment-method", method: .post, body: paymentMethod)
    return try await client.perform(request)
  }
  
  fileprivate func updateInvoice(
    _ invoice: StaxInvoice,
    with customer: StaxCustomer,
    and paymentMethod: StaxPaymentMethod
  ) async throws -> StaxInvoice {
    guard let id = invoice.id else {
      throw TakeMobileReaderPaymentException.couldNotUpdateInvoice(detail: "Invoice ID is required")
    }

    guard let paymentMethodId = paymentMethod.id else {
      throw TakeMobileReaderPaymentException.couldNotUpdateInvoice(detail: "Payment Method ID is required")
    }

    guard let customerId = customer.id else {
      throw TakeMobileReaderPaymentException.couldNotUpdateInvoice(detail: "Customer ID is required")
    }

    var update = invoice.updating()
    update.customerId = customerId
    update.paymentMethodId = paymentMethodId
    
    let invoiceRepository = StaxInvoiceRepositoryImpl(httpClient: client)
    return try await invoiceRepository.updateInvoice(id: id, update: update)
  }
  
  fileprivate func createTransaction(
    driver: MobileReaderDriver,
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
    transaction.source = "iOS|CPSDK|\(result.source)"
    transaction.customerId = customerId
    transaction.invoiceId = invoiceId
    transaction.response = gatewayResponseJson
    transaction.token = result.externalId
    transaction.message = result.message

    // Set the transaction to not refundable or voidable if the Omni backend cannot perform the refund
    // This is extremely important because it prevents a user from attempting a refund via the VT or the Omni API that
    // could never work. The reason it won't work is because Omni doesn't have a deep integration with all of our
    // third party vendors, such as AnywhereCommerce.
    if !type(of: driver).isStaxRefundsSupported {
      transaction.isRefundable = false
      transaction.isVoidable = false
    }

    // Mark the transaction as pre-auth, if necessary
    if request.preauth {
      transaction.type = .preAuth
      transaction.preAuth = true
      transaction.isCaptured = 0
      transaction.isVoidable = true
    }

    let request = StaxApiRequest<StaxTransaction>(path: "/transaction", method: .post, body: transaction)
    return try await client.perform(request)
  }
}
