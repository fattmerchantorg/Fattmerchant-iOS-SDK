//
//  ConnectMobileReader.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class ConnectMobileReader {

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var mobileReader: MobileReader

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, mobileReader: MobileReader) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.mobileReader = mobileReader
  }

  func start(completion: @escaping (Bool) -> Void) {
    mobileReaderDriverRepository.getDriverFor(mobileReader: mobileReader) { (driver) in
      if let driver = driver {
        driver.connect(reader: mobileReader, completion: completion)
      } else {
        completion(false)
      }

    }
  }

}
