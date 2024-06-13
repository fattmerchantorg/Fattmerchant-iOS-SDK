import Foundation

class PerformTransactionWithCardReaderJob: Job {
  typealias Output = Transaction
  
  private let request: TransactionRequest

  private var http: StaxHttpService {
    get throws {
      guard let service = Services.resolve(StaxHttpService.self) else {
        throw StaxNetworkingException.serviceNotInitialized
      }
      return service
    }
  }

  private var card: CardReaderService {
    get throws {
      guard let service = Services.resolve(CardReaderService.self) else {
        throw StaxInitializeException.mobileReaderPaymentsNotConfigured
      }
      return service
    }
  }
  
  public var signatureProvider: SignatureProviding? = nil
  public weak var transactionUpdateDelegate: TransactionUpdateDelegate? = nil
  
  public init(request: TransactionRequest) {
    self.request = request
  }
  
  public func start() async throws -> Transaction {
    // Register Listeners
    try card.setTransactionUpdateDelegate(transactionUpdateDelegate)
    try card.setSignatureProvider(signatureProvider)
    
    // Create the invoice or get an existing one from the `TransactionRequest`
    let invoice = try await getOrCreateInvoice()
    
    // Perform the Transaction against the Gateway
    let result = try await card.performTransaction(self.request)
    
    // Create a Customer & PaymentMethod
    let customer = try await createCustomer(result)
    let paymentMethod = try await createPaymentMethod(for: customer, from: result)
    
    // Attach the Customer & PaymentMethod to the invoice
    let updatedInvoice = try await updateInvoice(invoice, with: paymentMethod, and: customer)

    // Create and return the Transaction record
    let transaction = try await createTransaction(
      from: result,
      customer: customer,
      invoice: updatedInvoice,
      paymentMethod: paymentMethod
    )
    
    // Make sure the transaction from Stax has an id. This should be true pretty much all the time
    guard let transactionId = transaction.id else {
      let _ = try await voidAndFail(result)
      throw TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: nil)
    }

    // If the transaction is a pre-auth, then we don't need to capture it. Return!
    if self.request.preauth {
      return transaction
    }
    
    // If not a pre-auth, attempt to capture it
    let isSuccess = try await card.captureTransaction(transaction)
    if isSuccess {
      return transaction
    }
    
    // Couldn't capture...fail the Transaction in Stax and attempt a void
    let failed = transaction
    failed.success = false
    failed.message = "Error capturing the transaction"
    
    do {
      defer {
        Task {
          let _ = try await voidAndFail(result)
        }
      }
      return try await http.putTransaction(transaction: failed, id: failed.id!)
    }
  }
  
  private func getOrCreateInvoice() async throws -> Invoice {
    if let id = request.invoiceId {
      if id.isEmpty || id.isBlank() {
        throw TakeMobileReaderPaymentException.invoiceIdCannotBeBlank
      }
      
      return try await http.getInvoice(id: id)
    }
    
    let potentialInvoice = Invoice()
    guard let potentialInvoiceMeta = createInvoiceMeta() else {
      throw TakeMobileReaderPaymentException.couldNotCreateInvoice(detail: "Error generating json for meta")
    }
    potentialInvoice.total = request.amount.dollars()
    potentialInvoice.url = "https://fattpay.com/#/bill"
    potentialInvoice.meta = potentialInvoiceMeta
    
    do {
      let invoice = try await http.postInvoice(invoice: potentialInvoice)
      return invoice
    } catch {
      throw TakeMobileReaderPaymentException.couldNotCreateInvoice(detail: nil)
    }
  }
  
  /// Creates the json meta for the invoice as modeled in the given transaction request
  /// - Parameter request: a TransactionRequest
  /// - Returns: an instance of JSONValue; the meta in json form
  private func createInvoiceMeta() -> JSONValue? {
    var dict = [String: JSONValue?]()

    if let subtotal = request.subtotal {
      dict["subtotal"] = JSONValue(subtotal)
    } else {
      dict["subtotal"] = JSONValue(request.amount.dollars())
    }

    if let tax = request.tax { dict["tax"] = JSONValue(tax) }
    if let memo = request.memo { dict["memo"] = JSONValue(memo) }
    if let reference = request.reference { dict["reference"] = JSONValue(reference) }
    if let tip = request.tip { dict["tip"] = JSONValue(tip) }
    if let shippingAmount = request.shippingAmount { dict["shippingAmount"] = JSONValue(shippingAmount) }
    if let poNumber = request.poNumber { dict["poNumber"] = JSONValue(poNumber) }
    if let lineItems = request.lineItems { dict["lineItems"] = JSONValue(lineItems) }

    return dict.jsonValue()
  }
  
  private func updateInvoice(
    _ invoice: Invoice,
    with paymentMethod: PaymentMethod,
    and customer: Customer
  ) async throws -> Invoice {
    let updatedInvoice = Invoice()

    guard let id = invoice.id else {
      throw TakeMobileReaderPaymentException.couldNotUpdateInvoice(detail: "Invoice id is required")
    }

    guard let paymentMethodId = paymentMethod.id else {
      throw TakeMobileReaderPaymentException.couldNotUpdateInvoice(detail: "Payment method id is required")
    }

    guard let customerId = customer.id else {
      throw TakeMobileReaderPaymentException.couldNotUpdateInvoice(detail: "Customer id is required")
    }

    updatedInvoice.customerId = customerId
    updatedInvoice.paymentMethodId = paymentMethodId
    
    return try await http.putInvoice(invoice: updatedInvoice, id: id)
  }
  
  private func createCustomer(_ result: TransactionResult) async throws -> Customer {
    let first = result.cardHolderFirstName ?? "SWIPE"
    let last = result.cardHolderLastName ?? "CUSTOMER"

    var potentialCustomer = Customer(firstName: first, lastName: last)
    if let source = result.transactionSource {
      if source.lowercased().contains("contactless") {
        potentialCustomer.firstname = "Mobile Device"
        potentialCustomer.lastname = "Customer"
      }
    }

    do {
      let customer = try await http.postCustomer(customer: potentialCustomer)
      return customer
    } catch {
      throw TakeMobileReaderPaymentException.couldNotCreateCustomer(detail: nil)
    }
  }
  
  private func createPaymentMethod(for customer: Customer, from result: TransactionResult) async throws -> PaymentMethod {
    guard let customerId = customer.id else {
      throw TakeMobileReaderPaymentException.couldNotCreateCustomer(detail: "Customer id is required")
    }

    guard let lastFour = result.getLastFour() else {
      throw TakeMobileReaderPaymentException.couldNotCreatePaymentMethod(detail: "Could not retrieve masked pan")
    }

    guard let cardType = result.cardType else {
      throw TakeMobileReaderPaymentException.couldNotCreateCustomer(detail: "Card type is required")
    }

    let potentialPaymentMethod = PaymentMethod(customer: customer)
    potentialPaymentMethod.cardExp = result.cardExpiration
    potentialPaymentMethod.customerId = customerId
    potentialPaymentMethod.method = PaymentMethodType.card
    potentialPaymentMethod.cardLastFour = lastFour
    potentialPaymentMethod.cardType = cardType
    potentialPaymentMethod.personName = "\(customer.firstname) \(customer.lastname)"
    potentialPaymentMethod.tokenize = false
    potentialPaymentMethod.paymentToken = result.paymentToken

    // When the payment method was tokenized, we want to use the
    // postPaymentMethodToken method since it tells Stax to save the token
    if potentialPaymentMethod.paymentToken != nil {
      return try await http.postPaymentMethodToken(paymentMethod: potentialPaymentMethod)
    } else {
      return try await http.postPaymentMethod(paymentMethod: potentialPaymentMethod)
    }
  }
  
  private func createTransaction(
    from result: TransactionResult,
    customer: Customer,
    invoice: Invoice,
    paymentMethod: PaymentMethod
  ) async throws -> Transaction {

    guard let paymentMethodId = paymentMethod.id else {
      throw TakeMobileReaderPaymentException.couldNotUpdateInvoice(detail: "Payment method id is required")
    }

    guard let customerId = customer.id else {
      throw TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: "Customer id is required")
    }

    guard let invoiceId = invoice.id else {
      throw TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: "Invoice id is required")
    }
    
    guard let lastFour = result.getLastFour() else {
      throw TakeMobileReaderPaymentException.couldNotCreatePaymentMethod(detail: "Could not retrieve masked pan")
    }

    guard let transactionMeta = createTransactionMeta(from: result) else {
      throw TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: "Could not generate transaction meta json")
    }

    // Manually build JSON if from NMI and we have the auth code
    var gatewayResponseJson: JSONValue?
    if result.source.lowercased() == NmiCardReaderService.SOURCE.lowercased() {
      if let code = result.authCode {
        let response = [
          "gateway_specific_response_fields": [
            "nmi": [ "authcode": code ]
          ]
        ]
        gatewayResponseJson = response.jsonValue()
      }
    }

    let potentialTransaction = Transaction()
    potentialTransaction.paymentMethodId = paymentMethodId
    potentialTransaction.total = request.amount.dollars()
    potentialTransaction.success = result.success ?? false
    potentialTransaction.lastFour = lastFour
    potentialTransaction.meta = transactionMeta
    potentialTransaction.type = "charge"
    potentialTransaction.method = "card"
    potentialTransaction.source = "iOS|CPSDK|\(result.source)"
    potentialTransaction.customerId = customerId
    potentialTransaction.invoiceId = invoiceId
    potentialTransaction.response = gatewayResponseJson
    potentialTransaction.token = result.externalId
    potentialTransaction.message = result.message

    // Set the transaction to not refundable or voidable if the Omni backend cannot perform the refund
    // This is extremely important because it prevents a user from attempting a refund via the VT or the Omni API that
    // could never work. The reason it won't work is because Omni doesn't have a deep integration with all of our
    // third party vendors, such as AnywhereCommerce.
    // As of June 2024, `isRefundSupported` should always be true because we ONLY use NMI
    if !type(of: try card).isStaxRefundSupported {
      potentialTransaction.isRefundable = false
      potentialTransaction.isVoidable = false
    }

    // Mark the transaction as pre-auth, if necessary
    if self.request.preauth {
      potentialTransaction.type = "pre_auth"
      potentialTransaction.preAuth = true
      potentialTransaction.isCaptured = 0
      potentialTransaction.isVoidable = true
    }

    do {
      return try await http.postTransaction(transaction: potentialTransaction)
    } catch {
      throw error
    }
  }
  
  private func createTransactionMeta(from result: TransactionResult) -> JSONValue? {
    var dict = [String: JSONValue?]()

    #if !targetEnvironment(simulator)
    if result.source.contains(NmiCardReaderService.SOURCE) {
      if let user = result.userReference { dict["nmiUserRef"] = JSONValue(user) }
      if let local = result.localId { dict["cardEaseReference"] = JSONValue(local) }
      if let external = result.externalId { dict["nmiTransactionId"] = JSONValue(external) }
    }
    #endif

    if let gateway = result.gatewayResponse { dict["gatewayResponse"] = JSONValue(gateway) }
    if let lineItems = result.request?.lineItems { dict["lineItems"] = JSONValue(lineItems) }
    if let subtotal = result.request?.subtotal { dict["subtotal"] = JSONValue(subtotal) }
    if let tax = result.request?.tax { dict["tax"] = JSONValue(tax) }
    if let memo = result.request?.memo { dict["memo"] = JSONValue(memo) }
    if let reference = result.request?.reference { dict["reference"] = JSONValue(reference) }
    if let tip = result.request?.tip { dict["tip"] = JSONValue(tip) }

    return dict.jsonValue()
  }
  
  private func voidAndFail(_ result: TransactionResult) async throws -> Bool {
    return try await card.voidTransaction(result)
  }
}
