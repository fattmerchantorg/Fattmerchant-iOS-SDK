import Foundation

protocol NetworkService {
  /// Initialize a new Stax HTTP network service given an API Key
  /// > Warning: This does not check if the API Key is valid
  /// - Parameters
  ///   - token: The Stax API Key
  init(_ token: String)
  
  /// Gets the Stax API Key for the Network Service
  /// - returns: The Stax API Key
  func getApiKey() -> String
  
  /// Performs a generic HTTP request given a `URLRequest` parameter.
  /// - Parameters
  ///   - `request`: A `URLRequest` to perform
  ///   - `completion`: A callback function that returns the a `Tuple` containing a `Bool`
  ///     result for success or failure, and an `Any?` value for the data returned in the response.
  func fetch(request: URLRequest, completion: @escaping (Bool, Any?) -> Void)
  
  /// Performs a GET  request given a `String` path.
  /// - Parameters
  ///   - `path`: A `String` representation of the HTTP endpoint to hit.
  ///   - `completion`: A callback function that returns the a `Tuple` containing a `Bool`
  ///     result for success or failure, and an `Any?` value for the data returned in the response.
  func get(path: String, completion: @escaping (Bool, Any?) -> Void)
  
  /// Performs a POST request given a `String` path.
  /// - Parameters
  ///   - `path`: A `String` representation of the HTTP endpoint to hit.
  ///   - `body` An optional `Data` parameter for the request body. Defaults to `nil`.
  ///   - `completion`: A callback function that returns the a `Tuple` containing a `Bool`
  ///     result for success or failure, and an `Any?` value for the data returned in the response.
  func post(path: String, body: Data?, completion: @escaping (Bool, Any?) -> Void)
  
  /// Performs a PUT request given a `String` path.
  /// - Parameters
  ///   - `path`: A `String` representation of the HTTP endpoint to hit.
  ///   - `body` An optional `Data` parameter for the request body. Defaults to `nil`.
  ///   - `completion`: A callback function that returns the a `Tuple` containing a `Bool`
  ///     result for success or failure, and an `Any?` value for the data returned in the response.
  func put(path: String, body: Data?, completion: @escaping (Bool, Any?) -> Void)
  
  /// Performs a PATCH  request given a `String` path.
  /// - Parameters
  ///   - `path`: A `String` representation of the HTTP endpoint to hit.
  ///   - `body` An optional `Data` parameter for the request body. Defaults to `nil`.
  ///   - `completion`: A callback function that returns the a `Tuple` containing a `Bool`
  ///     result for success or failure, and an `Any?` value for the data returned in the response.
  func patch(path: String, body: Data?, completion: @escaping (Bool, Any?) -> Void)
  
  /// Performs a DELETE request given a `String` path.
  /// - Parameters
  ///   - `path`: A `String` representation of the HTTP endpoint to hit.
  ///   - `completion`: A callback function that returns the a `Tuple` containing a `Bool`
  ///     result for success or failure, and an `Any?` value for the data returned in the response.
  func delete(path: String, completion: @escaping (Bool, Any?) -> Void)
}
