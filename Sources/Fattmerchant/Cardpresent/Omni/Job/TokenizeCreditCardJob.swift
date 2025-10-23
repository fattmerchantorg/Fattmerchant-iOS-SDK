import Foundation

actor TokenizeCreditCardJob: Job {
  typealias ResultType = StaxPaymentMethod
  
  private let card: StaxCreditCard
  private let client: StaxHttpClientProtocol
  
  init(card: StaxCreditCard, client: StaxHttpClientProtocol) {
    self.card = card
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
    if let id = card.customerId {
      let request = StaxApiRequest<StaxCustomer>(path: "/customer/\(id)", method: .get)
      let customer = try await client.perform(request)
      return customer
    }
    
    let customer = StaxCustomer.from(name: card.cardholder)
    let request = StaxApiRequest<StaxCustomer>(path: "/customer", method: .post, body: customer)
    return try await client.perform(request)
  }
  
  fileprivate func createPaymentMethod(customer: StaxCustomer) async throws -> StaxPaymentMethod {
    let account = StaxPaymentMethod(
      customerId: customer.id,
      method: .card,
      personName: card.cardholder,
      cardNumber: card.number,
      cardExpiry: card.expiry,
      address1: card.address1,
      address2: card.address2,
      addressCity: card.addressCity,
      addressState: card.addressState,
      addressZip: card.addressZip
    )
    
    let request = StaxApiRequest<StaxPaymentMethod>(path: "/payment-method", method: .post, body: account)
    return try await client.perform(request)
  }
}
