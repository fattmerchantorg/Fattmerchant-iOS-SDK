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

  static var mess: String = "Could not refund transaction"

  var detail: String? {
    switch self {
    case .transactionNotRefundable(let d):
      return d ?? "Transaction is not refundable"

    case .couldNotFindMobileReaderForRefund:
      return "Mobile reader driver responsible for performing refund could not be found"
    }
  }
}

class RefundMobileReaderTransaction {

  typealias Exception = RefundException

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var transactionRepository: TransactionRepository
  var transaction: Transaction

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, transactionRepository: TransactionRepository, transaction: Transaction) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.transactionRepository = transactionRepository
    self.transaction = transaction
  }

  func start(completion: @escaping (Transaction) -> Void, failure: @escaping (OmniException) -> Void ) {
    // Ensure transaction isn't voided
    guard transaction.isVoided != true else {
      failure(Exception.transactionNotRefundable(details: "Can not refund voided transaction"))
      return
    }

    // Can't refund transaction that has been refunded already
    guard transaction.totalRefunded == "0" || transaction.totalRefunded == nil else {
      failure(Exception.transactionNotRefundable(details: "Transaction already refunded"))
      return
    }

    // Do the 3rd-party refund
    refund(transaction: transaction, failure: failure) { result in
      self.submitRefundToOmni(with: result, failure: failure, completion: completion)
    }

  }

  fileprivate func submitRefundToOmni(with result: TransactionResult, failure: (OmniException) -> Void, completion: (Transaction) -> Void) {
    let refundedTransaction = Transaction()
    refundedTransaction.total = transaction.total
    refundedTransaction.paymentMethodId = transaction.paymentMethodId
    refundedTransaction.success = true
    refundedTransaction.lastFour = transaction.lastFour
    refundedTransaction.type = "refund"
    refundedTransaction.source = transaction.source
    refundedTransaction.referenceId = transaction.id
    refundedTransaction.method = transaction.method
    refundedTransaction.customerId = transaction.customerId
    refundedTransaction.invoiceId = transaction.invoiceId
    transactionRepository.create(model: refundedTransaction, completion: completion, error: failure)
  }

  fileprivate func refund(transaction: Transaction, failure: (OmniException) -> Void, completion: @escaping (TransactionResult) -> Void) {
    mobileReaderDriverRepository.getDriverFor(transaction: transaction) { driver in
      if let driver = driver {
        driver.refund(transaction: transaction, completion: completion) // TODO: Handle error
      } else {
        failure(Exception.couldNotFindMobileReaderForRefund)
      }
    }
  }

}
