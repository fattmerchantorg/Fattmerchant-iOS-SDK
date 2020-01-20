//
//  CustomerRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum CreateCustomerException: OmniException {
  case Something(String)

  static var mess = "Could not create customer"

  var detail: String? {
    switch self {
    case .Something(let desc):
      return desc
    }
  }

}

class CustomerRepository: ModelRepository {
  typealias OmniModel = Customer
}
