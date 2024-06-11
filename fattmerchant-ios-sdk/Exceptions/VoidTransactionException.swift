enum VoidTransactionException: StaxException {
  static var mess: String = "Could not capture funds"

  case errorCapturing
}
