enum StaxNetworkingException: StaxException {
  case serviceNotInitialized
  case baseUrlNotFound
  case couldNotGetMerchantDetails
  case couldNotGetPaginatedTransactions
  case couldNotParseResponse(String?)
  case unknown(String?)

  static var mess: String = "Stax Networking Exception"

  var detail: String? {
    switch self {
    case .baseUrlNotFound:
      return "Base Url Not Found"
    case .couldNotParseResponse(let cause):
      return cause
    case .unknown(let cause):
      return cause
    case .couldNotGetMerchantDetails:
      return "Could not get merchant details from Omni"
    case .couldNotGetPaginatedTransactions:
      return "Could not get paginated transactions"
    case .serviceNotInitialized:
      return "NetworkService not initialized"
    }
  }
}
