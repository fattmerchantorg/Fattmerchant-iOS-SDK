/// Implementation of CustomerRepository that communicates with the Stax API.
final class StaxCatalogRepositoryImpl: StaxCatalogRepository {
  private let httpClient: StaxHttpClientProtocol
    
  init(httpClient: StaxHttpClientProtocol) {
    self.httpClient = httpClient
  }
  
  func getCatalogItem(id: String) async throws -> StaxCatalogItem {
    let request = StaxApiRequest<StaxCatalogItem>(
      path: "/item/\(id)",
      method: .get
    )
    return try await httpClient.perform(request)
  }
  
  func getCatalogCodes() async throws -> [String?] {
    let request = StaxApiRequest<[String?]>(
      path: "/item/code",
      method: .get
    )
    return try await httpClient.perform(request)
  }
  
  func createCatalogItem(item: StaxCatalogItem) async throws -> StaxCatalogItem {
    let request = StaxApiRequest<StaxCatalogItem>(
      path: "/item",
      method: .post,
      body: item
    )
    return try await httpClient.perform(request)
  }
  
  func updateCatalogItem(id: String, update: StaxCatalogItem.Update) async throws -> StaxCatalogItem {
    let request = StaxApiRequest<StaxCatalogItem>(
      path: "/item/\(id)",
      method: .put,
      body: update.apply()
    )
    return try await httpClient.perform(request)
  }
  
  func incrementCatalogItemStock(id: String, amount: Int) async throws -> StaxCatalogItem {
    let request = StaxApiRequest<StaxCatalogItem>(
      path: "/item/\(id)/increment",
      method: .put,
      body: ["quantity": amount]
    )
    return try await httpClient.perform(request)
  }
  
  func decrementCatalogItemStock(id: String, amount: Int) async throws -> StaxCatalogItem {
    let request = StaxApiRequest<StaxCatalogItem>(
      path: "/item/\(id)/decrement",
      method: .put,
      body: ["quantity": amount]
    )
    return try await httpClient.perform(request)
  }
  
  func deleteCatalogItem(id: String) async throws {
    let request = StaxApiRequest<StaxCatalogItem>(
      path: "/item/\(id)",
      method: .delete
    )
    let _ = try await httpClient.perform(request)
  }
}
