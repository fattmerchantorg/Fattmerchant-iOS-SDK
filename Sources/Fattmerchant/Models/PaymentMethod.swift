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
public class PaymentMethod: Model {
  public var id: String?
  public var method: PaymentMethodType?
  public var merchantId: String?
  public var customerId: String

  /// Whether or not Omni should tokenize this PaymentMethod
  ///
  /// - Note:
  /// If this field is true, `paymentToken` must be `null`
  var tokenize: Bool?

  /// The token that represents this payment method
  ///
  /// The only use-case for this field is storing the token within Omni. After cardpresent tokenization,
  /// we can create a PaymentMethod using this class. If we include the paymentToken, then we can later
  /// store it as an already-tokenized PaymentMethod
  ///
  /// - Important:
  ///   Omni performs transactions with this token. Therefore, it is crucial that only the actual payment
  ///   token be placed here
  ///
  /// - Note: If this field is not `null`, then `tokenize` must be `false`
  internal var paymentToken: String?
  internal var cardNumber: String?
  internal var cardCvv: String?

  /// The bank account number
  internal var bankAccount: String?

  /// The bank routing number
  internal var bankRouting: String?
  public var nickname: String?
  public var personName: String?
  public var cardType: String?
  public var cardLastFour: String?
  public var cardExp: String?
  public var bankName: String?
  public var bankType: String?
  public var bankHolderType: BankHolderType?

  init(customer: Customer) {
    self.customerId = customer.id ?? ""
  }

  init(card: CreditCard, customer: Customer) {
    self.customerId = customer.id ?? ""
    personName = customer.fullName()
    method = .card
    cardNumber = card.cardNumber
    cardExp = card.cardExp
  }

  init(bank: BankAccount, customer: Customer) {
    self.customerId = customer.id ?? ""
    personName = customer.fullName()
    method = .bank
    bankAccount = bank.bankAccount
    bankRouting = bank.bankRouting
    bankName = bank.bankName
    bankType = bank.bankType
  }

}
