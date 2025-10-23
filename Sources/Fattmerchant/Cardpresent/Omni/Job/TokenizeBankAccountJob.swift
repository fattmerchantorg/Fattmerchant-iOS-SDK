import Foundation

actor TokenizeBankAccountJob: Job {
  typealias ResultType = StaxPaymentMethod
  
  private let bank: StaxBankAccount
  private let client: StaxHttpClientProtocol
  
  init(bank: StaxBankAccount, client: StaxHttpClientProtocol) {
    self.bank = bank
    self.client = client
  }
  
  func start() async -> JobResult<StaxPaymentMethod> {
    do {
      let customer = try await getOrCreateCustomer()
      let output = try await createPaymentMethod(customer: customer)
      return JobResult.success(output)
    } catch {
      return JobResult.failure(error as! OmniException)
    }
  }
  
  fileprivate func getOrCreateCustomer() async throws -> StaxCustomer {
    // Look up customer if ID is passed in
    if let id = bank.customerId {
      let request = StaxApiRequest<StaxCustomer>(path: "/customer/\(id)", method: .get)
      let customer = try await client.perform(request)
      return customer
    }
    
    let customer = StaxCustomer.from(name: bank.accountHolderName)
    let request = StaxApiRequest<StaxCustomer>(path: "/customer", method: .post, body: customer)
    return try await client.perform(request)
  }
  
  fileprivate func createPaymentMethod(customer: StaxCustomer) async throws -> StaxPaymentMethod {
    let account = StaxPaymentMethod(
      customerId: customer.id,
      method: .bank,
      personName: customer.name,
      bankName: bank.bankName,
      bankAccount: bank.accountNumber,
      bankRouting: bank.routingNumber,
      bankType: bank.bankType,
      address1: bank.address1,
      address2: bank.address2,
      addressCity: bank.addressCity,
      addressState: bank.addressState,
      addressZip: bank.addressZip
    )
    
    let request = StaxApiRequest<StaxPaymentMethod>(path: "/payment-method", method: .post, body: account)
    return try await client.perform(request)
  }
}
