import XCTest

final class StaxCatalogRepositoryTests: XCTestCase {

  // MARK: - Properties

  private var httpClient: MockStaxHttpClient!
  private var repository: StaxCatalogRepositoryImpl!
  private let sampleItemId = "item-123"
    
  // MARK: - Setup & Teardown
  
  override func setUp() {
    super.setUp()
    httpClient = MockStaxHttpClient()
    repository = StaxCatalogRepositoryImpl(httpClient: httpClient)
  }
    
  override func tearDown() {
    httpClient = nil
    repository = nil
    super.tearDown()
  }
    
  // MARK: - Test Helpers
  
  private func createSampleCatalogItem() -> StaxCatalogItem {
    return StaxCatalogItem(
      id: nil,
      userId: "user-456",
      merchantId: "merchant-789",
      item: "Premium Widget",
      code: "WIDGET-01",
      category: "Electronics",
      details: "High-quality premium widget",
      isActive: true,
      isTaxable: true,
      isService: false,
      isDiscount: false,
      price: 49.99,
      amountInStock: 100,
      meta: nil,
      createdAt: nil,
      updatedAt: nil
    )
  }
    
    // MARK: - Get Catalog Item Tests
    
    func testGetCatalogItem() async throws {
      // Arrange
      let expectedItem = createSampleCatalogItem()
      httpClient.mockResponse = expectedItem
      
      // Act
      let item = try await repository.getCatalogItem(id: sampleItemId)
      
      // Assert
      XCTAssertEqual(httpClient.lastRequestPath, "/item/\(sampleItemId)")
      XCTAssertEqual(httpClient.lastRequestMethod, .get)
      XCTAssertEqual(item.item, expectedItem.item)
      XCTAssertEqual(item.code, expectedItem.code)
      XCTAssertEqual(item.price, expectedItem.price)
    }
    
    func testGetCatalogItemError() async {
      // Arrange
      let expectedError = StaxNetworkError.httpError(404)
      httpClient.mockError = expectedError
      
      // Act & Assert
      do {
        _ = try await repository.getCatalogItem(id: sampleItemId)
        XCTFail("Expected error was not thrown")
      } catch let error as StaxNetworkError {
        if case .httpError(let code) = error, code == 404 {
          XCTAssertEqual(httpClient.lastRequestPath, "/item/\(sampleItemId)")
          XCTAssertEqual(httpClient.lastRequestMethod, .get)
        } else {
          XCTFail("Incorrect error thrown: \(error)")
        }
      } catch {
        XCTFail("Unexpected error type: \(error)")
      }
    }
    
    // MARK: - Get Catalog Codes Tests
    
    func testGetCatalogCodes() async throws {
      // Arrange
      let expectedCodes = ["WIDGET-01", "GADGET-02", nil, "TOOL-03"]
      httpClient.mockResponse = expectedCodes
      
      // Act
      let codes = try await repository.getCatalogCodes()
      
      // Assert
      XCTAssertEqual(httpClient.lastRequestPath, "/item/code")
      XCTAssertEqual(httpClient.lastRequestMethod, .get)
      XCTAssertEqual(codes.count, expectedCodes.count)
      XCTAssertEqual(codes[0], expectedCodes[0])
      XCTAssertEqual(codes[1], expectedCodes[1])
      XCTAssertNil(codes[2])
      XCTAssertEqual(codes[3], expectedCodes[3])
    }
    
    // MARK: - Create Catalog Item Tests
    
    func testCreateCatalogItem() async throws {
      // Arrange
      let itemToCreate = createSampleCatalogItem()
      let expectedItem = StaxCatalogItem(
        id: "new-item-id",
        userId: itemToCreate.userId,
        merchantId: itemToCreate.merchantId,
        item: itemToCreate.item,
        code: itemToCreate.code,
        category: itemToCreate.category,
        details: itemToCreate.details,
        isActive: itemToCreate.isActive,
        isTaxable: itemToCreate.isTaxable,
        isService: itemToCreate.isService,
        isDiscount: itemToCreate.isDiscount,
        price: itemToCreate.price,
        amountInStock: itemToCreate.amountInStock,
        meta: itemToCreate.meta
      )
      httpClient.mockResponse = expectedItem
        
      // Act
      let createdItem = try await repository.createCatalogItem(item: itemToCreate)
      
      // Assert
      XCTAssertEqual(httpClient.lastRequestPath, "/item")
      XCTAssertEqual(httpClient.lastRequestMethod, .post)
      XCTAssertEqual(createdItem.id, "new-item-id")
      XCTAssertEqual(createdItem.item, itemToCreate.item)
      XCTAssertEqual(createdItem.code, itemToCreate.code)
      XCTAssertEqual(createdItem.price, itemToCreate.price)
      XCTAssertEqual(createdItem.amountInStock, itemToCreate.amountInStock)
      
      // Verify body data was passed correctly
      guard let sentItem = httpClient.lastRequestBody as? StaxCatalogItem else {
        XCTFail("Request body was not a StaxCatalogItem")
        return
      }
      XCTAssertEqual(sentItem.item, itemToCreate.item)
      XCTAssertEqual(sentItem.code, itemToCreate.code)
      XCTAssertEqual(sentItem.price, itemToCreate.price)
    }
    
    // MARK: - Update Catalog Item Tests
    
    func testUpdateCatalogItem() async throws {
      // Arrange
      let originalItem = createSampleCatalogItem()
    
      var itemUpdate = originalItem.updating()
      itemUpdate.item = "Premium Widget Pro"
      itemUpdate.price = 59.99
      itemUpdate.details = "Updated premium widget with extended features"
        
      let expectedUpdatedItem = StaxCatalogItem(
        id: sampleItemId,
        userId: originalItem.userId,
        merchantId: originalItem.merchantId,
        item: "Premium Widget Pro",
        code: originalItem.code,
        category: originalItem.category,
        details: "Updated premium widget with extended features",
        isActive: originalItem.isActive,
        isTaxable: originalItem.isTaxable,
        isService: originalItem.isService,
        isDiscount: originalItem.isDiscount,
        price: 59.99,
        amountInStock: originalItem.amountInStock,
        meta: originalItem.meta
      )
      httpClient.mockResponse = expectedUpdatedItem
      
      // Act
      let updatedItem = try await repository.updateCatalogItem(id: sampleItemId, update: itemUpdate)
      
      // Assert
      XCTAssertEqual(httpClient.lastRequestPath, "/item/\(sampleItemId)")
      XCTAssertEqual(httpClient.lastRequestMethod, .put)
      XCTAssertEqual(updatedItem.id, sampleItemId)
      XCTAssertEqual(updatedItem.item, "Premium Widget Pro")
      XCTAssertEqual(updatedItem.price, 59.99)
      XCTAssertEqual(updatedItem.details, "Updated premium widget with extended features")
      
      // Verify body data was passed correctly
      guard let sentItem = httpClient.lastRequestBody as? StaxCatalogItem else {
        XCTFail("Request body was not a StaxCatalogItem")
        return
      }
      XCTAssertEqual(sentItem.item, "Premium Widget Pro")
      XCTAssertEqual(sentItem.price, 59.99)
      XCTAssertEqual(sentItem.details, "Updated premium widget with extended features")
    }
    
    // MARK: - Increment Stock Tests
    
    func testIncrementCatalogItemStock() async throws {
      // Arrange
      let originalItem = createSampleCatalogItem()
      let incrementAmount = 25
      let expectedResult = StaxCatalogItem(
        id: sampleItemId,
        userId: originalItem.userId,
        merchantId: originalItem.merchantId,
        item: originalItem.item,
        code: originalItem.code,
        category: originalItem.category,
        details: originalItem.details,
        isActive: originalItem.isActive,
        isTaxable: originalItem.isTaxable,
        isService: originalItem.isService,
        isDiscount: originalItem.isDiscount,
        price: originalItem.price,
        amountInStock: (originalItem.amountInStock ?? 0) + incrementAmount,
        meta: originalItem.meta
      )
      httpClient.mockResponse = expectedResult
      
      // Act
      let updatedItem = try await repository.incrementCatalogItemStock(id: sampleItemId, amount: incrementAmount)
      
      // Assert
      XCTAssertEqual(httpClient.lastRequestPath, "/item/\(sampleItemId)/increment")
      XCTAssertEqual(httpClient.lastRequestMethod, .put)
      XCTAssertEqual(updatedItem.amountInStock, 125) // 100 + 25
      
      // Verify body data was passed correctly
      guard let body = httpClient.lastRequestBody as? [String: Int] else {
        XCTFail("Request body was not a [String: Int] dictionary")
        return
      }
      XCTAssertEqual(body["quantity"], incrementAmount)
    }
    
    // MARK: - Decrement Stock Tests
    
    func testDecrementCatalogItemStock() async throws {
      // Arrange
      let originalItem = createSampleCatalogItem()
      let decrementAmount = 15
      let expectedResult = StaxCatalogItem(
        id: sampleItemId,
        userId: originalItem.userId,
        merchantId: originalItem.merchantId,
        item: originalItem.item,
        code: originalItem.code,
        category: originalItem.category,
        details: originalItem.details,
        isActive: originalItem.isActive,
        isTaxable: originalItem.isTaxable,
        isService: originalItem.isService,
        isDiscount: originalItem.isDiscount,
        price: originalItem.price,
        amountInStock: (originalItem.amountInStock ?? 0) - decrementAmount,
        meta: originalItem.meta
      )
      httpClient.mockResponse = expectedResult
      
      // Act
      let updatedItem = try await repository.decrementCatalogItemStock(id: sampleItemId, amount: decrementAmount)
      
      // Assert
      XCTAssertEqual(httpClient.lastRequestPath, "/item/\(sampleItemId)/decrement")
      XCTAssertEqual(httpClient.lastRequestMethod, .put)
      XCTAssertEqual(updatedItem.amountInStock, 85) // 100 - 15
      
      // Verify body data was passed correctly
      guard let body = httpClient.lastRequestBody as? [String: Int] else {
        XCTFail("Request body was not a [String: Int] dictionary")
        return
      }
      XCTAssertEqual(body["quantity"], decrementAmount)
    }
    
    // MARK: - Delete Catalog Item Tests
    
    func testDeleteCatalogItem() async throws {
        // Arrange
        let deletedItem = createSampleCatalogItem()
        httpClient.mockResponse = deletedItem
        
        // Act
        try await repository.deleteCatalogItem(id: sampleItemId)
        
        // Assert
        XCTAssertEqual(httpClient.lastRequestPath, "/item/\(sampleItemId)")
        XCTAssertEqual(httpClient.lastRequestMethod, .delete)
    }
    
    func testDeleteCatalogItemError() async {
      // Arrange
      let expectedError = StaxNetworkError.httpError(403)
      httpClient.mockError = expectedError
      
      // Act & Assert
      do {
        try await repository.deleteCatalogItem(id: sampleItemId)
        XCTFail("Expected error was not thrown")
      } catch let error as StaxNetworkError {
        if case .httpError(let code) = error, code == 403 {
          XCTAssertEqual(httpClient.lastRequestPath, "/item/\(sampleItemId)")
          XCTAssertEqual(httpClient.lastRequestMethod, .delete)
        } else {
          XCTFail("Incorrect error thrown: \(error)")
        }
      } catch {
        XCTFail("Unexpected error type: \(error)")
      }
    }
}
