extension CCParameters {

    /// The param value to make NMI add a customer to the customer vault
    /// This should be used with the CCParamCustomerVaultCommand and passed into startTransaction()
    var ParamValueAddCustomer: String {
        return "add-customer"
    }

    /// Convert `TransactionRequest` POI `String` to ChipDna `CCParameterValue` type
    func getPOIValueTypeString(from string: String) -> String {
        switch string.lowercased() {
        case "PAYMENT_DEVICE":
            return CCValuePaymentDevice
        case "TAP_TO_MOBILE":
            return CCValueTapToMobile
        default:
            return CCValuePaymentDevice
        }
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
        self[CCParamTransactionPOI] = getPOIValueTypeString(from: transactionRequest.transactionPOI)

        if transactionRequest.tokenize {
            self[CCParamCustomerVaultCommand] = ParamValueAddCustomer
        }
    }

}
