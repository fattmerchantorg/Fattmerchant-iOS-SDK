import Foundation

/// An error in the Omni platform
public protocol OmniException: Error {
  static var mess: String { get }
  var message: String { get }
  var detail: String? { get }
}

extension OmniException {
  public var message: String {
    return Self.mess
  }

  var detail: String? {
    return String(describing: self)
  }
}
