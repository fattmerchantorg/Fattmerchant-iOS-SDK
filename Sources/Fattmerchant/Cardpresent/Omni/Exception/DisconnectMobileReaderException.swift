enum DisconnectMobileReaderException: OmniException {
  static var mess: String = "Could not disconnect mobile reader"
  case driverNotFound
}
