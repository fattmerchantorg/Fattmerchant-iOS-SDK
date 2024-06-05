import Foundation

extension NetworkService {
  func generateURLRequest(_ url: String, _ method: HttpMethod, _ data: Data? = nil) -> URLRequest {
    var request = URLRequest(url: URL(string: url)!)
    request.addValue("application/json", forHTTPHeaderField: "Content-Type")
    request.addValue("application/json", forHTTPHeaderField: "Accept")
    request.httpMethod = method.toString()

    // Set Stax Authorization header
    request.addValue("Bearer \(self.getApiKey())", forHTTPHeaderField: "Authorization")
    return request
  }
}
