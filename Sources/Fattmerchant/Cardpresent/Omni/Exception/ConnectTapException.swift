enum ConnectTapException: OmniException {
  static var mess: String = "Could not connect to Tap to Pay"

  case couldNotConnectToTap(detail: String?)
  
  // Connect and Configure errors
  case tapToMobileNotSupported
  case noPoiSelected
  case locationPermissionsNotGranted
  case applicationUpdateRequired
  
  // Connect and Configure Finished event errors
  case countryCodeInvalid
  case attestationFailed
  case accessTokenExpired
  case teamIdentifierMissing
  case invalidAppleAccount
  case missingBundleIdentifier
  case currentCountryNotAllowed
  case noLocationFound

  var detail: String? {
    switch self {
    case .couldNotConnectToTap(let detail):
      return detail
    default:
      return nil
    }
  }
  
  var localizedDescription: String {
    switch self {
    case .couldNotConnectToTap(let detail):
      return "Could not connect to Tap to Pay\(detail.map { ": \($0)" } ?? "")"
    case .tapToMobileNotSupported:
      return "Tap to Pay is not supported on this device"
    case .noPoiSelected:
      return "No point of interaction was selected for configuration"
    case .locationPermissionsNotGranted:
      return "Location permissions are required for Tap to Pay"
    case .applicationUpdateRequired:
      return "SDK update required. Please update to the latest version"
    case .countryCodeInvalid:
      return "Invalid country code for Tap to Pay"
    case .attestationFailed:
      return "Device attestation failed. Please try again"
    case .accessTokenExpired:
      return "Session token expired. Please reconnect"
    case .teamIdentifierMissing:
      return "Team identifier missing from Info.plist"
    case .invalidAppleAccount:
      return "Invalid Apple account. For development, ensure you're logged in to an Apple Sandbox account"
    case .missingBundleIdentifier:
      return "Bundle identifier not configured for Tap to Pay"
    case .currentCountryNotAllowed:
      return "Merchant account country doesn't match transaction location"
    case .noLocationFound:
      return "Unable to determine location. Check location permissions"
    }
  }
}
