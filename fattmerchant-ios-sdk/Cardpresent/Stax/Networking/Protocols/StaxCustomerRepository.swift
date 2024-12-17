/// Protocol defining the customer-related operations available in the Stax API.
public protocol StaxCustomerRepository {
  /// Retrieves a customer by their ID.
  /// - Parameter id: The unique identifier of the customer.
  /// - Returns: The customer if found.
  /// - Throws: StaxNetworkError if the request fails.
  func getCustomer(id: String) async throws -> Customer

  /// Creates a new customer.
  /// - Parameter request: The customer details to create.
  /// - Returns: The created customer.
  /// - Throws: StaxNetworkError if the request fails.
  func createCustomer(_ request: CustomerRequest) async throws -> Customer

  /// Updates an existing customer.
  /// - Parameters:
  ///   - id: The unique identifier of the customer to update.
  ///   - request: The updated customer details.
  /// - Returns: The updated customer.
  /// - Throws: StaxNetworkError if the request fails.
  func updateCustomer(id: String, request: CustomerRequest) async throws -> Customer

  /// Deletes a customer by their ID.
  /// - Parameter id: The unique identifier of the customer to delete.
  /// - Throws: StaxNetworkError if the request fails.
  func deleteCustomer(id: String) async throws
}
