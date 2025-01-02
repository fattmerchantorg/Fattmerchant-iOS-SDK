/// Protocol defining the customer-related operations available in the Stax API.
public protocol StaxCustomerRepository {
  /// Retrieves a customer by their ID.
  /// - Parameter id: The unique identifier of the customer.
  /// - Returns: The customer if found.
  /// - Throws: StaxNetworkError if the request fails.
  func getCustomer(id: String) async throws -> StaxCustomer

  /// Creates a new customer.
  /// - Parameter request: The customer details to create.
  /// - Returns: The created customer.
  /// - Throws: StaxNetworkError if the request fails.
  func createCustomer(_ request: StaxCustomer) async throws -> StaxCustomer

  /// Updates an existing customer.
  /// - Parameters:
  ///   - id: The unique identifier of the customer to update.
  ///   - update: The updated customer details.
  /// - Returns: The updated customer.
  /// - Throws: StaxNetworkError if the request fails.
  func updateCustomer(id: String, update: StaxCustomer.Update) async throws -> StaxCustomer

  /// Deletes a customer by their ID.
  /// - Parameter id: The unique identifier of the customer to delete.
  /// - Throws: StaxNetworkError if the request fails.
  func deleteCustomer(id: String) async throws
}
