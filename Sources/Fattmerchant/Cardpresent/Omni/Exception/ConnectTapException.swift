enum ConnectTapException: OmniException {
  static var mess: String = "Could not connect to Tap to Pay"

  case couldNotConnectToTap(detail: String?)

  var detail: String? {
    switch self {
    case .couldNotConnectToTap(let detail):
      return detail
    }
  }
}
