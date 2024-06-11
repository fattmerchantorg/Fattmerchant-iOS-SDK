import Foundation

protocol CardReaderService {
  static var isRefundSupported: Bool { get }
  
  func initialize(_ args: [String: Any]) throws -> Void
  func isInitialized() -> Bool
  func searchForReaders(_ args: [String: Any]) async -> [CardReader]
  func connectToReader(_ reader: CardReader) async -> CardReader?
  func getConnectedReader() async throws -> CardReader?
  func disconnectFromReader() async throws -> Bool
  func performTransaction(_ request: TransactionRequest) async -> TransactionResult
  func captureTransaction(_ transaction: Transaction) async -> Bool
  func cancelTransaction()
  func refundTransaction()
  func voidTransaction(_ result: TransactionResult) async -> Bool
 
  func getCardReaderConnectionStatusDelegate() -> CardReaderConnectionStatusDelegate?
  func setCardReaderConnectionStatusDelegate(_ delegate: CardReaderConnectionStatusDelegate?) -> Void
  
  func getUserNotificationDelegate() -> UserNotificationDelegate?
  func setUserNotificationDelegate(_ delegate: UserNotificationDelegate?) -> Void
  
  func getTransactionUpdateDelegate() -> TransactionUpdateDelegate?
  func setTransactionUpdateDelegate(_ delegate: TransactionUpdateDelegate?) -> Void
  
  func getSignatureProvider() -> SignatureProviding?
  func setSignatureProvider(_ provider: SignatureProviding?) -> Void
}

