//
//  InvoiceRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class InvoiceRepository: ModelRepository {
  typealias OmniModel = Invoice
  var omniApi: OmniApi

  required init(omniApi: OmniApi) {
    self.omniApi = omniApi
  }
}
