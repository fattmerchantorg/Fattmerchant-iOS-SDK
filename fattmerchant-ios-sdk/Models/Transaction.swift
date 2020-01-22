//
//  Transaction.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public class Transaction: Model {
  var id: String?
  var authId: String?
  var createdAt: String?
  var customerId: String?
  var gateway: String?
  var gatewayId: String?
  var gatewayName: String?
  var invoiceId: String?
  var isCaptured: Int?
  var isManual: Bool?
  var isMerchantPresent: Bool?
  var isRefundable: Bool?
  var issuerAuthCode: String?
  var isVoidable: Bool?
  var isVoided: Bool?
  var lastFour: String?
  var merchantId: String?
  var message: String?
  var meta: JSONValue?
  var method: String?
  var parentAuth: String?
  var paymentMethodId: String?
  var preAuth: Bool?
  var receiptEmailAt: String?
  var receiptSmsAt: String?
  var referenceId: String?
  var scheduleId: String?
  var settledAt: String?
  var source: String?
  var sourceIp: String?
  var response: JSONValue?
  var spreedlyToken: String?
  var success: Bool?
  var total: Double?
  var totalRefunded: Double?
  var type: String?
  var updatedAt: String?
  var userId: String?

//  func getMetaString(key: String) -> String? {
//    return meta?[key] as? String ?? nil
//  }
}
