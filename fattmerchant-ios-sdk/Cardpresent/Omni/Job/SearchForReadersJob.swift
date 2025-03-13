actor SearchForReadersJob: Job {
  var args: MobileReaderDriverSearchArgs
  
  init(args: MobileReaderDriverSearchArgs) {
    self.args = args
  }
  
  func start() async -> JobResult<[MobileReader]> {
    let drivers = await MobileReaderDriverRepository.shared.getInitializedDrivers()
    guard !drivers.isEmpty, let driver = drivers.first else {
      return JobResult.failure(SearchForReadersException.noMobileReaderAvailable)
    }
    
    let readers = await withCheckedContinuation { continuation in
      driver.searchForReaders(args: args) { readers in
        continuation.resume(returning: readers)
      }
    }

    return JobResult.success(readers)
  }

}
