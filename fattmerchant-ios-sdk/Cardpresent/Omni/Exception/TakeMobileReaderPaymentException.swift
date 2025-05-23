enum TakeMobileReaderPaymentException: OmniException {
  case mobileReaderNotFound
  case mobileReaderNotReady
  case invoiceNotFound
  case invoiceIdCannotBeBlank
  case couldNotCreateInvoice(detail: String?)
  case couldNotCreateCustomer(detail: String?)
  case couldNotCreatePaymentMethod(detail: String?)
  case couldNotUpdateInvoice(detail: String?)
  case couldNotCreateTransaction(detail: String?)
  case couldNotCaptureTransaction

  static var mess: String = "Error taking mobile reader payment"

  var detail: String? {
    switch self {
      case .mobileReaderNotFound: return "Mobile reader not found"
      case .mobileReaderNotReady: return "Mobile reader not ready to take payment"
      case .invoiceNotFound: return "Invoice with given id not found"
      case .couldNotCaptureTransaction: return "Could not capture transaction"
      case .couldNotCreateInvoice(let d): return d ?? "Could not create invoice"
      case .couldNotCreateCustomer(let d): return d ?? "Could not create customer"
      case .couldNotCreatePaymentMethod(let d): return d ?? "Could not create payment method"
      case .couldNotUpdateInvoice(let d): return d ?? "Could not update invoice"
      case .couldNotCreateTransaction(let d): return d ?? "Could not create transaction"
      case .invoiceIdCannotBeBlank: return "Could not create invoice"
    }
  }
}
