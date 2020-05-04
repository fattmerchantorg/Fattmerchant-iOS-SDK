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

  func allDrivers() -> [MockDriver] {
    return [driver]
  }

  #else
  var chipDnaDriver = ChipDnaDriver()
  var awcDriver = AWCDriver()

  func allDrivers() -> [MobileReaderDriver] {
    return [chipDnaDriver, awcDriver]
  }
  #endif

  func getDrivers(completion: ([MobileReaderDriver]) -> Void) {
    completion(allDrivers())
  }

  func getDriverFor(transaction: Transaction, completion: (MobileReaderDriver?) -> Void) {
    // Make sure the transaction is a CPSDK transaction
    guard transaction.source?.contains("CPSDK") == true else {
      return completion(nil)
    }

    // Find the driver that has the source matching the transaction
    let driverWithMatchingSource = allDrivers().first { driver in
      let driverSource = type(of: driver).source
      let transactionSource = transaction.source
      return transactionSource?.contains(driverSource) == true
    }

    completion(driverWithMatchingSource)
  }

  func getDriverFor(mobileReader: MobileReader, completion: (MobileReaderDriver?) -> Void) {
    completion(allDrivers().first!)
  }

}
