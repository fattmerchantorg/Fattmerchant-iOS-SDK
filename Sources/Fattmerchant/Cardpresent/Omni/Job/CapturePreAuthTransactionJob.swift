import Foundation

actor CapturePreAuthTransactionJob: Job {
  typealias ResultType = StaxTransaction
  
  private var transactionId: String
  private var amount: Amount?
  private var client: StaxHttpClientProtocol

  init(transactionId: String, client: StaxHttpClientProtocol, amount: Amount? = nil) {
    self.transactionId = transactionId
    self.amount = amount
    self.client = client
  }
  
  func start() async -> JobResult<StaxTransaction> {
    // As of 3/13/25, this code should only be hit by NMI since that's the only mobile gateway people use.
    // With only NMI in mind, we can hit the Stax API to handle this request
    let path = "/transaction/\(transactionId)/capture"
    var request = StaxApiRequest<StaxTransaction>(path: path, method: .post)
    if let amount = amount, let body = try? JSONEncoder().encode(["total": amount.dollars()]) {
      request.body = body
    }
    
    do {
      let response = try await client.perform(request)
      return JobResult.success(response)
    } catch {
      // TODO: Test bad auths
      return JobResult.failure(CapturePreauthTransactionException.errorCapturing(detail: "Unknown"))
    }
  }
}
