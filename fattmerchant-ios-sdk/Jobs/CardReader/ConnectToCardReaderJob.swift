import Foundation

class ConnectToCardReaderJob: Job {
  typealias Output = CardReader?
  
  private let reader: CardReader
  
  public weak var cardReaderConnectionStatusDelegate: CardReaderConnectionStatusDelegate?
  
  public init(_ reader: CardReader) {
    self.reader = reader
  }
  
  public func start() async throws -> CardReader? {
    guard let card = Services.resolve(CardReaderService.self) else {
      throw StaxInitializeException.mobileReaderPaymentsNotConfigured
    }
    card.setCardReaderConnectionStatusDelegate(cardReaderConnectionStatusDelegate)
    return await card.connectToReader(reader)
  }
}
