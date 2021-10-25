//
//  TakeMobileReaderPayment.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum TakeMobileReaderPaymentException: OmniException {
  case mobileReaderNotFound
  case mobileReaderNotReady
  case invoiceNotFound
  case invoiceIdCannotBeBlank
  case couldNotCreateInvoice(detail: String?)
  case couldNotCreateCustomer(detail: String?)
  case couldNotCreatePaymentMethod(detail: String?)
  case couldNotUpdateInvoice(detail: String?)
  case couldNotCreateTransaction(detail: String?)
  case couldNotCaptureTransaction

  static var mess: String = "Error taking mobile reader payment"

  var detail: String? {
    switch self {
    case .mobileReaderNotFound:
      return "Mobile reader not found"

    case .mobileReaderNotReady:
      return "Mobile reader not ready to take payment"

    case .invoiceNotFound:
      return "Invoice with given id not found"

    case .couldNotCaptureTransaction:
      return "Could not capture transaction"

    case .couldNotCreateInvoice(let d):
      return d ?? "Could not create invoice"

    case .couldNotCreateCustomer(let d):
      return d ?? "Could not create customer"

    case .couldNotCreatePaymentMethod(let d):
      return d ?? "Could not create payment method"

    case .couldNotUpdateInvoice(let d):
      return d ?? "Could not update invoice"

    case .couldNotCreateTransaction(let d):
      return d ?? "Could not create transaction"

    case .invoiceIdCannotBeBlank:
      return "Could not create invoice"
    }
  }
}

class TakeMobileReaderPayment {

  typealias Exception = TakeMobileReaderPaymentException

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var invoiceRepository: InvoiceRepository
  var customerRepository: CustomerRepository
  var paymentMethodRepository: PaymentMethodRepository
  var transactionRepository: TransactionRepository
  var request: TransactionRequest
  var signatureProvider: SignatureProviding?
  weak var transactionUpdateDelegate: TransactionUpdateDelegate?
  weak var userNotificationDelegate: UserNotificationDelegate?

  init(
    mobileReaderDriverRepository: MobileReaderDriverRepository,
    invoiceRepository: InvoiceRepository,
    customerRepository: CustomerRepository,
    paymentMethodRepository: PaymentMethodRepository,
    transactionRepository: TransactionRepository,
    request: TransactionRequest,
    signatureProvider: SignatureProviding?,
    transactionUpdateDelegate: TransactionUpdateDelegate?,
    userNotificationDelegate: UserNotificationDelegate?) {

    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.invoiceRepository = invoiceRepository
    self.customerRepository = customerRepository
    self.paymentMethodRepository = paymentMethodRepository
    self.transactionRepository = transactionRepository
    self.request = request
    self.signatureProvider = signatureProvider
    self.transactionUpdateDelegate = transactionUpdateDelegate
    self.userNotificationDelegate = userNotificationDelegate
  }

  func start(completion: @escaping (Transaction) -> Void, failure: @escaping (OmniException) -> Void) {
    availableMobileReaderDriver(mobileReaderDriverRepository, failure) { driver in
      self.getOrCreateInvoice(failure) { (createdInvoice) in
        self.takeMobileReaderPayment(with: driver,
                                     signatureProvider: self.signatureProvider,
                                     transactionUpdateDelegate: self.transactionUpdateDelegate,
                                     userNotificationDelegate: self.userNotificationDelegate,
                                     failure) { (mobileReaderPaymentResult) in

          // This is a callback that voids the transaction and calls the fail block
          let voidAndFail: (OmniException) -> Void = { exception in

            // By the time this is invoked, the NMI transaction went through fine but something happened while doing
            // one of the calls to Omni. Since the transaction is pending confirmation, then we need to void it *before*
            // invoking the failure block. That way the customer gets their money back
            driver.void(transactionResult: mobileReaderPaymentResult) { _ in
              failure(exception)
            }
          }

          self.createCustomer(mobileReaderPaymentResult, voidAndFail) { (createdCustomer) in

            self.createPaymentMethod(for: createdCustomer, mobileReaderPaymentResult, voidAndFail) { (createdPaymentMethod) in

              self.updateInvoice(createdInvoice, with: createdPaymentMethod, and: createdCustomer, voidAndFail) { (updatedInvoice) in

                self.createTransaction(
                  result: mobileReaderPaymentResult,
                  driver: driver,
                  paymentMethod: createdPaymentMethod,
                  customer: createdCustomer,
                  invoice: updatedInvoice,
                  voidAndFail) { completedTransaction in

                  // Make sure the transaction from Omni has an id. This should be true pretty much all the time
                  guard let transactionId = completedTransaction.id else {
                    voidAndFail(TakeMobileReaderPaymentException.couldNotCreateTransaction(detail: nil))
                    return
                  }

                  // If the transaction is a pre-auth, then we don't need to capture it
                  if self.request.preauth {
                    completion(completedTransaction)
                    return
                  }

                  driver.capture(transaction: completedTransaction) { (success) in
                    if success {
                      completion(completedTransaction)
                    } else {
                      /* We couldn't capture the transaction. So void the NMI transaction and mark it failed on Omni */

                      // Mark omni transaction failed
                      let failedTransaction = completedTransaction
                      failedTransaction.success = false
                      failedTransaction.message = "Error capturing the transaction"

                      // Fail the transaction in omni
                      self.transactionRepository.update(model: failedTransaction, id: transactionId, completion: { _ in
                        voidAndFail(TakeMobileReaderPaymentException.couldNotCaptureTransaction)
                        return
                      }, error: { _ in
                        voidAndFail(TakeMobileReaderPaymentException.couldNotCaptureTransaction)
                        return
                      })
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  internal func createTransaction(result: TransactionResult,
                                  driver: MobileReaderDriver,
                                  paymentMethod: PaymentMethod,
                                  customer: Customer,
                                  invoice: Invoice,
                                  _ failure: @escaping (OmniException) -> Void,
                                  _ completion: @escaping (Transaction) -> Void) {
    let transactionToCreate = Transaction()

    guard let paymentMethodId = paymentMethod.id else {
      failure(Exception.couldNotUpdateInvoice(detail: "Payment method id is required"))
      return
    }

    guard let lastFour = getLastFour(for: result.maskedPan) else {
      failure(Exception.couldNotCreatePaymentMethod(detail: "Could not retrieve masked pan"))
      return
    }

    guard let transactionMeta = createTransactionMeta(from: result) else {
      failure(Exception.couldNotCreateTransaction(detail: "Could not generate transaction meta json"))
      return
    }

    var gatewayResponseJson: JSONValue?

    if let authCode = result.authCode, result.source.lowercased() == "nmi" {
      let gatewayResponse = [
        "gateway_specific_response_fields": [
          "nmi": [
            "authcode": authCode
          ]
        ]
      ]

      gatewayResponseJson = gatewayResponse.jsonValue()
    }

    guard let customerId = customer.id else {
      failure(Exception.couldNotCreateTransaction(detail: "Customer id is required"))
      return
    }

    guard let invoiceId = invoice.id else {
      failure(Exception.couldNotCreateTransaction(detail: "Invoice id is required"))
      return
    }

    transactionToCreate.paymentMethodId = paymentMethodId
    transactionToCreate.total = request.amount.dollars()
    transactionToCreate.success = result.success ?? false
    transactionToCreate.lastFour = lastFour
    transactionToCreate.meta = transactionMeta
    transactionToCreate.type = "charge"
    transactionToCreate.method = "card"
    transactionToCreate.source = "iOS|CPSDK|\(result.source)"
    transactionToCreate.customerId = customerId
    transactionToCreate.invoiceId = invoiceId
    transactionToCreate.response = gatewayResponseJson
    transactionToCreate.token = result.externalId
    transactionToCreate.message = result.message

    // Set the transaction to not refundable or voidable if the Omni backend cannot perform the refund
    // This is extremely important because it prevents a user from attempting a refund via the VT or the Omni API that
    // could never work. The reason it won't work is because Omni doesn't have a deep integration with all of our
    // third party vendors, such as AnywhereCommerce.
    if !type(of: driver).omniRefundsSupported {
      transactionToCreate.isRefundable = false
      transactionToCreate.isVoidable = false
    }

    // Mark the transaction as pre-auth, if necessary
    if request.preauth {
      transactionToCreate.type = "pre_auth"
      transactionToCreate.preAuth = true
      transactionToCreate.isCaptured = 0
      transactionToCreate.isVoidable = true
    }

    transactionRepository.create(model: transactionToCreate, completion: completion, error: failure)
  }

  /// Creates a JSONValue object that from the transactionResult, including only the items that make up the TransactionMeta
  /// - Parameter transactionResult: the TransactionResult object to be converted into transaction meta
  fileprivate func createTransactionMeta(from transactionResult: TransactionResult) -> JSONValue? {
    var dict = [String: JSONValue?]()

    #if !targetEnvironment(simulator)
    if transactionResult.source.contains(ChipDnaDriver.source) {
      if let userRef = transactionResult.userReference {
        dict["nmiUserRef"] = JSONValue(userRef)
      }

      if let localId = transactionResult.localId {
        dict["cardEaseReference"] = JSONValue(localId)
      }

      if let externalId = transactionResult.externalId {
        dict["nmiTransactionId"] = JSONValue(externalId)
      }
    } else if transactionResult.source.contains(AWCDriver.source) {
      if let externalId = transactionResult.externalId {
        dict["awcTransactionId"] = JSONValue(externalId)
      }
    }
    #endif

    if let gatewayResponse = transactionResult.gatewayResponse {
      dict["gatewayResponse"] = JSONValue(gatewayResponse)
    }

    if let lineItemResponse = transactionResult.request?.lineItems {
      dict["lineItems"] = JSONValue(lineItemResponse)
    }

    if let subtotal = transactionResult.request?.subtotal {
      dict["subtotal"] = JSONValue(subtotal)
    }

    if let tax = transactionResult.request?.tax {
      dict["tax"] = JSONValue(tax)
    }

    if let memo = transactionResult.request?.memo {
      dict["memo"] = JSONValue(memo)
    }

    if let reference = transactionResult.request?.reference {
      dict["reference"] = JSONValue(reference)
    }

    if let tip = transactionResult.request?.tip {
      dict["tip"] = JSONValue(tip)
    }

    return dict.jsonValue()
  }

  fileprivate func createInvoiceMeta() -> JSONValue? {
    var dict = [String: JSONValue?]()

    if let subtotal = self.request.subtotal {
      dict["subtotal"] = JSONValue(subtotal)
    } else {
      // If the user does not specify a subtotal, we a
      dict["subtotal"] = JSONValue(request.amount.dollars())
    }

    if let tax = self.request.tax {
      dict["tax"] = JSONValue(tax)
    }

    if let memo = self.request.memo {
      dict["memo"] = JSONValue(memo)
    }

    if let reference = self.request.reference {
      dict["reference"] = JSONValue(reference)
    }

    if let tip = self.request.tip {
      dict["tip"] = JSONValue(tip)
    }

    if let lineItems = self.request.lineItems {
      dict["lineItems"] = JSONValue(lineItems)
    }

    return dict.jsonValue()
  }

  fileprivate func updateInvoice(_ invoice: Invoice,
                                 with paymentMethod: PaymentMethod,
                                 and customer: Customer,
                                 _ failure: @escaping (OmniException) -> Void,
                                 completion: @escaping (Invoice) -> Void) {
    let newInvoice = Invoice()

    guard let id = invoice.id else {
      failure(Exception.couldNotUpdateInvoice(detail: "Invoice id is required"))
      return
    }

    guard let paymentMethodId = paymentMethod.id else {
      failure(Exception.couldNotUpdateInvoice(detail: "Payment method id is required"))
      return
    }

    guard let customerId = customer.id else {
      failure(Exception.couldNotUpdateInvoice(detail: "Customer id is required"))
      return
    }

    newInvoice.customerId = customerId
    newInvoice.paymentMethodId = paymentMethodId
    invoiceRepository.update(model: newInvoice, id: id, completion: completion, error: failure)
  }

  fileprivate func getLastFour(for maskedPan: String?) -> String? {
    guard
      let maskedPan = maskedPan,
      maskedPan.count > 4,
      let lastFourIdx = maskedPan.index(maskedPan.endIndex, offsetBy: -4, limitedBy: maskedPan.startIndex) else {
        return nil
    }

    return String(maskedPan.suffix(from: lastFourIdx))
  }

  fileprivate func createPaymentMethod(for customer: Customer, _ result: TransactionResult, _ failure: @escaping (OmniException) -> Void, completion: @escaping (PaymentMethod) -> Void) {
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

  }

  fileprivate func createCustomer(_ transactionResult: TransactionResult, _ failure: @escaping (OmniException) -> Void, _ completion: @escaping (Customer) -> Void) {
    let firstname = transactionResult.cardHolderFirstName ?? "SWIPE"
    let lastname = transactionResult.cardHolderLastName ?? "CUSTOMER"
    var customerToCreate = Customer(firstName: firstname, lastName: lastname)

    if let transactionSource = transactionResult.transactionSource {
      if transactionSource.lowercased().contains("contactless") {
        customerToCreate.firstname = "Mobile Device"
        customerToCreate.lastname = "Customer"
      } else {
        customerToCreate.firstname = transactionResult.cardHolderFirstName ?? "SWIPE"
        customerToCreate.lastname = transactionResult.cardHolderLastName ?? "CUSTOMER"
      }
    } else {
      customerToCreate.firstname = transactionResult.cardHolderFirstName ?? "SWIPE"
      customerToCreate.lastname = transactionResult.cardHolderLastName ?? "CUSTOMER"
    }

    customerRepository.create(model: customerToCreate, completion: completion, error: failure)
  }

  fileprivate func takeMobileReaderPayment(with driver: MobileReaderDriver,
                                           signatureProvider: SignatureProviding?,
                                           transactionUpdateDelegate: TransactionUpdateDelegate?,
                                           userNotificationDelegate: UserNotificationDelegate?,
                                           _ failure: (OmniException) -> Void,
                                           _ completion: @escaping (TransactionResult) -> Void) {
    driver.performTransaction(with: self.request,
                              signatureProvider: signatureProvider,
                              transactionUpdateDelegate: transactionUpdateDelegate,
                              userNotificationDelegate: userNotificationDelegate,
                              completion: completion)
  }

  /// Gets the invoice with the id in the transaction request or creates a new one
  internal func getOrCreateInvoice(_ failure: @escaping (OmniException) -> Void, _ completion: @escaping (Invoice) -> Void) {
    // If an invoiceId was given in the transaction request, we should verify that an invoice with that id exists
    if let invoiceId = request.invoiceId {
      guard !invoiceId.isEmpty else {
        failure(TakeMobileReaderPaymentException.invoiceIdCannotBeBlank)
        return
      }
      invoiceRepository.getById(id: invoiceId, completion: completion) { (error) in
        failure(TakeMobileReaderPaymentException.invoiceNotFound)
      }
    } else {
      let invoiceToCreate = Invoice()
      invoiceToCreate.total = request.amount.dollars()
      invoiceToCreate.url = "https://fattpay.com/#/bill"

      guard let invoiceMetaJson = createInvoiceMeta() else {
        failure(Exception.couldNotCreateInvoice(detail: "Error generating json for meta"))
        return
      }

      invoiceToCreate.meta = invoiceMetaJson
      invoiceRepository.create(model: invoiceToCreate, completion: { createdInvoice in
        guard createdInvoice.id?.isEmpty != true else {
          return failure(TakeMobileReaderPaymentException.couldNotCreateInvoice(detail: nil))
        }
        completion(createdInvoice)
      }, error: failure)
    }
  }

  fileprivate func availableMobileReaderDriver(_ repo: MobileReaderDriverRepository, _ failure: @escaping (OmniException) -> Void, _ completion: @escaping (MobileReaderDriver) -> Void) {
    repo.getInitializedDrivers { initializedDrivers in
      // Get drivers that are ready for payment
      filter(items: initializedDrivers, predicate: { $0.isReadyToTakePayment }, completion: { driversReadyForPayment in
        guard let driver = driversReadyForPayment.first else {
          failure(TakeMobileReaderPaymentException.mobileReaderNotFound)
          return
        }

        completion(driver)
      })
    }
  }
}
