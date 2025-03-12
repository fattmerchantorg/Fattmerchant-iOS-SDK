/// Implementation of InvoiceRepository that communicates with the Stax API.
final class StaxInvoiceRepositoryImpl: StaxInvoiceRepository {
  private let httpClient: StaxHttpClientProtocol
    
  init(httpClient: StaxHttpClientProtocol) {
    self.httpClient = httpClient
  }
    
  func getInvoice(id: String) async throws -> StaxInvoice {
    let request = StaxApiRequest<StaxInvoice>(
      path: "/invoice/\(id)",
      method: .get
    )
    return try await httpClient.perform(request)
  }
  
  func createInvoice(_ request: StaxInvoice) async throws -> StaxInvoice {
    let request = StaxApiRequest<StaxInvoice>(
      path: "/invoice",
      method: .post,
      body: request
    )
    return try await httpClient.perform(request)
  }
    
  func updateInvoice(id: String, update: StaxInvoice.Update) async throws -> StaxInvoice {
    let request = StaxApiRequest<StaxInvoice>(
      path: "/invoice/\(id)",
      method: .put,
      body: update.apply()
    )
    return try await httpClient.perform(request)
  }
    
  func deleteInvoice(id: String) async throws {
    let request = StaxApiRequest<StaxInvoice>(
      path: "/invoice/\(id)",
      method: .delete
    )
    let _ = try await httpClient.perform(request)
  }
}
