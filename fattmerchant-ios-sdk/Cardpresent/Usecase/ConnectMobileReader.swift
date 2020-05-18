//
//  ConnectMobileReader.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum ConnectMobileReaderException: OmniException {
  static var mess: String = "Could not connect mobile reader"

  case couldNotConnectMobileReader(reader: MobileReader)
}

class ConnectMobileReader {

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var mobileReader: MobileReader

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, mobileReader: MobileReader) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.mobileReader = mobileReader
  }

  func start(onConnected: @escaping (MobileReader) -> Void, onFailed: @escaping(OmniException) -> Void) {

    // First, try to see if the MobileReaderDriverRepo knows which Driver this MobileReader belongs to
    mobileReaderDriverRepository.getDriverFor(mobileReader: mobileReader) { (driver) in

      if let driver = driver {
        driver.connect(reader: self.mobileReader, completion: { connected in
          if connected {
            onConnected(self.mobileReader)
          } else {
            onFailed(ConnectMobileReaderException.couldNotConnectMobileReader(reader: self.mobileReader))
          }
        })
      } else {
        // Try to connect the reader with each driver
        let drivers = self.mobileReaderDriverRepository.allDrivers()
        var callbackExecuted = false
        let semaphore = DispatchSemaphore(value: 1)

        DispatchQueue.global(qos: .userInitiated).async {
          for driver in drivers {
            semaphore.wait()

            if callbackExecuted {
              return
            }

            driver.connect(reader: self.mobileReader) { connected in
              if connected {
                onConnected(self.mobileReader)
                callbackExecuted = true
              } else {
                onFailed(ConnectMobileReaderException.couldNotConnectMobileReader(reader: self.mobileReader))
              }

              semaphore.signal()
              return
            }

          }
        }
      }

    }

  }

}
