//
//  Omni.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum OmniNetworkingException: OmniException {
  case couldNotGetMerchantDetails
  case couldNotGetPaginatedTransactions
  
  static var mess: String = "Omni Networking Exception"
  
  var detail: String? {
    switch self {
      case .couldNotGetMerchantDetails:
        return "Could not get merchant details from Omni"
        
      case .couldNotGetPaginatedTransactions:
        return "Could not get paginated transactions"
    }
  }
}

public enum OmniInitializeException: OmniException {
  case missingInitializationDetails
  case mobileReaderPaymentsNotConfigured
  case missingMobileReaderCredentials
  case invalidMobileReaderCredentials

  public static var mess: String = "Omni Initialization Exception"

  public var detail: String? {
    switch self {
      case .missingInitializationDetails:
        return "Missing initialization details"
      case .mobileReaderPaymentsNotConfigured:
        return "Your account is not configured to accept mobile reader payments"
      case .missingMobileReaderCredentials:
        return "Your account does not have mobile reader credentials"
      case .invalidMobileReaderCredentials:
        return "Your account has invalid mobile reader credentials"
    }
  }
}

public enum OmniGeneralException: OmniException {
  case uninitialized
  public static var mess: String = "Omni General Error"
  
  public var detail: String? {
    switch self {
      case .uninitialized:
        return "Omni has not been initialized yet"
    }
  }
}

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

  fileprivate func initNewRepos(httpClient: StaxHttpClient) {
    // Initialize the new repositories
    staxInvoiceRepository = StaxInvoiceRepositoryImpl(httpClient: httpClient)
    staxCatalogRepository = StaxCatalogRepositoryImpl(httpClient: httpClient)
    staxCustomerRepository = StaxCustomerRepositoryImpl(httpClient: httpClient)
    staxPaymentMethodRepository = StaxPaymentMethodRepositoryImpl(httpClient: httpClient)
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
  public func initialize(params: InitParams, completion: @escaping () -> Void, error: @escaping (OmniException) -> Void) {
    guard let appId = params.appId, params.apiKey != nil else {
      error(OmniInitializeException.missingInitializationDetails)
      return
    }
    
    // Setup USB Delegate
    if let delegate = usbAccessoryDelegate {
      accessoryHelper = AccessoryHelper(delegate: delegate)
      AccessoryHelper.isIdTechConnected()
    }
    
    omniApi.apiKey = params.apiKey
    omniApi.environment = params.environment
    let baseUrl = URL(string: "https://apiprod.fattlabs.com")!
    let httpClient = StaxHttpClient(baseURL: baseUrl, apiKey: params.apiKey!)
    initRepos(omniApi: omniApi)
    initNewRepos(httpClient: httpClient)
    
    // Verify the apikey corresponds to a real merchant
    omniApi.getSelf(completion: { myself in
      
      guard let merchant = myself.merchant else {
        error(OmniNetworkingException.couldNotGetMerchantDetails)
        return
      }
      
      // Assign merchant to self
      self.merchant = merchant
      
      // A dict that contains the initialization details for the drivers
      var args: [String: Any] = [ "appId": appId ]
      
      // Eagerly try to fill out the mobile reader settings from the merchant options.
      // The getMobileReaderSettings step will override these if successful, but if that step fails to grab the
      // settings from the gateways, then at least we have these as a fallback.
      // NMI
      if let emvPassword = merchant.emvPassword() {
        args["nmi"] = NMIDetails(securityKey: emvPassword)
      }
      
      // Try to get the details from the merchant gateways. This *should* rewrite the args dict
      self.omniApi.getMobileReaderSettings(completion: { mrDetails in
        if let nmiDetails = mrDetails.nmi {
          args.updateValue(nmiDetails, forKey: "nmi")
        }

        // Check if there are creds for NMI
        if args["nmi"] == nil {
          self.preferredQueue.async {
            completion()
          }
          return
        }

        //
        if (args["nmi"] as? NMIDetails)?.securityKey.isBlank() ?? true {
          self.preferredQueue.async {
            completion()
          }
          return
        }

        InitializeDrivers(mobileReaderDriverRepository: self.mobileReaderDriverRepository, args: args).start(completion: { _ in
          self.mobileReaderDriversInitialized = true
          self.preferredQueue.async(execute: completion)
        }, failure: { _ in
          self.mobileReaderDriversInitialized = true
          error(OmniInitializeException.invalidMobileReaderCredentials)
        })
      }, failure: { _ in

        // If the call to merchant gateways fails, try to init with the merchant options anyways
        if args["nmi"] == nil {
          self.preferredQueue.async {
            self.mobileReaderDriversInitialized = true
            error(OmniInitializeException.missingMobileReaderCredentials)
          }
          return
        }
        InitializeDrivers(mobileReaderDriverRepository: self.mobileReaderDriverRepository, args: args).start(completion: { _ in
          self.mobileReaderDriversInitialized = true
          self.preferredQueue.async(execute: completion)
        }, failure: { _ in
          self.mobileReaderDriversInitialized = true
          error(OmniInitializeException.invalidMobileReaderCredentials)
        })
      })
    }, failure: error)
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
  public func capturePreauthTransaction(transactionId: String,
                                        amount: Amount?,
                                        completion: @escaping (Transaction) -> Void,
                                        error: @escaping (OmniException) -> Void) {
    
    let job = CapturePreauthTransaction(transactionId: transactionId, captureAmount: amount, omniApi: omniApi)
    job.start(completion: completion, error: error)
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
    
    let job = CancelCurrentTransaction(mobileReaderDriverRepository: mobileReaderDriverRepository)
    job.start(completion: { success in
      self.preferredQueue.async {
        completion(success)
      }
    }, error: { err in
      self.preferredQueue.async {
        error(err)
      }
    })
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
    
    SearchForReaders(mobileReaderDriverRepository: mobileReaderDriverRepository, args: [:]).start(completion: { (readers) in
      self.preferredQueue.async { completion(readers) }
    }, failure: ({ exception in
      self.preferredQueue.async { error(exception) }
    }))
  }
  
  /// Returns the connected mobile reader
  /// - Parameters:
  ///   - completion: Receives the connected mobile reader, if any
  ///   - error: Receives any errors that happened while attempting the operation
  public func getConnectedReader(completion: @escaping (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    guard mobileReaderDriversInitialized else {
      return error(OmniGeneralException.uninitialized)
    }
    
    GetConnectedMobileReader(mobileReaderDriverRepository: mobileReaderDriverRepository).start(completion: { reader in
      self.preferredQueue.async { completion(reader) }
    }, failure: ({ exception in
      self.preferredQueue.async { error(exception) }
    }))
  }
  
  /// Attempts to connect to the given MobileReader
  ///
  /// - Parameters:
  ///   - reader: The MobileReader to connect
  ///   - completion: A completion block to call once finished. It will receive the connected MobileReader
  ///   - error: A block to call if this operation fails
  @available (*, deprecated, message: "Please use the connect method that provides error handling")
  public func connect(reader: MobileReader, completion: @escaping (MobileReader) -> Void, error: @escaping () -> Void) {
    guard mobileReaderDriversInitialized else {
      return error()
    }
    
    let task = ConnectMobileReader(mobileReaderDriverRepository: mobileReaderDriverRepository,
                                   mobileReader: reader,
                                   mobileReaderConnectionStatusDelegate: mobileReaderConnectionUpdateDelegate)
    task.start(onConnected: { connectedReader in
      completion(connectedReader)
    }, onFailed: { _ in
      error()
    })
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
    
    let task = ConnectMobileReader(mobileReaderDriverRepository: mobileReaderDriverRepository,
                                   mobileReader: reader,
                                   mobileReaderConnectionStatusDelegate: mobileReaderConnectionUpdateDelegate)
    task.start(onConnected: { connectedReader in
      self.preferredQueue.async {
        completion(connectedReader)
      }
    }, onFailed: { exception in
      self.preferredQueue.async {
        error(exception)
      }
    })
    
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
    
    let task = DisconnectMobileReader(mobileReaderDriverRepository: mobileReaderDriverRepository, mobileReader: reader)
    task.start(completion: { success in
      self.preferredQueue.async { completion(success) }
    }, failure: ({ exception in
      self.preferredQueue.async { error(exception) }
    }))
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
