extension Array {
  /// Allows a predicate that contains an `AsyncStream<Bool>` to return a callback with the result.
  /// Useful for legacy Stax callback code; allowing us to not deal with `DispatchQueue` directly.
  /// - Parameters:
  ///   - predicate: A function that contains an `AsyncStream<Bool>` that is evaluated to filter down.
  ///   - completion: A callback function that contains the filtered array.
  func filterAsync(
    predicate: @escaping (Element) -> AsyncStream<Bool>,
    completion: @escaping ([Element]) -> Void
  ) {
    // Start a `Task` to allow this chunk to be asynchronous
    Task {
      var filtered: [Element] = []
      for item in self {
        for await value in predicate(item) {
          if value {
            filtered.append(item)
          }
          break
        }
      }
      completion(filtered)
    }
  }
}
