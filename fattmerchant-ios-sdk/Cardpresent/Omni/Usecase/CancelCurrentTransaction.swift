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

  case noTransactionToCancel
  case unknown

  var detail: String? {
    switch self {
    case .noTransactionToCancel:
      return "There is no transaction to cancel"

    case .unknown:
      return "Unkown error"
    }
  }
}

class CancelCurrentTransaction {
  internal var mobileReaderDriverRepository: MobileReaderDriverRepository

  init(mobileReaderDriverRepository: MobileReaderDriverRepository) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
  }

  func start(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    mobileReaderDriverRepository.getInitializedDrivers { drivers in
      let semaphore: DispatchSemaphore? = DispatchSemaphore(value: 1)
      var success = true
      var omniException: OmniException?

      DispatchQueue.global(qos: .userInitiated).async {
        for driver in drivers {
          semaphore?.wait()

          driver.cancelCurrentTransaction(completion: { cancelled in
            success = success && cancelled
            semaphore?.signal()
          }, error: { err in
            omniException = err
            success = false
            semaphore?.signal()
          })
        }

        if let exception = omniException {
          error(exception)
        } else {
          completion(success)
        }
      }
    }
  }

}
