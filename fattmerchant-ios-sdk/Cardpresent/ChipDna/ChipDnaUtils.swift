//
//  ChipDnaUtils.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/17/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

#if targetEnvironment(simulator)

#else

extension MobileReader {
  static func from(pinPad: ChipDnaDriver.SelectablePinPad) -> MobileReader {
    return MobileReader(name: pinPad.name)
  }
}

extension CCParameters {

  /// The param value to make NMI add a customer to the customer vault
  ///
  /// This should be used with the CCParamCustomerVaultCommand and passed into startTransaction()
  var ParamValueAddCustomer: String {
    return "add-customer"
  }

  subscript(key: String) -> String? {
    get {
      guard
        let allKeys = allKeys() as? [String],
        allKeys.contains(key)
        else { return nil }

      return value(forKey: key)
    }

    set {
      setValue(newValue, forKey: key)
    }

  }

  convenience init(transactionRequest: TransactionRequest) {
    self.init()
    self[CCParamAmount] = transactionRequest.amount.centsString()
    self[CCParamAmountType] = CCValueAmountTypeActual
    self[CCParamCurrency] = "USD"
    self[CCParamUserReference] = generateChipDnaTransactionUserReference()
    self[CCParamPaymentMethod] = CCValueCard
    self[CCParamAutoConfirm] = CCValueTrue
    self[CCParamTransactionType] = CCValueSale

    if transactionRequest.tokenize {
      self[CCParamCustomerVaultCommand] = ParamValueAddCustomer
    }
  }

}

#endif
