import Foundation

/*
actor VoidTransactionJob: Job {
  typealias ResultType = StaxTransaction
  
  private let transactionId: String
  
  init(transactionId: String) {
    self.transactionId = transactionId
  }
  
  func start() async -> JobResult<StaxTransaction> {
    
  }

  typealias Exception = RefundException

  var omniApi: OmniApi
  var transactionId: String

  init(transactionId: String, captureAmount: Amount? = nil, omniApi: OmniApi) {
    self.transactionId = transactionId
    self.omniApi = omniApi
  }

  func start(completion: @escaping (Transaction) -> Void, error: @escaping (OmniException) -> Void) {
    self.omniApi.request(method: "POST",
                         urlString: "/transaction/\(transactionId)/void",
                         completion: completion,
                         failure: error)
  }
 }
*/

