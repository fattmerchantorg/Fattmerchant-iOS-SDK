//
//  RefundMobileReaderTransactionTests.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 2/10/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class RefundMobileReaderTransactionTests: XCTestCase {

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


  func testCanRefundPreviouslyRefundedTransaction() {
    XCTAssertNil(getRefundValidationError(total: 0.04, totalRefunded: 0.02, refundAmount: Amount(cents: 2)))
  }

  func testCanRefundTransactionThatHasntBeenRefunded() {
    XCTAssertNil(getRefundValidationError(total: 5.00, totalRefunded: 0.00, refundAmount: Amount(dollars: 5.00)))
    XCTAssertNil(getRefundValidationError(total: 5.00, totalRefunded: 0.00, refundAmount: Amount(cents: 500)))
    XCTAssertNil(getRefundValidationError(total: 5.49, totalRefunded: 0.00, refundAmount: Amount(cents: 549)))
  }

  func testCantRefundFullyRefundedTransaction() {
    XCTAssertNotNil(getRefundValidationError(total: 0.1, totalRefunded: 0.1, refundAmount: Amount(cents: 1)))
    XCTAssertNotNil(getRefundValidationError(total: 1.0, totalRefunded: 1.0, refundAmount: Amount(cents: 100)))
    XCTAssertNotNil(getRefundValidationError(total: 1.0, totalRefunded: 1.0, refundAmount: Amount(dollars: 1)))
  }

  func testCantRefundMoreThanLeftover() {
    XCTAssertNotNil(getRefundValidationError(total: 1.0, totalRefunded: 0.5, refundAmount: Amount(dollars: 0.6)))
  }

  func testCantRefundNegative() {
XCTAssertNotNil(getRefundValidationError(total: 1.0, totalRefunded: 0.0, refundAmount: Amount(dollars: -1.00)))
  }

  func testCantRefundZero() {
    XCTAssertNotNil(getRefundValidationError(total: 1.0, totalRefunded: 0.0, refundAmount: Amount(dollars: 0.00)))
  }

  func testShouldRefundUsingOmniIfDriverSupportsIt() {
    MockDriver.isStaxRefundsSupported = true

    let transactionToRefund = Transaction()
    let transactionId = UUID().uuidString
    transactionToRefund.source = "iOS|CPSDK|\(MockDriver.source)"
    transactionToRefund.id = transactionId
    modelStore = [transactionId: transactionToRefund]

    let expectation = XCTestExpectation(description: "Request to omni is build properly")
    let expectedMethod = "post"
    let expectedUrlString = "/transaction/\(transactionId)/void-or-refund"
    let expectedBody: Data? = nil

    // We want mock omni api to be called witha void-or-refund
    mockOmniApi.expectRequest(method: expectedMethod,
                              urlString: expectedUrlString,
                              body: expectedBody) { () -> Bool in
      expectation.fulfill()
      return false
    }

    // Start the refund job
    RefundMobileReaderTransaction(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      transactionRepository: transactionRepo,
      transaction: transactionToRefund,
      omniApi: mockOmniApi
    ).start(completion: { _ in
      XCTFail()
    }) { _ in
      XCTFail()
    }

    wait(for: [expectation], timeout: 2.0)
  }

  func testCantRefundVoidedTransaction() {
    let transaction = Transaction()
    transaction.total = 4.0
    transaction.isVoided = true
    XCTAssertNotNil(RefundMobileReaderTransaction.validateRefund(transaction: transaction, refundAmount: Amount(dollars: 1.00)))
  }

  fileprivate func getRefundValidationError(total: Double, totalRefunded: Double, refundAmount: Amount) -> OmniException? {
    let transaction = Transaction()
    transaction.total = total
    transaction.totalRefunded = totalRefunded
    return RefundMobileReaderTransaction.validateRefund(transaction: transaction, refundAmount: refundAmount)
  }

}
