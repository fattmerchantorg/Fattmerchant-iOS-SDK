//
//  VoidTransaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 5/21/21.
//  Copyright © 2021 Fattmerchant. All rights reserved.
//

import Foundation

enum VoidTransactionException: OmniException {
  static var mess: String = "Could not capture funds"

  case errorCapturing
}

class VoidTransaction {

  typealias Exception = RefundException

  var omniApi: OmniApi
  var transactionId: String

  init(transactionId: String, captureAmount: Amount? = nil, omniApi: OmniApi) {
    self.transactionId = transactionId
    self.omniApi = omniApi
  }

  func start(completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    self.omniApi.request(method: "POST",
                         urlString: "/transaction/\(transactionId)/void",
                         completion: completion,
                         failure: error)
  }

}
