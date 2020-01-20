//
//  Customer.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class Customer: Model {
  var id: String?
  var address1: String?
  var address2: String?
  var addressCity: String?
  var addressCountry: String?
  var addressState: String?
  var addressZip: String?
  var allowInvoiceCreditCardPayments: Bool?
  var ccEmails: String?
  var ccSms: String?
  var company: String?
  var createdAt: String?
  var deletedAt: String?
  var email: String?
  var firstname: String?
  var lastname: String?
  var merchantId: String?
  var notes: String?
  var phone: String?
  var reference: String?
  var updatedAt: String?
}
