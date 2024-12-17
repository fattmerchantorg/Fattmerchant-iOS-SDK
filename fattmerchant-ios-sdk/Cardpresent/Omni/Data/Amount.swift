//
//  Amount.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/**
 An object that represents an amount of money

 Use and instance of `Amount` to work safely with amounts of money. Avoiding the passing of dollars and cents eliminates ambiguity in the transaction flow

 ## Usage
 ```
 var amount = Amount(cents: 599)
 // or
 var amount = Amount(dollars: 5.99)
 ```
 */
public struct Amount {
  public let cents: Int

  public init(cents: Int) {
    self.cents = cents
  }

  /// Initializes the Amount with the given dollar amount
  ///
  /// - Note: This converts dollars into cents, truncating everything to two significant figures
  /// - Parameter dollars: the amount in dollars
  ///
  /// ### Example
  /// ```
  /// let amount = Amount(dollars: 4.9876)
  /// amount.cents // => 498
  /// ```
  public init(dollars: Double) {
    // Use Decimal to get the exact product of dollars and 100
    let centsDecimal = Decimal(dollars) * Decimal(100.0)

    // Turn it to a string and take everything before the decimal place.
    // This is a safe way to turn a Decimal into an Int
    if let centsString = centsDecimal.description.split(separator: ".", maxSplits: 1, omittingEmptySubsequences: true).first,
        let centsInt = Int(centsString) {
      self.cents = centsInt
      return
    }

    self.cents = Int(dollars * 100.0)
  }

  /// A string representation of the amount in cents
  ///
  /// ### Example
  /// ```
  /// let amount = Amount(dollars: 5.99378)
  /// amount.dollarsString() // => "599"
  /// ```
  public func centsString() -> String {
    return String(cents)
  }

  /// A string representation of the dollar amount with two significant figures. Does **not** have the dollar sign
  ///
  /// ### Example
  /// ```
  /// let amount = Amount(cents: 599)
  /// amount.dollarsString() // => "5.99"
  ///
  /// let amount2 = Amount(dollars: 5.9912838)
  /// amount2.dollarsString() // => "5.99"
  /// ```
  public func dollarsString() -> String {
    return String.init(format: "%.2f", dollars())
  }

  /// A string representation of the dollar amount with two significant figures. Does **not** have the dollar sign
  ///
  /// ### Example
  /// ```
  /// let amount = Amount(dollars: 5.99378)
  /// amount.dollars() // => "5.99"
  /// ```
  public func dollars() -> Double {
    return Double(cents) / 100.0
  }

  /// A string representation of the dollar amount with two significant figures and a dollar sign
  ///
  /// ### Example
  /// ```
  /// let amount = Amount(cents: 5.99378)
  /// amount.dollarsString() // => "$5.99"
  /// ```
  public func pretty() -> String {
    return "$\(dollarsString())"
  }

}
