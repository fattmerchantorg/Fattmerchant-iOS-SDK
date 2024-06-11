enum CreatePaymentMethodException: StaxException {
  case Something(String)

  static var mess = "Could not create payment method"

  var detail: String? {
    switch self {
    case .Something(let desc):
      return desc
    }
  }

}
