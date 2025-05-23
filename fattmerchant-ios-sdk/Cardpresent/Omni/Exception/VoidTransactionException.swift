enum VoidTransactionException: OmniException {
  static var mess: String = "Could not void transaction"
  case errorCapturing
}
