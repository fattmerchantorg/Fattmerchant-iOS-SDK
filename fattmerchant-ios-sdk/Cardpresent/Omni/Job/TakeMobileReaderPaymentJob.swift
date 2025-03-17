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
      let invoice = try await getOrCreateInvoice(id: request.invoiceId)
      let transactionResult = await withCheckedContinuation { continuation in
        let req = self.request
        let sig = self.signatureProvider
        let del = self.transactionUpdateDelegate

        driver.performTransaction(with: req, signatureProvider: sig, transactionUpdateDelegate: del) { result in
          continuation.resume(returning: result)
        }
      }
      let customer = try await createCustomer(from: transactionResult)
      
    } catch {
      return JobResult.failure(error as! OmniException)
    }
    
    return JobResult.success(StaxTransaction())
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
  
  fileprivate func createPaymentMethod(from customer: Customer, and result: TransactionResult) async throws -> StaxPaymentMethod {
    // TODO: Modify this
    /*
    let paymentMethodToCreate = PaymentMethod(customer: customer)

    guard let customerId = customer.id else {
      failure(Exception.couldNotCreateCustomer(detail: "Customer id is required"))
      return
    }

    guard let lastFour = getLastFour(for: result.maskedPan) else {
      failure(Exception.couldNotCreatePaymentMethod(detail: "Could not retrieve masked pan"))
      return
    }

    guard let cardType = result.cardType else {
      failure(Exception.couldNotCreateCustomer(detail: "Card type is required"))
      return
    }

    paymentMethodToCreate.cardExp = result.cardExpiration
    paymentMethodToCreate.customerId = customerId
    paymentMethodToCreate.method = PaymentMethodType.card
    paymentMethodToCreate.cardLastFour = lastFour
    paymentMethodToCreate.cardType = cardType
    paymentMethodToCreate.personName = "\(customer.firstname) \(customer.lastname)"
    paymentMethodToCreate.tokenize = false
    paymentMethodToCreate.paymentToken = result.paymentToken

    // When the payment method was tokenized, we want to use the
    // createTokenizedPaymentMethod method since it tells Omni to save the token
    if paymentMethodToCreate.paymentToken != nil {
      paymentMethodRepository.createTokenizedPaymentMethod(model: paymentMethodToCreate, completion: completion, error: failure)
    } else {
      paymentMethodRepository.create(model: paymentMethodToCreate, completion: completion, error: failure)
    }
     */
  }
}
