//
//  TakeMobileReaderPayment.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum TakeMobileReaderPaymentException: OmniException {
  case mobileReaderDriverNotFound
  case mobileReaderNotReady
  case couldNotCreateInvoice(detail: String?)
  case couldNotCreateCustomer(detail: String?)
  case couldNotCreatePaymentMethod(detail: String?)
  case couldNotUpdateInvoice(detail: String?)
  case couldNotCreateTransaction(detail: String?)

  static var mess: String = "Error taking mobile reader payment"

  var detail: String? {
    switch self {
    case .mobileReaderDriverNotFound:
      return "Mobile reader driver not found"

    case .mobileReaderNotReady:
      return "Mobile reader not ready to take payment"

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

  init(
    mobileReaderDriverRepository: MobileReaderDriverRepository,
    invoiceRepository: InvoiceRepository,
    customerRepository: CustomerRepository,
    paymentMethodRepository: PaymentMethodRepository,
    transactionRepository: TransactionRepository,
    request: TransactionRequest) {

    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.invoiceRepository = invoiceRepository
    self.customerRepository = customerRepository
    self.paymentMethodRepository = paymentMethodRepository
    self.transactionRepository = transactionRepository
    self.request = request
  }

  func start(completion: @escaping (Transaction) -> Void, failure: @escaping (OmniException) -> Void) {
    availableMobileReaderDriver(mobileReaderDriverRepository, failure) { driver in

      self.createInvoice(failure) { (createdInvoice) in

        self.takeMobileReaderPayment(with: driver, failure) { (mobileReaderPaymentResult) in

          self.createCustomer(mobileReaderPaymentResult, failure) { (createdCustomer) in

            self.createPaymentMethod(for: createdCustomer, mobileReaderPaymentResult, failure) { (createdPaymentMethod) in

              self.updateInvoice(createdInvoice, with: createdPaymentMethod, and: createdCustomer, failure) { (updatedInvoice) in

                self.createTransaction(
                  result: mobileReaderPaymentResult,
                  paymentMethod: createdPaymentMethod,
                  customer: createdCustomer,
                  invoice: updatedInvoice,
                  failure,
                  completion
                )
              }
            }
          }
        }
      }
    }
  }

  fileprivate func createTransaction(result: TransactionResult, paymentMethod: PaymentMethod, customer: Customer, invoice: Invoice, _ failure: @escaping (OmniException) -> Void, _ completion: @escaping (Transaction) -> Void) {
    let transactionToCreate = Transaction()

    guard let paymentMethodId = paymentMethod.id else {
      failure(Exception.couldNotUpdateInvoice(detail: "Payment method id is required"))
      return
    }

    guard let lastFour = getLastFour(for: result.maskedPan) else {
        failure(Exception.couldNotCreatePaymentMethod(detail: "Could not retrieve masked pan"))
        return
    }

    guard let userRef = result.userReference, let transactionMetaJson = [ "nmiUserRef": userRef ].jsonValue() else {
      failure(Exception.couldNotCreateTransaction(detail: "Could not generate transaction meta json"))
      return
    }

    guard let authCode = result.authCode else {
      failure(Exception.couldNotCreateTransaction(detail: "Auth code is required"))
      return
    }

    let gatewayResponse = [
      "gateway_specific_response_fields": [
        "nmi": [
          "authcode": authCode
        ]
      ]
    ]

    guard let gatewayResponseJson = gatewayResponse.jsonValue() else {
      failure(Exception.couldNotCreateTransaction(detail: "Could not generate gatewayResponse json"))
      return
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
    transactionToCreate.total = request.amount.dollarsString()
    transactionToCreate.success = result.success ?? false
    transactionToCreate.lastFour = lastFour
    transactionToCreate.meta = transactionMetaJson
    transactionToCreate.type = "charge"
    transactionToCreate.method = "card"
    transactionToCreate.source = "iOS|CPSDK|NMI"
    transactionToCreate.customerId = customerId
    transactionToCreate.invoiceId = invoiceId
    transactionToCreate.response = gatewayResponseJson
    transactionRepository.create(model: transactionToCreate, completion: completion, error: failure)
  }

  fileprivate func updateInvoice(_ invoice: Invoice,
                                 with paymentMethod: PaymentMethod,
                                 and customer: Customer,
                                 _ failure: @escaping (OmniException) -> Void,
                                 completion: @escaping (Invoice) -> Void) {
    let invoice = invoice

    guard let paymentMethodId = paymentMethod.id else {
      failure(Exception.couldNotUpdateInvoice(detail: "Payment method id is required"))
      return
    }

    guard let customerId = customer.id else {
      failure(Exception.couldNotUpdateInvoice(detail: "Customer id is required"))
      return
    }

    invoice.customerId = customerId
    invoice.paymentMethodId = paymentMethodId
    invoiceRepository.update(model: invoice, completion: completion, error: failure)
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
    let paymentMethodToCreate = PaymentMethod()

    guard let merchantId = customer.merchantId else {
      failure(Exception.couldNotCreateCustomer(detail: "Merchant id is required"))
      return
    }

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

    paymentMethodToCreate.merchantId = merchantId
    paymentMethodToCreate.customerId = customerId
    paymentMethodToCreate.method = PaymentMethodType.card
    paymentMethodToCreate.cardLastFour = lastFour
    paymentMethodToCreate.cardType = cardType
    paymentMethodToCreate.personName = "\(customer.firstname ?? "") \(customer.lastname ?? "")"
    paymentMethodToCreate.tokenize = false
    paymentMethodRepository.create(model: paymentMethodToCreate, completion: completion, error: failure)
  }

  fileprivate func createCustomer(_ transactionResult: TransactionResult, _ failure: @escaping (OmniException) -> Void, _ completion: @escaping (Customer) -> Void) {
    let customerToCreate = Customer()
    customerToCreate.firstname = transactionResult.cardHolderFirstName ?? "SWIPE"
    customerToCreate.lastname = transactionResult.cardHolderLastName ?? "CUSTOMER"
    customerRepository.create(model: customerToCreate, completion: completion, error: failure)
  }

  fileprivate func takeMobileReaderPayment(with driver: MobileReaderDriver, _ failure: (OmniException) -> Void, _ completion: @escaping (TransactionResult) -> Void) {
    driver.performTransaction(with: self.request, completion: completion) //TODO: Handle errors
  }

  fileprivate func createInvoice(_ failure: @escaping (OmniException) -> Void, _ completion: @escaping (Invoice) -> Void) {
    let invoiceToCreate = Invoice()
    invoiceToCreate.total = request.amount.dollarsString()
    invoiceToCreate.url = "https://fattpay.com/#/bill"
    let invoiceMeta = [
      "subtotal": self.request.amount.dollarsString()
    ]

    guard let invoiceMetaJson = invoiceMeta.jsonValue() else {
      failure(Exception.couldNotCreateInvoice(detail: "Error generating json for meta"))
      return
    }

    invoiceToCreate.meta = invoiceMetaJson
    invoiceRepository.create(model: invoiceToCreate, completion: completion, error: failure)
  }

  fileprivate func availableMobileReaderDriver(_ repo: MobileReaderDriverRepository, _ failure: @escaping (OmniException) -> Void, _ completion: @escaping (MobileReaderDriver) -> Void) {
    repo.getDrivers { (drivers) in
      // TODO: Support multiple drivers
      guard let driver = drivers.first else {
        failure(TakeMobileReaderPaymentException.mobileReaderDriverNotFound)
        return
      }

      driver.isReadyToTakePayment(completion: { (ready) in
        if ready {
          completion(driver)
        } else {
          failure(TakeMobileReaderPaymentException.mobileReaderNotReady)
        }
      })
    }
  }

}
