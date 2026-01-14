/// Implementation ofPaymentMethodRepository that communicates with the Stax API.
final class StaxPaymentMethodRepositoryImpl: StaxPaymentMethodRepository {
  private let httpClient: StaxHttpClientProtocol
    
  init(httpClient: StaxHttpClientProtocol) {
    self.httpClient = httpClient
  }
  
  func getPaymentMethod(id: String) async throws -> StaxPaymentMethod {
    let request = StaxApiRequest<StaxPaymentMethod>(
      path: "/payment-method/\(id)",
      method: .get
    )
    return try await httpClient.perform(request)
  }
  
  func getPaymentMethodSurcharge(id: String, dollars: Double) async throws -> StaxSurchargeReview {
    let request = StaxApiRequest<StaxSurchargeReview>(
      path: "surcharge/review?payment_method_id=\(id)&total=\(dollars)",
      method: .get
    )
    return try await httpClient.perform(request)
  }
  
  func updatePaymentMethod(id: String, update: StaxPaymentMethod.Update) async throws -> StaxPaymentMethod {
    let request = StaxApiRequest<StaxPaymentMethod>(
      path: "/payment-method/\(id)",
      method: .put,
      body: update.apply()
    )
    return try await httpClient.perform(request)
  }
  
  func deletePaymentMethod(id: String) async throws {
    let request = StaxApiRequest<StaxPaymentMethod>(
      path: "/payment-method/\(id)",
      method: .delete
    )
    let _ = try await httpClient.perform(request)
  }
}
