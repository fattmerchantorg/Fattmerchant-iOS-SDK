//
//  InitializeDrivers.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum InitializeDriversException: OmniException {
  static var mess: String = "Could not initialize driver"

  case noMobileReadersFound

  var detail: String? {
    switch self {
    case .noMobileReadersFound:
      return "Couldn't find any mobile readers"
    }
  }
}

class InitializeDrivers {

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var args: [String: Any]

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, args: [String: Any]) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.args = args
  }

  func start(completion: (Bool) -> Void, failure: (OmniException) -> Void) {
    mobileReaderDriverRepository.getDrivers { (drivers) in

      guard let driver = drivers.first else {
        failure(InitializeDriversException.noMobileReadersFound)
        return
      }

      driver.initialize(args: args, completion: completion)
    }
  }
}
