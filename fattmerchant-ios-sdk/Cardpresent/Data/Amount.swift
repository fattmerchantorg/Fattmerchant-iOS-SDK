//
//  Amount.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

struct Amount {
  let cents: Int

  init(cents: Int) {
    self.cents = cents
  }

  init(dollars: Double) {
    self.cents = Int(dollars * 100.0)
  }

  func centsString() -> String {
    return String(cents)
  }

  func dollarsString() -> String {
    return String.init(format: "%.2f", dollars())
  }

  func dollars() -> Double {
    return Double(cents) / 100.0
  }

  func pretty() -> String {
    return "$\(dollarsString())"
  }

}
