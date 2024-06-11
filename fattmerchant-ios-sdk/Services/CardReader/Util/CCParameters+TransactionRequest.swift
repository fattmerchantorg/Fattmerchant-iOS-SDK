import Foundation

extension CCParameters {
  static func fromTransactionRequest(_ request: TransactionRequest) -> CCParameters {
    var output = CCParameters()
    output[CCParamAmount] = request.amount.centsString()
    output[CCParamAmountType] = CCValueAmountTypeActual
    output[CCParamCurrency] = "USD"
    output[CCParamUserReference] = generateChipDnaTransactionUserReference()
    output[CCParamPaymentMethod] = CCValueCard
    output[CCParamTransactionType] = CCValueSale

    if request.tokenize {
      output[CCParamCustomerVaultCommand] = "add-customer"
    }
    return output
  }
}
