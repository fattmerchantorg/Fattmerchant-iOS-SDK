import Foundation

/// Gets notified of all messages that need to be displayed to a user
///
/// For example, "Please insert/swipe your card"
public protocol TransactionUpdateDelegate: class {
  func onTransactionUpdate(transactionUpdate: TransactionUpdate)
}
