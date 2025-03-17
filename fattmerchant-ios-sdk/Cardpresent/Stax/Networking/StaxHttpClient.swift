import Foundation

/// A client for making HTTP requests to the Stax API.
/// Handles both modern async/await and legacy callback-based networking depending on iOS version.
final class StaxHttpClient: Sendable {
  private let baseURL: URL
  private let defaultHeaders: [String:String]
  private let session: URLSession = .shared
  private let decoder: JSONDecoder = .init()

  /// Creates a new Stax HTTP client.
  /// - Parameter baseURL: The base URL for the Stax API.
  init(baseURL: URL, apiKey: String) {
    self.baseURL = baseURL
    self.defaultHeaders = [
      "Accept": "application/json",
      "Content-Type": "application/json",
      "Authorization": "Bearer \(apiKey)"
    ]
  }

  /// Performs an HTTP request and decodes the response into the specified type.
  /// - Parameter request: The request to perform, containing path, method, headers, and body.
  /// - Returns: The decoded response of type `T`.
  /// - Throws: `StaxNetworkError` if the request fails or the response cannot be decoded.
  func perform<T: Decodable>(_ request: StaxApiRequest<T>) async throws -> T {
    if #available(iOS 15.0, *) {
      return try await performAsync(request)
    } else {
      return try await performCallback(request)
    }
  }

  /// Performs an HTTP request using native async/await APIs available in iOS 15+.
  /// - Parameter request: The request to perform, containing path, method, headers, and body.
  /// - Returns: The decoded response of type `T`.
  /// - Throws: `StaxNetworkError` if the request fails or the response cannot be decoded.
  @available(iOS 15.0, *)
  private func performAsync<T: Decodable>(_ request: StaxApiRequest<T>) async throws -> T {
    let req = try createURLRequest(from: request)
    let (data, response) = try await session.data(for: req)
    
    guard let httpResponse = response as? HTTPURLResponse else {
      throw StaxNetworkError.invalidResponse
    }
    
    guard (200...299).contains(httpResponse.statusCode) else {
      throw StaxNetworkError.httpError(httpResponse.statusCode)
    }
    
    do {
      return try decoder.decode(T.self, from: data)
    } catch {
      throw StaxNetworkError.decodingError(error)
    }
  }

  /// Performs an HTTP request using callback-based APIs for iOS versions prior to 15.0.
  /// Wraps the completion-handler based networking in an async context.
  /// - Parameter request: The request to perform, containing path, method, headers, and body.
  /// - Returns: The decoded response of type `T`.
  /// - Throws: `StaxNetworkError` if the request fails or the response cannot be decoded.
  private func performCallback<T: Decodable>(_ request: StaxApiRequest<T>) async throws -> T {
    return try await withCheckedThrowingContinuation { continuation in
      do {
        let req = try createURLRequest(from: request)
        let task = session.dataTask(with: req) { data, response, error in
          if let error = error {
            continuation.resume(throwing: StaxNetworkError.underlying(error))
            return
          }

          guard let httpResponse = response as? HTTPURLResponse else {
            continuation.resume(throwing: StaxNetworkError.invalidResponse)
            return
          }

          guard (200...299).contains(httpResponse.statusCode) else {
            continuation.resume(throwing: StaxNetworkError.httpError(httpResponse.statusCode))
            return
          }

          guard let data = data else {
            continuation.resume(throwing: StaxNetworkError.invalidResponse)
            return
          }

          do {
            let decoded = try self.decoder.decode(T.self, from: data)
            continuation.resume(returning: decoded)
          } catch {
            continuation.resume(throwing: StaxNetworkError.decodingError(error))
          }
        }
        task.resume()
      } catch {
        continuation.resume(throwing: error)
      }
    }
  }
  
  /// Creates a URLRequest from a StaxApiRequest.
  /// - Parameter request: The StaxApiRequest to convert.
  /// - Returns: A configured URLRequest ready to be performed.
  /// - Throws: An error if the request body cannot be encoded.
  private func createURLRequest<T>(from request: StaxApiRequest<T>) throws -> URLRequest {
    let url = baseURL.appendingPathComponent(request.path)
    var urlRequest = URLRequest(url: url)
    urlRequest.httpMethod = request.method.rawValue

    defaultHeaders.forEach { key, value in
      urlRequest.setValue(value, forHTTPHeaderField: key)
    }
    
    request.headers?.forEach { key, value in
      urlRequest.setValue(value, forHTTPHeaderField: key)
    }

    if let body = request.body {
      let encoder = JSONEncoder()
      encoder.keyEncodingStrategy = .convertToSnakeCase
      urlRequest.httpBody = try encoder.encode(body)
      urlRequest.setValue("application/json", forHTTPHeaderField: "Content-Type")
    }

    return urlRequest
  }
}
