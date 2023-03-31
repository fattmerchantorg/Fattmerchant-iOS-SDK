//
//  PaymentMethodRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum CreatePaymentMethodException: StaxException {
    case Something(String)

    static var mess = "Could not create payment method"

    var detail: String? {
        switch self {
        case .Something(let desc):
            return desc
        }
    }

}

class PaymentMethodRepository: ModelRepository {
    typealias StaxModel = PaymentMethod
    var staxApi: StaxApi

    required init(staxApi: StaxApi) {
        self.staxApi = staxApi
    }

    /// Creates an already-tokenized PaymentMethod in Stax
    /// - Parameters:
    ///   - model: The PaymentMethod to tokenize
    ///   - completion: A block to run once the operation is finished. Takes a PaymentMethod
    ///   - error: A block to run if the operation fails
    func createTokenizedPaymentMethod(model: PaymentMethod, completion: @escaping (PaymentMethod) -> Void, error: @escaping ErrorHandler) {
        let data = try? jsonEncoder().encode(model)
        let urlString = "/payment-method/token"
        staxApi.request(method: "post", urlString: urlString, body: data, completion: completion, failure: error)
    }

}
