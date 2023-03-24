import Foundation

/// An error in the Stax platform
public protocol StaxError: Error {
  static var mess: String { get }
  var message: String { get }
  var detail: String { get }
}

/// An error that occurs when communicating over Http
public class StaxHttpError: StaxError {
  public static var mess: String = "Stax Http Error"
  public let message: String
  public let detail: String
  public let data: Data
  public let response: HTTPURLResponse

  public init(message: String, detail: String, data: Data, response: HTTPURLResponse) {
    self.detail = detail
    self.message = message
    self.data = data
    self.response = response
  }
}
