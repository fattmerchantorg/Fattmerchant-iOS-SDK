#if targetEnvironment(simulator)
#else

extension TransactionUpdate {

  /// Makes an Omni TransactionUpdate from a ChipDna TransactionUpdate string
  init?(chipDnaTransactionUpdate: String) {
    switch chipDnaTransactionUpdate {
      case TransactionUpdateCardEntryPrompted: self = .PromptInsertSwipeCard
      case TransactionUpdateCardSwiped: self = .CardSwiped
      case TransactionUpdateSmartcardInserted: self = .CardInserted
      case TransactionUpdateCardSwipeError: self = .CardSwipeError
      case TransactionUpdateSmartcardRemovePrompted: self = .PromptRemoveCard
      case TransactionUpdateSmartcardRemoved: self = .CardRemoved
      case TransactionUpdateOnlineAuthorisation: self = .Authorizing
      case TransactionUpdateAmountConfirmationStarted: self = .PromptConfirmAmountOnReader
      default: return nil
    }
  }
}

#endif
