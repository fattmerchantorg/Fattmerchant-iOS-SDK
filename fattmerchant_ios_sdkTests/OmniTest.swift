//
//  OmniTest.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/31/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class OmniTest: XCTestCase {

  func testUninitializedOmniThrowsError() {

    let omni = Omni()

    func errorBlock(expectation: XCTestExpectation) -> (OmniException) -> Void {
      return { exception in
        switch exception {
        case OmniGeneralException.uninitialized:
          expectation.fulfill()
        default:
          XCTFail("Omni expected to throw an uninitialized exception, but didn't")
        }
      }
    }

    // Teset that connect reader fails
    let connectReaderFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.connect(reader: MockMobileReader(name: "reader"), completion: { _ in
      XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }) {
      connectReaderFails.fulfill()
    }

    // Test that get available readers fails
    let getAvailableReadersFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.getAvailableReaders(completion: { _ in
      XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: getAvailableReadersFails))

    // Test that get mobile reader transactions fails
    let getMobileReaderTransactionsFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.getMobileReaderTransactions(completion: { _ in
      XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: getMobileReaderTransactionsFails))

    // Test that refund fails
    let refundMobileReaderFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.refundMobileReaderTransaction(transaction: Transaction(), completion: { _ in
      XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: refundMobileReaderFails))

    wait(for: [
      connectReaderFails,
      getAvailableReadersFails,
      getMobileReaderTransactionsFails,
      refundMobileReaderFails
    ], timeout: 10)
  }

}
