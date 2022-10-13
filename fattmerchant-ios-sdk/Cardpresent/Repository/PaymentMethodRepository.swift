//
//  PaymentMethodRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum CreatePaymentMethodException: OmniException {
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
    typealias OmniModel = PaymentMethod
    var omniApi: OmniApi

    required init(omniApi: OmniApi) {
        self.omniApi = omniApi
    }

    /// Creates an already-tokenized PaymentMethod in Omni
    /// - Parameters:
    ///   - model: The PaymentMethod to tokenize
    ///   - completion: A block to run once the operation is finished. Takes a PaymentMethod
    ///   - error: A block to run if the operation fails
    func createTokenizedPaymentMethod(model: PaymentMethod, completion: @escaping (PaymentMethod) -> Void, error: @escaping ErrorHandler) {
        let data = try? jsonEncoder().encode(model)
        let urlString = "/payment-method/token"
        omniApi.request(method: "post", urlString: urlString, body: data, completion: completion, failure: error)
    }

}
