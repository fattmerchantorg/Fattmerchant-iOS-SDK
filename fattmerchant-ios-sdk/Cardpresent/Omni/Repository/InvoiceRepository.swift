//
//  InvoiceRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum CreateInvoiceException: OmniException {
  case Something(String)

  static var mess = "Could not create customer"

  var detail: String? {
    switch self {
    case .Something(let desc):
      return desc
    }
  }

}

class InvoiceRepository: ModelRepository {
  typealias OmniModel = Invoice
  var omniApi: OmniApi

  required init(omniApi: OmniApi) {
    self.omniApi = omniApi
  }
}
