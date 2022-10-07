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
    weak var mobileReaderConnectionStatusDelegate: MobileReaderConnectionStatusDelegate?

    init(mobileReaderDriverRepository: MobileReaderDriverRepository,
         mobileReader: MobileReader,
         mobileReaderConnectionStatusDelegate: MobileReaderConnectionStatusDelegate? = nil) {
        self.mobileReaderDriverRepository = mobileReaderDriverRepository
        self.mobileReader = mobileReader
        self.mobileReaderConnectionStatusDelegate = mobileReaderConnectionStatusDelegate
    }

    func start(onConnected: @escaping (MobileReader) -> Void, onFailed: @escaping(OmniException) -> Void) {

        // First, try to see if the MobileReaderDriverRepo knows which Driver this MobileReader belongs to
        mobileReaderDriverRepository.getDriverFor(mobileReader: mobileReader) { (driver) in
            if var driver = driver {
                driver.mobileReaderConnectionStatusDelegate = self.mobileReaderConnectionStatusDelegate
                driver.connect(reader: self.mobileReader, completion: { connectedReader in
                    if let connectedReader = connectedReader {
                        onConnected(connectedReader)
                    } else {
                        onFailed(ConnectMobileReaderException.couldNotConnectMobileReader(reader: self.mobileReader))
                    }
                })
            } else {
                // Try to connect the reader with each initialized driver
                self.mobileReaderDriverRepository.getInitializedDrivers { drivers in
                    var callbackExecuted = false
                    let semaphore = DispatchSemaphore(value: 1)

                    DispatchQueue.global(qos: .userInitiated).async {
                        for var driver in drivers {
                            driver.mobileReaderConnectionStatusDelegate = self.mobileReaderConnectionStatusDelegate
                            semaphore.wait()

                            if callbackExecuted {
                                return
                            }

                            driver.connect(reader: self.mobileReader) { connectedReader in
                                if let connectedReader = connectedReader {
                                    onConnected(connectedReader)
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

}
