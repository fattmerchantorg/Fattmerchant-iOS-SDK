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
}
