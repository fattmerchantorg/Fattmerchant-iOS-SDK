enum SearchForReadersException: StaxException {
  static var mess: String = "Couldnt find mobile readers"
  case noMobileReaderAvailable
}
