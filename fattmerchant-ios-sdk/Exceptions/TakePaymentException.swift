enum TakePaymentException: OmniException {
  case couldNotTokenizePaymentMethod(detail: String? = nil)

  static var mess: String = "Error taking mobile reader payment"

  var detail: String? {
    switch self {
    case .couldNotTokenizePaymentMethod(let d):
      return d ?? "Could not tokenize payment method"
    }
  }
}
