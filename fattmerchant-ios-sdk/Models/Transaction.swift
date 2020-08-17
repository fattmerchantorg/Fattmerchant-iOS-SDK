//
//  Transaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// A Transaction in the Omni Platform
public class Transaction: Model, Codable {
  public init() {}
  /// The id of the transaction in Omni
  public var id: String?
  /// True when the transaction was successful, false otherwise
  public var success: Bool?
  /// The total amount for the transaction
  public var total: Double?
  /// The amount refunded for the transaction
  public var totalRefunded: Double?
  /// The type of transaction. "charge", "refund", "void"
  public var type: String?
  /// The id of the Customer associated with this Transaction in Omni
  public var customerId: String?
  /// The id of the Invoice associated with this Transaction in Omni
  public var invoiceId: String?
  /// True when this transaction was voided, false otherwise
  public var isVoided: Bool?
  /// The last four digits of the PAN of the card that was used in this transaction
  public var lastFour: String?
  /// The id of the merchant associated with this Transaction
  public var merchantId: String?
  /// A message with some information about the Transaction.
  ///
  /// This can be something like, "Succeeded!"
  public var message: String?
  /// The id of the user who performed this Transaction
  public var userId: String?
  /// The id of the PaymentMethod associated with this Transaction
  public var paymentMethodId: String?
  /// The email address to which the receipt was sent
  public var receiptEmailAt: String?
  /// True when this is a preauthorize transaction, false otherwise
  public var preAuth: Bool?
  /// The id of the schedule Transaction is under
  public var scheduleId: String?
  /// The date that this transaction was created.
  ///
  /// - Note: This is in the format:  "2019-12-25 06:00:00"
  public var createdAt: String?
  /// Metadata that is associated with this Transaction
  public var meta: TransactionMeta?
  /// The method used in this Transaction.
  ///
  /// Typically "card" or "bank"
  public var method: String?
  /// The transaction 'token'
  ///
  /// This is technically the id of the transaction in a foreign platform. The Omni API expects this value when we
  /// create a transaction, but we will likely never receive it.
  internal var token: String?
  public var source: String?
  var authId: String?
  var gateway: String?
  var gatewayId: String?
  var gatewayName: String?
  var isCaptured: Int?
  var isManual: Bool?
  var isMerchantPresent: Bool?
  var isRefundable: Bool?
  var issuerAuthCode: String?
  var isVoidable: Bool?
  var parentAuth: String?
  var receiptSmsAt: String?
  var referenceId: String?
  var settledAt: String?
  var sourceIp: String?
  var response: JSONValue?
  var updatedAt: String?
}

public struct TransactionMeta: Codable {
  public var nmiUserRef: String?
  public var cardEaseReference: String?
  public var nmiTransactionId: String?
  public var awcTransactionId: String?
  public var gatewayResponse: String?
  public var lineItems: [CatalogItem]?
  public enum CodingKeys: CodingKey {
    case nmiUserRef, cardEaseReference, nmiTransactionId, awcTransactionId, gatewayResponse, lineItems
  }
}

extension TransactionMeta {
  public init(from decoder: Decoder) throws {
    do {
      if let container = try? decoder.container(keyedBy: CodingKeys.self) {
        lineItems = try container.decodeIfPresent([CatalogItem].self, forKey: .lineItems)
        nmiUserRef = try container.decodeIfPresent(String.self, forKey: .nmiUserRef)
        cardEaseReference = try container.decodeIfPresent(String.self, forKey: .cardEaseReference)
        nmiTransactionId = try container.decodeIfPresent(String.self, forKey: .nmiTransactionId)
        awcTransactionId = try container.decodeIfPresent(String.self, forKey: .awcTransactionId)
        gatewayResponse = try container.decodeIfPresent(String.self, forKey: .gatewayResponse)
      }
    } catch {
      lineItems = nil
      nmiUserRef = nil
      cardEaseReference = nil
      nmiTransactionId = nil
      awcTransactionId = nil
      gatewayResponse = nil
    }
  }
}
