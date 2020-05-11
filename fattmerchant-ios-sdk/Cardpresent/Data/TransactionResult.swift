//
//  TransactionResult.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// An object that represents the result of a transaction
///
/// This object contains necessary data about the transaction that occured, such as how much money was
/// collected and details about the payment method used
struct TransactionResult {

  var request: TransactionRequest?

  var success: Bool?

  /// The place where the transaction took place. For example, "NMI" or "AWC"
  var source: String = ""

  /**
   * The masked card number
   *
   * This is *not* the entire card number! The card number will have a mask on so only the last four digits are shown
   *
   * For example: "4111111111111111" should be "************1111"
   * */
  internal var maskedPan: String?

  /** The first name of the cardholder */
  var cardHolderFirstName: String?

  /** The last name of the cardholder */
  var cardHolderLastName: String?

  /** The code that authorizes the sale */
  var authCode: String?

  /** Sale, Refund, etc */
  var transactionType: String?

  /** Amount of money that was exchanged */
  var amount: Amount?

  /** VISA, Mastercard, JCB, etc */
  var cardType: String?

  /** A user-defined string used to refer to the transaction */
  var userReference: String?

  /// The ID of this transaction in the 3rd party responsible for performing it.
  ///
  /// For example, in the case of NMI, this would be the TransactionID
  var externalId: String?

  /// The ID of this transaction in the local database
  ///
  /// For example, in the case of NMI, this would be the CardEaseReferenceId
  var localId: String?

  /// The gateway response in its entirety
  var gatewayResponse: String?

  /// The token that represents this payment method
  internal var paymentToken: String?
}
