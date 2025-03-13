actor InitializeDriversJob: Job {
  typealias ResultType = Void
  
  var args: MobileReaderDriverInitializationArgs
  
  init(args: MobileReaderDriverInitializationArgs) {
    self.args = args
  }
  
  func start() async -> JobResult<Void> {
    let drivers = MobileReaderDriverRepository.shared.getDrivers()
    guard !drivers.isEmpty else {
      return JobResult.failure(InitializeDriversException.noMobileReadersFound)
    }
    
    // Right now we only have 1 driver. For simplicity, initialize the 1st driver.
    guard let driver = drivers.first, drivers.count == 1 else {
      return JobResult.failure(InitializeDriversException.incorrectMobileReaderSettings)
    }
    
    let isSuccessful = await withCheckedContinuation { continuation in
      driver.initialize(args: args) { isSuccessful in
        continuation.resume(returning: isSuccessful)
      }
    }

    if isSuccessful {
      return JobResult.success(())
    }
    
    return JobResult.failure(InitializeDriversException.incorrectMobileReaderSettings)
  }

}
