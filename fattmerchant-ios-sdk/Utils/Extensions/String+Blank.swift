extension String {

  /// Checks to see if the receiver contains non-whitespace characters
  /// - Returns: true if the string is only made of whitespace
  func isBlank() -> Bool {
    let trimmed = trimmingCharacters(in: CharacterSet.whitespacesAndNewlines)
    return trimmed.isEmpty
  }
}
