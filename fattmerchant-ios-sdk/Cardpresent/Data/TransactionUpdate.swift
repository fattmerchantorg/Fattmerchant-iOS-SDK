//
//  TransactionUpdate.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 6/6/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Represents an update in the transaction
///
/// This object will provide information about events with the transaction. Some examples of events
/// are "Transaction Started" or "Signature Provided"
public struct TransactionUpdate {

  /// Describes what the transaction status is
  ///
  /// Example: "Prompt Insert Swipe Card"
  public let value: String

  /// A message that you can show to the end user
  ///
  /// Example: "Please insert or swipe card"
  public let userFriendlyMessage: String?

  public init(_ value: String, _ userFriendlyMessage: String? = nil) {
    self.value = value
    self.userFriendlyMessage = userFriendlyMessage
  }

  /// Request card be swiped or inserted
  public static let PromptInsertSwipeCard = TransactionUpdate("Prompt Insert Swipe Card", "Please insert or swipe card")

  /// Request card be swiped
  public static let PromptSwipeCard = TransactionUpdate("Prompt Swipe Card", "Please swipe card")

  /// Card was swiped
  public static let CardSwiped = TransactionUpdate("Card Swiped")

  /// Card was inserted
  public static let CardInserted = TransactionUpdate("Card Inserted")

  /// Card Swipe error
  public static let CardSwipeError = TransactionUpdate("Card Swipe Error", "Card swipe error. Please try again")

  /// Request card be removed
  public static let PromptRemoveCard = TransactionUpdate("Prompt Remove Card", "Please remove card")

  /// Transaction is being authorized
  public static let Authorizing = TransactionUpdate("Authorizing", "Authorizing. Please wait")

  /// Card was removed
  public static let CardRemoved = TransactionUpdate("Card Removed")

  /// Prompt provide signature
  public static let PromptProvideSignature = TransactionUpdate("Request Signature", "Please provide signature")

  /// Signature provided
  public static let SignatureProvided = TransactionUpdate("Signature Provided")

  /// Prompt user to confirm transaction amount on reader
  ///
  /// This `TransactionUpdate` is only emitted when the mobile reader supports on-device amount confirmation, like the Miura M010
  public static let PromptConfirmAmountOnReader = TransactionUpdate("Prompt Confirm Amount On Reader", "Please confirm amount on reader")
}
