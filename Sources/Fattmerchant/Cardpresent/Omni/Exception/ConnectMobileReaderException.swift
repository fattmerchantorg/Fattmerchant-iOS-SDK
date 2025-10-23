enum ConnectMobileReaderException: OmniException {
  static var mess: String = "Could not connect mobile reader"
  case couldNotConnectMobileReader(reader: MobileReader)
}
