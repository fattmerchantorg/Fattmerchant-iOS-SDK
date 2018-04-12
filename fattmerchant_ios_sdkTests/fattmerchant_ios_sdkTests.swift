//
//  fattmerchant_ios_sdkTests.swift
//  fattmerchant_ios_sdkTests
//
//  Created by Tulio Troncoso on 3/30/18.
//  Copyright © 2018 Fattmerchant, inc. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

import XCTest
@testable import Fattmerchant

class fattmerchant_ios_sdkTests: XCTestCase {

  var configuration: FattmerchantConfiguration!
  var fattClient: FattmerchantApi!

  override func setUp() {
    super.setUp()
    configuration = FattmerchantConfiguration(hostedPaymentsId: "fattwars")
    configuration.baseUrl = URL(string: "https://apidev01.fattlabs.com")!
    fattClient = FattmerchantApi(configuration!)
  }

  /// Tests creating a token out of a valid credit card
  func testTokenizingCreditCard() {

    let expectation = XCTestExpectation(description: "Get a response from Fattmerchant")

    var card = CreditCard(personName: "Joan Parsnip",
                              cardNumber: "4111111111111111",
                              cardExp: "1220",
                              addressZip: "32814")

    card.email = "testdude@fattmerchant.com"
    card.phone = "3059269999"
    card.address1 = "123 Somestreet"
    card.address2 = "Unit 304"
    card.addressCity = "Orlando"
    card.note = "hello, note-o"

    fattClient.tokenize(card) { response in

      switch response {
      case .success(let paymentMethod):
        XCTAssertEqual(paymentMethod.cardLastFour, "1111")
        XCTAssertEqual(paymentMethod.personName, "Joan Parsnip")

      case .failure:
        XCTFail("Did not get Payment Method from tokenizing credit card")
      }

      expectation.fulfill()
    }

    wait(for: [expectation], timeout: 10.0)
  }

  /// Tests creating a token out of a valid bank account
  func testTokenizingBankAccount() {
    let expectation = XCTestExpectation(description: "Get a response from Fattmerchant")

    var bankAccount = BankAccount(personName: "Jim Parsnip",
                                  bankType: "savings",
                                  bankHolderType: .personal,
                                  bankAccount: "9876543210",
                                  bankRouting: "021000021",
                                  addressZip: "32814")

    bankAccount.email = "testdude@fattmerchant.com"
    bankAccount.phone = "3059269999"
    bankAccount.address1 = "123 Somestreet"
    bankAccount.address2 = "Unit 304"
    bankAccount.addressCity = "Orlando"
    bankAccount.note = "hello, note-o"

    fattClient.tokenize(bankAccount) { (response) in
      switch response {
      case .success(let paymentMethod):
        XCTAssertEqual(paymentMethod.cardLastFour, "3210")

      case .failure:
        XCTFail("Did not get Payment Method from tokenizing credit card")
      }

      expectation.fulfill()
    }

    wait(for: [expectation], timeout: 10.0)
  }

  func testTokenizingTestCreditCard() {
    let expectation = XCTestExpectation(description: "Get a response from Fattmerchant")
    fattClient.tokenize(CreditCard.testCreditCard()) { (response) in
      switch response {
      case .success(let paymentMethod):
        XCTAssertEqual(paymentMethod.cardLastFour, "1111")
        XCTAssertEqual(paymentMethod.personName, "Joan Parsnip")

      case .failure:
        XCTFail("Did not get Payment Method from tokenizing credit card")
      }

      expectation.fulfill()
    }
    wait(for: [expectation], timeout: 10.0)
  }

  func testFailingTestCreditCard() {
    let expectation = XCTestExpectation(description: "Get a response from Fattmerchant")
    fattClient.tokenize(CreditCard.failingTestCreditCard()) { (response) in
      switch response {
      case .success:
        XCTFail("Tokenized a card when expected to fail")

      case .failure(let error):
        if case let FattmerchantApi.Error.tokenizationError(messages) = error {
          XCTAssertTrue(messages.count > 0)
        } else {
          XCTFail()
        }
      }
      expectation.fulfill()
    }
    wait(for: [expectation], timeout: 10.0)
  }

  func testTokenizingTestBankAccount() {
    let expectation = XCTestExpectation(description: "Get a response from Fattmerchant")
    fattClient.tokenize(BankAccount.testBankAccount()) { (response) in
      guard case let FattmerchantApi.Response.success(paymentMethod) = response else {
        XCTFail("Did not get payment method from tokenizing bank account")
        return
      }
      XCTAssertEqual(paymentMethod.cardLastFour, "3210")
      expectation.fulfill()
    }
    wait(for: [expectation], timeout: 10.0)
  }

  func testFailingTestBankAccount() {
    let expectation = XCTestExpectation(description: "Get a response from Fattmerchant")
    fattClient.tokenize(BankAccount.failingTestBankAccount()) { (response) in
      guard case FattmerchantApi.Response.failure(_) = response else {
        XCTFail("Was expecint errors, but didn't get any")
        return
      }
      expectation.fulfill()
    }
    wait(for: [expectation], timeout: 10.0)
  }

}
