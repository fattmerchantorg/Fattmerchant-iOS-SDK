enum TokenizePaymentMethodException: OmniException {
  static var mess: String = "Could not tokenize payment method"

  case tokenizationError
  case couldNotParsePaymentMethodError
  case merchantMissingHostedPaymentsToken
  case noCreditCardOrBankAccountSupplied
}
