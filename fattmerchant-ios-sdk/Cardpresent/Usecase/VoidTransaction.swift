//
//  VoidTransaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 5/21/21.
//  Copyright © 2021 Fattmerchant. All rights reserved.
//

import Foundation

enum VoidTransactionException: StaxException {
    static var mess: String = "Could not capture funds"

    case errorCapturing
}

class VoidTransaction {

    typealias Exception = RefundException

    var staxApi: StaxApi
    var transactionId: String

    init(transactionId: String, captureAmount: Amount? = nil, staxApi: StaxApi) {
        self.transactionId = transactionId
        self.staxApi = staxApi
    }

    func start(completion: @escaping (Transaction) -> Void, error: @escaping (StaxException) -> Void) {
        self.staxApi.request(method: "POST",
                             urlString: "/transaction/\(transactionId)/void",
                             completion: completion,
                             failure: error)
    }

}
