import Foundation

class RefundCardReaderTransactionJob: Job {
  typealias Output = Transaction
  
  private let transaction: Transaction
  private let amount: Amount
  
  public init(transaction: Transaction, amount: Amount? = nil) throws {
    self.transaction = transaction
    
    if let a = amount {
      self.amount = a
    } else if let total = transaction.total {
      self.amount = Amount(dollars: total)
    } else {
      throw RefundException.transactionNotRefundable(details: "Transaction does not have a total amount")
    }
  }
  
  func start() async throws -> Transaction {
    // Make sure the transaction has an id
    guard let id = transaction.id else {
      throw RefundException.missingTransactionId
    }
    
    guard let card = Services.resolve(CardReaderService.self) else {
      throw StaxGeneralException.uninitialized
    }
    
    guard let http = Services.resolve(StaxHttpService.self) else {
      throw StaxGeneralException.uninitialized
    }
    
    // If Stax can handle refund, do the refund via the POST /void-or-refund route
    if type(of: card).isStaxRefundSupported {
      return try await http.postVoidOrRefund(id: id)
    } else {
      // Validate we can refund, otherwise throw an error
      let (isAbleToRefund, error) = validateRefund(transaction: transaction, amount: amount)
      if !isAbleToRefund {
        throw error!
      }
      
      // 3rd Party Refund via the CardReaderService
      let result = try card.refundTransaction(transaction, amount)

      // Log via Stax API
      let potentialTransaction = Transaction()
      potentialTransaction.total = result.amount?.dollars()
      potentialTransaction.paymentMethodId = transaction.paymentMethodId
      potentialTransaction.success = result.success
      potentialTransaction.lastFour = transaction.lastFour
      potentialTransaction.type = "refund"
      potentialTransaction.source = transaction.source
      potentialTransaction.referenceId = transaction.id
      potentialTransaction.method = transaction.method
      potentialTransaction.customerId = transaction.customerId
      potentialTransaction.invoiceId = transaction.invoiceId
      return try await http.postTransaction(transaction: potentialTransaction)
    }
  }

  private func validateRefund(transaction: Transaction, amount: Amount) -> (Bool, RefundException?) {
    let isVoided = RefundException.transactionNotRefundable(details: "Can not refund voided transaction")
    let isAlreadyRefunded = RefundException.transactionNotRefundable(details: "Can not refund transaction that has been fully refunded")
    let isTooMuchToRefund = RefundException.transactionNotRefundable(details: "Can not refund more than the original transaction total")
    let isTooLittleToRefund = RefundException.transactionNotRefundable(details: "Can not refund zero or negative amounts")
    
    // Ensure transaction isn't voided
    if transaction.isVoided == true {
      return (false, isVoided)
    }

    // Account for previous refunds
    if let totalRefunded = transaction.totalRefunded, let transactionTotal = transaction.total {
      let remaining = transactionTotal - totalRefunded
      
      // Can't refund transaction that has already been refunded
      if remaining < 0.01 {
        return (false, isAlreadyRefunded)
      }

      // Can't refund more than there is left to refund
      if amount.dollars() > remaining {
        return (false, isTooMuchToRefund)
      }

      // Can't refund zero amount
      if amount.dollars() <= 0.0 {
        return (false, isTooLittleToRefund)
      }
    }
    
    return (true, nil)
  }
}
