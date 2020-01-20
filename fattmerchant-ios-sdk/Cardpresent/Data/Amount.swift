//
//  Amount.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public struct Amount {
  public let cents: Int

  public init(cents: Int) {
    self.cents = cents
  }

  public init(dollars: Double) {
    self.cents = Int(dollars * 100.0)
  }

  public func centsString() -> String {
    return String(cents)
  }

  public func dollarsString() -> String {
    return String.init(format: "%.2f", dollars())
  }

  public func dollars() -> Double {
    return Double(cents) / 100.0
  }

  public func pretty() -> String {
    return "$\(dollarsString())"
  }

}
