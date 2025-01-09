/// Protocol defining the catalog-related operations available in the Stax API.
public protocol StaxCatalogRepository {
  /// Retrieves a `StaxCatalogItem` by their ID.
  /// - Parameter id: The unique identifier of the `StaxCatalogItem`.
  /// - Returns: The `StaxCatalogItem`if found.
  /// - Throws: `StaxNetworkError` if the request fails.
  func getCatalogItem(id: String) async throws -> StaxCatalogItem
  
  /// Retrieves a list of item codes in the Stax Catalog.
  /// - Returns: An array of unique`StaxCatalogItem.code` stored in the catalog
  /// - Throws: `StaxNetworkError` if the request fails.
  func getCatalogCodes() async throws -> [String?]
  
  /// Creates a new `StaxCatalogItem`.
  /// - Parameter request: The `StaxCatalogItem` details to create.
  /// - Returns: The created `StaxCatalogItem`.
  /// - Throws: `StaxNetworkError` if the request fails.
  func createCatalogItem(item: StaxCatalogItem) async throws -> StaxCatalogItem

  /// Updates an existing `StaxCatalogItem`.
  /// - Parameters:
  ///   - id: The unique identifier of the `StaxCatalogItem` to update.
  ///   - update: The updated `StaxCatalogItem.Update` details.
  /// - Returns: The updated `StaxCatalogItem`.
  /// - Throws: `StaxNetworkError` if the request fails.
  func updateCatalogItem(id: String, update: StaxCatalogItem.Update) async throws -> StaxCatalogItem
  
  /// Increments an existing `StaxCatalogItem.in_stock` amount.
  /// - Parameters:
  ///   - id: The unique identifier of the `StaxCatalogItem` to update.
  ///   - amount: The amount to add to the `in_stock` value of the given item.
  /// - Returns: The updated `StaxCatalogItem`.
  /// - Throws: `StaxNetworkError` if the request fails.
  func incrementCatalogItemStock(id: String, amount: Int) async throws -> StaxCatalogItem
  
  /// Decrements an existing `StaxCatalogItem.in_stock` amount.
  /// - Parameters:
  ///   - id: The unique identifier of the `StaxCatalogItem` to update.
  ///   - amount: The amount to add to the `in_stock` value of the given item.
  /// - Returns: The updated `StaxCatalogItem`.
  /// - Throws: `StaxNetworkError` if the request fails.
  func decrementCatalogItemStock(id: String, amount: Int) async throws -> StaxCatalogItem

  /// Deletes a `StaxCatalogItem` by their ID.
  /// - Parameter id: The unique identifier of the customer to delete.
  /// - Throws: `StaxNetworkError` if the request fails.
  func deleteCatalogItem(id: String) async throws
}
