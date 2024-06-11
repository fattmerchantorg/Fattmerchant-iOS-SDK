enum CapturePreauthTransactionException: StaxException {
  static var mess: String = "Could not capture funds"

  case errorCapturing(detail: String?)
  case captureAmountExceedsAuth(detail: String)

  var detail: String? {
    switch self {
    case .errorCapturing(let d):
      return d ?? "Unknown error"

    case .captureAmountExceedsAuth(let d):
      return d
    }
  }
}
