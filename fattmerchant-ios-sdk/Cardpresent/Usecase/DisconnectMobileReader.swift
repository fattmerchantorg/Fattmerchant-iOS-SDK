//
//  DisconnectMobileReader.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 4/20/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum DisconnectMobileReaderException: OmniException {
  static var mess: String = "Could not disconnect mobile reader"

  case driverNotFound
}

class DisconnectMobileReader {
  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var mobileReader: MobileReader

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, mobileReader: MobileReader) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.mobileReader = mobileReader
  }

  func start(completion: @escaping (Bool) -> Void, failure: @escaping (OmniException) -> Void) {
    mobileReaderDriverRepository.getDriverFor(mobileReader: mobileReader) {
      if let driver = $0 {
        driver.disconnect(reader: self.mobileReader, completion: completion, error: failure)
      } else {
        failure(DisconnectMobileReaderException.driverNotFound)
      }
    }
  }
}
