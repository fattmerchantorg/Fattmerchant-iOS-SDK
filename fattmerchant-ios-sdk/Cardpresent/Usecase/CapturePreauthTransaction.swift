//
//  CapturePreauthTransaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 5/21/21.
//  Copyright © 2021 Fattmerchant. All rights reserved.
//

import Foundation

class CapturePreauthTransaction {

  typealias Exception = RefundException

  var omniApi: OmniApi
  var transactionId: String
  var captureAmount: Amount?

  init(transactionId: String, captureAmount: Amount? = nil, omniApi: OmniApi) {
    self.transactionId = transactionId
    self.captureAmount = captureAmount
    self.omniApi = omniApi
  }

  func start(completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    // As of 5/21/21, only NMI supports preauth and only NMI is offered to partners so we really shouldn't
    // be hitting this code for anything except NMI. With that assumption, we can get away with asking the Stax API
    // to perform the capture for us

    self.omniApi.captureTransaction(id: transactionId, captureAmount: captureAmount, completion: completion) { (errorString) in
      error(self.exception(from: errorString))
    }
  }

  func exception(from errorString: String?) -> CapturePreauthTransactionException {
    guard let error = errorString else { return .errorCapturing(detail: errorString) }

    if error.contains("exceeds the authorization amount") {
      return .captureAmountExceedsAuth(detail: error)
    }

    return .errorCapturing(detail: error)
  }

}
