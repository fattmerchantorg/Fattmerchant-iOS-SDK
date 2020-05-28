//
//  DisconnectMobileReaderTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 4/20/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class DisconnectMobileReaderTests: XCTestCase {

  func testCanDisconnectMobileReader() {
    let reader = MobileReader(name: "Mock Reader", serialNumber: "1234")
    let driverRepo = MobileReaderDriverRepository()
    driverRepo.driver.reader = reader
    driverRepo.driver.familiarSerialNumbers.append(reader.serialNumber!)

    let expectation = XCTestExpectation(description: "Mobile Reader was disconnected")
    DisconnectMobileReader(mobileReaderDriverRepository: driverRepo, mobileReader: reader).start(completion: { success in
      XCTAssert(success)
      expectation.fulfill()
    }) { error in

    }

    wait(for: [expectation], timeout: 10.0)
  }

}
