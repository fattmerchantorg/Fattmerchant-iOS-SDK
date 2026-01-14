import Foundation

actor VoidTransactionJob: Job {
  typealias ResultType = StaxTransaction
  
  private let id: String
  private let client: StaxHttpClientProtocol
  
  init(id: String, client: StaxHttpClientProtocol) {
    self.id = id
    self.client = client
  }
  
  func start() async -> JobResult<StaxTransaction> {
    do {
      let path = "/transaction/\(id)/void"
      let request = StaxApiRequest<StaxTransaction>(path: path, method: .post)
      let response = try await client.perform(request)
      return JobResult.success(response)
    } catch {
      return JobResult.failure(error as! OmniException)
    }
  }
}

