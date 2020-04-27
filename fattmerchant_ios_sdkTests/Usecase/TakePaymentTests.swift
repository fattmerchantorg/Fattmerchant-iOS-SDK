//
//  TakePaymentTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 4/17/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import XCTest

class TakePaymentTests: XCTestCase {

  var merchant = Merchant()
  var omniApi = MockOmniApi()

  override func setUp() {
    merchant = Merchant()
    merchant.hostedPaymentsToken = "hostedpaymentstoken"
    omniApi = MockOmniApi()
  }

  func testCanInitialize() {
    let req = TransactionRequest(amount: Amount(cents: 10))
    let takePayment = TakePayment(request: req, omniApi: OmniApi(), merchant: Merchant())
    XCTAssertNotNil(takePayment)
  }

  func testCreateChargeRequest() {
    let amount = Amount(dollars: 10.00)
    let paymentMethodId = "123"
    let chargeReq = TakePayment.createChargeRequest(amount: amount, paymentMethodId: paymentMethodId)

    let expectedMeta = [
      "subtotal": "10.00"
    ]
    XCTAssertEqual(chargeReq.meta, expectedMeta)
    XCTAssertEqual(chargeReq.preAuth, false)
    XCTAssertEqual(chargeReq.paymentMethodId, paymentMethodId)
    XCTAssertEqual(chargeReq.total, "10.00")
  }

  func testCanTakePayment() {
    let card = CreditCard.testCreditCard()
    let amount = Amount(cents: 10)
    let transactionRequest = TransactionRequest(amount: amount, card: card)
    let transaction = Transaction()
    transaction.id = "transactionid"

    // Stub out payment method
    let body = try! omniApi.jsonEncoder().encode(card)
    let paymentMethod = PaymentMethod()
    paymentMethod.id = "123"
    omniApi.stub("post", "/webpayment/hostedpaymentstoken/tokenize", body: body, response: .success(paymentMethod))

    // Stub out charge
    let chargeRequest = ChargeRequest(paymentMethodId: "123", total: "0.10", meta: [
      "subtotal": "0.10"
    ])
    let chargeBody = Data(chargeRequest: chargeRequest)
    omniApi.stub("post", "/charge", body: chargeBody, response: .success(transaction))

    // Perform operation
    let paymentSuccessful = XCTestExpectation(description: "Successful payment")
    TakePayment(request: transactionRequest, omniApi: omniApi, merchant: merchant).start(completion: { completedTrans in
      XCTAssertNotNil(completedTrans)
      XCTAssertEqual(transaction.id, completedTrans.id)
      paymentSuccessful.fulfill()
    }) { (error) in
      XCTFail()
    }

    wait(for: [paymentSuccessful], timeout: 10.0)
  }

  func testFailsWhenNoPaymentMethodGiven() {
    let req = TransactionRequest(amount: Amount(cents: 10))
    let takePayment = TakePayment(request: req, omniApi: MockOmniApi(), merchant: Merchant())

    let exception = XCTestExpectation(description: "TakePayment fails because no card was provided")

    takePayment.start(completion: { _ in
      XCTFail("Exception not thrown")
    }) { (error) in
      if case TakePayment.Exception.couldNotTokenizePaymentMethod(_) = error {
        XCTAssertNotNil(error.detail)
        exception.fulfill()
        return
      }

      XCTFail("Wrong exception thrown")
    }
    wait(for: [exception], timeout: 10.0)
  }

  func testFailsWhenTokenizationFails() {
    let badCard = CreditCard(personName: "", cardNumber: "", cardExp: "", addressZip: "")
    let amount = Amount(cents: 10)
    let transactionRequest = TransactionRequest(amount: amount, card: badCard)
    let takePayment = TakePayment(request: transactionRequest, omniApi: omniApi, merchant: merchant)
    let expectedError = TokenizePaymentMethodException.couldNotParsePaymentMethodError

    // Stub out the tokenize call
    let body = try! omniApi.jsonEncoder().encode(badCard)
    omniApi.stub("post", "/webpayment/hostedpaymentstoken/tokenize", body: body, response: .failure(expectedError))

    let exception = XCTestExpectation(description: "TakePayment fails because no card was provided")

    takePayment.start(completion: { _ in
      XCTFail("Exception not thrown")
    }) { (error) in
      XCTAssertEqual(error as! TokenizePaymentMethodException, expectedError)
      XCTAssertNotNil(error.detail)
      exception.fulfill()
    }

    wait(for: [exception], timeout: 10.0)
  }

  func testFailsWhenTokenizedPaymentMethodDoesntHaveId() {
    let badCard = CreditCard(personName: "Jane Tester", cardNumber: "4111111111111111", cardExp: "123", addressZip: "32823")
    let amount = Amount(cents: 10)
    let transactionRequest = TransactionRequest(amount: amount, card: badCard)
    let takePayment = TakePayment(request: transactionRequest, omniApi: omniApi, merchant: merchant)

    // Create bad payment method
    let paymentMethodWithoutId = PaymentMethod()
    paymentMethodWithoutId.id = nil

    // Stub out the tokenize call
    let body = try! omniApi.jsonEncoder().encode(badCard)
    omniApi.stub("post", "/webpayment/hostedpaymentstoken/tokenize", body: body, response: .success(paymentMethodWithoutId))

    let exception = XCTestExpectation(description: "TakePayment fails because tokenized paymentmethod doesnt have id")

    takePayment.start(completion: { _ in
      XCTFail("Exception not thrown")
    }) { (error) in
      if case TakePayment.Exception.couldNotTokenizePaymentMethod(_) = error {
        XCTAssertNotNil(error.detail)
        exception.fulfill()
        return
      }

      XCTFail("Wrong exception thrown")
    }

    wait(for: [exception], timeout: 10.0)
  }

}
