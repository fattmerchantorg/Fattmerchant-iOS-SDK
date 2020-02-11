//
//  SearchForReaders.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum SearchForReadersException: OmniException {
  static var mess: String = "Couldnt find mobile readers"

  case noMobileReaderAvailable
}

class SearchForReaders {
  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var args: [String: Any]

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, args: [String: Any]) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.args = args
  }

  func start(completion: @escaping ([MobileReader]) -> Void, failure: (OmniException) -> Void) {
    mobileReaderDriverRepository.getDrivers { (drivers) in
      guard let driver = drivers.first else {
        failure(SearchForReadersException.noMobileReaderAvailable)
        return
      }

      driver.searchForReaders(args: args, completion: completion)
    }
  }
}
