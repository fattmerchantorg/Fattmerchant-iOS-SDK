//
//  CatalogItem.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 8/4/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public struct CatalogItem: Model, Codable {
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
  
  public init(id: String?, item: String?, details: String?, quantity: Int, price: Double) {
     self.id = id
     self.item = item
     self.details = details
     self.quantity = quantity
     self.price = price
   }
}
