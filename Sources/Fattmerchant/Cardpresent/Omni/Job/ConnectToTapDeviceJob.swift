@available(iOS 17.4, *)
actor ConnectToTapJob: Job {

    private weak var connectionStatusDelegate: TapConnectionStatusDelegate?

    init(connectionStatusDelegate: TapConnectionStatusDelegate? = nil) {
        self.connectionStatusDelegate = connectionStatusDelegate
    }

    func start() async -> JobResult<Bool> {
        var driver = TapPaymentDriverRepository.shared.all().first

        DispatchQueue.main.async {
            driver.tapConnectionUpdateDelegate = self.tapConnectionUpdateDelegate
        }
        
        var connectTapException = ConnectTapException.couldNotConnectToTap(detail: "")
        let connected: Bool = await withCheckedContinuation {
            continuation in
            driver?.connectToTap { success, error in
                if (error != nil) {
                    connectTapException = error as! ConnectTapException
                }
                continuation.resume(returning: success)
            }
        }

        if connected{
            return JobResult.success(true)
        }

        return JobResult.failure(
            connectTapException
        )
    }
}
