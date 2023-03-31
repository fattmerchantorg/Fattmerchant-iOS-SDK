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
  var staxApi = MockStaxApi()
  var customerRepo: CustomerRepository!
  var paymentMethodRepo: PaymentMethodRepository!


  override func setUp() {
    merchant = Merchant()
    merchant.hostedPaymentsToken = "hostedpaymentstoken"
    staxApi = MockStaxApi()
    customerRepo = CustomerRepository(staxApi: staxApi)
    paymentMethodRepo = PaymentMethodRepository(staxApi: staxApi)
  }

  func testCanInitialize() {
    let req = TransactionRequest(amount: Amount(cents: 10))
    let takePayment = TakePayment(request: req, customerRepository: customerRepo, paymentMethodRepository: paymentMethodRepo)
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
    modelStore = [:]
    var card = CreditCard.testCreditCard()
    card.personName = "Bob Will"
    let amount = Amount(cents: 10)
    let transactionRequest = TransactionRequest(amount: amount, card: card)
    let transaction = Transaction()
    transaction.id = "transactionid"

    // Stub out customer
    let customer = Customer(firstName: "Bob", lastName: "Will")
    customer.id = "customer_id_bobwill"
    let createCustomerBody = try! staxApi.jsonEncoder().encode(customer)
    staxApi.stub("post", "/customer", body: createCustomerBody, response: .success(customer))

    // Stub out payment method
    let paymentMethod = PaymentMethod(customer: customer)
    paymentMethod.customerId = "customer_id_bobwill"
    paymentMethod.personName = "Bob Will"
    paymentMethod.method = .card
    paymentMethod.cardNumber = card.cardNumber
    paymentMethod.cardExp = card.cardExp
    let createPaymentMethodBody = try! staxApi.jsonEncoder().encode(paymentMethod)
    paymentMethod.id = "payment_method_id_bob_will"
    staxApi.stub("post", "/payment-method", body: createPaymentMethodBody, response: .success(paymentMethod))

    // Stub out charge
    let chargeRequest = ChargeRequest(paymentMethodId: "payment_method_id_bob_will", total: "0.10", meta: [
      "subtotal": "0.10"
    ])
    let chargeBody = Data(chargeRequest: chargeRequest)
    staxApi.stub("post", "/charge", body: chargeBody, response: .success(transaction))

    // Perform operation
    let paymentSuccessful = XCTestExpectation(description: "Successful payment")
    TakePayment(request: transactionRequest, customerRepository: customerRepo, paymentMethodRepository: paymentMethodRepo)
      .start(completion: { completedTrans in
      XCTAssertNotNil(completedTrans)
      paymentSuccessful.fulfill()
    }) { (error) in
      XCTFail()
    }

    wait(for: [paymentSuccessful], timeout: 10.0)
  }

  func testFailsWhenNoPaymentMethodGiven() {
    let req = TransactionRequest(amount: Amount(cents: 10))
    let takePayment = TakePayment(request: req, customerRepository: customerRepo, paymentMethodRepository: paymentMethodRepo)

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

}
