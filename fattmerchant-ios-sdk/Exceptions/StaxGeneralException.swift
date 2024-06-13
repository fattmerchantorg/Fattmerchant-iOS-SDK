public enum StaxGeneralException: StaxException {
  case uninitialized
  public static var mess: String = "Stax General Error"
  
  public var detail: String? {
    switch self {
      case .uninitialized:
        return "Stax has not been initialized yet"
    }
  }
}
