//
//  MobileReaderDriverRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class MobileReaderDriverRepository {

  #if (arch(i386) || arch(x86_64))
  var driver = MockDriver()
  #else
  var driver = ChipDnaDriver()
  #endif

  func getDrivers(completion: ([MobileReaderDriver]) -> Void) {
    print("getDrivers, returning chipDnaDriver")
    completion([driver])
  }

  func getDriverFor(transaction: Transaction, completion: (MobileReaderDriver?) -> Void) {
    print("getDriverForTransaction")
    completion(transaction.source?.contains("CPSDK") == true ? driver : nil)
  }

  func getDriverFor(mobileReader: MobileReader, completion: (MobileReaderDriver?) -> Void) {
    print("getDriverForMobileReader")
    completion(driver)
  }

}
