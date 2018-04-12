//
//  CreditCard.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 4/5/18.
//  Copyright © 2018 Fattmerchant, inc. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

import Foundation

/// A credit card
public struct CreditCard: Codable {
  public var personName: String
  public var cardNumber: String
  public var cardExp: String
  public var addressZip: String
  public var address1: String?
  public var address2: String?
  public var addressCity: String?
  public var addressState: String?
  public var note: String?
  public var phone: String?
  public var email: String?
  private var method: String = "card"

  public init(personName: String, cardNumber: String, cardExp: String, addressZip: String) {
    self.personName = personName
    self.cardNumber = cardNumber
    self.cardExp = cardExp
    self.addressZip = addressZip
  }

  private enum CodingKeys: String, CodingKey {
    case note, phone, email, method,
    personName = "person_name",
    cardNumber = "card_number",
    cardExp = "card_exp",
    addressZip = "address_zip",
    address1 = "address_1",
    address2 = "address_2",
    addressCity = "address_city",
    addressState = "address_state"
  }

  /// Creates a test credit card
  ///
  /// - Returns: a test credit card
  public static func testCreditCard() -> CreditCard {
    var creditCard = CreditCard(personName: "Joan Parsnip", cardNumber: "4111111111111111", cardExp: "1230", addressZip: "32822")
    creditCard.address1 = "123 Orange Ave"
    creditCard.address2 = "Unit 309"
    creditCard.addressCity = "Orlando"
    creditCard.addressState = "FL"
    creditCard.phone = "3210000000"
    creditCard.email = "fatt@merchant.com"
    creditCard.note = "This is a test credit card"

    return creditCard
  }

  /// Creates a test credit card that fails processing
  ///
  /// - Returns: a test credit card
  public static func failingTestCreditCard() -> CreditCard {
    var creditCard = CreditCard(personName: "Joan Parsnip", cardNumber: "4111111111111111", cardExp: "", addressZip: "32822")
    creditCard.address1 = "123 Orange Ave"
    creditCard.address2 = "Unit 309"
    creditCard.addressCity = "Orlando"
    creditCard.addressState = "FL"
    creditCard.phone = "3210000000"
    creditCard.email = "fatt@merchant.com"
    creditCard.note = "This is a test credit card"

    return creditCard
  }
}
