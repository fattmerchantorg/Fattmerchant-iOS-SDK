/// Protocol defining the HTTP client interface for easier testing
protocol StaxHttpClientProtocol {
  func perform<T: Decodable>(_ request: StaxApiRequest<T>) async throws -> T
}

extension StaxHttpClient: StaxHttpClientProtocol {}
