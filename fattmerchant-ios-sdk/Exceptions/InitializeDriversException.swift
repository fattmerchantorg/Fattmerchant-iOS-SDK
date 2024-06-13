enum InitializeDriversException: StaxException {
  static var mess: String = "Could not initialize driver"

  case noMobileReadersFound
  case incorrectMobileReaderSettings
  case noAppIdFound

  var detail: String? {
    switch self {
      case .noMobileReadersFound:
        return "Couldn't find any mobile readers"
      case .incorrectMobileReaderSettings:
        return "The mobile reader settings in your account are incorrect"
      case .noAppIdFound:
        return "No App ID was passed in while initializing the application"
    }
  }
}
