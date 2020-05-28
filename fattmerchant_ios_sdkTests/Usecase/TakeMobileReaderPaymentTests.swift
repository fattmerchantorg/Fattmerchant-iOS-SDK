//
//  TakeMobileReaderTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 5/19/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class TakeMobileReaderPaymentTests: XCTestCase {

  var mockOmniApi: MockOmniApi = MockOmniApi()
  var mobileReaderDriverRepo: MobileReaderDriverRepository!
  var invoiceRepo: InvoiceRepository!
  var customerRepo: CustomerRepository!
  var paymentMethodRepo: PaymentMethodRepository!
  var transactionRepo: TransactionRepository!

  override func setUp() {
    invoiceRepo = InvoiceRepository(omniApi: mockOmniApi)
    customerRepo = CustomerRepository(omniApi: mockOmniApi)
    paymentMethodRepo = PaymentMethodRepository(omniApi: mockOmniApi)
    transactionRepo = TransactionRepository(omniApi: mockOmniApi)
    mobileReaderDriverRepo = MobileReaderDriverRepository()
  }

  func testCanTakeMobileReaderPayment() {
    let transactionCompleted = expectation(description: "Transaction was successfully completed")

    let transactionRequest = TransactionRequest(amount: Amount(cents: 1), tokenize: true)
    TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest
    ).start(completion: { completedTransaction in
      transactionCompleted.fulfill()
    }) { exception in
      XCTFail("Transaction failed")
    }

    wait(for: [transactionCompleted], timeout: 10.0)
  }

  func testCreateTransactionIsCreatedWithToken() {
    let transactionRequest = TransactionRequest(amount: Amount(cents: 1), tokenize: true)
    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest
    )

    // Verify that the externalId is put in the transaction
    let transactionHasExternalId = expectation(description: "Transaction has external id")
    let paymentMethod = PaymentMethod()
    paymentMethod.id = "payment-method-id"

    let customer = Customer()
    customer.id = "customer-id"

    let invoice = Invoice()
    invoice.id = "invoice-id"

    var result = TransactionResult()
    result.externalId = "externalId"
    result.maskedPan = "41111111111111111"

    job.createTransaction(result: result,
                          paymentMethod: paymentMethod,
                          customer: customer,
                          invoice: invoice,
                          { exception in
                            XCTFail()
                          }, { transaction in
                            XCTAssertEqual(transaction.token, result.externalId)
                            transactionHasExternalId.fulfill()
                          })

    wait(for: [transactionHasExternalId], timeout: 10.0)
  }

}
