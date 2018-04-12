//
//  PaymentMethod.swift
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

/// A payment method such as a credit card or a bank account
public struct PaymentMethod: Codable {
  public var id: String
  public var method: PaymentMethodType
  public var nickname: String
  public var hasCvv: Bool
  public var personName: String
  public var cardType: String?
  public var cardLastFour: String?
  public var cardExp: String?
  public var bankName: String?
  public var bankType: String?
  public var bankHolderType: BankHolderType?

  enum CodingKeys: String, CodingKey {
    case id, method, nickname,
    hasCvv = "has_cvv",
    personName = "person_name",
    cardType = "card_type",
    cardLastFour = "card_last_four",
    cardExp = "card_exp",
    bankName = "bank_name",
    bankType = "bank_type",
    bankHolderType = "bank_holder_type"
  }
}
