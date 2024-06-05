//
//  RefundMobileReaderTransaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class RefundMobileReaderTransaction {

  typealias Exception = RefundException

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var transactionRepository: TransactionRepository
  var omniApi: OmniApi
  var transaction: Transaction
  var refundAmount: Amount?

  init(mobileReaderDriverRepository: MobileReaderDriverRepository,
       transactionRepository: TransactionRepository,
       transaction: Transaction,
       refundAmount: Amount? = nil,
       omniApi: OmniApi) {

    // Get the amount to refund
    var amount: Amount?
    if let amt = refundAmount {
      amount = amt
    } else if let total = transaction.total {
      amount = Amount(dollars: total)
    }

    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.transactionRepository = transactionRepository
    self.transaction = transaction
    self.refundAmount = amount
    self.omniApi = omniApi
  }

  func start(completion: @escaping (Transaction) -> Void, failure: @escaping (OmniException) -> Void ) {
    // Make sure the transaction has an id
    guard let transactionId = transaction.id else {
      return failure(RefundException.missingTransactionId)
    }

    // Get the driver
    mobileReaderDriverRepository.getDriverFor(transaction: transaction) { driver in
      guard let driver = driver else {
        return failure(Exception.couldNotFindMobileReaderForRefund)
      }

      // If omni can do the refund, then we should call out to Omni to do it
      if type(of: driver).omniRefundsSupported {
        self.omniApi.request(method: "post",
                        urlString: "/transaction/\(transactionId)/void-or-refund", body: nil,
                        completion: completion, failure: failure)
      } else {
        // Omni can *not* do the refund, so we have to do it
        if let error = RefundMobileReaderTransaction.validateRefund(transaction: transaction, refundAmount: refundAmount) {
          failure(error)
          return
        }

        // Do the 3rd-party refund
        driver.refund(transaction: transaction, refundAmount: refundAmount, completion: { result in
          self.postRefundedTransaction(with: result, failure: failure, completion: completion)
        }, error: failure)
      }
    }
  }

  fileprivate func postRefundedTransaction(with result: TransactionResult, failure: @escaping (OmniException) -> Void, completion: @escaping (Transaction) -> Void) {
    let refundedTransaction = Transaction()
    refundedTransaction.total = result.amount?.dollars()
    refundedTransaction.paymentMethodId = transaction.paymentMethodId
    refundedTransaction.success = result.success
    refundedTransaction.lastFour = transaction.lastFour
    refundedTransaction.type = "refund"
    refundedTransaction.source = transaction.source
    refundedTransaction.referenceId = transaction.id
    refundedTransaction.method = transaction.method
    refundedTransaction.customerId = transaction.customerId
    refundedTransaction.invoiceId = transaction.invoiceId
    transactionRepository.create(model: refundedTransaction, completion: completion, error: failure)
  }

  /// Verifies that the refund about to happen is acceptable
  /// - Returns: OmniException explaining why the refund should not happen. `nil` if the refund is acceptable
  internal static func validateRefund(transaction: Transaction, refundAmount: Amount? = nil) -> OmniException? {
    // Ensure transaction isn't voided
    if transaction.isVoided == true {
      return Exception.transactionNotRefundable(details: "Can not refund voided transaction")
    }

    // Account for previous refunds
    if let totalRefunded = transaction.totalRefunded, let transactionTotal = transaction.total {

      // Can't refund transaction that has already been refunded
      if transactionTotal - totalRefunded < 0.01 {
        return Exception.transactionNotRefundable(details: "Can not refund transaction that has been fully refunded")
      }

      // Can't refund more than there is left to refund
      if let refundAmount = refundAmount, refundAmount.dollars() > (transactionTotal - totalRefunded) {
        return Exception.transactionNotRefundable(details: "Can not refund more than the original transaction total")
      }

      // Can't refund zero amount
      if let refundAmount = refundAmount?.dollars(), refundAmount <= 0.0 {
        return Exception.transactionNotRefundable(details: "Can not refund zero or negative amounts")
      }
    }

    return nil
  }

}
