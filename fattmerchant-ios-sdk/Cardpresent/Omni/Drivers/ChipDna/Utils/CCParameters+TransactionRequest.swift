#if targetEnvironment(simulator)
#else

extension CCParameters {

  /// The param value to make NMI add a customer to the customer vault
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
    self[CCParamTransactionType] = CCValueSale

    if transactionRequest.tokenize {
      self[CCParamCustomerVaultCommand] = ParamValueAddCustomer
    }
  }

}

#endif
