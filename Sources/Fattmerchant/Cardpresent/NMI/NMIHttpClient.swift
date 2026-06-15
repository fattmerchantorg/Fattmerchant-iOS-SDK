import Foundation

/// Protocol defining the NMI HTTP client interface.
/// Enables testing and dependency injection.
protocol NMIHttpClientProtocol: Sendable {
  /// Performs an HTTP request to the NMI API.
  /// - Parameter request: The request to perform.
  /// - Returns: The decoded response of type `T`.
  /// - Throws: `NMINetworkError` if the request fails.
  func perform<T: Decodable>(_ request: NMIApiRequest<T>) async throws -> T
}

/// A client for making HTTP requests to the NMI API.
/// Handles both modern async/await and legacy callback-based networking depending on iOS version.
final class NMIHttpClient: NMIHttpClientProtocol, Sendable {
  /// Base URL for the NMI API
  private let baseURL: URL
  
  /// API key for authentication
  private let apiKey: String
  
  /// Default HTTP headers
  private let defaultHeaders: [String: String]
  
  /// URL session for making requests
  private let session: URLSession = .shared
  
  /// JSON decoder for response parsing
  private let decoder: JSONDecoder = .init()
  
  /// Creates a new NMI HTTP client.
  /// - Parameters:
  ///   - baseURL: The base URL for the NMI API. Defaults to the production URL.
  ///   - apiKey: The NMI API key for authentication.
  init(baseURL: URL = URL(string: "https://secure.nmi.com")!, apiKey: String) {
    self.baseURL = baseURL
    self.apiKey = apiKey
    self.defaultHeaders = [
      "Accept": "application/json",
      "Content-Type": "application/json"
    ]
    
    decoder.keyDecodingStrategy = .convertFromSnakeCase
  }

  /// Performs an HTTP request and decodes the response into the specified type.
  /// - Parameter request: The request to perform, containing path, method, headers, and body.
  /// - Returns: The decoded response of type `T`.
  /// - Throws: `NMINetworkError` if the request fails or the response cannot be decoded.
  func perform<T: Decodable>(_ request: NMIApiRequest<T>) async throws -> T {
    if #available(iOS 15.0, *) {
      return try await performAsync(request)
    } else {
      return try await performCallback(request)
    }
  }

  /// Performs an HTTP request using native async/await APIs available in iOS 15+.
  /// - Parameter request: The request to perform, containing path, method, headers, and body.
  /// - Returns: The decoded response of type `T`.
  /// - Throws: `NMINetworkError` if the request fails or the response cannot be decoded.
  @available(iOS 15.0, *)
  private func performAsync<T: Decodable>(_ request: NMIApiRequest<T>) async throws -> T {
    let req = try createURLRequest(from: request)
    let (data, response) = try await session.data(for: req)
    
    guard let httpResponse = response as? HTTPURLResponse else {
      throw NMINetworkError.invalidResponse
    }
    
    #if DEBUG
    logResponse(data: data, response: httpResponse, for: req)
    #endif
    
    guard (200...299).contains(httpResponse.statusCode) else {
      throw NMINetworkError.httpError(httpResponse.statusCode)
    }
    
    do {
      return try decoder.decode(T.self, from: data)
    } catch {
      throw NMINetworkError.decodingError(error)
    }
  }

  /// Performs an HTTP request using callback-based APIs for iOS versions prior to 15.0.
  /// Wraps the completion-handler based networking in an async context.
  /// - Parameter request: The request to perform, containing path, method, headers, and body.
  /// - Returns: The decoded response of type `T`.
  /// - Throws: `NMINetworkError` if the request fails or the response cannot be decoded.
  private func performCallback<T: Decodable>(_ request: NMIApiRequest<T>) async throws -> T {
    return try await withCheckedThrowingContinuation { continuation in
      do {
        let req = try createURLRequest(from: request)
        let task = session.dataTask(with: req) { data, response, error in
          if let error = error {
            continuation.resume(throwing: NMINetworkError.underlying(error))
            return
          }

          guard let httpResponse = response as? HTTPURLResponse else {
            continuation.resume(throwing: NMINetworkError.invalidResponse)
            return
          }

          guard (200...299).contains(httpResponse.statusCode) else {
            #if DEBUG
            if let data = data {
              self.logResponse(data: data, response: httpResponse, for: req)
            } else {
              self.logResponse(data: Data(), response: httpResponse, for: req)
            }
            #endif
            continuation.resume(throwing: NMINetworkError.httpError(httpResponse.statusCode))
            return
          }

          guard let data = data else {
            continuation.resume(throwing: NMINetworkError.invalidResponse)
            return
          }

          #if DEBUG
          self.logResponse(data: data, response: httpResponse, for: req)
          #endif

          do {
            let decoded = try self.decoder.decode(T.self, from: data)
            continuation.resume(returning: decoded)
          } catch {
            continuation.resume(throwing: NMINetworkError.decodingError(error))
          }
        }
        task.resume()
      } catch {
        continuation.resume(throwing: error)
      }
    }
  }
  
  /// Creates a URLRequest from an NMIApiRequest.
  /// - Parameter request: The NMIApiRequest to convert.
  /// - Returns: A configured URLRequest ready to be performed.
  /// - Throws: An error if the request body cannot be encoded.
  private func createURLRequest<T>(from request: NMIApiRequest<T>) throws -> URLRequest {
    let url = baseURL.appendingPathComponent(request.path)
    var urlRequest = URLRequest(url: url)
    urlRequest.httpMethod = request.method.rawValue

    defaultHeaders.forEach { key, value in
      urlRequest.setValue(value, forHTTPHeaderField: key)
    }
    
    // Add API key to headers
    urlRequest.setValue(apiKey, forHTTPHeaderField: "X-Api-Key")
    
    request.headers?.forEach { key, value in
      urlRequest.setValue(value, forHTTPHeaderField: key)
    }

    if let body = request.body {
      let encoder = JSONEncoder()
      encoder.keyEncodingStrategy = .convertToSnakeCase
      urlRequest.httpBody = try encoder.encode(body)
      urlRequest.setValue("application/json", forHTTPHeaderField: "Content-Type")
    }

    logRequest(urlRequest)
    return urlRequest
  }

  // ⚠️ TEMP DEBUG — REMOVE BEFORE SHIPPING.
  /// Logs the full outgoing NMI request (method, URL, every header, body) so the
  /// exact request to endpoints like `/v4/processors/reports` can be inspected in
  /// the host app's console. Intentionally NOT gated behind `#if DEBUG` so it shows
  /// regardless of how the SDK is compiled. Prints credentials (X-Api-Key /
  /// Authorization) in plaintext — do not leave this in a release build.
  private func logRequest(_ request: URLRequest) {
    print("📡➡️ NMI Request")
    print("Method: \(request.httpMethod ?? "nil")")
    print("URL: \(request.url?.absoluteString ?? "nil")")
    if let headers = request.allHTTPHeaderFields, !headers.isEmpty {
      print("Headers:")
      for (key, value) in headers {
        print("  \(key): \(value)")
      }
    } else {
      print("Headers: (none)")
    }
    if let body = request.httpBody, let bodyString = String(data: body, encoding: .utf8) {
      print("Body: \(bodyString)")
    }
  }

  #if DEBUG
  /// Logs HTTP response details for debugging purposes.
  /// - Parameters:
  ///   - data: The response data.
  ///   - response: The HTTP response.
  ///   - request: The original URL request.
  private func logResponse(data: Data, response: HTTPURLResponse, for request: URLRequest) {
    print("📡 NMI Response")
    print("URL: \(request.url?.absoluteString ?? "Unknown")")
    print("Status: \(response.statusCode)")
    if let body = String(data: data, encoding: .utf8) {
      print("Body: \(body)")
    }
  }
  #endif
}
