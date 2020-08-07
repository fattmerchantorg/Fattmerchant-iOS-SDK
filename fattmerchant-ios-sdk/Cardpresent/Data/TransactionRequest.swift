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
/// ## Paying a specific invoice
/// If you want to associate the transaction with a specific invoice, include the id of the invoice in the
/// `invoiceId` field
public struct TransactionRequest {

  /// The `Amount` to be collected during the transaction
  public var amount: Amount
  /// The `CreditCard` to charge
  public var card: CreditCard?
  /// The `LineItem`s being passed to the transaction
  public var lineItems: [CatalogItem]?
  /// The option to tokenize the payment method for later usage
  ///
  /// - Note: Defaults to true
  ///
  /// Set this to false if you do not want the payment method stored
  public var tokenize: Bool = true
  /// The id of the invoice that this payment should be applied to
  ///
  /// If nil, then a new invoice will be created
  public var invoiceId: String?
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
  /// Initializes a TransactionRequest with the given amount and explicitly sets the tokenize value
  /// - Parameters:
  ///   - amount: The `Amount` to be collected during the transaction
  ///   - tokenize: A value that dictates whether or not the payment method used in the transaction
  ///   should be tokenized. Defaults to `true`
  ///   - invoiceId: The invoice to associate this payment with. If nil, a new invoice is created. Defaults to nil.
  public init(amount: Amount, tokenize: Bool = true, invoiceId: String? = nil) {
    self.amount = amount
    self.tokenize = tokenize
    self.invoiceId = invoiceId
  }
  /// Initializes a TransactionRequest with the given amount and a list of line items
  /// - Parameters:
  ///   - amount: The `Amount` to be collected during the transaction
  ///   - lineItems: A list of  `LineItem` that are attached to the transaction
  public init(amount: Amount, lineItems: [CatalogItem]?) {
    self.amount = amount
    self.lineItems = lineItems
  }
  /// Initializes a TransactionRequest with the given amount and a list of line items
  /// - Parameters:
  ///   - amount: The `Amount` to be collected during the transaction
  ///   - card: A `CreditCard` to charge
  ///   - lineItems: A list of  `LineItem` that are attached to the transaction
  public init(amount: Amount, card: CreditCard, lineItems: [CatalogItem]?) {
    self.amount = amount
    self.card = card
    self.lineItems = lineItems
  }
  /// Initializes a TransactionRequest with the given amount, explicitly sets the tokenize value and a contains a list of line items
  /// - Parameters:
  ///   - amount: The `Amount` to be collected during the transaction
  ///   - tokenize: A value that dictates whether or not the payment method used in the transaction
  ///   should be tokenized.
  ///   - card: A `CreditCard` to charge
  ///   - lineItems: A list of  `LineItem` that are attached to the transaction
  public init(amount: Amount, tokenize: Bool, card: CreditCard, lineItems: [CatalogItem]?) {
    self.amount = amount
    self.tokenize = tokenize
    self.card = card
    self.lineItems = lineItems
  }
}
