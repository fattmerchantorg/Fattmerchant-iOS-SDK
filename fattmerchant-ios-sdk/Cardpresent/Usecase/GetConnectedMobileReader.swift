//
//  GetConnectedMobileReader.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 4/20/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum GetConnectedMobileReaderException: OmniException {
  static var mess: String = "Could not get connected mobile reader"

  case noReaderAvailable
}

class GetConnectedMobileReader {
  var mobileReaderDriverRepository: MobileReaderDriverRepository

  init(mobileReaderDriverRepository: MobileReaderDriverRepository) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
  }

  func start(completion: @escaping (MobileReader?) -> Void, failure: @escaping (OmniException) -> Void) {
    mobileReaderDriverRepository.getDrivers { drivers in
      guard let driver = drivers.first else {
        failure(GetConnectedMobileReaderException.noReaderAvailable)
        return
      }

      driver.getConnectedReader(completion: completion, error: failure)
    }
  }
}
