import Foundation

public func convertJsonStringToDictionary(text: String?) -> [String: Any]? {
  if let data = text?.data(using: .utf8) {
    do {
      return try JSONSerialization.jsonObject(with: data, options: []) as? [String: Any]
    } catch {
      print(error.localizedDescription)
    }
  }
  return nil
}
