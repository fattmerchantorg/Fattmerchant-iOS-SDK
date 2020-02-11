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
