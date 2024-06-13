import Foundation

extension String {
  /// Checks to see if the receiver contains non-whitespace characters
  /// - Returns: true if the string is only made of whitespace
  public func isBlank() -> Bool {
    return trimmingCharacters(in: CharacterSet.whitespacesAndNewlines).isEmpty
  }
}
