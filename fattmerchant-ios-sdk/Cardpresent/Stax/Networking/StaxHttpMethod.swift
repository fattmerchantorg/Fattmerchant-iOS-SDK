/// Represents HTTP methods supported by the Stax API.
///
/// Each case corresponds to a standard HTTP method, with the raw value matching
/// the method string specified in the HTTP/1.1 protocol.
public enum StaxHttpMethod: String {
  /// HTTP GET method.
  /// Used for retrieving resources without modifying server state.
  case get = "GET"

  /// HTTP POST method.
  /// Used for creating new resources or submitting data for processing.
  case post = "POST"

  /// HTTP PUT method.
  /// Used for updating existing resources with a complete replacement.
  case put = "PUT"

  /// HTTP DELETE method.
  /// Used for removing resources from the server.
  case delete = "DELETE"
}
