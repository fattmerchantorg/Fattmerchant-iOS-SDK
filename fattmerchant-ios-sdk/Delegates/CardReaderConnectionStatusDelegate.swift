import Foundation

public protocol CardReaderConnectionStatusDelegate: class {

  /// Called when `CardReader` has a new `MobileReaderConnectionStatus`
  /// - Parameters:
  ///   - status: The new `CardReaderConnectionStatus` of the `reader`
  func cardReaderConnectionStatusUpdate(status: CardReaderConnectionStatus)
}
