//
//  Merchant.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class Merchant: Model, Codable {
  var id: String?
  var options: JSONValue?

  func emvPassword() -> String? {
    return getOptionString("emv_password")
  }

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
