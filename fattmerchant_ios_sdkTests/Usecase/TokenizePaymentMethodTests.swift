//
//  TokenizePaymentMethodTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 4/19/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class TokenizePaymentMethodTests: XCTestCase {

  let hostedPaymentsToken = "hostedpaymentstoken"
  var merchant: Merchant = Merchant()
  var mockOmniApi: MockOmniApi = MockOmniApi()
  var card = CreditCard.testCreditCard()
  var paymentMethod = PaymentMethod()

  override func setUp() {
    merchant.hostedPaymentsToken = hostedPaymentsToken
  }

  func testCanTokenize() {
    // Stub out payment method
    let body = try! mockOmniApi.jsonEncoder().encode(card)
    paymentMethod.id = "123"
    mockOmniApi.stub("post", "/webpayment/\(hostedPaymentsToken)/tokenize", body: body, response: .success(paymentMethod))

    // Setup the expectation
    let expectation = XCTestExpectation(description: "Card is tokenized")

    // Start the task
    let job = TokenizePaymentMethod(omniApi: mockOmniApi, merchant: merchant)
    job.start(codablePaymentMethod: card, completion: { (tokenizedPaymentMethod) in
      XCTAssertEqual(tokenizedPaymentMethod.id, self.paymentMethod.id)
      expectation.fulfill()
    }) { (error) in
      XCTFail("Tokenization failed")
    }

    wait(for: [expectation], timeout: 10.0)
  }

  func testFailsWhenMerchantMissingToken() {
    // Set up bad merchant
    merchant.hostedPaymentsToken = nil

    // Setup the expectation
    let expectation = XCTestExpectation(description: "Error is thrown")
    let expectedError = TokenizePaymentMethodException.merchantMissingHostedPaymentsToken

    // Start the task
    let job = TokenizePaymentMethod(omniApi: MockOmniApi(), merchant: merchant)
    job.start(codablePaymentMethod: CreditCard.testCreditCard(), completion: { (tokenizedPaymentMethod) in
      XCTFail("Tokenization succeeded, but should not have")
    }) { (error) in
      XCTAssertEqual(error as! TokenizePaymentMethodException, expectedError)
      XCTAssertNotNil(error.detail)
      expectation.fulfill()
    }

    wait(for: [expectation], timeout: 10.0)
  }

  func testFailsWhenTryingToDecodeBadObject() {
    // This is obviously not a payment method. But TokenizePaymentMethod just takes a Codable, so it will accept this
    let badPaymentMethod: [Double] = [Double.infinity]

    // Setup the expectation
    let expectation = XCTestExpectation(description: "Error is thrown")
    let expectedError = TokenizePaymentMethodException.couldNotParsePaymentMethodError

    // Start the task
    let job = TokenizePaymentMethod(omniApi: MockOmniApi(), merchant: merchant)
    job.start(codablePaymentMethod: badPaymentMethod, completion: { (tokenizedPaymentMethod) in
      XCTFail("Tokenization succeeded, but should not have")
    }) { (error) in
      XCTAssertEqual(error as! TokenizePaymentMethodException, expectedError)
      XCTAssertNotNil(error.detail)
      expectation.fulfill()
    }

    wait(for: [expectation], timeout: 10.0)
  }

}
