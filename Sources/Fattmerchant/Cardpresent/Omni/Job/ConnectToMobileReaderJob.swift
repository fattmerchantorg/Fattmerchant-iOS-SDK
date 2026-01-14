actor ConnectToMobileReaderJob: Job {
  typealias ResultType = MobileReader
  
  private weak var connectionStatusDelegate: MobileReaderConnectionStatusDelegate?
  private var reader: MobileReader

  init(reader: MobileReader, connectionStatusDelegate: MobileReaderConnectionStatusDelegate? = nil) {
    self.reader = reader
    self.connectionStatusDelegate = connectionStatusDelegate
  }

  func start() async -> JobResult<MobileReader> {
    guard var driver = MobileReaderDriverRepository.shared.getDrivers().first else {
      return JobResult.failure(ConnectMobileReaderException.couldNotConnectMobileReader(reader: reader))
    }

    driver.mobileReaderConnectionStatusDelegate = connectionStatusDelegate
    let connected: MobileReader? = await withCheckedContinuation { continuation in
      driver.connect(reader: reader) { connected in
        if let connected = connected {
          continuation.resume(returning: connected)
        } else {
          continuation.resume(returning: nil)
        }
      }
    }
    
    if let connected = connected {
      return JobResult.success(connected)
    }
    
    return JobResult.failure(ConnectMobileReaderException.couldNotConnectMobileReader(reader: reader))
  }
}
