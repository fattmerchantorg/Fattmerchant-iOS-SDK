enum CancelCurrentTransactionException: OmniException {
  static var mess: String = "Could not cancel current transaction"

  case noTransactionToCancel
  case unknown

  var detail: String? {
    switch self {
      case .noTransactionToCancel: return "There is no transaction to cancel"
      case .unknown: return "Unkown error"
    }
  }
}
