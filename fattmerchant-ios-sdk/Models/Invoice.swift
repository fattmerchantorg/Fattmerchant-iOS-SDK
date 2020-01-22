//
//  Invoice.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class Invoice: Model {
  var id: String?
  var balanceDue: Double?
  var createdAt: String?
  var customerId: String?
  var deletedAt: String?
  var dueAt: String?
  var isMerchantPresent: Bool?
  var isWebpayment: Bool?
  var merchantId: String?
  var meta: JSONValue?
  var paidAt: String?
  var paymentAttemptFailed: Bool?
  var paymentAttemptMessage: String?
  var paymentMethodId: String?
  var scheduleId: String?
  var sentAt: String?
  var status: String?
  var total: Double?
  var totalPaid: Double?
  var updatedAt: String?
  var url: String?
  var userId: String?
  var viewedAt: String?
}
