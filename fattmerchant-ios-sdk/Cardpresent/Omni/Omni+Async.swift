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
  /// - Parameter completion: A `(StaxTransaction) -> Void` callback run after transaction has finished.
  /// - Parameter error: A `(OmniException) -> Void` error handler run if the SDK runs in to an error when capturing
  public func capturePreAuthTransaction(transactionId: String, amount: Amount? = nil) async throws -> StaxTransaction {
    guard let client = staxHttpClient else { throw OmniGeneralException.uninitialized; }
    
    let job = CapturePreAuthTransactionJob(transactionId: transactionId, client: client, amount: amount)
    let result = await job.start()
    switch result {
      case .success(let transaction): return transaction
      case .failure(let fail): throw fail
    }
  }
}
