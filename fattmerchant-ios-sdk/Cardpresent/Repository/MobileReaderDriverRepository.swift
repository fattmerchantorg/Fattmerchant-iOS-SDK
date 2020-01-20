//
//  MobileReaderDriverRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class MobileReaderDriverRepository {

  var chipDnaDriver = ChipDnaDriver()

  func getDrivers(completion: ([MobileReaderDriver]) -> Void) {
    print("getDrivers, returning chipDnaDriver")
    completion([chipDnaDriver])
  }

  func getDriverFor(transaction: Transaction, completion: (MobileReaderDriver?) -> Void) {
    print("getDriverForTransaction")
    completion(transaction.source?.contains("CPSDK") == true ? chipDnaDriver : nil)
  }

  func getDriverFor(mobileReader: MobileReader, completion: (MobileReaderDriver?) -> Void) {
    print("getDriverForMobileReader")
    completion(chipDnaDriver)
  }

}
