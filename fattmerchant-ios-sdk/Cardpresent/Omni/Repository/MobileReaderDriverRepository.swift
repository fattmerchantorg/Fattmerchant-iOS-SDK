//
//  MobileReaderDriverRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class MobileReaderDriverRepository {

  func all() -> [MobileReaderDriver] {
    #if targetEnvironment(simulator)
      return [MockDriver()]
    #else
      return [ChipDnaDriver()]
    #endif
  }

  /// Gets all MobileReaderDrivers
  func getDrivers() -> [MobileReaderDriver] {
    return all()
  }

  /// Gets the MobileReaderDrivers which have been initialized
  func getInitializedDrivers(completion: @escaping ([MobileReaderDriver]) -> Void) {
    all().filterAsync(predicate: { driver in
      AsyncStream { continuation in
        driver.isInitialized { result in
          continuation.yield(result)
          continuation.finish()
        }
      }
    }) { filtered in
      completion(filtered)
    }
  }

  func getDriverFor(transaction: Transaction, completion: (MobileReaderDriver?) -> Void) {
    // Make sure the transaction is a CPSDK transaction
    guard transaction.source?.contains("CPSDK") == true else {
      return completion(nil)
    }

    // Find the driver that has the source matching the transaction
    let driverWithMatchingSource = all().first { driver in
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
