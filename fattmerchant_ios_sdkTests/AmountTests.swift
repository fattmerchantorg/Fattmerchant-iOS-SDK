//
//  AmountTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import XCTest

class AmountTest: XCTestCase {

  func testCentsString() {
    XCTAssertEqual(Amount(cents: 50).centsString(), "50")
    XCTAssertEqual(Amount(dollars: 50.30).centsString(), "5030")
    XCTAssertEqual(Amount(dollars: 50.30).centsString(), "5030")
    XCTAssertEqual(Amount(dollars: 50.309).centsString(), "5030")
    XCTAssertEqual(Amount(dollars: 0.09).centsString(), "9")
  }

  func testDollarsString() {
    XCTAssertEqual(Amount(cents: 50).dollarsString(), "0.50")
    XCTAssertEqual(Amount(dollars: 50.30).dollarsString(), "50.30")
    XCTAssertEqual(Amount(dollars: 50.30).dollarsString(), "50.30")
    XCTAssertEqual(Amount(dollars: 50.309).dollarsString(), "50.30")
    XCTAssertEqual(Amount(dollars: 0.09).dollarsString(), "0.09")
  }

  func testPretty() {
    XCTAssertEqual(Amount(cents: 50).pretty(), "$0.50")
    XCTAssertEqual(Amount(dollars: 50.30).pretty(), "$50.30")
    XCTAssertEqual(Amount(dollars: 50.30).pretty(), "$50.30")
    XCTAssertEqual(Amount(dollars: 50.309).pretty(), "$50.30")
    XCTAssertEqual(Amount(dollars: 0.09).pretty(), "$0.09")
  }

  func testDollars() {
    XCTAssertEqual(Amount(cents: 50).dollars(), 0.50)
    XCTAssertEqual(Amount(dollars: 50.30).dollars(), 50.30)
    XCTAssertEqual(Amount(dollars: 50.30).dollars(), 50.30)
    XCTAssertEqual(Amount(dollars: 50.309).dollars(), 50.30)
    XCTAssertEqual(Amount(dollars: 0.09).dollars(), 0.09)
  }

  func testGetCents() {
    XCTAssertEqual(Amount(cents: 50).cents, 50)
    XCTAssertEqual(Amount(dollars: 50.30).cents, 5030)
    XCTAssertEqual(Amount(dollars: 50.30).cents, 5030)
    XCTAssertEqual(Amount(dollars: 50.309).cents, 5030)
    XCTAssertEqual(Amount(dollars: 0.09).cents, 9)
  }
}
