import Foundation

actor DisconnectMobileReaderJob: Job {
  typealias ResultType = Bool
  
  func start() async -> JobResult<Bool> {
    let drivers = await MobileReaderDriverRepository.shared.getInitializedDrivers()
    guard let driver = drivers.first, drivers.count == 1 else {
      return JobResult.failure(DisconnectMobileReaderException.driverNotFound)
    }
    
    do {
      let result = try await withCheckedThrowingContinuation { continuation in
        driver.disconnect { result in
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
