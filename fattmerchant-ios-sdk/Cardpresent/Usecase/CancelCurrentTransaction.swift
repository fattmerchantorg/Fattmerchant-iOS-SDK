//
//  CancelCurrentTransaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 6/18/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum CancelCurrentTransactionException: OmniException {
static var mess: String = "Could not cancel current transaction"
}

class CancelCurrentTransaction {
  internal var mobileReaderDriverRepository: MobileReaderDriverRepository

  init(mobileReaderDriverRepository: MobileReaderDriverRepository) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
  }

  func start(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    mobileReaderDriverRepository.getInitializedDrivers { drivers in
      let semaphore = DispatchSemaphore(value: 1)
      var success = true

      DispatchQueue.global(qos: .userInitiated).async {
        for driver in drivers {
          semaphore.wait()

          driver.cancelCurrentTransaction(completion: { cancelled in
            success = success && cancelled
          }) { _ in
            success = false
          }

          semaphore.signal()
        }

        semaphore.wait()
        completion(success)
      }
    }
  }

}
