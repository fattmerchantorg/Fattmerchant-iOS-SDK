import Foundation

internal enum HttpMethod : String {
  case get = "GET"
  case post = "POST"
  case patch = "PATCH"
  case put = "PUT"
  case delete = "DELETE"
  
  /// Converts a string representation of an HTTP method into the enum
  static func fromString(_ str: String) -> HttpMethod {
    let string = str.uppercased()
    switch string {
      case "GET":     return .get
      case "POST":    return .post
      case "PUT":     return .put
      case "PATCH":   return .patch
      case "DELETE":  return .delete
      default:        return .post
    }
  }
  
  /// Converts the enum to a string
  func toString() -> String {
    return self.rawValue
  }
}
