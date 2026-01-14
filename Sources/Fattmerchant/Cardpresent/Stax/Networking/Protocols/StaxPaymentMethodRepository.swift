/// Protocol defining the invoice-related operations available in the Stax API.
public protocol StaxPaymentMethodRepository {
  /// Retrieves a `StaxPaymentMethod` by their ID.
  /// - Parameter id: The unique identifier of the `StaxPaymentMethod`.
  /// - Returns: The `StaxPaymentMethod`if found.
  /// - Throws: `StaxNetworkError` if the request fails.
  func getPaymentMethod(id: String) async throws -> StaxPaymentMethod

  /// Retrieves the surcharge amount for a given payment method
  /// - Parameters:
  ///   - id: The unique identifier of the `StaxPaymentMethod` to review.
  ///   - dollars: The amount in dollars to check against the surcharge amount.
  /// - Returns: The surcharge review  as a `StaxSurchargeReview`.
  func getPaymentMethodSurcharge(id: String, dollars: Double) async throws -> StaxSurchargeReview

  /// Updates an existing `StaxPaymentMethod`.
  /// - Parameters:
  ///   - id: The unique identifier of the `StaxPaymentMethod` to update.
  ///   - update: The updated `StaxPaymentMethod.Update` details.
  /// - Returns: The updated `StaxPaymentMethod`.
  /// - Throws: `StaxNetworkError` if the request fails.
  func updatePaymentMethod(id: String, update: StaxPaymentMethod.Update) async throws -> StaxPaymentMethod

  /// Deletes a `StaxPaymentMethod` by their ID.
  /// - Parameter id: The unique identifier of the customer to delete.
  /// - Throws: `StaxNetworkError` if the request fails.
  func deletePaymentMethod(id: String) async throws
}
