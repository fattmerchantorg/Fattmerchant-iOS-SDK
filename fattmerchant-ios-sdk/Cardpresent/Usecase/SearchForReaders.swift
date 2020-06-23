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

  func start(completion: @escaping ([MobileReader]) -> Void, failure: @escaping (OmniException) -> Void) {
    mobileReaderDriverRepository.getInitializedDrivers { (drivers) in
      guard !drivers.isEmpty else {
        failure(SearchForReadersException.noMobileReaderAvailable)
        return
      }

      let dispatchGroup = DispatchGroup()
      var allReaders: [MobileReader] = []

      drivers.forEach { driver in
        dispatchGroup.enter()
        driver.searchForReaders(args: self.args) { readers in
          allReaders.append(contentsOf: readers)
          dispatchGroup.leave()
        }
      }

      dispatchGroup.notify(queue: .global(qos: .userInitiated)) {
        completion(allReaders)
      }
    }
  }
}
