enum CreateBankAccountException: OmniException {
  static var mess: String = "Could not create bank account object."

  case zipCodeNotValid
  case unknown

  var detail: String? {
    switch self {
      case .zipCodeNotValid: return "The passed in ZIP code is nil or not valid"
      case .unknown: return "Unkown error"
    }
  }
}
