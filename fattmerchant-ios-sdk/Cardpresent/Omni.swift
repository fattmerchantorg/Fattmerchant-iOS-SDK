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

enum OmniInitializeException: OmniException {
  case missingInitializationDetails

  static var mess: String = "Omni Initialization Exception"

  var detail: String? {
    switch self {
    case .missingInitializationDetails:
      return "Missing initialization details"
    }
  }
}

enum OmniGeneralException: OmniException {
  case uninitialized

  static var mess: String = "Omni General Error"

  var detail: String? {
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

  internal var initialized: Bool = false
  internal var omniApi = OmniApi()
  internal var transactionRepository: TransactionRepository!
  internal var invoiceRepository: InvoiceRepository!
  internal var customerRepository: CustomerRepository!
  internal var paymentMethodRepository: PaymentMethodRepository!
  internal var mobileReaderDriverRepository = MobileReaderDriverRepository()
  internal var merchant: Merchant?

  /// The queue that Omni should use to communicate back with its listeners
  public var preferredQueue: DispatchQueue = DispatchQueue.main

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
    return initialized
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

    omniApi.apiKey = params.apiKey
    omniApi.environment = params.environment
    initRepos(omniApi: omniApi)

    // Verify the apikey corresponds to a real merchant
    omniApi.getSelf(completion: { myself in

      guard let merchant = myself.merchant else {
        error(OmniNetworkingException.couldNotGetMerchantDetails)
        return
      }

      // Assign merchant to self
      self.merchant = merchant

      let args: [String: Any] = [
        "appId": appId,
        "merchant": merchant
      ]

      InitializeDrivers(mobileReaderDriverRepository: self.mobileReaderDriverRepository, args: args).start(completion: { _ in
        self.initialized = true
        self.preferredQueue.async(execute: completion)
      }, failure: error)
    }, failure: error)

  }

  /// Captures a transaction without a mobile reader
  /// - Parameters:
  ///   - transactionRequest: A request for a Transaction
  ///   - completion: Called when the operation is completed successfully. Receives a Transaction
  ///   - error: Receives any errors that happened while attempting the operation
  public func pay(transactionRequest: TransactionRequest, completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    guard let merchant = self.merchant else {
      error(OmniGeneralException.uninitialized)
      return
    }

    let job = TakePayment(request: transactionRequest, omniApi: omniApi, merchant: merchant)
    job.start(completion: completion, failure: error)
  }

  /// Captures a mobile reader transaction
  /// - Parameters:
  ///   - request: A request for a Transaction
  ///   - completion: Called when the operation is complete successfully. Receives a Transaction
  ///   - error: Receives any errors that happened while attempting the operation
  public func takeMobileReaderTransaction(request: TransactionRequest, completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    guard initialized else {
      return error(OmniGeneralException.uninitialized)
    }

    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepository,
      invoiceRepository: invoiceRepository,
      customerRepository: customerRepository,
      paymentMethodRepository: paymentMethodRepository,
      transactionRepository: transactionRepository,
      request: request
    )

    job.start(completion: completion, failure: error)
  }

  /// Refunds the given transaction and returns a new Transaction that represents the refund in Omni
  /// - Parameters:
  ///   - transaction: the transaction to refund
  ///   - refundAmount: the amount of money to refund. When present, this **must** be greater than zero and lesser than or equal to the transaction total
  ///   - completion: Receives the Transaction that represents the refund in Omni
  ///   - error: Receives any errors that happened while attempting the operation
  public func refundMobileReaderTransaction(transaction: Transaction, refundAmount: Amount? = nil, completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    guard initialized else {
      return error(OmniGeneralException.uninitialized)
    }

    let job = RefundMobileReaderTransaction(mobileReaderDriverRepository: mobileReaderDriverRepository,
                                            transactionRepository: transactionRepository,
                                            transaction: transaction,
                                            refundAmount: refundAmount)
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
    guard initialized else {
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
    guard initialized else {
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
    guard initialized else {
      return error()
    }

    let task = ConnectMobileReader(mobileReaderDriverRepository: mobileReaderDriverRepository, mobileReader: reader)
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
    guard initialized else {
      return error(OmniGeneralException.uninitialized)
    }

    let task = ConnectMobileReader(mobileReaderDriverRepository: mobileReaderDriverRepository, mobileReader: reader)
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
    guard initialized else {
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
    guard initialized else {
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

}
