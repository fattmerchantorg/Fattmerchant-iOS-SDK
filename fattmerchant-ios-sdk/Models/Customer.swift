//
//  Customer.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// A Customer in the Omni platform
public class Customer: Model {
  public var id: String?
  public var address1: String?
  public var address2: String?
  public var addressCity: String?
  public var addressCountry: String?
  public var addressState: String?
  public var addressZip: String?
  public var company: String?
  public var createdAt: String?
  public var deletedAt: String?
  public var email: String?
  public var firstname: String?
  public var lastname: String?
  public var merchantId: String?
  public var notes: String?
  public var phone: String?
  public var reference: String?
  public var updatedAt: String?
}
