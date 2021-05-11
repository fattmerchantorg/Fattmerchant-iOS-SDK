//
//  TakeCNPPayment.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 3/31/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum TakePaymentException: OmniException {
  case couldNotTokenizePaymentMethod(detail: String? = nil)

  static var mess: String = "Error taking mobile reader payment"

  var detail: String? {
    switch self {
    case .couldNotTokenizePaymentMethod(let d):
      return d ?? "Could not tokenize payment method"
    }
  }

}

/// Takes a payment with the given TransactionRequest
///
/// `TakePayment` will create a payment in Omni using the payment method given in `request`. It will charge the `Amount`
/// specified in `request`
class TakePayment {

  typealias Exception = TakePaymentException

  /// The TransactionRequest that holds all the data necessary to take this payment
  var request: TransactionRequest

  /// Responsible for communicating with Omni
  var omniApi: OmniApi

  private var customerRepository: CustomerRepository
  private var paymentMethodRepository: PaymentMethodRepository

  init(request: TransactionRequest,
       customerRepository: CustomerRepository,
       paymentMethodRepository: PaymentMethodRepository) {
    self.request = request
    self.customerRepository = customerRepository
    self.paymentMethodRepository = paymentMethodRepository
    self.omniApi = customerRepository.omniApi
  }

  /// Kicks off the process of taking the payment
  /// - Parameters:
  ///   - completion: A block to call once finished. This will receive the completed `Transaction`
  ///   - failure: a block to call if a failure occurs. This will receive an `OmniException`
  func start(completion: @escaping (Transaction) -> Void, failure: @escaping (OmniException) -> Void) {
    let tokenize = tokenizeJob()
    guard let tokenizeJob = tokenize.0 else {
      return failure(tokenize.1 ?? Exception.couldNotTokenizePaymentMethod(detail: "No payment method provided"))
    }

    // Tokenize the payment method first
    tokenizeJob.start(completion: { (tokenizedPaymentMethod) in
      // Ensure that the tokenized payment method has an ID
      guard let paymentMethodId = tokenizedPaymentMethod.id else {
        failure(Exception.couldNotTokenizePaymentMethod())
        return
      }

      // Create the request body
      let chargeRequest = Self.createChargeRequest(amount: self.request.amount, paymentMethodId: paymentMethodId)
      let body = Data(chargeRequest: chargeRequest)

      // Make the request to Omni
      self.omniApi.request(method: "post", urlString: "/charge", body: body, completion: completion, failure: failure)
    }, failure: failure)
  }

  /// Creates a TokenizePaymentMethod job from the TransactionRequest
  internal func tokenizeJob() -> (TokenizePaymentMethod?, OmniException?) {
    var job: TokenizePaymentMethod

    if let card = request.card {
      job = TokenizePaymentMethod(customerRepository: customerRepository,
                                   paymentMethodRepository: paymentMethodRepository,
                                   creditCard: card)
    } else if let bank = request.bankAccount {
      job = TokenizePaymentMethod(customerRepository: customerRepository,
                                   paymentMethodRepository: paymentMethodRepository,
                                   bankAccount: bank)
    } else {
      return (nil, Exception.couldNotTokenizePaymentMethod(detail: "No payment method provided"))
    }

    return (job, nil)
  }

  /// Creates a ChargeRequest from an Amount and a PaymentMethod id
  /// - Parameters:
  ///   - amount: the Amount to charge
  ///   - paymentMethodId: the id of the PaymentMethod to charge
  /// - Returns: a ChargeRequest
  internal static func createChargeRequest(amount: Amount, paymentMethodId: String) -> ChargeRequest {
    let chargeRequestMeta = [
      "subtotal": amount.dollarsString()
    ]

    return ChargeRequest(paymentMethodId: paymentMethodId,
                                      total: amount.dollarsString(),
                                      preAuth: false,
                                      meta: chargeRequestMeta)
  }

}
