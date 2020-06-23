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

  /// Gets all MobileReaderDrivers
  func getDrivers(completion: ([MobileReaderDriver]) -> Void) {
    completion(allDrivers())
  }

  /// Gets the MobileReaderDrivers which have been initialized
  func getInitializedDrivers(completion: @escaping ([MobileReaderDriver]) -> Void) {
    filter(items: allDrivers(), predicate: { $0.isInitialized }, completion: completion)
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

  func getDriverFor(mobileReader: MobileReader, completion: @escaping (MobileReaderDriver?) -> Void) {
    // Try to get a driver who is familiar with this reader first
    guard let serialNumber = mobileReader.serialNumber else {
      return completion(nil)
    }

    // Try to get the driver which is familiar with this mobileReader
    getInitializedDrivers { initializedDrivers in
      let familiarDriver = initializedDrivers.first { driver in
        driver.familiarSerialNumbers.contains(serialNumber)
      }

      completion(familiarDriver)
    }
  }

}
