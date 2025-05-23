import Foundation

actor GetConnectedMobileReaderJob: Job {
  typealias ResultType = MobileReader?
  
  func start() async -> JobResult<MobileReader?> {
    let drivers = await MobileReaderDriverRepository.shared.getInitializedDrivers()
    guard let driver = drivers.first, drivers.count == 1 else {
      return JobResult.failure(GetConnectedMobileReaderException.noReaderAvailable)
    }
    
    do {
      let result = try await withCheckedThrowingContinuation { continuation in
        driver.getConnectedReader { reader in
          continuation.resume(returning: reader)
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
