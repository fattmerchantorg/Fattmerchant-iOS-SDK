//
//  TransactionRequest.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public struct TransactionRequest {
  public var amount: Amount

  /// The option to tokenize the payment method for later usage
  ///
  /// - Note: Defaults to true
  ///
  /// Set this to false if you do not want the payment method stored
  public var tokenize: Bool = true

  public init(amount: Amount) {
    self.amount = amount
  }

  public init(amount: Amount, tokenize: Bool) {
    self.amount = amount
    self.tokenize = tokenize
  }
}
