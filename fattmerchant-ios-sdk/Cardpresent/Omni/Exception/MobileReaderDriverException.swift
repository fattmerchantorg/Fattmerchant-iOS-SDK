enum MobileReaderDriverException: OmniException {
  case couldNotInitialize(detail: String?)

  static var mess: String = "Mobile reader driver error"

  var detail: String? {
    switch self {
      case .couldNotInitialize(let d): return d ?? "Could not initialize mobile reader driver"
    }
  }
}
