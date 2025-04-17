import Foundation

class MockStaxHttpClient: StaxHttpClientProtocol {
  var mockResponse: Any?
  var mockError: Error?
  
  var lastRequestPath: String?
  var lastRequestMethod: StaxHttpMethod?
  var lastRequestBody: Any?
  
  func perform<T: Decodable>(_ request: StaxApiRequest<T>) async throws -> T {
    lastRequestPath = request.path
    lastRequestMethod = request.method
    lastRequestBody = request.body
    
    if let error = mockError {
      throw error
    }
    
    guard let response = mockResponse as? T else {
      throw StaxNetworkError.decodingError(NSError(domain: "test", code: -1))
    }
    
    return response
  }
}
