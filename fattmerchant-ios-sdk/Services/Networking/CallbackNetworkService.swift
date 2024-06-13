import Foundation

@available(*, deprecated, renamed: "ConcurrentNetworkService", message: "If you are targeting iOS 15 and higher, use the ConcurrentNetworkService.")
class CallbackNetworkService : NetworkService {
  private let token: String
  
  required init(_ token: String) {
    self.token = token
  }
  
  func getApiKey() -> String {
    return token
  }
  
  func fetch(request: URLRequest, completion: @escaping (Bool, Any?) -> Void) {
    let req = urlRequest(path: request.url?.absoluteString ?? "", body: request.httpBody)
    dataTask(request: req, method: request.httpMethod ?? "POST", completion: completion)
  }
  
  func get(path: String, completion: @escaping (Bool, Any?) -> Void) {
    dataTask(request: urlRequest(path: path), method: "GET", completion: completion)
  }
  
  func post(path: String, body: Data? = nil, completion: @escaping (Bool, Any?) -> Void) {
    let request = urlRequest(path: path, body: body)
    dataTask(request: request, method: "POST", completion: completion)
  }
  
  func put(path: String, body: Data?, completion: @escaping (Bool, Any?) -> Void) {
    let request = urlRequest(path: path, body: body)
    dataTask(request: request, method: "PUT", completion: completion)
  }
  
  func patch(path: String, body: Data?, completion: @escaping (Bool, Any?) -> Void) {
    let request = urlRequest(path: path, body: body)
    dataTask(request: request, method: "PATCH", completion: completion)
  }
  
  func delete(path: String, completion: @escaping (Bool, Any?) -> Void) {
    dataTask(request: urlRequest(path: path), method: "DELETE", completion: completion)
  }
  
  private func dataTask(request: NSMutableURLRequest, method: String, completion: @escaping (Bool, Any?) -> Void) {
    request.httpMethod = method
    let session = URLSession(configuration: .default)

    session.dataTask(with: request as URLRequest) { (data, urlResponse, nil) in
      if let response = urlResponse as? HTTPURLResponse {
        switch response.statusCode {
        case 0..<300:
          completion(true, data)
        case 300..<500:
          completion(false, data)
        default:
          completion(data != nil, data)
        }
      } else {
        completion(data != nil, data)
      }
    }.resume()
  }
  
  private func urlRequest(path: String, body: Data? = nil) -> NSMutableURLRequest {
    // Set url
    let baseUrl = Environment.LIVE.baseUrl()!
    let url = baseUrl.appendingPathComponent(path)
    let request = NSMutableURLRequest(url: url)

    // Set headers
    request.addValue("application/json", forHTTPHeaderField: "Content-Type")
    request.addValue("application/json", forHTTPHeaderField: "Accept")
    request.addValue("Bearer \(getApiKey())", forHTTPHeaderField: "Authorization")

    // Set body
    request.httpBody = body

    return request
  }
}
