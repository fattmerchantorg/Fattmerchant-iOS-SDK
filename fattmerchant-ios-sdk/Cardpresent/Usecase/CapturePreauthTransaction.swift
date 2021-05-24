//
//  CapturePreauthTransaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 5/21/21.
//  Copyright © 2021 Fattmerchant. All rights reserved.
//

import Foundation

enum CapturePreauthTransactionException: OmniException {
  static var mess: String = "Could not capture funds"

  case errorCapturing
}

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

    var body: Data?

    if let amount = captureAmount, let data = try? JSONEncoder().encode( ["total": amount.dollars()] ) {
      body = data
    }

    self.omniApi.request(method: "POST",
                         urlString: "/transaction/\(transactionId)/capture",
                         body: body,
                         completion: completion,
                         failure: error)
  }

}
