enum RefundException: StaxException {
  case transactionNotRefundable(details: String?)
  case missingTransactionId
  case couldNotFindMobileReaderForRefund
  case errorRefunding(details: String?)

  static var mess: String = "Could not refund transaction"

  var detail: String? {
    switch self {
    case .transactionNotRefundable(let d):
      return d ?? "Transaction is not refundable"

    case .couldNotFindMobileReaderForRefund:
      return "Mobile reader driver responsible for performing refund could not be found"

    case .errorRefunding(let d):
      return d ?? "Error while performing refund"

    case .missingTransactionId:
      return "Transaction id not provided"
    }
  }
}
