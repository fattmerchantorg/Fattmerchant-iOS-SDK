public enum OmniGeneralException: OmniException {
  case uninitialized
  public static var mess: String = "Omni General Error"
  
  public var detail: String? {
    switch self {
      case .uninitialized: return "Omni has not been initialized yet"
    }
  }
}
