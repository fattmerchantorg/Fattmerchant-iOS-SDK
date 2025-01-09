/// Protocol defining the invoice-related operations available in the Stax API.
public protocol StaxInvoiceRepository {
  /// Retrieves a invoice by their ID.
  /// - Parameter id: The unique identifier of the invoice.
  /// - Returns: The customer if found.
  /// - Throws: StaxNetworkError if the request fails.
  func getInvoice(id: String) async throws -> StaxInvoice

  /// Creates a new invoice.
  /// - Parameter request: The invoice details to create.
  /// - Returns: The created invoice.
  /// - Throws: StaxNetworkError if the request fails.
  func createInvoice(_ request: StaxInvoice) async throws -> StaxInvoice

  /// Updates an existing invoice.
  /// - Parameters:
  ///   - id: The unique identifier of the customer to update.
  ///   - update: The updated invoice details.
  /// - Returns: The updated invoice.
  /// - Throws: StaxNetworkError if the request fails.
  func updateInvoice(id: String, update: StaxInvoice.Update) async throws -> StaxInvoice

  /// Deletes a customer by their ID.
  /// - Parameter id: The unique identifier of the customer to delete.
  /// - Throws: StaxNetworkError if the request fails.
  func deleteInvoice(id: String) async throws
}
