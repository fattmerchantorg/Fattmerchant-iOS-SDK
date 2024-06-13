public enum StaxInitializeException: StaxException {
  case missingInitializationDetails
  case mobileReaderPaymentsNotConfigured
  case missingMobileReaderCredentials
  case invalidMobileReaderCredentials

  public static var mess: String = "Stax Initialization Exception"

  public var detail: String? {
    switch self {
      case .missingInitializationDetails:
        return "Missing initialization details"
      case .mobileReaderPaymentsNotConfigured:
        return "Your account is not configured to accept mobile reader payments"
      case .missingMobileReaderCredentials:
        return "Your account does not have mobile reader credentials"
      case .invalidMobileReaderCredentials:
        return "Your account has invalid mobile reader credentials"
    }
  }
}
