//
//  CustomerRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum CreateCustomerException: StaxException {
    case customerNameNotSupplied
    case unknown

    static var mess = "Could not create customer"

    var detail: String? {
        switch self {
        case .customerNameNotSupplied:
            return "Customer name is required"

        default:
            return "Unknown error creating customer"
        }
    }

}

class CustomerRepository: ModelRepository {
    typealias StaxModel = Customer
    var staxApi: StaxApi

    required init(staxApi: StaxApi) {
        self.staxApi = staxApi
    }
}
