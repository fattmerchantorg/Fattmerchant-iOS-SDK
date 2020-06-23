//
//  InitializeDrivers.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum InitializeDriversException: OmniException {
  static var mess: String = "Could not initialize driver"

  case noMobileReadersFound

  var detail: String? {
    switch self {
    case .noMobileReadersFound:
      return "Couldn't find any mobile readers"
    }
  }
}

class InitializeDrivers {

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var args: [String: Any]

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, args: [String: Any]) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.args = args
  }

  func start(completion: @escaping (Bool) -> Void, failure: (OmniException) -> Void) {
    mobileReaderDriverRepository.getDrivers { (drivers) in

      // Make sure we have at least one available driver
      guard !drivers.isEmpty else {
        failure(InitializeDriversException.noMobileReadersFound)
        return
      }

      /*
       Initialize all the drivers.

       The strategy here is to initialize all drivers concurrently. Since they each take a completion block, we will
       kick off the initialize function for all of them and wait for the completion blocks to resolve.

       We know we are done once all the completion blocks from the initialize functions have been executed. However,
       since they can come back in different orders and at unpredictable times, we need to keep track of how many of
       them have been executed. For this reason, we have a variable, `completionBlocksExecuted`. When that count hits
       the number of available drivers, we know all drivers have attempted initialization and we are free to call the
       completion block for the start(:) method.

       Keeping an accurate count of `completionBlocksExecuted` is tricky, because we don't know which line is being
       executed for each initialize completion block. A problem that might occur is completion block A can be checking
       for `completionBlocksExecuted` == `someValue` while completion block B is mutating `completionBlocksExecuted`,
       which can mess up the equality check. For this reason, we create a Semaphore that makes a completion block wait
       until the currently-executing one is finished.
       */

      let semaphore = DispatchSemaphore(value: 1)
      var driversInitialized = 0
      var completionBlocksExecuted = 0

      for i in 0..<drivers.count {
        let driver = drivers[i]
        driver.initialize(args: args) { success in
          semaphore.wait()
          completionBlocksExecuted += 1

          if success {
            driversInitialized += 1
          }

          // If we've finished initializing them all, then call the completion block
          if completionBlocksExecuted == drivers.count {
            completion(driversInitialized > 0)
            semaphore.signal()
            return
          }

          semaphore.signal()
        }
      }

    }
  }
}
