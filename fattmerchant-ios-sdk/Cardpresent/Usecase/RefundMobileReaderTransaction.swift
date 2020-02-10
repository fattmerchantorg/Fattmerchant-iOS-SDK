//
//  RefundMobileReaderTransaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum RefundException: OmniException {
  case transactionNotRefundable(details: String?)
  case couldNotFindMobileReaderForRefund
  case errorRefunding(details: String?)

  static var mess: String = "Could not refund transaction"

  var detail: String? {
    switch self {
    case .transactionNotRefundable(let d):
      return d ?? "Transaction is not refundable"

    case .couldNotFindMobileReaderForRefund:
      return "Mobile reader driver responsible for performing refund could not be found"

    case .errorRefunding(let d):
      return d ?? "Error while performing refund"
    }
  }
}

class RefundMobileReaderTransaction {

  typealias Exception = RefundException

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var transactionRepository: TransactionRepository
  var transaction: Transaction
  var refundAmount: Amount?

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, transactionRepository: TransactionRepository, transaction: Transaction, refundAmount: Amount? = nil) {
    // Get the amount to refund
    var amount: Amount? = nil
    if let amt = refundAmount {
      amount = amt
    } else if let total = transaction.total {
      amount = Amount(dollars: total)
    }

    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.transactionRepository = transactionRepository
    self.transaction = transaction
    self.refundAmount = amount
  }

  func start(completion: @escaping (Transaction) -> Void, failure: @escaping (OmniException) -> Void ) {
    if let error = validateRefund() {
      failure(error)
      return
    }

    // Do the 3rd-party refund
    refund(transaction: transaction, refundAmount: refundAmount, failure: failure) { result in
      self.submitRefundToOmni(with: result, failure: failure, completion: completion)
    }

  }

  fileprivate func submitRefundToOmni(with result: TransactionResult, failure: @escaping (OmniException) -> Void, completion: @escaping (Transaction) -> Void) {
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

  fileprivate func refund(transaction: Transaction, refundAmount: Amount?, failure: @escaping (OmniException) -> Void, completion: @escaping (TransactionResult) -> Void) {
    mobileReaderDriverRepository.getDriverFor(transaction: transaction) { driver in
      if let driver = driver {
        driver.refund(transaction: transaction, refundAmount: refundAmount, completion: completion, error: failure) // TODO: Handle error
      } else {
        failure(Exception.couldNotFindMobileReaderForRefund)
      }
    }
  }


  /// Verifies that the refund about to happen is acceptable
  /// - Returns: OmniException explaining why the refund should not happen. `nil` if the refund is acceptable
  fileprivate func validateRefund() -> OmniException? {
    // Ensure transaction isn't voided
    if transaction.isVoided == true {
      return Exception.transactionNotRefundable(details: "Can not refund voided transaction")
    }

    // Ensure Omni doesn't say the transaction is not refundable
    if transaction.isRefundable == false {
      return Exception.transactionNotRefundable(details: "Transaction is not refundable")
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
