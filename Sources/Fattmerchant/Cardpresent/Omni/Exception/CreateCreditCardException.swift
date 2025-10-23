enum CreateCreditCardException: OmniException {
  static var mess: String = "Could not create credit card object."

  case zipCodeNotValid
  case expiryNotValid
  case unknown

  var detail: String? {
    switch self {
      case .zipCodeNotValid: return "The passed in ZIP code is nil or not valid"
      case .expiryNotValid: return "The passed in expiry date is nil or not valid"
      case .unknown: return "Unkown error"
    }
  }
}
