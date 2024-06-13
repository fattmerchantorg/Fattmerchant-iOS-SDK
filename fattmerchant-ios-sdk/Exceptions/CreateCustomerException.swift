enum CreateCustomerException: StaxException {
  case customerNameNotSupplied
  case unknown

  static var mess = "Could not create customer"

  var detail: String? {
    switch self {
    case .customerNameNotSupplied:
      return "Customer name is required"

    default:
      return "Unknown error creating customer"
    }
  }

}
