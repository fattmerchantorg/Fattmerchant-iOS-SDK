//
//  AmountTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import XCTest

class AmountTest: XCTestCase {

  func testInitWithDollars() {
    XCTAssertEqual(Amount(dollars: 5.9876).cents, 598)
    XCTAssertEqual(Amount(dollars: 2.07).cents, 207)
    XCTAssertEqual(Amount(dollars: 606.30).cents, 60630)

    // This code below checks every dollar amount from min to max to make sure that the Amount object
    // performs the dollars -> cents conversion correctly
    let min = 0.0
    let max = 1_000.0
    let increment = 0.01

    for dollars in stride(from: min, to: max, by: increment) {
      let expectedCents = Decimal(dollars) * Decimal(100.0)
      let expectedCentsInt = Int(expectedCents.description.split(separator: ".", maxSplits: 1, omittingEmptySubsequences: true).first!)!
      let amount = Amount(dollars: dollars)
      XCTAssertEqual(amount.cents, expectedCentsInt)
    }

  }

  func testCentsString() {
    XCTAssertEqual(Amount(cents: 50).centsString(), "50")
    XCTAssertEqual(Amount(dollars: 50.30).centsString(), "5030")
    XCTAssertEqual(Amount(dollars: 50.30).centsString(), "5030")
    XCTAssertEqual(Amount(dollars: 50.309).centsString(), "5030")
    XCTAssertEqual(Amount(dollars: 0.09).centsString(), "9")
    XCTAssertEqual(Amount(dollars: 606.30).centsString(), "60630")
  }

  func testDollarsString() {
    XCTAssertEqual(Amount(cents: 50).dollarsString(), "0.50")
    XCTAssertEqual(Amount(cents: 60630).dollarsString(), "606.30")
    XCTAssertEqual(Amount(dollars: 50.30).dollarsString(), "50.30")
    XCTAssertEqual(Amount(dollars: 50.30).dollarsString(), "50.30")
    XCTAssertEqual(Amount(dollars: 50.309).dollarsString(), "50.30")
    XCTAssertEqual(Amount(dollars: 0.09).dollarsString(), "0.09")
    XCTAssertEqual(Amount(dollars: 606.30).dollarsString(), "606.30")
  }

  func testPretty() {
    XCTAssertEqual(Amount(cents: 50).pretty(), "$0.50")
    XCTAssertEqual(Amount(dollars: 50.30).pretty(), "$50.30")
    XCTAssertEqual(Amount(dollars: 50.30).pretty(), "$50.30")
    XCTAssertEqual(Amount(dollars: 50.309).pretty(), "$50.30")
    XCTAssertEqual(Amount(dollars: 0.09).pretty(), "$0.09")
    XCTAssertEqual(Amount(dollars: 606.30).pretty(), "$606.30")
  }

  func testDollars() {
    XCTAssertEqual(Amount(cents: 50).dollars(), 0.50)
    XCTAssertEqual(Amount(dollars: 50.30).dollars(), 50.30)
    XCTAssertEqual(Amount(dollars: 50.30).dollars(), 50.30)
    XCTAssertEqual(Amount(dollars: 50.309).dollars(), 50.30)
    XCTAssertEqual(Amount(dollars: 0.09).dollars(), 0.09)
    XCTAssertEqual(Amount(dollars: 606.30).dollars(), 606.30)
  }

  func testGetCents() {
    XCTAssertEqual(Amount(cents: 50).cents, 50)
    XCTAssertEqual(Amount(dollars: 50.30).cents, 5030)
    XCTAssertEqual(Amount(dollars: 50.30).cents, 5030)
    XCTAssertEqual(Amount(dollars: 50.309).cents, 5030)
    XCTAssertEqual(Amount(dollars: 0.09).cents, 9)
    XCTAssertEqual(Amount(dollars: 606.30).cents, 60630)
  }
}
