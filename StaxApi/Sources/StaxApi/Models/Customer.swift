import Foundation

/// A Customer in the Stax platform
public class Customer: Codable {
  public var id: String?
  public var address1: String?
  public var address2: String?
  public var addressCity: String?
  public var addressCountry: String?
  public var addressState: String?
  public var addressZip: String?
  public var company: String?
  public var createdAt: String?
  public var deletedAt: String?
  public var email: String?
  public var firstname: String
  public var lastname: String
  public var merchantId: String?
  public var notes: String?
  public var phone: String?
  public var reference: String?
  public var updatedAt: String?

  /// Takes a full name and splits it into first and last
  ///
  /// This is the opposite of joinName
  /// - Parameter name: the full name
  /// - Returns: a tuple containing the first and last names
  static func splitName(_ name: String) -> (first: String, last: String) {
    var first = ""
    var last = ""
    var name = name.split(separator: " ")
    if !name.isEmpty {
      first = String(name.removeFirst())
      last = name.joined(separator: " ")
    }

    return (first, last)
  }

  /// Takes a first and last name and joins it into one full name
  ///
  /// This is the opposite of splitName
  static func joinName(first: String, last: String) -> String {
    return "\(first) \(last)"
  }
  
  init(firstName: String, lastName: String) {
    self.firstname = firstName
    self.lastname = lastName
  }
  

  /// Initializes a Customer with the full name
  ///
  /// This takes the full name and splits it into first and last
  /// - Parameter fullName: the customers full name
  init(fullName: String) {
    let names = Customer.splitName(fullName)
    self.firstname = names.first
    self.lastname = names.last
  }
  
  func fullName() -> String {
      return Customer.joinName(first: firstname, last: lastname)
  }
}
