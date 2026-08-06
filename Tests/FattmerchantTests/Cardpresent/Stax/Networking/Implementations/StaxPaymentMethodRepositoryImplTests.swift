import XCTest

final class StaxPaymentMethodRepositoryTests: XCTestCase {
    
  // MARK: - Properties
  
  private var httpClient: MockStaxHttpClient!
  private var repository: StaxPaymentMethodRepositoryImpl!
  private let samplePaymentMethodId = "pm-123"
  
  // MARK: - Setup & Teardown
  
  override func setUp() {
    super.setUp()
    httpClient = MockStaxHttpClient()
    repository = StaxPaymentMethodRepositoryImpl(httpClient: httpClient)
  }
  
  override func tearDown() {
    httpClient = nil
    repository = nil
    super.tearDown()
  }
  
  // MARK: - Test Helpers
  
  private func createSamplePaymentMethod() -> StaxPaymentMethod {
    // Create a sample payment method
    return StaxPaymentMethod(
      id: samplePaymentMethodId,
      nickname: "Test Card",
      method: .card,
      cardLastFour: "4242",
      cardExpiry: "1225"
    )
  }
  
  private func createSampleSurchargeReview() -> StaxSurchargeReview {
    // Create a sample surcharge review
    return StaxSurchargeReview(
      binType: "",
      surchargeRate: 3.0,
      surchargeAmount: 100.0,
      totalWithSurchargeAmount: 103.0
    )
  }
  
  // MARK: - Get Payment Method Tests
  
  func testGetPaymentMethod() async throws {
    // Arrange
    let expectedPaymentMethod = createSamplePaymentMethod()
    httpClient.mockResponse = expectedPaymentMethod
    
    // Act
    let paymentMethod = try await repository.getPaymentMethod(id: samplePaymentMethodId)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/payment-method/\(samplePaymentMethodId)")
    XCTAssertEqual(httpClient.lastRequestMethod, .get)
    
    // Verify the returned payment method matches the expected one
    XCTAssertEqual(paymentMethod.id, expectedPaymentMethod.id)
    XCTAssertEqual(paymentMethod.method, expectedPaymentMethod.method)
    XCTAssertEqual(paymentMethod.cardLastFour, expectedPaymentMethod.cardLastFour)
    XCTAssertEqual(paymentMethod.cardExpiry, expectedPaymentMethod.cardExpiry)
    XCTAssertEqual(paymentMethod.nickname, expectedPaymentMethod.nickname)
  }
  
  func testGetPaymentMethodError() async {
    // Arrange
    let expectedError = StaxNetworkError.httpError(404)
    httpClient.mockError = expectedError
    
    // Act & Assert
    do {
      _ = try await repository.getPaymentMethod(id: samplePaymentMethodId)
      XCTFail("Expected error was not thrown")
    } catch let error as StaxNetworkError {
        if case .httpError(let code) = error, code == 404 {
          // Success - expected error was thrown
          XCTAssertEqual(httpClient.lastRequestPath, "/payment-method/\(samplePaymentMethodId)")
          XCTAssertEqual(httpClient.lastRequestMethod, .get)
        } else {
          XCTFail("Incorrect error thrown: \(error)")
        }
    } catch {
      XCTFail("Unexpected error type: \(error)")
    }
  }
  
  // MARK: - Get Payment Method Surcharge Tests
  
  func testGetPaymentMethodSurcharge() async throws {
    // Arrange
    let dollars = 100.0
    let expectedSurcharge = createSampleSurchargeReview()
    httpClient.mockResponse = expectedSurcharge
    
    // Act
    let surcharge = try await repository.getPaymentMethodSurcharge(id: samplePaymentMethodId, dollars: dollars)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "surcharge/review?payment_method_id=\(samplePaymentMethodId)&total=\(dollars)")
    XCTAssertEqual(httpClient.lastRequestMethod, .get)
    
    // Verify the returned surcharge matches the expected one
    XCTAssertEqual(surcharge.binType, expectedSurcharge.binType)
    XCTAssertEqual(surcharge.surchargeAmount, expectedSurcharge.surchargeAmount)
    XCTAssertEqual(surcharge.surchargeRate, expectedSurcharge.surchargeRate)
    XCTAssertEqual(surcharge.totalWithSurchargeAmount, expectedSurcharge.totalWithSurchargeAmount)
  }
  
  func testGetPaymentMethodSurchargeWithPreciseAmount() async throws {
    // Arrange
    let dollars = 99.99
    let expectedSurcharge = createSampleSurchargeReview()
    httpClient.mockResponse = expectedSurcharge
    
    // Act
    let surcharge = try await repository.getPaymentMethodSurcharge(id: samplePaymentMethodId, dollars: dollars)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "surcharge/review?payment_method_id=\(samplePaymentMethodId)&total=\(dollars)")
    XCTAssertEqual(httpClient.lastRequestMethod, .get)
    
    // Verify path contains the exact decimal value
    XCTAssertTrue(httpClient.lastRequestPath?.contains("total=99.99") ?? false)
  }
  
  // MARK: - Update Payment Method Tests
  
  func testUpdatePaymentMethod() async throws {
    // Arrange
    let originalPaymentMethod = createSamplePaymentMethod()

    var update = originalPaymentMethod.updating()
    update.nickname = "Updated Card"
    
    let expectedUpdatedPaymentMethod = StaxPaymentMethod(
      id: samplePaymentMethodId,
      nickname: "Updated Card",
      method: .card,
      cardLastFour: "4242",
      cardExpiry: "1225"
    )
    httpClient.mockResponse = expectedUpdatedPaymentMethod
    
    // Act
    let updatedPaymentMethod = try await repository.updatePaymentMethod(id: samplePaymentMethodId, update: update)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/payment-method/\(samplePaymentMethodId)")
    XCTAssertEqual(httpClient.lastRequestMethod, .put)
    
    // Verify the returned payment method has the updated nickname
    XCTAssertEqual(updatedPaymentMethod.nickname, "Updated Card")
    
    // Verify the rest of the payment method remains unchanged
    XCTAssertEqual(updatedPaymentMethod.id, originalPaymentMethod.id)
    XCTAssertEqual(updatedPaymentMethod.method, originalPaymentMethod.method)
    XCTAssertEqual(updatedPaymentMethod.cardLastFour, originalPaymentMethod.cardLastFour)
    XCTAssertEqual(updatedPaymentMethod.cardExpiry, originalPaymentMethod.cardExpiry)
    
    // Verify body data was passed correctly
    guard let sentPaymentMethod = httpClient.lastRequestBody as? StaxPaymentMethod else {
      XCTFail("Request body was not a StaxPaymentMethod")
      return
    }
  }
  
  // MARK: - Delete Payment Method Tests
  
  func testDeletePaymentMethod() async throws {
    // Arrange
    let deletedPaymentMethod = createSamplePaymentMethod()
    httpClient.mockResponse = deletedPaymentMethod
    
    // Act
    try await repository.deletePaymentMethod(id: samplePaymentMethodId)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/payment-method/\(samplePaymentMethodId)")
    XCTAssertEqual(httpClient.lastRequestMethod, .delete)
  }
  
  func testDeletePaymentMethodError() async {
    // Arrange
    let expectedError = StaxNetworkError.httpError(403)
    httpClient.mockError = expectedError
    
    // Act & Assert
    do {
      try await repository.deletePaymentMethod(id: samplePaymentMethodId)
      XCTFail("Expected error was not thrown")
    } catch let error as StaxNetworkError {
        if case .httpError(let code) = error, code == 403 {
          // Success - expected error was thrown
          XCTAssertEqual(httpClient.lastRequestPath, "/payment-method/\(samplePaymentMethodId)")
          XCTAssertEqual(httpClient.lastRequestMethod, .delete)
        } else {
          XCTFail("Incorrect error thrown: \(error)")
        }
    } catch {
      XCTFail("Unexpected error type: \(error)")
    }
  }
}
