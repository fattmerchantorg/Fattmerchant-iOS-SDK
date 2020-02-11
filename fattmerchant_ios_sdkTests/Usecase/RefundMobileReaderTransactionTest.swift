//
//  RefundMobileReaderTransactionTest.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 2/7/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class RefundMobileReaderTransactionTest: XCTestCase {

  class MockApi: OmniApi {

  }

  func testCanRefundRefundableTransaction() {
    let refundComplete = XCTestExpectation(description: "Refund performed")

    let refundableTransaction = Transaction()
    refundableTransaction.total = 1.00
    refundableTransaction.totalRefunded = 0.00
    refundableTransaction.isRefundable = true
    refundableTransaction.isVoided = false

    let job = RefundMobileReaderTransaction(
      mobileReaderDriverRepository: MockMobileReaderDriverRepository(),
      transactionRepository: TransactionRepository(omniApi: MockApi()),
      transaction: refundableTransaction
    )

    job.validateRefund()

    wait(for: [refundComplete], timeout: 10.0)
  }

}
