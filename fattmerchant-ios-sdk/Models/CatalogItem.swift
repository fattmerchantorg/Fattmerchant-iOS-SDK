//
//  CatalogItem.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 8/4/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public struct CatalogItem: Model, Codable, Equatable {
  /// The omni id of the item
  public var id: String?
  /// The name of the item
  public var item: String?
  /// The description of the item
  public var details: String?
  ///. The number of items to be purchased
  public var quantity: Int
  /// The price of the item in dollars
  public var price: Double
  /// Is the item a discount
  public var isDiscount: Bool
  /// Is the item a percentage amount
  public var isPercentage: Bool
  /// Is the item active
  public var isActive: Bool
  /// Is the item taxable
  public var isTaxable: Bool
  /// Is the item a service
  public var isService: Bool
  /// Is the item discountable
  public var isDiscountable: Bool

  public var meta: JSONValue?

  public enum CodingKeys: CodingKey {
    case id, item, details, quantity, price, isDiscount, isPercentage, isActive, isTaxable, isService, isDiscountable
  }

  public init(id: String?,
              item: String?,
              details: String?,
              quantity: Int,
              price: Double,
              isActive: Bool = true,
              isTaxable: Bool = true,
              isDiscountable: Bool = true,
              isService: Bool = false,
              isDiscount: Bool = false,
              isPercentage: Bool = false) {
    self.id = id
    self.item = item
    self.details = details
    self.quantity = quantity
    self.price = price
    self.isDiscount = isDiscount
    self.isPercentage = isPercentage
    self.isDiscountable = isDiscountable
    var dict = [String: JSONValue?]()
    dict["is_percentage"] = JSONValue(self.isPercentage)
    dict["is_discountable"] = JSONValue(self.isDiscountable)
    self.meta = dict.jsonValue()
    self.isActive = isActive
    self.isTaxable = isTaxable
    self.isService = isService
  }
}
