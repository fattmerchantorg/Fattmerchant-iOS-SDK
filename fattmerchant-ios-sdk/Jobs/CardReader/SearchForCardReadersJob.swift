import Foundation

class SearchForCardReadersJob: Job {
  typealias Output = [CardReader]
  
  public func start() async throws -> [CardReader] {
    guard let card = Services.resolve(CardReaderService.self) else {
      throw StaxInitializeException.mobileReaderPaymentsNotConfigured
    }
    
    return await card.searchForReaders([:])
  }
}
