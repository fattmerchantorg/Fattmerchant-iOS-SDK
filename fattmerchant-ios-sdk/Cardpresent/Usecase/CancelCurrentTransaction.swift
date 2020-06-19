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
    mobileReaderDriverRepository.driver.cancelCurrentTransaction(completion: completion, error: error)
  }

}
