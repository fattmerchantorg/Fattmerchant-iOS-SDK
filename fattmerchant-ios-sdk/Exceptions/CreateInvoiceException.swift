enum CreateInvoiceException: StaxException {
  case Something(String)

  static var mess = "Could not create customer"

  var detail: String? {
    switch self {
    case .Something(let desc):
      return desc
    }
  }

}
