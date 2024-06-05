enum InitializeDriversException: OmniException {
  static var mess: String = "Could not initialize driver"

  case noMobileReadersFound
  case incorrectMobileReaderSettings

  var detail: String? {
    switch self {
    case .noMobileReadersFound:
      return "Couldn't find any mobile readers"

    case .incorrectMobileReaderSettings:
      return "The mobile reader settings in your account are incorrect"
    }
  }
}
