//
//  TransactionRequest.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// An object that represents an intent to make a transaction
///
/// This object contains necessary data about transaction parameters, such as how much money should
/// be collected and whether or not the payment method should be tokenized
///
/// ## Example usage with Tokenization
/// ```
/// // With tokenization
/// let amount = Amount(cents: 1999) // $19.99
/// let request = TransactionRequest(amount: amount)
/// ```
/// ## Sample without Tokenization
/// ```
/// // Without tokenization
/// let amount = Amount(cents: 1999) // $19.99
/// let request = TransactionRequest(amount: amount, tokenize: false)
/// ```
/// ## Charging a card without a mobile reader
/// ```
/// // With given card
/// let card = CreditCard(personName: "Joan Parsnip", cardNumber: "4111111111111111", cardExp: "1230", addressZip: "32822")
/// let request = TransactionRequest(amount: amount, card: card)
/// ```
public struct TransactionRequest {

  /// The `Amount` to be collected during the transaction
  public var amount: Amount

  /// The `CreditCard` to charge
  public var card: CreditCard?

  /// The option to tokenize the payment method for later usage
  ///
  /// - Note: Defaults to true
  ///
  /// Set this to false if you do not want the payment method stored
  public var tokenize: Bool = true

  /// Initializes a TransactionRequest with the given amount.
  ///
  /// - Parameter amount: The  `Amount` to be collected during the transaction
  public init(amount: Amount) {
    self.amount = amount
  }

  /// Initializes a TransactionRequest with the given amount.
  ///
  /// - Parameter amount: The  `Amount` to be collected during the transaction
  public init(amount: Amount, card: CreditCard) {
    self.amount = amount
    self.card = card
  }

  /// Initializes a TransactionRequest with the given amount and explicitly sets the tokenize value
  /// - Parameters:
  ///   - amount: The `Amount` to be collected during the transaction
  ///   - tokenize: A value that dictates whether or not the payment method used in the transaction
  ///   should be tokenized
  public init(amount: Amount, tokenize: Bool) {
    self.amount = amount
    self.tokenize = tokenize
  }

  /// Initializes a TransactionRequest with the given amount and explicitly sets the tokenize value
  /// - Parameters:
  ///   - amount: The `Amount` to be collected during the transaction
  ///   - tokenize: A value that dictates whether or not the payment method used in the transaction
  ///   should be tokenized
  ///   - card: A `CreditCard` to charge
  public init(amount: Amount, tokenize: Bool, card: CreditCard) {
    self.amount = amount
    self.tokenize = tokenize
    self.card = card
  }
}
