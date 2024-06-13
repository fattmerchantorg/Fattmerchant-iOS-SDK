/// An error in the Stax platform
public protocol StaxException: Error {
  static var mess: String { get }
  var message: String { get }
  var detail: String? { get }
}

extension StaxException {
  public var message: String {
    return Self.mess
  }

  var detail: String? {
    return String(describing: self)
  }
}
