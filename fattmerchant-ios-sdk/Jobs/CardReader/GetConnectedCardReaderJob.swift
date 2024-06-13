import Foundation

class GetConnectedCardReaderJob: Job {
  typealias Output = CardReader?
  
  public init() {
    
  }
  
  public func start() async throws -> CardReader? {
    guard let card = Services.resolve(CardReaderService.self) else {
      throw StaxInitializeException.mobileReaderPaymentsNotConfigured
    }

    return try await card.getConnectedReader()
  }
}
