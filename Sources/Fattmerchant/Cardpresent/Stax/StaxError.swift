public enum StaxError: Error {
  case initialization(InitializationError)
  case reader(ReaderError)
  case transaction(TransactionError)
  case api(APIError)
    
  public enum InitializationError {
    case missingCredentials
    case invalidCredentials
    case mobileReaderNotConfigured
  }
    
  public enum ReaderError {
    case notInitialized
    case connectionFailed
    case disconnectionFailed
    case noReadersFound
  }
    
  public enum TransactionError {
    case cancelled
    case refundFailed
    case invalidAmount
    case processingError(String)
  }
    
  public enum APIError {
    case network(Error)
    case invalidResponse
    case merchantNotFound
  }
}
