import Foundation

actor CapturePreauthTransactionJob: Job {
  typealias ResultType = StaxTransaction
  
  private var transactionId: String
  private var amount: Amount?
  private var token: String

  init(transactionId: String, token: String, amount: Amount? = nil) {
    self.transactionId = transactionId
    self.amount = amount
    self.token = token
  }
  
  func start() async -> JobResult<StaxTransaction> {
    // As of 3/13/25, this code should only be hit by NMI since that's the only mobile gateway people use.
    // With only NMI in mind, we can hit the Stax API to handle this request
    let baseUrl = URL(string: "https://apiprod.fattlabs.com")!
    let client = StaxHttpClient(baseURL: baseUrl)
    var request = StaxApiRequest<StaxTransaction>(
      path: "/transaction/\(transactionId)/capture",
      method: .post,
      headers: [
        "Accept": "application/json",
        "Content-Type": "application/json",
        "Authorization": "Bearer \(token)"
      ]
    )
    
    // Set request body if Amount is passed in
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
