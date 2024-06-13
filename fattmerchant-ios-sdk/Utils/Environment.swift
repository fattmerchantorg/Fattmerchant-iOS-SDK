import Foundation

/// The Stax environment to use. `DEV` is temporarily deprecated. For sandbox accounts, use `LIVE`.
public enum Environment {
  case LIVE
  case DEV

  func baseUrlString() -> String {
    switch self {
      case .LIVE: return "https://apiprod.fattlabs.com"
      case .DEV: return "https://apidev.fattlabs.com"
    }
  }

  func baseUrl() -> URL? {
    return URL(string: baseUrlString())
  }
}
