//
//  ChipDnaXMLTransactionParserTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 6/1/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class ChipDnaXMLTransactionParserTests: XCTestCase {

  func testCanParseCcExpirationDateFromTransaction() {
    let expectation = XCTestExpectation(description: "Parsed cc expiration date")
    let xmlData = ChipDnaTransactionXmlResponse.xml.data(using: .utf8)!
    let transactionId = ChipDnaTransactionXmlResponse.transactionId
    let expectedCcExpiration = ChipDnaTransactionXmlResponse.ccExpiration
    let parser = ChipDnaXMLTransactionParser()

    parser.parseExpirationDate(from: xmlData, transactionId: transactionId) { ccExpiration in
      XCTAssertEqual(ccExpiration, expectedCcExpiration)
      expectation.fulfill()
    }

    wait(for: [expectation], timeout: 3.0)
  }

  func testFailsIfEmptyTransactionIdGiven() {
    let expectation = XCTestExpectation(description: "Nil ccExpiration returned")
    let xmlData = ChipDnaTransactionXmlResponse.xml.data(using: .utf8)!
    let parser = ChipDnaXMLTransactionParser()

    parser.parseExpirationDate(from: xmlData, transactionId: "") { ccExpiration in
      XCTAssertNil(ccExpiration)
      expectation.fulfill()
    }

    wait(for: [expectation], timeout: 3.0)
  }
}
