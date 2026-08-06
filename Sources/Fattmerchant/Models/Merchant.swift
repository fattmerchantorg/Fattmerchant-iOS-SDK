//
//  Merchant.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// A Merchant in the Omni platform
class Merchant: Model, Codable {

  /// The omni id of the merchant
  public var id: String?

  /// A token that enables hosted webpayments
  internal var hostedPaymentsToken: String?

  /// An object that holds optional information about the merchant
  ///
  /// Inside options, there is
  internal var options: JSONValue?

  /// Gets the 'emvPassword' option from the Merchant
  ///
  /// The 'emvPassword' is used for authenticating with a third-party provider of mobile reader
  /// - Returns: The 'emvPassword', if any. Nil if none found
  func emvPassword() -> String? {
    return getOptionString("emv_password")
  }

  /// Gets the 'emvTerminalSecret' option from the Merchant
  ///
  /// The 'emvTerminalSecret' is used for authenticating with a third-party provider of mobile reader
  /// - Returns: The 'emvTerminalSecret', if any. Nil if none found
  func emvTerminalSecret() -> String? {
    return getOptionString("emv_terminal_secret")
  }

  /// Gets the 'emvTerminalId' option from the Merchant
  ///
  /// The 'emvTerminalId' is used for authenticating with a third-party provider of mobile reader
  /// - Returns: The 'emvTerminalId', if any. Nil if none found
  func emvTerminalId() -> String? {
    return getOptionString("emv_terminal_id")
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

    if
      case .object(let dict) = options,
      let valueJson = dict[key],
      valueJson != nil,
      case .string(let value) = valueJson!  {
      return value
    }

    return nil
  }

}
