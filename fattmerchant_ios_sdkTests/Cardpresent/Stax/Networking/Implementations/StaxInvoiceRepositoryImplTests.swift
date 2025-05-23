import XCTest

final class StaxInvoiceRepositoryTests: XCTestCase {
    
  // MARK: - Properties
  
  private var httpClient: MockStaxHttpClient!
  private var repository: StaxInvoiceRepositoryImpl!
  private let sampleInvoiceId = "invoice-123"
    
  // MARK: - Setup & Teardown
  
  override func setUp() {
    super.setUp()
    httpClient = MockStaxHttpClient()
    repository = StaxInvoiceRepositoryImpl(httpClient: httpClient)
  }
    
  override func tearDown() {
    httpClient = nil
    repository = nil
    super.tearDown()
  }
    
  // MARK: - Test Helpers
  
  private func createSampleInvoice() -> StaxInvoice {
    // Create and return a sample invoice with test data
    // Adjust according to your StaxInvoice implementation
    return StaxInvoice(
      id: nil,
      customerId: "customer-456",
      status: .sent,
      total: 100.0
    )
  }
    
  // MARK: - Get Invoice Tests
  
  func testGetInvoice() async throws {
    // Arrange
    let expectedInvoice = createSampleInvoice()
    httpClient.mockResponse = expectedInvoice
      
    // Act
    let invoice = try await repository.getInvoice(id: sampleInvoiceId)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/invoice/\(sampleInvoiceId)")
    XCTAssertEqual(httpClient.lastRequestMethod, .get)
    XCTAssertEqual(invoice.customerId, expectedInvoice.customerId)
    XCTAssertEqual(invoice.total, expectedInvoice.total)
    XCTAssertEqual(invoice.status, expectedInvoice.status)
  }
    
  func testGetInvoiceError() async {
    // Arrange
    let expectedError = StaxNetworkError.httpError(404)
    httpClient.mockError = expectedError
    
    // Act & Assert
    do {
      _ = try await repository.getInvoice(id: sampleInvoiceId)
      XCTFail("Expected error was not thrown")
    } catch let error as StaxNetworkError {
      // Compare the error type - exact equality comparison depends on your error implementation
      if case .httpError(let code) = error, code == 404 {
        // Success - expected error was thrown
        XCTAssertEqual(httpClient.lastRequestPath, "/invoice/\(sampleInvoiceId)")
        XCTAssertEqual(httpClient.lastRequestMethod, .get)
      } else {
        XCTFail("Incorrect error thrown: \(error)")
      }
    } catch {
      XCTFail("Unexpected error type: \(error)")
    }
  }
    
  // MARK: - Create Invoice Tests
  
  func testCreateInvoice() async throws {
    // Arrange
    let invoiceToCreate = createSampleInvoice()
    let expectedInvoice = StaxInvoice(
      id: "new-invoice-id",
      customerId: invoiceToCreate.customerId,
      status: invoiceToCreate.status,
      total: invoiceToCreate.total
    )
    httpClient.mockResponse = expectedInvoice
    
    // Act
    let createdInvoice = try await repository.createInvoice(invoiceToCreate)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/invoice")
    XCTAssertEqual(httpClient.lastRequestMethod, .post)
    XCTAssertEqual(createdInvoice.id, "new-invoice-id")
    XCTAssertEqual(createdInvoice.customerId, invoiceToCreate.customerId)
    XCTAssertEqual(createdInvoice.total, invoiceToCreate.total)
    XCTAssertEqual(createdInvoice.status, invoiceToCreate.status)
    
    // Verify body data was passed correctly
    guard let sentInvoice = httpClient.lastRequestBody as? StaxInvoice else {
      XCTFail("Request body was not a StaxInvoice")
      return
    }
    XCTAssertEqual(sentInvoice.customerId, invoiceToCreate.customerId)
    XCTAssertEqual(sentInvoice.total, invoiceToCreate.total)
    XCTAssertEqual(sentInvoice.status, invoiceToCreate.status)
  }
    
    // MARK: - Update Invoice Tests
    
    func testUpdateInvoice() async throws {
      // Arrange
      let originalInvoice = createSampleInvoice()
      
      var invoiceUpdate = originalInvoice.updating()
      invoiceUpdate.total = 150.0
      invoiceUpdate.status = "sent"
      
      let expectedUpdatedInvoice = StaxInvoice(
        id: sampleInvoiceId,
        customerId: originalInvoice.customerId,
        status: .viewed,
        total: 150.0
      )
      httpClient.mockResponse = expectedUpdatedInvoice
      
      // Act
      let updatedInvoice = try await repository.updateInvoice(id: sampleInvoiceId, update: invoiceUpdate)
      
      // Assert
      XCTAssertEqual(httpClient.lastRequestPath, "/invoice/\(sampleInvoiceId)")
      XCTAssertEqual(httpClient.lastRequestMethod, .put)
      XCTAssertEqual(updatedInvoice.id, expectedUpdatedInvoice.id)
      XCTAssertEqual(updatedInvoice.total, 150.0)
      XCTAssertEqual(updatedInvoice.status, .viewed)
      
      // Verify body data was passed correctly
      guard let sentInvoice = httpClient.lastRequestBody as? StaxInvoice else {
        XCTFail("Request body was not a StaxInvoice")
        return
      }
      XCTAssertEqual(sentInvoice.total, 150.0)
      XCTAssertEqual(sentInvoice.status, .sent)
    }
    
    // MARK: - Delete Invoice Tests
    
    func testDeleteInvoice() async throws {
      // Arrange - Set up a mock response
      httpClient.mockResponse = StaxInvoice()
      
      // Act
      try await repository.deleteInvoice(id: sampleInvoiceId)
      
      // Assert
      XCTAssertEqual(httpClient.lastRequestPath, "/invoice/\(sampleInvoiceId)")
      XCTAssertEqual(httpClient.lastRequestMethod, .delete)
    }
    
    func testDeleteInvoiceError() async {
      // Arrange
      let expectedError = StaxNetworkError.httpError(403)
      httpClient.mockError = expectedError
      
      // Act & Assert
      do {
        try await repository.deleteInvoice(id: sampleInvoiceId)
        XCTFail("Expected error was not thrown")
      } catch {
        // Success - expected error was thrown
        XCTAssertEqual(httpClient.lastRequestPath, "/invoice/\(sampleInvoiceId)")
        XCTAssertEqual(httpClient.lastRequestMethod, .delete)
      }
    }
}
