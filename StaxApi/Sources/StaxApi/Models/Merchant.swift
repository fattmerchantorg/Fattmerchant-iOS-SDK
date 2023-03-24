import Foundation

/// A Merchant in the Stax platform
class Merchant: Codable {
  public var id: String?
  private var companyName: String?

  var hostedPaymentsToken: String?
  var options: JSONValue?

  public var name: String? {
    get {
      return companyName
    }
  }
  
  public var emvPassword: String? {
    get {
      return getOptionString("emv_password")
    }
  }

  public var emvTerminalSecret: String? {
    get {
      return getOptionString("emv_terminal_secret")
    }
  }


  public var emvTerminalId: String? {
    get {
      return getOptionString("emv_terminal_id")
    }
  }

  /// Gets an 'option' as a String from the `options` object.
  ///
  /// ## Example:
  /// Given that a merchant has an option called "emv_password", you can get that value by calling this function
  /// ```
  /// // Merchant {
  /// //   id: "123",
  /// //   options: {
  /// //     "emv_password": "foobar"
  /// //   }
  /// // }
  /// getOptionString("emv_password") // -> "foobar"
  /// getOptionString("color") // -> nil
  /// ```
  ///
  /// - Parameter key: the key of the option
  /// - Returns: the value of the option, if found. nil otherwise
  private func getOptionString(_ key: String) -> String? {
    guard let options = options else { return nil }

    if case .object(let dict) = options, let valueJson = dict[key], valueJson != nil,
       case .string(let value) = valueJson! {
      return value
    }

    return nil
  }
}
