import Foundation

/**
 Handles cardpresent payments
 
 This is the object you used for communicating with the Omni platform
 
 ## Usage
 Create an instance of Omni
 ```
 let omni = Omni()
 ```
 Then initialize, passing in an instance of InitParams
 ```
 let omniInitParms = Omni.InitParams(appId: "Myappid", apiKey: "omniephemeralkey")
 omni.initialize(omniInitParams, completion: ..., error: ...)
 ```
 
 Once initialized, you can call its methods like `getAvailableReaders` and `takeMobileReaderTransaction`
 */
public class Omni: NSObject {
  internal var staxHttpClient: StaxHttpClientProtocol? = nil
  
  internal var mobileReaderDriversInitialized: Bool = false
  internal var omniApi = OmniApi()
  internal var transactionRepository: TransactionRepository!
  internal var invoiceRepository: InvoiceRepository!
  internal var customerRepository: CustomerRepository!
  internal var paymentMethodRepository: PaymentMethodRepository!
  internal var mobileReaderDriverRepository = MobileReaderDriverRepository()

  internal var staxInvoiceRepository: StaxInvoiceRepository!
  internal var staxCatalogRepository: StaxCatalogRepository!
  internal var staxPaymentMethodRepository: StaxPaymentMethodRepository!
  internal var staxCustomerRepository: StaxCustomerRepository!
  internal var merchant: Merchant?

  internal var accessoryHelper: AccessoryHelper?
  
  /// The queue that Omni should use to communicate back with its listeners
  public var preferredQueue: DispatchQueue = DispatchQueue.main
  
  /// Responsible for providing signatures for transactions, when required
  public var signatureProvider: SignatureProviding?
  
  /// Receives notifications about transaction events such as when a card is swiped
  public weak var transactionUpdateDelegate: TransactionUpdateDelegate?
  
  /// Receives notifications about user-facing transaction events such as when a user swipes a chip card
  public weak var userNotificationDelegate: UserNotificationDelegate?
  
  /// Receives notifications about reader connection events
  public weak var mobileReaderConnectionUpdateDelegate: MobileReaderConnectionStatusDelegate?
  
  public weak var usbAccessoryDelegate: UsbAccessoryDelegate?
    
  
  /// True when Omni is initialized. False otherwise
  public var isInitialized: Bool {
    return mobileReaderDriversInitialized
  }
  
  /// Contains all the data necessary to initialize `Omni`
  @available(*, deprecated, message: "Deprecated in favor of `InitializationArgs`.")
  public struct InitParams {
    /// An id for your application
    public var appId: String?
    
    /// An ephemeral Omni api key
    public var apiKey: String?
    
    /// The Omni enviroment to use
    public var environment: Environment
    
    /// The Omni webpayments token
    ///
    /// This is used for tokenizing and charging payment methods
    public var webpaymentsToken: String?
      
    
    public init(appId: String, apiKey: String, environment: Environment = Environment.LIVE) {
      self.appId = appId
      self.apiKey = apiKey
      self.environment = environment
    }
  }
  
  fileprivate func initNewRepos(httpClient: StaxHttpClientProtocol) {
    // Initialize the new repositories
    staxInvoiceRepository = StaxInvoiceRepositoryImpl(httpClient: httpClient)
    staxCatalogRepository = StaxCatalogRepositoryImpl(httpClient: httpClient)
    staxCustomerRepository = StaxCustomerRepositoryImpl(httpClient: httpClient)
    staxPaymentMethodRepository = StaxPaymentMethodRepositoryImpl(httpClient: httpClient)
  }
    
  
  
  /// Setup and prepare the `Omni` instance.
  /// - Parameter args: The `InitializationArgs` required for bootstrapping the SDK.
  /// - Parameter completion: A `() -> Void` callback run after the SDK is initialized.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when initializing.
  public func initialize(args: InitializationArgs, completion: @Sendable @escaping () -> Void, error: @escaping (OmniException) -> Void) {
    let apiKey = args.ephemeralToken
    let url = URL(string: "https://apiprod.fattlabs.com")!
    self.staxHttpClient = StaxHttpClient(baseURL: url, apiKey: apiKey)
    
    omniApi.apiKey = args.ephemeralToken
    omniApi.environment = .LIVE
    
    transactionRepository = TransactionRepository(omniApi: omniApi)
    invoiceRepository = InvoiceRepository(omniApi: omniApi)
    customerRepository = CustomerRepository(omniApi: omniApi)
    paymentMethodRepository = PaymentMethodRepository(omniApi: omniApi)
    
    initNewRepos(httpClient: staxHttpClient!)
    
    initializeUsbDelegate()
    
    Task {
      // Get "/self" and "/team/gateway/hardware/mobile" settings from API
      guard let auth = try? await getStaxSelf(apiKey), (auth?.merchant) != nil else {
        error(OmniNetworkingException.couldNotGetMerchantDetails)
        return
      }
      
      guard
        let details = try? await getMobileReaderAuthDetails(apiKey),
        let nmiKeys = details?.nmi
      else {
        error(OmniInitializeException.missingMobileReaderCredentials)
        return
      }
      
      // Set the InitArgs based on environment type
      #if targetEnvironment(simulator)
      let initArgs = MockInitializationArgs(appId: args.applicationId)
      #else
      let initArgs = ChipDnaInitializationArgs(appId: args.applicationId, keys: nmiKeys)
      #endif

      let result = await InitializeDriversJob(args: initArgs).start()
      switch result {
      case .success:
        self.mobileReaderDriversInitialized = true
        self.preferredQueue.async(execute: completion)
      case .failure(let fail):
        self.mobileReaderDriversInitialized = true
        error(fail)
      }
    }
  }
  
  /// Setup and prepare the `Omni` instance.
  /// - Parameter params: The `InitParams` required for bootstrapping the SDK.
  /// - Parameter completion: A `() -> Void` callback run after the SDK is initialized.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when initializing.
  @available(*, deprecated, message: "Deprecated in favor of the `initialize` with `InitializationArgs`.")
  public func initialize(params: InitParams, completion: @Sendable @escaping () -> Void, error: @escaping (OmniException) -> Void) {
    guard let appId = params.appId, let apiKey = params.apiKey else {
      return error(OmniInitializeException.missingInitializationDetails)
    }

    let args = InitializationArgs(applicationId: appId, ephemeralToken: apiKey)
    self.initialize(args: args, completion: completion, error: error)
  }
  
  /// Creates a `PaymentMethod` out of a `BankAccount` object for reuse with Stax Pay.
  /// - Parameter bankAccount: Contains the `BankAccount` details to tokenize with Stax Pay.
  /// - Parameter completion: A `(PaymentMethod) -> Void` callback run after the bank has been tokenized.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when tokenizing.
  @available(*, deprecated, message: "Deprecated in favor of the `tokenizeBankAccount` function.")
  public func tokenize(_ bankAccount: BankAccount, _ completion: @escaping (PaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    TokenizePaymentMethod(customerRepository: customerRepository,
                          paymentMethodRepository: paymentMethodRepository,
                          bankAccount: bankAccount
    ).start(completion: completion, failure: error)
  }
  
  /// Creates a `PaymentMethod` out of a `BankAccount` object for reuse with Stax Pay.
  /// - Parameter bankAccount: Contains the `BankAccount` details to tokenize with Stax Pay.
  /// - Parameter completion: A `(PaymentMethod) -> Void` callback run after the bank has been tokenized.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when tokenizing.
  @available(*, deprecated, message: "Deprecated in favor of the `tokenizeBankAccount` function.")
  public func tokenize(bankAccount: BankAccount, completion: @escaping (PaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    let job = TokenizePaymentMethod(
      customerRepository: customerRepository,
      paymentMethodRepository: paymentMethodRepository,
      bankAccount: bankAccount
    )
    
    job.start(completion: completion, failure: error)
  }
  
  /// Creates a `StaxPaymentMethod` out of a `StaxBankAccount` object for reuse with Stax Pay.
  /// - Parameter bank: Contains the `StaxBankAccount` details to tokenize with Stax Pay.
  /// - Parameter completion: A `(StaxPaymentMethod) -> Void` callback run after the bank has been tokenized.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when tokenizing.
  public func tokenizeBankAccount(_ bank: StaxBankAccount, completion: @escaping (StaxPaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    guard let client = staxHttpClient else { return error(OmniGeneralException.uninitialized); }
    
    let job = TokenizeBankAccountJob(bank: bank, client: client)
    Task {
      let result = await job.start()
      switch result {
        case .success(let tokenized): completion(tokenized)
        case .failure(let fail): error(fail )
      }
    }
  }
  
  /// Creates a `PaymentMethod` out of a `CreditCard` object for reuse with Stax Pay.
  /// - Parameter creditCard: Contains the `CreditCard` object to tokenize
  /// - Parameter completion: A `(PaymentMethod) -> Void` callback run after the bank has been tokenized.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when tokenizing.
  @available(*, deprecated, message: "Deprecated in favor of the `tokenizeCreditCard` function.")
  public func tokenize(_ creditCard: CreditCard, _ completion: @escaping (PaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    TokenizePaymentMethod(customerRepository: customerRepository,
                          paymentMethodRepository: paymentMethodRepository,
                          creditCard: creditCard
    ).start(completion: completion, failure: error)
  }
  
  /// Creates a `PaymentMethod` out of a `CreditCard` object for reuse with Stax Pay.
  /// - Parameter card: Contains the `CreditCard` object to tokenize
  /// - Parameter completion: A `(PaymentMethod) -> Void` callback run after the bank has been tokenized.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when tokenizing.
  @available(*, deprecated, message: "Deprecated in favor of the `tokenizeCreditCard` function.")
  public func tokenize(card: CreditCard, completion: @escaping (PaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    let job = TokenizePaymentMethod(
      customerRepository: customerRepository,
      paymentMethodRepository: paymentMethodRepository,
      creditCard: card
    )
    
    job.start(completion: completion, failure: error)
  }
  
  /// Creates a `StaxPaymentMethod` out of a `StaxCreditCard` object for reuse with Stax Pay.
  /// - Parameter card: Contains the `StaxCreditCard` object to tokenize
  /// - Parameter completion: A `(StaxPaymentMethod) -> Void` callback run after the bank has been tokenized.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when tokenizing.
  public func tokenizeCreditCard(_ card: StaxCreditCard, _ completion: @escaping (StaxPaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    guard let client = staxHttpClient else { return error(OmniGeneralException.uninitialized); }
    
    let job = TokenizeCreditCardJob(card: card, client: client)
    Task {
      let result = await job.start()
      switch result {
        case .success(let tokenized): completion(tokenized)
        case .failure(let fail): error(fail )
      }
    }
  }
  
  /// Captures a transaction without a mobile reader
  /// - Parameters:
  ///   - transactionRequest: A request for a Transaction
  ///   - completion: Called when the operation is completed successfully. Receives a Transaction
  ///   - error: Receives any errors that happened while attempting the operation
  public func pay(transactionRequest: TransactionRequest, completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    let job = TakePayment(request: transactionRequest, customerRepository: customerRepository, paymentMethodRepository: paymentMethodRepository)
    job.start(completion: completion, failure: error)
  }
  
  /// Uses the connected `MobileReader` to take a payment with the provided `TransactionRequest` object.
  /// - Parameter request: A `TransactionRequest` object containing the details needed to make a payment.
  /// - Parameter completion: A `(Transaction) -> Void` callback run after the payment has been processed.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when processing.
  /// - Note: `Omni` should be assigned a `SignatureProviding` object by the time this transaction is called.
  @available(*, deprecated, message: "Deprecated in favor of the `takeMobileReaderTransaction` function that returns `StaxTransaction`")
  public func takeMobileReaderTransaction(
    request: TransactionRequest,
    completion: @escaping (Transaction) -> Void,
    error: @escaping (OmniException) -> Void
  ) {
    guard isInitialized else { return error(OmniGeneralException.uninitialized); }

    let job: TakeMobileReaderPayment = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepository,
      invoiceRepository: invoiceRepository,
      customerRepository: customerRepository,
      paymentMethodRepository: paymentMethodRepository,
      transactionRepository: transactionRepository,
      request: request,
      signatureProvider: signatureProvider,
      transactionUpdateDelegate: transactionUpdateDelegate,
      userNotificationDelegate: userNotificationDelegate
    )
    
    job.start(completion: completion, failure: error)
  }
  
  /// Uses the connected `MobileReader` to take a payment with the provided `TransactionRequest` object.
  /// - Parameter request: A `TransactionRequest` object containing the details needed to make a payment.
  /// - Parameter completion: A `(StaxTransaction) -> Void` callback run after the payment has been processed.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when processing.
  /// - Note: `Omni` should be assigned a `SignatureProviding` object by the time this transaction is called.
  public func takeMobileReaderTransaction(
    with request: TransactionRequest,
    completion: @escaping (StaxTransaction) -> Void,
    error: @escaping (OmniException) -> Void
  ) {
    guard let client = staxHttpClient else { return error(OmniGeneralException.uninitialized); }
    
    let job = TakeMobileReaderPaymentJob(
      request: request,
      client: client,
      signatureProvider: signatureProvider,
      transactionUpdateDelegate: transactionUpdateDelegate,
      userNotificationDelegate: userNotificationDelegate
    )
    Task {
      let result = await job.start()
      switch result {
        case .success(let transaction): completion(transaction)
        case .failure(let fail): error(fail )
      }
    }
  }
  
  /// Captures a previously-authorized `Transaction`.
  /// - Parameter transactionId: The ID of the `Transaction` you want to capture.
  /// - Parameter amount: The `Amount` that you want to capture. If `nil`, then the total pre-authorized amount will be captured.
  /// - Parameter completion: A `(Transaction) -> Void` callback run after transaction has finished.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when capturing
  @available(*, deprecated, message: "Deprecated in favor of the `capturePreAuthTransaction` function.")
  public func capturePreauthTransaction(
    transactionId: String,
    amount: Amount? = nil,
    completion: @escaping (Transaction) -> Void,
    error: @escaping (OmniException) -> Void
  ) {
    let job = CapturePreauthTransaction(transactionId: transactionId, captureAmount: amount, omniApi: omniApi)
    job.start(completion: completion, error: error)
  }
  
  /// Captures a previously-authorized `StaxTransaction`.
  /// - Parameter transactionId: The ID of the `StaxTransaction` you want to capture.
  /// - Parameter amount: The `Amount` that you want to capture. If `nil`, then the total pre-authorized amount will be captured.
  /// - Parameter completion: A `(StaxTransaction) -> Void` callback run after transaction has finished.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when capturing
  public func capturePreAuthTransaction(
    transactionId: String,
    amount: Amount?,
    completion: @escaping (StaxTransaction) -> Void,
    error: @escaping (OmniException) -> Void
  ) {
    guard let client = staxHttpClient else { return error(OmniGeneralException.uninitialized); }
    
    let job = CapturePreAuthTransactionJob(transactionId: transactionId, client: client, amount: amount)
    Task {
      let result = await job.start()
      switch result {
        case .success(let transaction): completion(transaction)
        case .failure(let fail): error(fail )
      }
    }
  }
  
  /// Voids a `Transaction` via the Stax Pay API.
  /// - Parameter transactionId: The id of the `Transaction` you want to void.
  /// - Parameter completion: A `(Transaction) -> Void` callback run after transaction has finished.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when voiding.
  @available(*, deprecated, message: "Deprecated in favor of the `voidTransaction` function with `StaxTransaction` as return type.")
  public func voidTransaction(
    transactionId: String,
    completion: @escaping (Transaction) -> Void,
    error: @escaping (OmniException) -> Void
  ) {
    let job = VoidTransaction(transactionId: transactionId, omniApi: omniApi)
    job.start(completion: completion, error: error)
  }
  
  /// Voids a `StaxTransaction` via the Stax Pay API.
  /// - Parameter id: The id of the `StaxTransaction` you want to void.
  /// - Parameter completion: A `(StaxTransaction) -> Void` callback run after transaction has finished.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when voiding.
  public func voidTransaction(
    id: String,
    completion: @escaping (StaxTransaction) -> Void,
    error: @escaping (OmniException) -> Void
  ) {
    guard let client = staxHttpClient else { return error(OmniGeneralException.uninitialized); }
    
    let job = VoidTransactionJob(id: id, client: client)
    Task {
      let result = await job.start()
      switch result {
        case .success(let transaction): completion(transaction)
        case .failure(let fail): error(fail )
      }
    }
  }
  
  /// Attempts to cancel the current transaction taken using a `MobileReader`.
  /// - Parameter completion: A `(Bool) -> Void` callback run with the result of the cancelation.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when canceling.
  public func cancelMobileReaderTransaction(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    guard isInitialized else { return error(OmniGeneralException.uninitialized); }
    
    let job = CancelCurrentTransactionJob()
    Task {
      let result = await job.start()
      switch result {
        case .success(let result): self.preferredQueue.async { completion(result) }
        case .failure(let fail): self.preferredQueue.async { error(fail) }
      }
    }
  }
  
  /// Refunds the given transaction and returns a new Transaction that represents the refund in Omni
  /// - Parameters:
  ///   - transaction: the transaction to refund
  ///   - refundAmount: the amount of money to refund. When present, this **must** be greater than zero and lesser than or equal to the transaction total
  ///   - completion: Receives the Transaction that represents the refund in Omni
  ///   - error: Receives any errors that happened while attempting the operation
  public func refundMobileReaderTransaction(transaction: Transaction, refundAmount: Amount? = nil, completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    guard isInitialized else { return error(OmniGeneralException.uninitialized)}
    
    let job = RefundMobileReaderTransaction(mobileReaderDriverRepository: mobileReaderDriverRepository,
                                            transactionRepository: transactionRepository,
                                            transaction: transaction,
                                            refundAmount: refundAmount,
                                            omniApi: omniApi)
    job.start(completion: completion, failure: error)
  }
  
  /// Refunds the given transaction and returns a new Transaction that represents the refund in Omni
  /// - Parameters:
  ///   - transaction: the transaction to refund
  ///   - completion: Receives the Transaction that represents the refund in Omni
  ///   - error: Receives any errors that happened while attempting the operation
  public func refundMobileReaderTransaction(transaction: Transaction, completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    refundMobileReaderTransaction(transaction: transaction, refundAmount: nil, completion: completion, error: error)
  }
  
  /// Finds all available `MobileReader` devices that can be connected to.
  /// - Parameter completion: A `([MobileReader]) -> Void` callback run with the available `MobileReader` devices.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when searching.
  public func getAvailableReaders(completion: @escaping ([MobileReader]) -> Void, error: @escaping (OmniException) -> Void) {
    guard isInitialized else { return error(OmniGeneralException.uninitialized); }
    
    #if targetEnvironment(simulator)
    let args = MockSearchArgs()
    #else
    let args = ChipDnaSearchArgs(allowed: [.ble, .bt, .usb])
    #endif
    
    let job = SearchForMobileReadersJob(args: args)
    Task {
      let result = await job.start()
      switch result {
        case .success(let readers): self.preferredQueue.async { completion(readers) }
        case .failure(let fail): self.preferredQueue.async { error(fail) }
      }
    }
  }
  
  /// Returns the connected `MobileReader`.
  /// - Parameter completion: A `(MobileReader?) -> Void` callback containing the `MobileReader?` if connected; `nil` if not.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when getting the active device.
  public func getConnectedReader(completion: @escaping (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    guard isInitialized else { return error(OmniGeneralException.uninitialized); }
    
    let job = GetConnectedMobileReaderJob()
    Task {
      let result = await job.start()
      switch result {
        case .success(let reader): self.preferredQueue.async { completion(reader) }
        case .failure(let fail): self.preferredQueue.async { error(fail) }
      }
    }
  }
  
  /// Attempts to connect to the given MobileReader
  ///
  /// - Parameters:
  ///   - reader: The MobileReader to connect
  ///   - completion: A completion block to call once finished. It will receive the connected MobileReader
  ///   - error: A block to call if this operation fails
  @available(*, deprecated, message: "Please use the connect method that provides error handling")
  public func connect(reader: MobileReader, completion: @escaping (MobileReader) -> Void, error: @escaping () -> Void) {
    guard isInitialized else { return error(); }
    
    let job = ConnectToMobileReaderJob(reader: reader, connectionStatusDelegate: mobileReaderConnectionUpdateDelegate)
    Task {
      let result = await job.start()
      switch result {
        case .success(let reader): self.preferredQueue.async { completion(reader) }
        case .failure( _): self.preferredQueue.async { error() }
      }
    }
  }
  
  /// Attempts to connect to the provided `MobileReader`.
  /// - Parameter reader: The `MobileReader` to connect to.
  /// - Parameter completion: A `(MobileReader) -> Void` callback containing the `MobileReader` after connecting.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when connecting to the device.
  public func connect(reader: MobileReader, completion: @escaping (MobileReader) -> Void, error: @escaping (OmniException) -> Void) {
    guard isInitialized else { return error(OmniGeneralException.uninitialized); }
    
    let job = ConnectToMobileReaderJob(reader: reader, connectionStatusDelegate: mobileReaderConnectionUpdateDelegate)
    Task {
      let result = await job.start()
      switch result {
        case .success(let reader): self.preferredQueue.async { completion(reader) }
        case .failure(let fail): self.preferredQueue.async { error(fail) }
      }
    }
  }
  
  /// Attempts to disconnect from the provided `MobileReader`
  /// - Parameter reader: The `MobileReader` to disconnect from.
  /// - Parameter completion: A `(Bool) -> Void` callback containing the `Bool` result after disconnecting.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when disconnecting to the device.
  public func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    guard isInitialized else { return error(OmniGeneralException.uninitialized); }
    
    let job = DisconnectMobileReaderJob()
    Task {
      let result = await job.start()
      switch result {
        case .success(let result): self.preferredQueue.async { completion(result) }
        case .failure(let fail): self.preferredQueue.async { error(fail) }
      }
    }
  }
  
  /// Retrieves a list of the most recent mobile reader transactions from Omni
  /// - Parameters:
  ///   - completion: Receives a list of Transactions
  ///   - error: A block to call if this operation fails
  public func getMobileReaderTransactions(completion: @escaping ([Transaction]) -> Void, error: @escaping (OmniException) -> Void) {
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }
    
    transactionRepository.getList(completion: ({ paginatedTransactions in
      guard let transactions = paginatedTransactions.data else {
        error(OmniNetworkingException.couldNotGetPaginatedTransactions)
        return
      }
      completion(transactions)
    }), error: error)
  }
  
  internal func initializeUsbDelegate() {
    if let delegate = usbAccessoryDelegate {
      accessoryHelper = AccessoryHelper(delegate: delegate)
      let _ = AccessoryHelper.isIdTechConnected()
    }
  }
  
  internal func getStaxSelf(_ apiKey: String) async throws -> StaxSelf? {
    guard let client = staxHttpClient else { throw OmniGeneralException.uninitialized; }

    do {
      let request = StaxApiRequest<StaxSelf>(path: "/self", method: .get)
      return try await client.perform(request)
    } catch {
      return nil
    }
  }
  
  internal func getMobileReaderAuthDetails(_ apiKey: String) async throws -> MobileReaderDetails? {
    guard let client = staxHttpClient else { throw OmniGeneralException.uninitialized; }
    
    do {
      let path = "/team/gateway/hardware/mobile"
      let request = StaxApiRequest<MobileReaderDetails>(path: path, method: .get)
      return try await client.perform(request)
    } catch {
      return nil
    }
  }
    
// StaxInvoiceRepository
    
  /// Retrieves an Invoice by ID
  /// - Parameters:
  ///   - invoiceId: The ID of an exisiting invoice record
  ///   - completion: Receives the returned invoice
  ///   - error: A block to call if this operation fails
  public func getInvoice(invoiceID: String, completion: @escaping (StaxInvoice) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let invoice = try await staxInvoiceRepository.getInvoice(id: invoiceID)
              completion(invoice)
          } catch let err {
              error(err)
          }
      }
   }
    
  /// Creates and returns a net new Invoice
  /// - Parameters:
  ///   - request: A StaxInvoice object containing necessary values to create a new invoice record
  ///   - completion: Receives the newly created invoice
  ///   - error: A block to call if this operation fails
  public func createInvoice(request: StaxInvoice, completion: @escaping (StaxInvoice) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let invoice = try await staxInvoiceRepository.createInvoice(request)
              completion(invoice)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Updates an Invoice by ID
  /// - Parameters:
  ///   - id: The ID of an exisiting invoice record
  ///   - update: A StaxInvoice.Update object containing the values to update an exisiting invoice record
  ///   - completion: Receives the returned updated invoice
  ///   - error: A block to call if this operation fails
  public func updateInvoice(id: String, update: StaxInvoice.Update, completion: @escaping (StaxInvoice) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let updatedInvoice = try await staxInvoiceRepository.updateInvoice(id: id, update: update)
              completion(updatedInvoice)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Deletes an Invoice by ID
  /// - Parameters:
  ///   - id: The ID of an exisiting invoice record
  ///   - completion: Called upon success
  ///   - error: A block to call if this operation fails
  public func deleteInvoice(id: String, completion: @escaping () -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              try await staxInvoiceRepository.deleteInvoice(id: id)
              completion()
          } catch let err {
              error(err)
          }
      }
  }

// StaxCatalogRepository
    
  /// Retrieves an catalog item by ID
  /// - Parameters:
  ///   - id: The ID of an exisiting catalog item
  ///   - completion: Receives the returned Catalog Item
  ///   - error: A block to call if this operation fails
  public func getCatalogItem(id: String, completion: @escaping (StaxCatalogItem) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let item = try await staxCatalogRepository.getCatalogItem(id: id)
              completion(item)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Retrieves all Catalog Codes for the merchant
  /// - Parameters:
  ///   - completion: Receives the returned catalog codes
  ///   - error: A block to call if this operation fails
  public func getCatalogCodes(completion: @escaping ([String?]) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let codes = try await staxCatalogRepository.getCatalogCodes()
              completion(codes)
          } catch let err {
              error(err)
          }
      }
  }

  /// Creates a net new catalog item
  /// - Parameters:
  ///   - item: A StaxCatalogItem object containing necessary values to create a new catalog item
  ///   - completion: Receives the newly created Catalog Item
  ///   - error: A block to call if this operation fails
  public func createCatalogItem(item: StaxCatalogItem, completion: @escaping (StaxCatalogItem) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let createdItem = try await staxCatalogRepository.createCatalogItem(item: item)
              completion(createdItem)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Updates an existing catalog item by ID
  /// - Parameters:
  ///   - id: the ID of an exisiting Catalog Item
  ///   - update: A StaxCatalogItem.Update object containing the values to update an exisiting catalog item record
  ///   - completion: Receives the updated Catalog Item
  ///   - error: A block to call if this operation fails
  public func updateCatalogItem(id: String, update: StaxCatalogItem.Update, completion: @escaping (StaxCatalogItem) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let updatedItem = try await staxCatalogRepository.updateCatalogItem(id: id, update: update)
              completion(updatedItem)
          } catch let err {
              error(err)
          }
      }
  }

  /// Increments the quantity of an existing catalog item by ID
  /// - Parameters:
  ///   - id: The ID of the exisiting Catalog Item to increment
  ///   - amount: An integer amount that will be used to update the catalog item quantity
  ///   - completion: Receives the updated Catalog Item
  ///   - error: A block to call if this operation fails
  public func incrementCatalogItemStock(id: String, amount: Int, completion: @escaping (StaxCatalogItem) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let updatedItem = try await staxCatalogRepository.incrementCatalogItemStock(id: id, amount: amount)
              completion(updatedItem)
          } catch let err {
              error(err)
          }
      }
  }

  /// Decrements the quantity of an existing catalog item by ID
  /// - Parameters:
  ///   - id: The ID of the exisiting Catalog Item to decrement
  ///   - amount: An integer amount that will be used to update the catalog item quantity
  ///   - completion: Receives the updated Catalog Item
  ///   - error: A block to call if this operation fails
  public func decrementCatalogItemStock(id: String, amount: Int, completion: @escaping (StaxCatalogItem) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let updatedItem = try await staxCatalogRepository.decrementCatalogItemStock(id: id, amount: amount)
              completion(updatedItem)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Deletes an existing catalog item by ID
  /// - Parameters:
  ///   - id: The ID of the exisiting Catalog Item to delete
  ///   - completion:Called upon success
  ///   - error: A block to call if this operation fails
  public func deleteCatalogItem(id: String, completion: @escaping () -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              try await staxCatalogRepository.deleteCatalogItem(id: id)
              completion()
          } catch let err {
              error(err)
          }
      }
  }
    
// StaxCustomerRepository
    
  /// Retrieves a customer record by ID
  /// - Parameters:
  ///   - id: the ID of the customer record to retrieve
  ///   - completion: Receives the returned customer record
  ///   - error: A block to call if this operation fails
  public func getCustomer(id: String, completion: @escaping (StaxCustomer) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let customer = try await staxCustomerRepository.getCustomer(id: id)
              completion(customer)
          } catch let err {
              error(err)
          }
      }
  }

  /// Retrieves all payment methods for a customer by ID
  /// - Parameters:
  ///   - id: The customer ID to retrieve payment methods for
  ///   - completion: Receives the returned payment method records
  ///   - error: A block to call if this operation fails
  public func getPaymentMethodsForCustomer(id: String, completion: @escaping ([StaxPaymentMethod]) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let paymentMethods = try await staxCustomerRepository.getPaymentMethodsForCustomer(id: id)
              completion(paymentMethods)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Creates a net new customer record
  /// - Parameters:
  ///   - request: A StaxCustomer object containing necessary values to create a new customer record
  ///   - completion: Receives the newly created customer record
  ///   - error: A block to call if this operation fails
  public func createCustomer(request: StaxCustomer, completion: @escaping (StaxCustomer) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let createdCustomer = try await staxCustomerRepository.createCustomer(request)
              completion(createdCustomer)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Updates an existing customer record by ID
  /// - Parameters:
  ///   - id: The ID of the customer record to update
  ///   - update: A StaxCustomer.Update object containing the values to update an exisiting customer record
  ///   - completion: Receives the updated customer record
  ///   - error: A block to call if this operation fails
  public func updateCustomer(id: String, update: StaxCustomer.Update, completion: @escaping (StaxCustomer) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let updatedCustomer = try await staxCustomerRepository.updateCustomer(id: id, update: update)
              completion(updatedCustomer)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Deletes an existing customer record by ID
  /// - Parameters:
  ///   - id: The ID of the exisiting customer to delete
  ///   - completion:Called upon success
  ///   - error: A block to call if this operation fails
  public func deleteCustomer(id: String, completion: @escaping () -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              try await staxCustomerRepository.deleteCustomer(id: id)
              completion()
          } catch let err {
              error(err)
          }
      }
  }
    
// StaxPaymentMethodRepository
    
  /// Retrieves a payment method record by ID
  /// - Parameters:
  ///   - id: the ID of the payment method record to retrieve
  ///   - completion: Receives the returned payment method record
  ///   - error: A block to call if this operation fails
  public func getPaymentMethod(id: String, completion: @escaping (StaxPaymentMethod) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let paymentMethod = try await staxPaymentMethodRepository.getPaymentMethod(id: id)
              completion(paymentMethod)
          } catch let err {
              error(err)
          }
      }
  }
    
  ///  Calculates surcharge amount for a particular payment method and payment total
  /// - Parameters:
  ///   - id: The ID of the payment method to use to calculate surcharge amount
  ///   - dollars: The dollar amount to calculate surcharge for
  ///   - completion: Receives the returned surcharge amount
  ///   - error: A block to call if this operation fails
  public func getPaymentMethodSurcharge(id: String, dollars: Double, completion: @escaping (StaxSurchargeReview) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let surcharge = try await staxPaymentMethodRepository.getPaymentMethodSurcharge(id: id, dollars: dollars)
              completion(surcharge)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Updates an exisiting payment method by ID
  /// - Parameters:
  ///   - id: the ID of the payment method record to update
  ///   - update: A StaxPaymentMethod.Update object containing the values to update an exisiting payment method record
  ///   - completion: Receives the returned payment method record
  ///   - error: A block to call if this operation fails
  public func updatePaymentMethod(id: String, update: StaxPaymentMethod.Update, completion: @escaping (StaxPaymentMethod) -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              let updatedPaymentMethod = try await staxPaymentMethodRepository.updatePaymentMethod(id: id, update: update)
              completion(updatedPaymentMethod)
          } catch let err {
              error(err)
          }
      }
  }
    
  /// Deletes an existing payment method record by ID
  /// - Parameters:
  ///   - id: The ID of the exisiting payment method to delete
  ///   - completion:Called upon success
  public func deletePaymentMethod(id: String, completion: @escaping () -> Void, error: @escaping (Error) -> Void) {
      Task {
          do {
              try await staxPaymentMethodRepository.deletePaymentMethod(id: id)
              completion()
          } catch let err {
              error(err)
          }
      }
  }
}
