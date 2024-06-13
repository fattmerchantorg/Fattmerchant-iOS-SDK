import Foundation

public class Stax {
  private var merchant: Merchant
  
  /// Responsible for providing signatures for transactions, when required
  public var signatureProvider: SignatureProviding?
  
  /// Receives notifications about transaction events such as when a card is swiped
  public weak var transactionUpdateDelegate: TransactionUpdateDelegate?
  
  /// Receives notifications about user-facing transaction events such as when a user swipes a chip card
  public weak var userNotificationDelegate: UserNotificationDelegate?
  
  /// Receives notifications about reader connection events
  public weak var cardReaderConnectionStatusDelegate: CardReaderConnectionStatusDelegate?
  
  
  /// Initializes the Stax SDK with the Gateway and returns a `Stax` instance that you should use for all future options
  /// - Parameters:
  ///   - appId: A unique identifying `appId` used to identify your merchant''s transactions on the Stax backend
  ///   - apiKey: Your Stax API Key. Available to view within the Stax web portal.
  /// - Throws:
  ///   - `StaxNetworkingException.couldNotGetMerchantDetails` If the `/self` endpoint is not reachable.
  ///   - `StaxInitializeException.missingMobileReaderCredentials` If the mobile reader credentials are not set in Connect Lite.
  public init(appId: String, apiKey: String, environment: Environment = .LIVE) async throws {
    // Initialize the StaxHttpService
    var networking: NetworkService = ConcurrentNetworkService(apiKey)
    if #unavailable(iOS 15) {
      networking = CallbackNetworkService(apiKey)
    }
    
    let http: StaxHttpService = StaxHttpServiceImpl(networking)
    Services.register(StaxHttpService.self, service: http)
    
    // Verify that you can get the merchant /self
    let myself = try await http.getSelf()
    guard let merchant = myself.merchant else {
      throw StaxNetworkingException.couldNotGetMerchantDetails
    }
    self.merchant = merchant
    
    // Get values from Connect Lite for Initializing the CardReaderService with NMI
    var initArgs: [String: Any] = ["appId": appId]
    do {
      let response = try await http.getCardReaderSettings()
      if let settings = response.nmi {
        initArgs.updateValue(settings, forKey: "nmi")
      }
    } catch {
      // Fallback on default credentials
      if let password = merchant.emvPassword() {
        initArgs["nmi"] = NMIDetails(securityKey: password)
      }
    }
    
    // If no creds, do an early return
    let nmiArgs = initArgs["nmi"]
    if nmiArgs == nil || ((nmiArgs as? NMIDetails)?.securityKey.isBlank() ?? true) {
      throw StaxInitializeException.missingMobileReaderCredentials
    }

    // We still here? Good. Initialize the CardReaderService
    let cardReaderService: CardReaderService = NmiCardReaderService()
    try cardReaderService.initialize(initArgs)
    
    Services.register(CardReaderService.self, service: cardReaderService)
  }

  /// Tokenizes a `CreditCard` object to be used in the Stax platform. This does not charge the card, but stores it for later use
  /// - Parameters:
  ///   - card: The `CreditCard` object to create in Stax.
  /// - Returns: The Stax `PaymentMethod` object with the assosciated Stax ID.
  /// - Throws:
  ///   - `StaxNetworkingException.serviceNotInitialized` if the HTTP service is not initialized.
  ///   - `URLError.badURL` if the API endpoint  is down or unable to process your request.
  ///   - `URLError.badServerResponse` If the server responds with malformed data.
  ///   - `StaxNetworkingException.couldNotParseResponse` If the server responds with a 422 or 5XX response.
  ///   - `StaxNetworkingException.unknown` If the error could not be determined.
  public func tokenize(card: CreditCard) async throws -> PaymentMethod {
    let job = TokenizeCreditCardJob(card: card)
    return try await job.start()
  }
  
  /// Tokenizes a `BankAccount` object to be used in the Stax platform. This does not charge the card, but stores it for later use
  /// - Parameters:
  ///   - bank: The `BankAccount` object to create in Stax.
  /// - Returns: The Stax `PaymentMethod` object with the assosciated Stax ID.
  /// - Throws:
  ///   - `StaxNetworkingException.serviceNotInitialized` if the HTTP service is not initialized.
  ///   - `URLError.badURL` if the API endpoint  is down or unable to process your request.
  ///   - `URLError.badServerResponse` If the server responds with malformed data.
  ///   - `StaxNetworkingException.couldNotParseResponse` If the server responds with a 422 or 5XX response.
  ///   - `StaxNetworkingException.unknown` If the error could not be determined.
  public func tokenize(bank: BankAccount) async throws -> PaymentMethod {
    let job = TokenizeBankAccountJob(bank: bank)
    return try await job.start()
  }
  
  /// Prompts the card reader using `TransactionRequest` to perform a transaction.
  /// - Parameters:
  ///   - request: The `TransactionRequest` object to prompt the reader with
  /// - Returns: The created Stax `Transaction` record.
  public func chargeWithReader(request: TransactionRequest) async throws -> Transaction {
    guard
      let card = Services.resolve(CardReaderService.self),
      try await card.getConnectedReader() != nil
    else {
      throw TakeMobileReaderPaymentException.mobileReaderNotFound
    }
    
    let job = PerformTransactionWithCardReaderJob(request: request)
    job.signatureProvider = self.signatureProvider
    job.transactionUpdateDelegate = self.transactionUpdateDelegate
    return try await job.start()
  }
  
  /// Attempts to perform a refund on a `Transaction` for a given `Amount`.
  /// - Parameters:
  ///   - transaction: The Stax `Transaction` record to attempt a refund on.
  ///   - amount: An optional `Amount` to refund. If this is not passed in, the refund will be for the `transaction.total` value.
  /// - Returns: A Stax `Transaction` record that has been successfully refunded.
  public func cancelCharge() async throws -> Bool {
    guard
      let card = Services.resolve(CardReaderService.self),
      try await card.getConnectedReader() != nil
    else {
      throw TakeMobileReaderPaymentException.mobileReaderNotFound
    }
    
    let job = CancelCurrentCardReaderTransactionJob()
    return try await job.start()
  }

  public func charge() {}
  public func preAuthorizeTransaction() {}
  public func captureTransaction() {}
  public func voidTransaction() {}
  
  /// Attempts to perform a refund on a `Transaction` for a given `Amount`.
  /// - Parameters:
  ///   - transaction: The Stax `Transaction` record to attempt a refund on.
  ///   - amount: An optional `Amount` to refund. If this is not passed in, the refund will be for the `transaction.total` value.
  /// - Returns: A Stax `Transaction` record that has been successfully refunded.
  public func refundTransaction(transaction: Transaction, amount: Amount? = nil) async throws -> Transaction {
    let job = try RefundCardReaderTransactionJob(transaction: transaction, amount: amount)
    return try await job.start()
  }

  /// Searches for all `CardReader` objects ready to be connected to. None of the `CardReader`s will be able to take a payment until they are connected.
  /// - Returns: A `[CardReader]` with all of the `CardReader` objects available to connect to.
  /// - Throws:
  ///   - `StaxInitializeException.mobileReaderPaymentsNotConfigured` If the merchant account is not configured to take mobile reader payments
  public func getAvailableCardReaders() async throws -> [CardReader] {
    let job = SearchForCardReadersJob()
    return try await job.start()
  }

  /// Connects to an individual `CardReader` to allow it to take payments
  /// - Parameters:
  ///   - reader: The `CardReader` to attempt a connection to
  /// - Returns: A new `CardReader` object that contains the additional card reader information. May return `nil` if the connection failed.
  /// - Throws:
  ///   - `StaxInitializeException.mobileReaderPaymentsNotConfigured` If the merchant account is not configured to take mobile reader payments
  public func connectToCardReader(reader: CardReader) async throws -> CardReader? {
    let job = ConnectToCardReaderJob(reader)
    job.cardReaderConnectionStatusDelegate = cardReaderConnectionStatusDelegate
    return try await job.start()
  }
  
  /// Gets the actively connected `CardReader`
  /// - Parameters:
  ///   - reader: The `CardReader` that is actively connected
  /// - Returns: The current `CardReader` object that contains the additional card reader information. May return `nil` if nothing is connected.
  /// - Throws:
  ///   - `StaxInitializeException.mobileReaderPaymentsNotConfigured` If the merchant account is not configured to take mobile reader payments
  public func getActiveCardReader() async throws -> CardReader? {
    let job = GetConnectedCardReaderJob()
    return try await job.start()
  }

  /// Disconnects the `CardReader` object and re-initializes the Hardware SDK.
  /// - Returns: `True` if the `CardReader` is disconnected and the Hardware SDK is ready to re-connect to other hardware.
  /// - Throws:
  ///   - `StaxInitializeException.missingInitializationDetails` If the Hardware SDK was never initially initialized.
  public func disconnectFromCardReader() async throws -> Bool {
    let job = DisconnectCardReaderJob()
    return try await job.start()
  }

  public func getRecentTransactions() {}
}
