import Foundation

/**
 A basic Stax Api Client that communicates with the Stax API.
 
 The Stax API Clent is able to do basic API actions such as retriving, modifying & creating Stax resources that
 include, ``Customer``, ``Invoice``, ``Transaction`` and more.
 
 To use the Stax API client, just create an instance of it with your Stax API Key.
 ```
 let api = StaxApi("MyApiKey")
 ```
 
 The Stax API client is an [actor](https://docs.swift.org/swift-book/documentation/the-swift-programming-language/concurrency/#Actors), and uses [Swift Concurrency](https://docs.swift.org/swift-book/documentation/the-swift-programming-language/concurrency/). To use it, you should be familiar with `async` and `await` programming.
 */
public actor StaxApi {
  private let encoder = JSONEncoder()
  private let decoder = JSONDecoder()
  private let token: String
  
  private static let baseUrl = "https://apiprod.fattlabs.com"
  
  /**
   Create a new instance Stax API http client
   - parameter token: Your Stax API token
   */
  public init (_ token: String) {
    encoder.keyEncodingStrategy = .convertToSnakeCase
    decoder.keyDecodingStrategy = .convertFromSnakeCase
    self.token = token
  }
  
  /**
  Makes a `GET` http request to `https://apiprod.fattlabs.com/self`
   - returns: A ``StaxSelf`` instance with details about your Stax merchant.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned.
   - throws: A ``DecodingError.dataCorrupted`` error if the returned JSON is malformed.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  public func getSelf() async throws -> StaxSelf {
    let data = try await get(resource: "/self")
    return try decoder.decode(StaxSelf.self, from: data)
  }
  
  /**
   Makes a paginated `GET` http request to `https://apiprod.fattlabs.com/customer`.
   - parameter page: The customer page to get data from. Defaults to 1.
   - parameter size: The size of the paginated data. Defaults to 50.
   - returns: A list of Stax ``Customer`` objects as `[Customer]`.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned
   - throws: A ``DecodingError.dataCorrupted`` error if the returned JSON is malformed.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  public func getCustomers(page: Int = 1, size: Int = 50) async throws -> [Customer] {
    let data = try await get(resource: "/customer?page=\(page)&per_page=\(size)")
    let decoded = try decoder.decode(PaginatedResponse<Customer>.self, from: data)
    return decoded.data ?? []
  }
  
  /**
   Makes a `GET` http request to `https://apiprod.fattlabs.com/customer/{id}`.
   - parameter id: The Stax customer ID to get data from.
   - returns: An optional ``Customer`` object assosciated with the provided ID.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned.
   - throws: A ``DecodingError.dataCorrupted`` error if the returned JSON is malformed.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  public func getCustomer(id: String) async throws -> Customer? {
    let data = try await get(resource: "/customer/\(id)")
    return try? decoder.decode(Customer.self, from: data)
  }
  
  /**
   Makes a `POST` http request to `https://apiprod.fattlabs.com/customer`.
   - parameter customer: The Stax ``Customer`` object to create via the Stax API.
   - returns: The newly created ``Customer`` return from the Stax API.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned
   - throws: A ``DecodingError.dataCorrupted`` error if the returned JSON is malformed.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  public func createCustomer(customer: Customer) async throws -> Customer? {
    let body = try encoder.encode(customer)
    let data = try await post(resource: "/customer", body: body)
    return try? decoder.decode(Customer.self, from: data)
  }
  
  /**
   Makes a `PUT` http request to `https://apiprod.fattlabs.com/customer`.
   - parameter id: The Stax customer ID assosciated with the ``Customer`` to update.
   - parameter customer: The Stax ``Customer`` object to modify via the Stax API.
   - returns: The newly updated ``Customer`` returned from the Stax API.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned
   - throws: A ``DecodingError.dataCorrupted`` error if the returned JSON is malformed.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  public func updateCustomer(id: String, customer: Customer) async throws -> Customer? {
    let body = try encoder.encode(customer)
    let data = try await put(resource: "/customer/\(id)", body: body)
    return try? decoder.decode(Customer.self, from: data)
  }
  
  /**
   Makes a `DELETE` http request to `https://apiprod.fattlabs.com/customer/{id}`.
   - parameter id: The Stax customer ID assosciated with the ``Customer`` to delete.
   - returns: The recently deleted ``Customer`` returned from the Stax API.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned
   - throws: A ``DecodingError.dataCorrupted`` error if the returned JSON is malformed.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  public func deleteCustomer(id: String) async throws -> Customer? {
    let data = try await delete(resource: "/customer/\(id)")
    return try? decoder.decode(Customer.self, from: data)
  }
  
  /**
   Makes a generic `GET` http request.
   - parameter resource: The resource of the baseUrl to hit.
   - returns: The ``Data`` class returned from the response.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  private func get(resource: String) async throws -> Data {
    return try await self.request(method: "GET", resource: resource)
  }
  
  /**
   Makes a generic `POST` http request
   - parameter resource: The resource of the baseUrl to hit.
   - parameter body: The optional request body. Defaults to `nil`.
   - returns: The ``Data`` class returned from the response.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  private func post(resource: String, body: Data? = nil) async throws -> Data {
    return try await self.request(method: "POST", resource: resource, body: body)
  }
  
  /**
   Makes a generic `PUT` http request
   - parameter resource: The resource of the baseUrl to hit.
   - parameter body: The optional request body. Defaults to `nil`.
   - returns: The ``Data`` class returned from the response.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  private func put(resource: String, body: Data? = nil) async throws -> Data {
    return try await self.request(method: "PUT", resource: resource, body: body)
  }
  
  /**
   Makes a generic `PATCH` http request
   - parameter resource: The resource of the baseUrl to hit.
   - parameter body: The optional request body. Defaults to `nil`.
   - returns: The ``Data`` class returned from the response.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  private func patch(resource: String, body: Data? = nil) async throws -> Data {
    return try await self.request(method: "PATCH", resource: resource, body: body)
  }
  
  /**
   Makes a generic `DELETE` http request
   - parameter resource: The resource of the baseUrl to hit.
   - returns: The ``Data`` class returned from the response.
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned.
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  private func delete(resource: String) async throws -> Data {
    return try await self.request(method: "DELETE", resource: resource)
  }
  
  /**
   A generic http request
   - parameter method : The http method used for the request.
   - parameter resource: The resource of the baseUrl to hit.
   - returns: The ``Data`` class returned from the response
   - throws: A ``StaxHttpError`` if a 4XX or 5XX status code is returned
   - throws: A ``URLError.badServerResponse`` if the http response is malformed.
   */
  private func request(method: String, resource: String, body: Data? = nil) async throws -> Data {
    guard let url = URL(string: StaxApi.baseUrl + resource) else {
      throw URLError(.badURL)
    }
    
    var request = URLRequest(url: url)
    request.addValue("application/json", forHTTPHeaderField: "Content-Type")
    request.addValue("application/json", forHTTPHeaderField: "Accept")
    request.addValue("Bearer \(token)", forHTTPHeaderField: "Authorization")
    request.httpMethod = method
    request.httpBody = body

    let (data, response)  = try await URLSession.shared.data(for: request)
    guard let httpResponse = response as? HTTPURLResponse else {
      throw URLError(.badServerResponse)
    }
    
    switch httpResponse.statusCode {
      case 400...499: throw handle400Error(data, httpResponse)
      case 500...599: throw handle500Error(data, httpResponse)
      case 200...299: fallthrough
      default: return data
    }
  }
  
  private func handle400Error(_ data: Data, _ response: HTTPURLResponse) -> StaxError {
    let str = String(bytes: data, encoding: .utf8)
    if let dictionary = convertJsonStringToDictionary(text: str), let errorMsg = dictionary["error"] as? String {
      return StaxHttpError(
        message: "HTTP \(response.statusCode)",
        detail: errorMsg,
        data: data,
        response: response
      )
    }
    
    return StaxHttpError(
      message: "HTTP \(response.statusCode)",
      detail: "Unexpected 400 level error received when attempting to connect to the Stax API",
      data: data,
      response: response
    )
  }
  
  private func handle500Error(_ data: Data, _ response: HTTPURLResponse) -> StaxError {
    return StaxHttpError(
      message: "HTTP \(response.statusCode)",
      detail: "Unexpected 500 level error received when attempting to connect to the Stax API",
      data: data,
      response: response
    )
  }
}