actor CancelCurrentTransactionJob: Job {
  typealias ResultType = Bool

  func start() async -> JobResult<Bool> {
    guard var driver = MobileReaderDriverRepository.shared.getDrivers().first else {
      return JobResult.failure(CancelCurrentTransactionException.unknown)
    }

    do {
      let result: Bool = try await withCheckedThrowingContinuation { continuation in
        driver.cancelCurrentTransaction { result in
          continuation.resume(returning: result)
        } error: { exception in
          continuation.resume(throwing: exception)
        }
      }
      return JobResult.success(result)
    } catch {
      return JobResult.failure(error as! OmniException)
    }
  }
}
