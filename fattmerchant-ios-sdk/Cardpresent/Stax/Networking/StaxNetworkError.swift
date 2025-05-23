/// Represents errors that can occur during Stax API network operations.
///
/// These errors encompass various failure points in the HTTP request lifecycle,
/// from URL formation to response handling and decoding.
public enum StaxNetworkError: OmniException {
  public static var mess = "Stax Network Error"
  public var detail: String? {
    switch self {
      case .invalidURL: return "Invalid URL"
      case .invalidResponse: return "Invalid Response"
      case .httpError(let code): return "HTTP Error Code: \(code)"
      case .decodingError(let error): return "HTTP Decoding Error: \(error)"
      case .underlying(let error): return "Underlying System Error: \(error)"
    }
  }
  
  /// The URL for the request could not be constructed properly.
  /// This typically occurs when the path or query parameters are malformed.
  case invalidURL

  /// The response received was not a valid HTTP response.
  /// This can occur if the server returns a malformed response or if the response
  /// cannot be interpreted as an HTTP response.
  case invalidResponse

  /// A non-successful HTTP status code was received.
  /// - Parameter Int: The HTTP status code returned by the server.
  ///
  /// Common status codes:
  /// - 400: Bad Request
  /// - 401: Unauthorized
  /// - 404: Not Found
  /// - 500: Internal Server Error
  case httpError(Int)

  /// The response data could not be decoded into the expected type.
  /// - Parameter Error: The underlying Swift decoding error that occurred.
  /// This typically happens when the response JSON doesn't match the expected model structure.
  case decodingError(Error)

  /// An underlying system error occurred during the network request.
  /// - Parameter Error: The original error from the networking layer.
  /// This could be due to network connectivity issues, SSL problems, etc.
  case underlying(Error)
}
