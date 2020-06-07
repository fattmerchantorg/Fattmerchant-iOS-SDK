//
//  TransactionUpdateDelegate.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 5/28/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Gets notified of all messages that need to be displayed to a user
///
/// For example, "Please insert/swipe your card"
public protocol TransactionUpdateDelegate: class {
  func onTransactionUpdate(transactionUpdate: TransactionUpdate)
}
