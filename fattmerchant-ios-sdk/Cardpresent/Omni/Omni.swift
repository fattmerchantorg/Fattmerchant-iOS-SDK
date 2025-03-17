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
  
  internal var mobileReaderDriversInitialized: Bool = false
  internal var omniApi = OmniApi()
  internal var transactionRepository: TransactionRepository!
  internal var invoiceRepository: InvoiceRepository!
  internal var customerRepository: CustomerRepository!
  internal var paymentMethodRepository: PaymentMethodRepository!
  internal var mobileReaderDriverRepository = MobileReaderDriverRepository()
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
  
  /// Contains all the data necessary to initialize `Omni`
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
  
  fileprivate func initRepos(omniApi: OmniApi) {
    transactionRepository = TransactionRepository(omniApi: omniApi)
    invoiceRepository = InvoiceRepository(omniApi: omniApi)
    customerRepository = CustomerRepository(omniApi: omniApi)
    paymentMethodRepository = PaymentMethodRepository(omniApi: omniApi)
  }
  
  /// True when Omni is initialized. False otherwise
  public var isInitialized: Bool {
    return mobileReaderDriversInitialized
  }
  
  /// Used to initialize the Omni object
  /// - Parameters:
  ///   - params: an instance of InitParams which contains all necessary information to initialize omni
  ///   - completion: a completion block to run once finished
  ///   - error: an error block to run in case something goes wrong
  public func initialize(params: InitParams, completion: @Sendable @escaping () -> Void, error: @escaping (OmniException) -> Void) {
    guard let appId = params.appId, let apiKey = params.apiKey else {
      error(OmniInitializeException.missingInitializationDetails)
      return
    }
    
    omniApi.apiKey = params.apiKey
    omniApi.environment = params.environment
    initRepos(omniApi: omniApi)
    
    initializeUsbDelegate()
    
    Task {
      // Get "/self" and "/team/gateway/hardware/mobile" settings from API
      let auth = await getStaxSelf(apiKey)
      guard let merchant = auth?.merchant else {
        error(OmniNetworkingException.couldNotGetMerchantDetails)
        return
      }
      
      let readerAuth = await getMobileReaderAuthDetails(apiKey)
      guard let readerAuth = readerAuth, let nmiKeys = readerAuth.nmi else {
        error(OmniInitializeException.missingMobileReaderCredentials)
        return
      }
      
      // Set the InitArgs based on environment type
      #if targetEnvironment(simulator)
      let initArgs = MockInitializationArgs(appId: params.appId)
      #else
      let initArgs = ChipDnaInitializationArgs(appId: appId, keys: nmiKeys)
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
  
  /// Creates a PaymentMethod out of a CreditCard object for reuse with Omni
  /// - Parameters:
  ///   - creditCard: Contains the details of the payment method to tokenize
  ///   - completion: Called when the operation is completed successfully. Receives a PaymentMethod
  ///   - error: Receives any errors that happened while attempting the operation
  public func tokenize(_ bankAccount: BankAccount, _ completion: @escaping (PaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    TokenizePaymentMethod(customerRepository: customerRepository,
                          paymentMethodRepository: paymentMethodRepository,
                          bankAccount: bankAccount
    ).start(completion: completion, failure: error)
  }
  
  /// Creates a PaymentMethod out of a CreditCard object for reuse with Omni
  /// - Parameters:
  ///   - creditCard: Contains the details of the payment method to tokenize
  ///   - completion: Called when the operation is completed successfully. Receives a PaymentMethod
  ///   - error: Receives any errors that happened while attempting the operation
  public func tokenize(_ creditCard: CreditCard, _ completion: @escaping (PaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    TokenizePaymentMethod(customerRepository: customerRepository,
                          paymentMethodRepository: paymentMethodRepository,
                          creditCard: creditCard
    ).start(completion: completion, failure: error)
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
  
  /// Creates a Fattmerchant PaymentMethod out of the given CreditCard
  ///
  /// - Parameters:
  ///   - card: The CreditCard to be tokenized
  ///   - completion: Called when the operation is completed successfully. Receives a PaymentMethod
  ///   - error: Receives any errors that happened while attempting the operation
  public func tokenize(card: CreditCard, completion: @escaping (PaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    let job = TokenizePaymentMethod(
      customerRepository: customerRepository,
      paymentMethodRepository: paymentMethodRepository,
      creditCard: card
    )
    
    job.start(completion: completion, failure: error)
  }
  
  /// Creates a Fattmerchant PaymentMethod out of the given BankAccount
  ///
  /// - Parameters:
  ///   - bankAccount: The BankAccount to be tokenized
  ///   - completion: Called when the operation is completed successfully. Receives a PaymentMethod
  ///   - error: Receives any errors that happened while attempting the operation
  public func tokenize(bankAccount: BankAccount, completion: @escaping (PaymentMethod) -> Void, error: @escaping (OmniException) -> Void) {
    let job = TokenizePaymentMethod(
      customerRepository: customerRepository,
      paymentMethodRepository: paymentMethodRepository,
      bankAccount: bankAccount
    )
    
    job.start(completion: completion, failure: error)
  }
  
  /// Captures a mobile reader transaction
  ///
  /// - Note: `Omni` should be assigned a `SignatureProviding` object by the time this transaction is called. This
  /// object is responsible for providing a signature, in case one is required to complete the transaction
  ///
  /// - Parameters:
  ///   - request: A request for a Transaction
  ///   - completion: Called when the operation is complete successfully. Receives a Transaction
  ///   - error: Receives any errors that happened while attempting the operation
  public func takeMobileReaderTransaction(request: TransactionRequest, completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }

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
  
  /// Captures a previously-authorized transaction
  ///
  /// - Parameters:
  ///   - transactionId: The id of the transaction you want to capture
  ///   - amount: the amount that you want to capture. If nil, then the original transaction amount will be captured
  ///   - completion: Called when the operation is complete successfully. Receives a Transaction
  ///   - error: Receives any errors that happened while attempting the operation
  public func capturePreauthTransaction(
    transactionId: String,
    amount: Amount?,
    completion: @escaping (StaxTransaction) -> Void,
    error: @escaping (OmniException) -> Void
  ) {
    guard let apiKey = omniApi.apiKey else {
      error(OmniInitializeException.missingInitializationDetails)
      return
    }
    
    let job = CapturePreauthTransactionJob(transactionId: transactionId, token: apiKey,amount: amount)
    Task {
      let result = await job.start()
      switch result {
        case .success(let transaction): completion(transaction)
        case .failure(let fail): error(fail )
      }
    }
  }
  
  /// Voids a transaction
  ///
  /// - Parameters:
  ///   - transactionId: The id of the transaction you want to void
  ///   - completion: Called when the operation is complete successfully. Receives a Transaction
  ///   - error: Receives any errors that happened while attempting the operation
  public func voidTransaction(transactionId: String,
                              completion: @escaping (Transaction) -> Void,
                              error: @escaping (OmniException) -> Void) {
    let job = VoidTransaction(transactionId: transactionId, omniApi: omniApi)
    job.start(completion: completion, error: error)
  }
  
  /// Cancels the current mobile reader transaction
  ///
  /// - Parameters:
  ///   - completion: called when the operation is complete. Receives true when the transaction was cancelled. False
  ///   otherwise
  ///   - error: receives any errors that happened while attempting the operation
  public func cancelMobileReaderTransaction(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }
    
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
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }
    
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
  
  /// Finds the available readers that Omni can connect to
  /// - Parameters:
  ///   - completion: Receives an array of MobileReaders that are available
  ///   - error: Receives any errors that happened while attempting the operation
  public func getAvailableReaders(completion: @escaping ([MobileReader]) -> Void, error: @escaping (OmniException) -> Void) {
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }
    
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
  
  /// Returns the connected mobile reader
  /// - Parameters:
  ///   - completion: Receives the connected mobile reader, if any
  ///   - error: Receives any errors that happened while attempting the operation
  public func getConnectedReader(completion: @escaping (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }
    
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
    guard mobileReaderDriversInitialized else {
      return error()
    }
    
    let job = ConnectToMobileReaderJob(reader: reader, connectionStatusDelegate: mobileReaderConnectionUpdateDelegate)
    Task {
      let result = await job.start()
      switch result {
        case .success(let reader): self.preferredQueue.async { completion(reader) }
        case .failure( _): self.preferredQueue.async { error() }
      }
    }
  }
  
  /// Attempts to connect to the given MobileReader
  ///
  /// - Parameters:
  ///   - reader: The MobileReader to connect
  ///   - completion: A completion block to call once finished. It will receive the connected MobileReader
  ///   - error: A block to call if this operation fails. Receives an OmniException
  public func connect(reader: MobileReader, completion: @escaping (MobileReader) -> Void, error: @escaping (OmniException) -> Void) {
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }
    
    let job = ConnectToMobileReaderJob(reader: reader, connectionStatusDelegate: mobileReaderConnectionUpdateDelegate)
    Task {
      let result = await job.start()
      switch result {
        case .success(let reader): self.preferredQueue.async { completion(reader) }
        case .failure(let fail): self.preferredQueue.async { error(fail) }
      }
    }
  }
  
  /// Attempts to disconnect the given MobileReader
  ///
  /// - Parameters:
  ///   - reader: The MobileReader to disconnect
  ///   - completion: A completion block to call once finished. It will receive the connected MobileReader
  ///   - error: A block to call if this operation fails
  public func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }
    
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
  
  fileprivate func initializeUsbDelegate() {
    if let delegate = usbAccessoryDelegate {
      accessoryHelper = AccessoryHelper(delegate: delegate)
      let _ = AccessoryHelper.isIdTechConnected()
    }
  }
  
  fileprivate func getStaxSelf(_ apiKey: String) async -> StaxSelf? {
    let url = URL(string: "https://apiprod.fattlabs.com")!
    let client = StaxHttpClient(baseURL: url, apiKey: apiKey)
    let request = StaxApiRequest<StaxSelf>(
      path: "/self",
      method: .get
    )

    do {
      let response = try await client.perform(request)
      return response
    } catch {
      return nil
    }
  }
  
  fileprivate func getMobileReaderAuthDetails(_ apiKey: String) async -> MobileReaderDetails? {
    let url = URL(string: "https://apiprod.fattlabs.com")!
    let client = StaxHttpClient(baseURL: url, apiKey: apiKey)
    let request = StaxApiRequest<MobileReaderDetails>(
      path: "/team/gateway/hardware/mobile",
      method: .get
    )
    
    do {
      let response = try await client.perform(request)
      return response
    } catch {
      return nil
    }
  }
}
