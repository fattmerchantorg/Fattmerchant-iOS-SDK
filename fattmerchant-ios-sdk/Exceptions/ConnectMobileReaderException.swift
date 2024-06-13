enum ConnectMobileReaderException: StaxException {
  static var mess: String = "Could not connect mobile reader"
  case couldNotConnectMobileReader(reader: MobileReader)
}
