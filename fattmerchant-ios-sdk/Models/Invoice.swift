//
//  Invoice.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// An Invoice in the Omni Platform
class Invoice: Model {
  public var id: String?
  public var balanceDue: Double?
  public var createdAt: String?
  public var customerId: String?
  public var deletedAt: String?
  public var dueAt: String?
  public var isMerchantPresent: Bool?
  public var isWebpayment: Bool?
  public var merchantId: String?
  public var meta: JSONValue?
  public var paidAt: String?
  public var paymentAttemptFailed: Bool?
  public var paymentAttemptMessage: String?
  public var paymentMethodId: String?
  public var scheduleId: String?
  public var sentAt: String?
  public var status: String?
  public var total: Double?
  public var totalPaid: Double?
  public var updatedAt: String?
  public var url: String?
  public var userId: String?
  public var viewedAt: String?
  var channel: String = "ios"
}
