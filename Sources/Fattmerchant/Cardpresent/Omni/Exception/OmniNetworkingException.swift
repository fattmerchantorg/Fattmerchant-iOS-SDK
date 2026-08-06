enum OmniNetworkingException: OmniException {
  case couldNotGetMerchantDetails
  case couldNotGetPaginatedTransactions
  
  static var mess: String = "Omni Networking Exception"
  
  var detail: String? {
    switch self {
      case .couldNotGetMerchantDetails: return "Could not get merchant details from Omni"
      case .couldNotGetPaginatedTransactions: return "Could not get paginated transactions"
    }
  }
}
