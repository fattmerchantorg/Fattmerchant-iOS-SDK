//
//  MobileReaderDriverRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class MobileReaderDriverRepository {

  #if targetEnvironment(simulator)
  var driver = MockDriver()
  #else
  var driver = ChipDnaDriver()
  #endif

  func getDrivers(completion: ([MobileReaderDriver]) -> Void) {
    completion([driver])
  }

  func getDriverFor(transaction: Transaction, completion: (MobileReaderDriver?) -> Void) {
    completion(transaction.source?.contains("CPSDK") == true ? driver : nil)
  }

  func getDriverFor(mobileReader: MobileReader, completion: (MobileReaderDriver?) -> Void) {
    completion(driver)
  }

}
