//
//  CustomerRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class CustomerRepository: ModelRepository {
  typealias OmniModel = Customer
  var omniApi: OmniApi

  required init(omniApi: OmniApi) {
    self.omniApi = omniApi
  }
}
