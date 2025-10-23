import XCTest

final class StaxCatalogItemTests: XCTestCase {
    
    // MARK: - Test Data
    
    let catalogItemJsonString = """
    {
      "id": "item-123",
      "user_id": "user-456",
      "merchant_id": "merchant-789",
      "item": "Premium Widget",
      "code": "WIDGET-01",
      "category": "Electronics",
      "details": "High-quality premium widget with extended warranty",
      "is_active": true,
      "is_taxable": true,
      "is_service": false,
      "is_discount": false,
      "price": 49.99,
      "in_stock": 100,
      "meta": {
        "color": "blue",
        "weight": "2.5kg",
        "dimensions": "10x5x2"
      },
      "created_at": "2023-01-15T12:30:45Z",
      "updated_at": "2023-02-10T09:15:22Z",
      "deleted_at": null
    }
    """
    
    let validCatalogItemData: [String: Any] = [
      "id": "item-123",
      "user_id": "user-456",
      "merchant_id": "merchant-789",
      "item": "Premium Widget",
      "code": "WIDGET-01",
      "category": "Electronics",
      "details": "High-quality premium widget with extended warranty",
      "is_active": true,
      "is_taxable": true,
      "is_service": false,
      "is_discount": false,
      "price": 49.99,
      "in_stock": 100,
      "meta": [
        "color": "blue",
        "weight": 2.5,
        "dimensions": "10x5x2"
      ],
      "created_at": "2023-01-15T12:30:45Z",
      "updated_at": "2023-02-10T09:15:22Z",
    ]
    
    // MARK: - Encoding Tests
    
    func testEncoding() throws {
      // Create a catalog item instance
      let catalogItem = StaxCatalogItem(
        id: "item-123",
        userId: "user-456",
        merchantId: "merchant-789",
        item: "Premium Widget",
        code: "WIDGET-01",
        category: "Electronics",
        details: "High-quality premium widget with extended warranty",
        isActive: true,
        isTaxable: true,
        isService: false,
        isDiscount: false,
        price: 49.99,
        amountInStock: 100,
        meta: JSONCodable.object([
          "color": .string("blue"),
          "weight": .double(2.5),
          "dimensions": .string("10x5x2")
        ]),
        createdAt: ISO8601DateFormatter().date(from: "2023-01-15T12:30:45Z"),
        updatedAt: ISO8601DateFormatter().date(from: "2023-02-10T09:15:22Z")
      )
      
      // Encode to data
      let encoder = JSONEncoder()
      encoder.dateEncodingStrategy = .iso8601
      let data = try encoder.encode(catalogItem)
      
      // Decode JSON to verify structure
      let json = try JSONSerialization.jsonObject(with: data) as! [String: Any]
      
      // Verify key mapping
      XCTAssertEqual(json["id"] as? String, "item-123")
      XCTAssertEqual(json["user_id"] as? String, "user-456")
      XCTAssertEqual(json["merchant_id"] as? String, "merchant-789")
      XCTAssertEqual(json["item"] as? String, "Premium Widget")
      XCTAssertEqual(json["code"] as? String, "WIDGET-01")
      XCTAssertEqual(json["is_active"] as? Bool, true)
      XCTAssertEqual(json["price"] as? Double, 49.99)
      XCTAssertEqual(json["in_stock"] as? Int, 100)
      
      
      // Meta mapping
      let meta = json["meta"] as? [String: Any]
      XCTAssertEqual(meta?["color"] as! String, "blue")
      XCTAssertEqual(meta?["weight"] as! Double, 2.5)
      XCTAssertEqual(meta?["dimensions"] as! String, "10x5x2")
    }
    
    // MARK: - Decoding Tests
    
    func testDecodingFromJsonString() throws {
      // Decode from the provided JSON string
      let jsonData = catalogItemJsonString.data(using: .utf8)!
      
      // Configure decoder
      let decoder = JSONDecoder()
      decoder.dateDecodingStrategy = .iso8601
      
      // Decode catalog item
      let catalogItem = try decoder.decode(StaxCatalogItem.self, from: jsonData)
      
      // Verify key properties
      XCTAssertEqual(catalogItem.id, "item-123")
      XCTAssertEqual(catalogItem.userId, "user-456")
      XCTAssertEqual(catalogItem.merchantId, "merchant-789")
      XCTAssertEqual(catalogItem.item, "Premium Widget")
      XCTAssertEqual(catalogItem.code, "WIDGET-01")
      XCTAssertEqual(catalogItem.category, "Electronics")
      XCTAssertEqual(catalogItem.isActive, true)
      XCTAssertEqual(catalogItem.price, 49.99)
      XCTAssertEqual(catalogItem.amountInStock, 100)
      
      // Assert not nil for complex properties
      XCTAssertNotNil(catalogItem.meta)
      
      // Verify dates
      let dateFormatter = ISO8601DateFormatter()
      XCTAssertEqual(catalogItem.createdAt, dateFormatter.date(from: "2023-01-15T12:30:45Z"))
      XCTAssertEqual(catalogItem.updatedAt, dateFormatter.date(from: "2023-02-10T09:15:22Z"))
    }
    
    func testDecoding() throws {
      // Create JSON data
      let jsonData = try JSONSerialization.data(withJSONObject: validCatalogItemData)
      
      // Decode catalog item
      let decoder = JSONDecoder()
      decoder.dateDecodingStrategy = .iso8601
      let catalogItem = try decoder.decode(StaxCatalogItem.self, from: jsonData)
      
      // Verify all properties were decoded correctly
      XCTAssertEqual(catalogItem.id, "item-123")
      XCTAssertEqual(catalogItem.userId, "user-456")
      XCTAssertEqual(catalogItem.merchantId, "merchant-789")
      XCTAssertEqual(catalogItem.item, "Premium Widget")
      XCTAssertEqual(catalogItem.code, "WIDGET-01")
      XCTAssertEqual(catalogItem.category, "Electronics")
      XCTAssertEqual(catalogItem.details, "High-quality premium widget with extended warranty")
      XCTAssertEqual(catalogItem.isActive, true)
      XCTAssertEqual(catalogItem.isTaxable, true)
      XCTAssertEqual(catalogItem.isService, false)
      XCTAssertEqual(catalogItem.isDiscount, false)
      XCTAssertEqual(catalogItem.price, 49.99)
      XCTAssertEqual(catalogItem.amountInStock, 100)
      
      // Verify dates
      let dateFormatter = ISO8601DateFormatter()
      XCTAssertEqual(catalogItem.createdAt, dateFormatter.date(from: "2023-01-15T12:30:45Z"))
      XCTAssertEqual(catalogItem.updatedAt, dateFormatter.date(from: "2023-02-10T09:15:22Z"))
    }
    
    // MARK: - Update Tests
    
    func testUpdating() {
      // Create initial catalog item
      let catalogItem = StaxCatalogItem(
        id: "item-123",
        userId: "user-456",
        merchantId: "merchant-789",
        item: "Premium Widget",
        code: "WIDGET-01",
        category: "Electronics",
        details: "Old details",
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
        
      // Create update object
      var update = catalogItem.updating()
      
      // Modify properties
      update.item = "Premium Widget Pro"
      update.price = 59.99
      update.details = "Updated premium widget with extended features"
      update.amountInStock = 75
      
      // Apply changes
      let updatedItem = update.apply()
      
      // Verify changes
      XCTAssertEqual(updatedItem.item, "Premium Widget Pro")
      XCTAssertEqual(updatedItem.price, 59.99)
      XCTAssertEqual(updatedItem.details, "Updated premium widget with extended features")
      XCTAssertEqual(updatedItem.amountInStock, 75)
      
      // Verify unchanged properties
      XCTAssertEqual(updatedItem.id, "item-123")
      XCTAssertEqual(updatedItem.userId, "user-456")
      XCTAssertEqual(updatedItem.merchantId, "merchant-789")
      XCTAssertEqual(updatedItem.code, "WIDGET-01")
      XCTAssertEqual(updatedItem.category, "Electronics")
    }
    
    func testModifiedFields() {
      // Create initial catalog item
      let catalogItem = StaxCatalogItem(
        id: "item-123",
        userId: "user-456",
        merchantId: "merchant-789",
        item: "Premium Widget",
        code: "WIDGET-01",
        category: "Electronics",
        details: nil,
        isActive: true,
        isTaxable: nil,
        isService: nil,
        isDiscount: nil,
        price: 49.99,
        amountInStock: nil,
        meta: nil,
        createdAt: nil,
        updatedAt: nil
      )
        
      // Create update object
      var update = catalogItem.updating()
      
      // Modify properties
      update.item = "Premium Widget Pro"
      update.price = 59.99
      update.isTaxable = true
      
      // Check modified fields
      let modifiedFields = update.modifiedFields
      XCTAssertEqual(modifiedFields.count, 3)
      XCTAssertEqual(modifiedFields["item"] as? String, "Premium Widget Pro")
      XCTAssertEqual(modifiedFields["price"] as? Double, 59.99)
      XCTAssertEqual(modifiedFields["isTaxable"] as? Bool, true)
      
      // Check an unmodified field isn't in the list
      XCTAssertNil(modifiedFields["code"])
    }
    
    // MARK: - Edge Cases
    
    func testNilValues() throws {
      // Create JSON with nil values
      var nilValueData = validCatalogItemData
      nilValueData["code"] = nil
      nilValueData["details"] = nil
      nilValueData["price"] = nil
      
      // The catalog item should still decode with nil values
      let jsonData = try JSONSerialization.data(withJSONObject: nilValueData)
      let decoder = JSONDecoder()
      decoder.dateDecodingStrategy = .iso8601
      let catalogItem = try decoder.decode(StaxCatalogItem.self, from: jsonData)
      
      XCTAssertNil(catalogItem.code)
      XCTAssertNil(catalogItem.details)
      XCTAssertNil(catalogItem.price)
      XCTAssertNotNil(catalogItem.item) // Should still have item
    }
    
    func testRoundTripCoding() throws {
      // Create a catalog item
      let catalogItem = StaxCatalogItem(
        id: "item-123",
        userId: "user-456",
        merchantId: "merchant-789",
        item: "Premium Widget",
        code: "WIDGET-01",
        category: "Electronics",
        details: "High-quality widget",
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
      
      // Encode to data
      let encoder = JSONEncoder()
      let data = try encoder.encode(catalogItem)
      
      // Decode back to catalog item
      let decoder = JSONDecoder()
      let decodedItem = try decoder.decode(StaxCatalogItem.self, from: data)
      
      // Verify properties match
      XCTAssertEqual(decodedItem.id, catalogItem.id)
      XCTAssertEqual(decodedItem.userId, catalogItem.userId)
      XCTAssertEqual(decodedItem.merchantId, catalogItem.merchantId)
      XCTAssertEqual(decodedItem.item, catalogItem.item)
      XCTAssertEqual(decodedItem.code, catalogItem.code)
      XCTAssertEqual(decodedItem.category, catalogItem.category)
      XCTAssertEqual(decodedItem.details, catalogItem.details)
      XCTAssertEqual(decodedItem.isActive, catalogItem.isActive)
      XCTAssertEqual(decodedItem.isTaxable, catalogItem.isTaxable)
      XCTAssertEqual(decodedItem.isService, catalogItem.isService)
      XCTAssertEqual(decodedItem.isDiscount, catalogItem.isDiscount)
      XCTAssertEqual(decodedItem.price, catalogItem.price)
      XCTAssertEqual(decodedItem.amountInStock, catalogItem.amountInStock)
    }
}
