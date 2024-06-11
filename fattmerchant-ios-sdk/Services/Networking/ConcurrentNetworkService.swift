import Foundation

class ConcurrentNetworkService : NetworkService {
  private var token: String
  
  required init(_ token: String) {
    self.token = token
  }

  func getApiKey() -> String {
    return self.token
  }
  
  func fetch(request: URLRequest, completion: @escaping (Bool, Any?) -> Void) {
    Task {
      let (success, data) = await asyncFetch(request)
      completion(success, data)
    }
  }
  
  func get(path: String, completion: @escaping (Bool, Any?) -> Void) {
    Task {
      let request = generateURLRequest(path, .get)
      let (success, data) = await asyncFetch(request)
      completion(success, data)
    }
  }
  
  func post(path: String, body: Data? = nil, completion: @escaping (Bool, Any?) -> Void) {
    Task {
      let request = generateURLRequest(path, .post, body)
      let (success, data) = await asyncFetch(request)
      completion(success, data)
    }
  }
  
  func put(path: String, body: Data? = nil, completion: @escaping (Bool, Any?) -> Void) {
    Task {
      let request = generateURLRequest(path, .put, body)
      let (success, data) = await asyncFetch(request)
      completion(success, data)
    }
  }
  
  func patch(path: String, body: Data? = nil, completion: @escaping (Bool, Any?) -> Void) {
    Task {
      let request = generateURLRequest(path, .patch, body)
      let (success, data) = await asyncFetch(request)
      completion(success, data)
    }
  }
  
  func delete(path: String, completion: @escaping (Bool, Any?) -> Void) {
    Task {
      let request = generateURLRequest(path, .delete)
      let (success, data) = await asyncFetch(request)
      completion(success, data)
    }
  }
  
  /// A private helper function for running the async request. Catches any errors
  private func asyncFetch(_ req: URLRequest) async -> (Bool, Any?) {
    // Modify the request to use the specified HTTP method
    var request = req
        
    do {
      let session = URLSession(configuration: .default)
      let (data, res) = try await session.data(for: request as URLRequest)
        
      if let response = res as? HTTPURLResponse {
        switch response.statusCode {
          case 0..<300: return (true, data)
          case 300..<600: return (false, data)
          default: return (!data.isEmpty, data)
          }
      } else {
        return (!data.isEmpty, data)
      }
    } catch {
        // Handle any errors that occurred during the request
        return (false, error)
    }
  }
}
