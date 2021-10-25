//
//  MockMobileReaderDriverRepository.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/31/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class MockMobileReaderDriverRepository: MobileReaderDriverRepository {

  /// The available drivers
  fileprivate let drivers: [MobileReaderDriver] = [
    MockDriver()
  ]

  override func getDrivers(completion: ([MobileReaderDriver]) -> Void) {
    completion(drivers)
  }

}
