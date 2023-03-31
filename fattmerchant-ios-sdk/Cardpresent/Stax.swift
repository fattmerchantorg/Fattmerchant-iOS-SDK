//
//  Stax.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum StaxNetworkingException: StaxException {
    case couldNotGetMerchantDetails
    case couldNotGetPaginatedTransactions

    static var mess: String = "Stax Networking Exception"

    var detail: String? {
        switch self {
        case .couldNotGetMerchantDetails:
            return "Could not get merchant details from Stax"
        case .couldNotGetPaginatedTransactions:
            return "Could not get paginated transactions"
        }
    }
}

public enum StaxInitializeException: StaxException {
    case missingInitializationDetails
    case mobileReaderPaymentsNotConfigured
    case missingMobileReaderCredentials
    case invalidMobileReaderCredentials

    public static var mess: String = "Stax Initialization Exception"

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

public enum StaxGeneralException: StaxException {
    case uninitialized
    public static var mess: String = "Stax General Error"

    public var detail: String? {
        switch self {
        case .uninitialized:
            return "Stax has not been initialized yet"
        }
    }
}

/**
 Handles cardpresent payments

 This is the object you used for communicating with the Stax platform

 ## Usage
 Create an instance of Stax
 ```
 let stax = Stax()
 ```
 Then initialize, passing in an instance of InitParams
 ```
 let staxInitParms = Stax.InitParams(appId: "Myappid", apiKey: "staxephemeralkey")
 stax.initialize(staxInitParams, completion: ..., error: ...)
 ```

 Once initialized, you can call its methods like `getAvailableReaders` and `takeMobileReaderTransaction`
 */
public class Stax: NSObject {

    internal var mobileReaderDriversInitialized: Bool = false
    internal var staxApi = StaxApi()
    internal var transactionRepository: TransactionRepository!
    internal var invoiceRepository: InvoiceRepository!
    internal var customerRepository: CustomerRepository!
    internal var paymentMethodRepository: PaymentMethodRepository!
    internal var mobileReaderDriverRepository = MobileReaderDriverRepository()
    internal var merchant: Merchant?

    /// The queue that Stax should use to communicate back with its listeners
    public var preferredQueue: DispatchQueue = DispatchQueue.main

    /// Responsible for providing signatures for transactions, when required
    public var signatureProvider: SignatureProviding?

    /// Receives notifications about transaction events such as when a card is swiped
    public weak var transactionUpdateDelegate: TransactionUpdateDelegate?

    /// Receives notifications about user-facing transaction events such as when a user swipes a chip card
    public weak var userNotificationDelegate: UserNotificationDelegate?

    /// Receives notifications about reader connection events
    public weak var mobileReaderConnectionUpdateDelegate: MobileReaderConnectionStatusDelegate?

    /// Contains all the data necessary to initialize `Stax`
    public struct InitParams {
        /// An id for your application
        public var appId: String?

        /// An ephemeral Stax api key
        public var apiKey: String?

        /// The Stax enviroment to use
        public var environment: Environment

        /// The Stax webpayments token
        ///
        /// This is used for tokenizing and charging payment methods
        public var webpaymentsToken: String?

        public init(appId: String, apiKey: String, environment: Environment = Environment.LIVE) {
            self.appId = appId
            self.apiKey = apiKey
            self.environment = environment
        }
    }

    fileprivate func initRepos(staxApi: StaxApi) {
        transactionRepository = TransactionRepository(staxApi: staxApi)
        invoiceRepository = InvoiceRepository(staxApi: staxApi)
        customerRepository = CustomerRepository(staxApi: staxApi)
        paymentMethodRepository = PaymentMethodRepository(staxApi: staxApi)
    }

    /// True when Stax is initialized. False otherwise
    public var isInitialized: Bool {
        return mobileReaderDriversInitialized
    }

    /// Used to initialize the Stax object
    /// - Parameters:
    ///   - params: an instance of InitParams which contains all necessary information to initialize stax
    ///   - completion: a completion block to run once finished
    ///   - error: an error block to run in case something goes wrong
    public func initialize(params: InitParams, completion: @escaping () -> Void, error: @escaping (StaxException) -> Void) {
        guard let appId = params.appId, params.apiKey != nil else {
            error(StaxInitializeException.missingInitializationDetails)
            return
        }

        staxApi.apiKey = params.apiKey
        staxApi.environment = params.environment
        initRepos(staxApi: staxApi)

        // Verify the apikey corresponds to a real merchant
        staxApi.getSelf(completion: { myself in

            guard let merchant = myself.merchant else {
                error(StaxNetworkingException.couldNotGetMerchantDetails)
                return
            }

            // Assign merchant to self
            self.merchant = merchant

            // A dict that contains the initialization details for the drivers
            var args: [String: Any] = [ "appId": appId ]

            // Eagerly try to fill out the mobile reader settings from the merchant options.
            // The getMobileReaderSettings step will override these if successful, but if that step fails to grab the
            // settings from the gateways, then at least we have these as a fallback.

            // AWC
            if let emvTerminalSecret = merchant.emvTerminalSecret(), let emvTerminalId = merchant.emvTerminalId() {
                args["awc"] = AWCDetails(terminalId: emvTerminalId, terminalSecret: emvTerminalSecret)
            }

            // NMI
            if let emvPassword = merchant.emvPassword() {
                args["nmi"] = NMIDetails(securityKey: emvPassword)
            }

            // Try to get the details from the merchant gateways. This *should* rewrite the args dict
            self.staxApi.getMobileReaderSettings(completion: { mrDetails in
                if let awcDetails = mrDetails.anywhereCommerce {
                    args.updateValue(awcDetails, forKey: "awc")
                }
                if let nmiDetails = mrDetails.nmi {
                    args.updateValue(nmiDetails, forKey: "nmi")
                }

                // Check if there are creds for AWC or NMI
                if args["awc"] == nil && args["nmi"] == nil {
                    self.preferredQueue.async {
                        completion()
                    }
                    return
                }

                //
                if (args["awc"] as? AWCDetails)?.terminalId.isBlank() ?? true
                    && (args["awc"] as? AWCDetails)?.terminalSecret.isBlank() ?? true
                    && (args["nmi"] as? NMIDetails)?.securityKey.isBlank() ?? true {
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
                    error(StaxInitializeException.invalidMobileReaderCredentials)
                })
            }, failure: { _ in

                // If the call to merchant gateways fails, try to init with the merchant options anyways
                if args["awc"] == nil && args["nmi"] == nil {
                    self.preferredQueue.async {
                        self.mobileReaderDriversInitialized = true
                        error(StaxInitializeException.missingMobileReaderCredentials)
                    }
                    return
                }
                InitializeDrivers(mobileReaderDriverRepository: self.mobileReaderDriverRepository, args: args).start(completion: { _ in
                    self.mobileReaderDriversInitialized = true
                    self.preferredQueue.async(execute: completion)
                }, failure: { _ in
                    self.mobileReaderDriversInitialized = true
                    error(StaxInitializeException.invalidMobileReaderCredentials)
                })
            })
        }, failure: error)
    }

    /// Creates a PaymentMethod out of a CreditCard object for reuse with Stax
    /// - Parameters:
    ///   - creditCard: Contains the details of the payment method to tokenize
    ///   - completion: Called when the operation is completed successfully. Receives a PaymentMethod
    ///   - error: Receives any errors that happened while attempting the operation
    public func tokenize(_ bankAccount: BankAccount, _ completion: @escaping (PaymentMethod) -> Void, error: @escaping (StaxException) -> Void) {
        TokenizePaymentMethod(customerRepository: customerRepository,
                              paymentMethodRepository: paymentMethodRepository,
                              bankAccount: bankAccount
        ).start(completion: completion, failure: error)
    }

    /// Creates a PaymentMethod out of a CreditCard object for reuse with Stax
    /// - Parameters:
    ///   - creditCard: Contains the details of the payment method to tokenize
    ///   - completion: Called when the operation is completed successfully. Receives a PaymentMethod
    ///   - error: Receives any errors that happened while attempting the operation
    public func tokenize(_ creditCard: CreditCard, _ completion: @escaping (PaymentMethod) -> Void, error: @escaping (StaxException) -> Void) {
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
    public func pay(transactionRequest: TransactionRequest, completion: @escaping (Transaction) -> Void, error: @escaping (StaxException) -> Void) {
        let job = TakePayment(request: transactionRequest, customerRepository: customerRepository, paymentMethodRepository: paymentMethodRepository)
        job.start(completion: completion, failure: error)
    }

    /// Creates a Fattmerchant PaymentMethod out of the given CreditCard
    ///
    /// - Parameters:
    ///   - card: The CreditCard to be tokenized
    ///   - completion: Called when the operation is completed successfully. Receives a PaymentMethod
    ///   - error: Receives any errors that happened while attempting the operation
    public func tokenize(card: CreditCard, completion: @escaping (PaymentMethod) -> Void, error: @escaping (StaxException) -> Void) {
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
    public func tokenize(bankAccount: BankAccount, completion: @escaping (PaymentMethod) -> Void, error: @escaping (StaxException) -> Void) {
        let job = TokenizePaymentMethod(
            customerRepository: customerRepository,
            paymentMethodRepository: paymentMethodRepository,
            bankAccount: bankAccount
        )

        job.start(completion: completion, failure: error)
    }

    /// Captures a mobile reader transaction
    ///
    /// - Note: `Stax` should be assigned a `SignatureProviding` object by the time this transaction is called. This
    /// object is responsible for providing a signature, in case one is required to complete the transaction
    ///
    /// - Parameters:
    ///   - request: A request for a Transaction
    ///   - completion: Called when the operation is complete successfully. Receives a Transaction
    ///   - error: Receives any errors that happened while attempting the operation
    public func takeMobileReaderTransaction(request: TransactionRequest, completion: @escaping (Transaction) -> Void, error: @escaping (StaxException) -> Void) {
        guard mobileReaderDriversInitialized else {
            return error(StaxGeneralException.uninitialized)
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
                                          error: @escaping (StaxException) -> Void) {

        let job = CapturePreauthTransaction(transactionId: transactionId, captureAmount: amount, staxApi: staxApi)
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
                                error: @escaping (StaxException) -> Void) {
        let job = VoidTransaction(transactionId: transactionId, staxApi: staxApi)
        job.start(completion: completion, error: error)
    }

    /// Cancels the current mobile reader transaction
    ///
    /// - Parameters:
    ///   - completion: called when the operation is complete. Receives true when the transaction was cancelled. False
    ///   otherwise
    ///   - error: receives any errors that happened while attempting the operation
    public func cancelMobileReaderTransaction(completion: @escaping (Bool) -> Void, error: @escaping (StaxException) -> Void) {
        guard mobileReaderDriversInitialized else {
            return error(StaxGeneralException.uninitialized)
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

    /// Refunds the given transaction and returns a new Transaction that represents the refund in Stax
    /// - Parameters:
    ///   - transaction: the transaction to refund
    ///   - refundAmount: the amount of money to refund. When present, this **must** be greater than zero and lesser than or equal to the transaction total
    ///   - completion: Receives the Transaction that represents the refund in Stax
    ///   - error: Receives any errors that happened while attempting the operation
    public func refundMobileReaderTransaction(transaction: Transaction, refundAmount: Amount? = nil, completion: @escaping (Transaction) -> Void, error: @escaping (StaxException) -> Void) {
        guard mobileReaderDriversInitialized else {
            return error(StaxGeneralException.uninitialized)
        }

        let job = RefundMobileReaderTransaction(mobileReaderDriverRepository: mobileReaderDriverRepository,
                                                transactionRepository: transactionRepository,
                                                transaction: transaction,
                                                refundAmount: refundAmount,
                                                staxApi: staxApi)
        job.start(completion: completion, failure: error)
    }

    /// Refunds the given transaction and returns a new Transaction that represents the refund in Stax
    /// - Parameters:
    ///   - transaction: the transaction to refund
    ///   - completion: Receives the Transaction that represents the refund in Stax
    ///   - error: Receives any errors that happened while attempting the operation
    public func refundMobileReaderTransaction(transaction: Transaction, completion: @escaping (Transaction) -> Void, error: @escaping (StaxException) -> Void) {
        refundMobileReaderTransaction(transaction: transaction, refundAmount: nil, completion: completion, error: error)
    }

    /// Finds the available readers that Stax can connect to
    /// - Parameters:
    ///   - completion: Receives an array of MobileReaders that are available
    ///   - error: Receives any errors that happened while attempting the operation
    public func getAvailableReaders(completion: @escaping ([MobileReader]) -> Void, error: @escaping (StaxException) -> Void) {
        guard mobileReaderDriversInitialized else {
            return error(StaxGeneralException.uninitialized)
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
    public func getConnectedReader(completion: @escaping (MobileReader?) -> Void, error: @escaping (StaxException) -> Void) {
        guard mobileReaderDriversInitialized else {
            return error(StaxGeneralException.uninitialized)
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
    ///   - error: A block to call if this operation fails. Receives an StaxException
    public func connect(reader: MobileReader, completion: @escaping (MobileReader) -> Void, error: @escaping (StaxException) -> Void) {
        guard mobileReaderDriversInitialized else {
            return error(StaxGeneralException.uninitialized)
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
    public func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (StaxException) -> Void) {
        guard mobileReaderDriversInitialized else {
            return error(StaxGeneralException.uninitialized)
        }

        let task = DisconnectMobileReader(mobileReaderDriverRepository: mobileReaderDriverRepository, mobileReader: reader)
        task.start(completion: { success in
            self.preferredQueue.async { completion(success) }
        }, failure: ({ exception in
            self.preferredQueue.async { error(exception) }
        }))
    }

    /// Retrieves a list of the most recent mobile reader transactions from Stax
    /// - Parameters:
    ///   - completion: Receives a list of Transactions
    ///   - error: A block to call if this operation fails
    public func getMobileReaderTransactions(completion: @escaping ([Transaction]) -> Void, error: @escaping (StaxException) -> Void) {
        guard mobileReaderDriversInitialized else {
            return error(StaxGeneralException.uninitialized)
        }

        transactionRepository.getList(completion: ({ paginatedTransactions in
            guard let transactions = paginatedTransactions.data else {
                error(StaxNetworkingException.couldNotGetPaginatedTransactions)
                return
            }
            completion(transactions)
        }), error: error)
    }

}
