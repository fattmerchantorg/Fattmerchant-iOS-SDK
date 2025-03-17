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
}
