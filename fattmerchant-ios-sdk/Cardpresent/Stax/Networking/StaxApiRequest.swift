/// A type-safe representation of an HTTP request to the Stax API.
/// The generic parameter `Response` defines the expected response type that will be decoded.
///
/// Example usage:
/// ```
/// let request = StaxApiRequest<Transaction>(
///     path: "/transactions/123",
///     method: .get
/// )
/// ```
struct StaxApiRequest<Response: Decodable> {
  /// The path component of the URL, relative to the base URL.
  /// Should start with a forward slash, e.g. "/transactions".
  let path: String
  
  /// The HTTP method to use for the request.
  let method: StaxHttpMethod
  
  /// Optional body data to be encoded as JSON and sent with the request.
  /// Will be automatically encoded using `JSONEncoder`.
  let body: Encodable?
  
  /// Optional HTTP headers to be included with the request.
  /// Content-Type will be automatically set to application/json when a body is present.
  let headers: [String: String]?
  
  /// Creates a new API request.
  /// - Parameters:
  ///   - path: The path component of the URL, relative to the base URL.
  ///   - method: The HTTP method to use. Defaults to `.get`.
  ///   - body: Optional data to be encoded as JSON and sent with the request. Defaults to `nil`.
  ///   - headers: Optional HTTP headers to include with the request. Defaults to `nil`.
  public init(
    path: String,
    method: StaxHttpMethod = .get,
    body: Encodable? = nil,
    headers: [String: String]? = nil
  ) {
    self.path = path
    self.method = method
    self.body = body
    self.headers = headers
  }
}
