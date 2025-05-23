import XCTest

final class StaxCustomerRepositoryTests: XCTestCase {
  
  // MARK: - Properties
  
  private var httpClient: MockStaxHttpClient!
  private var repository: StaxCustomerRepositoryImpl!
  private let sampleCustomerId = "customer-123"
  
  // MARK: - Setup & Teardown
  
  override func setUp() {
    super.setUp()
    httpClient = MockStaxHttpClient()
    repository = StaxCustomerRepositoryImpl(httpClient: httpClient)
  }
  
  override func tearDown() {
    httpClient = nil
    repository = nil
    super.tearDown()
  }
  
  // MARK: - Test Helpers
  
  private func createSampleCustomer() -> StaxCustomer {
    return StaxCustomer(
      firstname: "John",
      lastname: "Doe",
      email: "john.doe@example.com"
    )
  }
  
  // MARK: - Get Customer Tests
  
  func testGetCustomer() async throws {
    // Arrange
    let expectedCustomer = createSampleCustomer()
    httpClient.mockResponse = expectedCustomer
    
    // Act
    let customer = try await repository.getCustomer(id: sampleCustomerId)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/customer/\(sampleCustomerId)")
    XCTAssertEqual(httpClient.lastRequestMethod, .get)
    XCTAssertEqual(customer.firstname, expectedCustomer.firstname)
    XCTAssertEqual(customer.lastname, expectedCustomer.lastname)
    XCTAssertEqual(customer.email, expectedCustomer.email)
  }
  
  func testGetCustomerError() async {
    // Arrange
    let expectedError = StaxNetworkError.invalidResponse
    httpClient.mockError = expectedError
    
    // Act & Assert
    do {
      _ = try await repository.getCustomer(id: sampleCustomerId)
      XCTFail("Expected error was not thrown")
    } catch let error as StaxNetworkError {
      XCTAssertEqual(error.localizedDescription, expectedError.localizedDescription)
      XCTAssertEqual(httpClient.lastRequestPath, "/customer/\(sampleCustomerId)")
      XCTAssertEqual(httpClient.lastRequestMethod, .get)
    } catch {
      XCTFail("Unexpected error type: \(error)")
    }
  }
  
  // MARK: - Get Payment Methods Tests
  
  func testGetPaymentMethodsForCustomer() async throws {
    // TODO: Finish implementation with PM tests
    
    /*
    // Arrange
    let expectedPaymentMethods = [StaxPaymentMethod(/* initialize with test data */)]
    httpClient.mockResponse = expectedPaymentMethods
    
    // Act
    let paymentMethods = try await repository.getPaymentMethodsForCustomer(id: sampleCustomerId)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/customer/\(sampleCustomerId)/payment-method")
    XCTAssertEqual(httpClient.lastRequestMethod, .get)
    XCTAssertEqual(paymentMethods.count, expectedPaymentMethods.count)
    */
  }
  
  // MARK: - Create Customer Tests
  
  func testCreateCustomer() async throws {
    // Arrange
    let customerToCreate = createSampleCustomer()
    let expectedCustomer = StaxCustomer(
      firstname: customerToCreate.firstname,
      lastname: customerToCreate.lastname,
      email: customerToCreate.email
    )
    httpClient.mockResponse = expectedCustomer
    
    // Act
    let createdCustomer = try await repository.createCustomer(customerToCreate)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/customer")
    XCTAssertEqual(httpClient.lastRequestMethod, .post)
    XCTAssertEqual(createdCustomer.firstname, customerToCreate.firstname)
    XCTAssertEqual(createdCustomer.lastname, customerToCreate.lastname)
    XCTAssertEqual(createdCustomer.email, customerToCreate.email)
    
    // Verify body data was passed correctly
    guard let sentCustomer = httpClient.lastRequestBody as? StaxCustomer else {
        XCTFail("Request body was not a StaxCustomer")
        return
    }
    XCTAssertEqual(sentCustomer.firstname, customerToCreate.firstname)
    XCTAssertEqual(sentCustomer.lastname, customerToCreate.lastname)
    XCTAssertEqual(sentCustomer.email, customerToCreate.email)
  }
  
  // MARK: - Update Customer Tests
  
  func testUpdateCustomer() async throws {
    // Arrange
    let originalCustomer = createSampleCustomer()
    var customerUpdate = originalCustomer.updating()
    customerUpdate.firstname = "Jane"
    customerUpdate.phone = "123-456-7890"
    
    let expectedUpdatedCustomer = StaxCustomer(
      firstname: "Jane",
      lastname: originalCustomer.lastname,
      email: originalCustomer.email,
      phone: "123-456-7890"
    )
    httpClient.mockResponse = expectedUpdatedCustomer
    
    // Act
    let updatedCustomer = try await repository.updateCustomer(id: sampleCustomerId, update: customerUpdate)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/customer/\(sampleCustomerId)")
    XCTAssertEqual(httpClient.lastRequestMethod, .put)
    XCTAssertEqual(updatedCustomer.firstname, "Jane")
    XCTAssertEqual(updatedCustomer.lastname, originalCustomer.lastname)
    XCTAssertEqual(updatedCustomer.phone, "123-456-7890")
    
    // Verify body data was passed correctly
    guard let sentCustomer = httpClient.lastRequestBody as? StaxCustomer else {
      XCTFail("Request body was not a StaxCustomer")
      return
    }
    XCTAssertEqual(sentCustomer.firstname, "Jane")
    XCTAssertEqual(sentCustomer.phone, "123-456-7890")
  }
  
  // MARK: - Delete Customer Tests
  
  func testDeleteCustomer() async throws {
    // Arrange
    let deletedCustomer = createSampleCustomer()
    httpClient.mockResponse = deletedCustomer
    
    // Act
    try await repository.deleteCustomer(id: sampleCustomerId)
    
    // Assert
    XCTAssertEqual(httpClient.lastRequestPath, "/customer/\(sampleCustomerId)")
    XCTAssertEqual(httpClient.lastRequestMethod, .delete)
  }
}
