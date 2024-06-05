enum VoidTransactionException: OmniException {
  static var mess: String = "Could not capture funds"

  case errorCapturing
}
