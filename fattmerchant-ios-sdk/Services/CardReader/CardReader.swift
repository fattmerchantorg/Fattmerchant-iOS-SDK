import Foundation

public struct CardReader: CustomStringConvertible {
  public var name: String?
  public var firmware: String?
  public var make: String?
  public var model: String?
  public var serial: String?
  public var connection: String?

  public var description: String {
    return name ?? "<No Name Provided>"
  }
}
