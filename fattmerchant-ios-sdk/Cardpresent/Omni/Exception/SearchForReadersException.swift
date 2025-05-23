enum SearchForReadersException: OmniException {
  static var mess: String = "Couldnt find mobile readers"
  case noMobileReaderAvailable
}
