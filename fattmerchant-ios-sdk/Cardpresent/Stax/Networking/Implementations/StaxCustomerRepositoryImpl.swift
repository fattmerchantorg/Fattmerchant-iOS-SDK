/// Implementation of CustomerRepository that communicates with the Stax API.
final class StaxCustomerRepositoryImpl: StaxCustomerRepository {
  private let httpClient: StaxHttpClientProtocol
    
  init(httpClient: StaxHttpClientProtocol) {
    self.httpClient = httpClient
  }
    
  func getCustomer(id: String) async throws -> StaxCustomer {
    let request = StaxApiRequest<StaxCustomer>(
      path: "/customer/\(id)",
      method: .get,
    )
    return try await httpClient.perform(request)
  }
    
  func getPaymentMethodsForCustomer(id: String) async throws -> [StaxPaymentMethod] {
    let request = StaxApiRequest<[StaxPaymentMethod]>(
      path: "/customer/\(id)/payment-method",
      method: .get
    )
    return try await httpClient.perform(request)
  }
  
  func createCustomer(_ request: StaxCustomer) async throws -> StaxCustomer {
    let request = StaxApiRequest<StaxCustomer>(
      path: "/customer",
      method: .post,
      body: request
    )
    return try await httpClient.perform(request)
  }
    
  func updateCustomer(id: String, update: StaxCustomer.Update) async throws -> StaxCustomer {
    let request = StaxApiRequest<StaxCustomer>(
      path: "/customer/\(id)",
      method: .put,
      body: update.apply()
    )
    return try await httpClient.perform(request)
  }
    
  func deleteCustomer(id: String) async throws {
    let request = StaxApiRequest<StaxCustomer>(
      path: "/customer/\(id)",
      method: .delete
    )
    let _ = try await httpClient.perform(request)
  }
}
