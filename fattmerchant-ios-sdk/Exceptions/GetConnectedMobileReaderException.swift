enum GetConnectedMobileReaderException: OmniException {
  static var mess: String = "Could not get connected mobile reader"

  case noReaderAvailable
}
