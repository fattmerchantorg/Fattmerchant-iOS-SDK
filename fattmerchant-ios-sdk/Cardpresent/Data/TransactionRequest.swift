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

  public init(amount: Amount) {
    self.amount = amount
  }
}
