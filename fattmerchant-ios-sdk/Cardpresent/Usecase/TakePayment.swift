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

  /// The Merchant that will be associated with the Transaction
  var merchant: Merchant

  /// Initializes a `TakePayment` with a `TransactionRequest`, an `OmniApi`, and a `Merchant`
  init(request: TransactionRequest, omniApi: OmniApi, merchant: Merchant) {
    self.request = request
    self.merchant = merchant
    self.omniApi = omniApi
  }

  /// Kicks off the process of taking the payment
  /// - Parameters:
  ///   - completion: A block to call once finished. This will receive the completed `Transaction`
  ///   - failure: a block to call if a failure occurs. This will receive an `OmniException`
  func start(completion: @escaping (Transaction) -> Void, failure: @escaping (OmniException) -> Void) {
    // Make sure a valid payment method was provided
    guard let card = request.card else {
      failure(Exception.couldNotTokenizePaymentMethod(detail: "No payment method provided"))
      return
    }

    // Tokenize the payment method first then pay using the Omni API and the tokenized Payment Method
    let tokenizeJob = TokenizePaymentMethod(omniApi: omniApi, merchant: merchant)
    tokenizeJob.start(codablePaymentMethod: card, completion: { (tokenizedPaymentMethod) in
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
