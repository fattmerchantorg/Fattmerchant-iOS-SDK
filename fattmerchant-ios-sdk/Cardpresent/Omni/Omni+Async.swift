extension Omni {
  /// Setup and prepare the `Omni` instance.
  /// - Parameter args: The `InitializationArgs` required for bootstrapping the SDK.
  /// - Throws: `OmniException` If there was a problem initializing the SDK.
  public func initialize(args: InitializationArgs) async throws -> Void {
    return try await withCheckedThrowingContinuation { continuation in
      initialize(args: args) {
        continuation.resume(returning: ())
      } error: { exception in
        continuation.resume(throwing: exception)
      }
    }
  }
  
  /// Creates a `StaxPaymentMethod` out of a `StaxBankAccount` object for reuse with Stax Pay.
  /// - Parameter bank: Contains the `StaxBankAccount` details to tokenize with Stax Pay.
  /// - Throws: `OmniException` If there was a problem tokenizing the `StaxBankAccount`.
  /// - Returns: A tokenized `StaxPaymentMethod` object with the `StaxBankAccount` details.
  public func tokenizeBankAccount(_ bank: StaxBankAccount) async throws -> StaxPaymentMethod {
    guard let client = staxHttpClient else { throw OmniGeneralException.uninitialized; }
    
    let job = TokenizeBankAccountJob(bank: bank, client: client)
    let result = await job.start()
    switch result {
      case .success(let tokenized): return tokenized
      case .failure(let fail): throw fail
    }
  }
  
  /// Creates a `StaxPaymentMethod` out of a `StaxCreditCard` object for reuse with Stax Pay.
  /// - Parameter card: Contains the `StaxCreditCard` object to tokenize
  /// - Throws: `OmniException` If there was a problem tokenizing the `StaxCreditCard`.
  /// - Returns: A tokenized `StaxPaymentMethod` object with the `StaxCreditCard` details.
  public func tokenizeCreditCard(_ card: StaxCreditCard) async throws -> StaxPaymentMethod {
    guard let client = staxHttpClient else { throw OmniGeneralException.uninitialized; }
    
    let job = TokenizeCreditCardJob(card: card, client: client)
    let result = await job.start()
    switch result {
      case .success(let tokenized): return tokenized
      case .failure(let fail): throw fail
    }
  }
  
  /// Captures a previously-authorized `StaxTransaction`.
  /// - Parameter transactionId: The ID of the `StaxTransaction` you want to capture.
  /// - Parameter amount: The `Amount` that you want to capture. If `nil`, then the total pre-authorized amount will be captured.
  /// - Throws: `OmniException` If there was a problem capturing the `StaxTransaction`.
  /// - Returns: A `StaxTransaction`  with the result of the transaction.
  public func capturePreAuthTransaction(transactionId: String, amount: Amount? = nil) async throws -> StaxTransaction {
    guard let client = staxHttpClient else { throw OmniGeneralException.uninitialized; }
    
    let job = CapturePreAuthTransactionJob(transactionId: transactionId, client: client, amount: amount)
    let result = await job.start()
    switch result {
      case .success(let transaction): return transaction
      case .failure(let fail): throw fail
    }
  }
  
  /// Voids a `StaxTransaction` via the Stax Pay API.
  /// - Parameter id: The id of the `StaxTransaction` you want to void.
  /// - Throws: `OmniException` If there was a problem capturing the `StaxTransaction`.
  /// - Returns: A `StaxTransaction`  with the result of the transaction.
  public func voidTransaction(id: String) async throws -> StaxTransaction {
    guard let client = staxHttpClient else { throw OmniGeneralException.uninitialized; }
    
    let job = VoidTransactionJob(id: id, client: client)
    let result = await job.start()
    switch result {
      case .success(let transaction): return transaction
      case .failure(let fail): throw fail
    }
  }
  
  /// Attempts to cancel the current transaction taken using a `MobileReader`.
  /// - Throws: `OmniException` If there was a problem capturing the `StaxTransaction`.
  /// - Returns: A `Bool`  with the result of the transaction cancelation.
  public func cancelMobileReaderTransaction() async throws -> Bool {
    guard isInitialized else { throw OmniGeneralException.uninitialized; }
    
    let job = CancelCurrentTransactionJob()
    let result = await job.start()
    switch result {
      case .success(let result): return result
      case .failure(let fail): throw fail
    }
  }
  
  /// Finds all available `MobileReader` devices that can be connected to. Returns `[MobileReader]`.
  /// - Throws: `OmniException` If there was a problem searching for the devices.
  /// - Returns: A `[MobileReader]` array with the available `MobileReader` devices.
  public func getAvailableReaders() async throws -> [MobileReader] {
    guard isInitialized else { throw OmniGeneralException.uninitialized; }
    
    #if targetEnvironment(simulator)
    let args = MockSearchArgs()
    #else
    let args = ChipDnaSearchArgs(allowed: [.ble, .bt, .usb])
    #endif
    
    let job = SearchForMobileReadersJob(args: args)
    let result = await job.start()
    switch result {
      case .success(let readers): return readers
      case .failure(let fail): throw fail
    }
  }
  
  /// Returns the connected `MobileReader`.
  /// - Throws: `OmniException` If there was a problem getting the active device.
  /// - Returns: A `MobileReader?` if connected and `nil` if not.
  public func getConnectedReader() async throws -> MobileReader? {
    guard isInitialized else { throw OmniGeneralException.uninitialized; }
    
    let job = GetConnectedMobileReaderJob()
    let result = await job.start()
    switch result {
      case .success(let reader): return reader
      case .failure(let fail): throw fail
    }
  }
  
  /// Attempts to connect to the provided `MobileReader`.
  /// - Parameter reader: The `MobileReader` to connect to.
  /// - Throws: `OmniException` If there was a problem connecting to the device.
  /// - Returns: The newly connected `MobileReader`.
  public func connectTo(reader: MobileReader) async throws -> MobileReader {
    guard isInitialized else { throw OmniGeneralException.uninitialized; }
    
    let job = ConnectToMobileReaderJob(reader: reader, connectionStatusDelegate: mobileReaderConnectionUpdateDelegate)
    let result = await job.start()
    switch result {
      case .success(let reader): return reader
      case .failure(let fail): throw fail
    }
  }
  
  /// Attempts to disconnect from the provided `MobileReader`
  /// - Parameter reader: The `MobileReader` to disconnect from.
  /// - Throws: `OmniException` If there was a problem disconnecting to the device.
  /// - Returns: `true` if successfully disconnected.
  public func disconnectFrom(reader: MobileReader) async throws -> Bool {
    guard isInitialized else { throw OmniGeneralException.uninitialized; }
    
    let job = DisconnectMobileReaderJob()
    let result = await job.start()
    switch result {
      case .success(let result): return result
      case .failure(let fail): throw fail
    }
  }
}
