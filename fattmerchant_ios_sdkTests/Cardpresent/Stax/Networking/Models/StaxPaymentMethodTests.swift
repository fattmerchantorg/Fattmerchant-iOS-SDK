import XCTest

final class StaxPaymentMethodTests: XCTestCase {
    
  // MARK: - Test Data
  
  let paymentMethodJsonString = """
  {
    "id": "pm-12345",
    "customer_id": "cust-789",
    "merchant_id": "merch-456",
    "user_id": "user-123",
    "nickname": "Business Credit Card",
    "is_default": 1,
    "method": "card",
    "meta": {
      "note": "Primary payment method",
      "internal_ref": "REF-001"
    },
    "bin_type": "credit",
    "person_name": "John Smith",
    "card_type": "visa",
    "card_last_four": "4242",
    "card_exp": "12/25",
    "bank_note": "First National Bank",
    "bank_holder_type": "personal",
    "bank_type": "checking",
    "address_1": "123 Main St",
    "address_2": "Apt 4B",
    "address_city": "San Francisco",
    "address_state": "CA",
    "address_zip": "94105",
    "address_country": "US",
    "created_at": "2023-01-15T12:30:45Z",
    "updated_at": "2023-02-10T09:15:22Z",
    "purged_at": null
  }
  """
    
  // MARK: - Initialization Tests
  
  func testInitialization() {
    // Create a payment method with the initializer
    let paymentMethod = StaxPaymentMethod(
      id: "pm-12345",
      customerId: "cust-789",
      merchantId: "merch-456",
      userId: "user-123",
      nickname: "Business Credit Card",
      method: .card,
      cardType: "visa",
      cardLastFour: "4242",
      cardExpiry: "12/25"
    )
    
    // Verify properties
    XCTAssertEqual(paymentMethod.id, "pm-12345")
    XCTAssertEqual(paymentMethod.customerId, "cust-789")
    XCTAssertEqual(paymentMethod.merchantId, "merch-456")
    XCTAssertEqual(paymentMethod.userId, "user-123")
    XCTAssertEqual(paymentMethod.nickname, "Business Credit Card")
    XCTAssertEqual(paymentMethod.method, .card)
    XCTAssertEqual(paymentMethod.cardType, "visa")
    XCTAssertEqual(paymentMethod.cardLastFour, "4242")
    XCTAssertEqual(paymentMethod.cardExpiry, "12/25")
    
    // Optional properties should be nil
    XCTAssertNil(paymentMethod.meta)
    XCTAssertNil(paymentMethod.binType)
    XCTAssertNil(paymentMethod.bankName)
    XCTAssertNil(paymentMethod.bankType)
    XCTAssertNil(paymentMethod.bankHolderType)
    XCTAssertNil(paymentMethod.address1)
    XCTAssertNil(paymentMethod.addressZip)
  }
    
  // MARK: - Decoding Tests
  
  func testDecoding() throws {
    // Decode from the JSON string
    let jsonData = paymentMethodJsonString.data(using: .utf8)!
    
    // Configure decoder
    let decoder = JSONDecoder()
    decoder.dateDecodingStrategy = .iso8601
    
    // Decode payment method
    let paymentMethod = try decoder.decode(StaxPaymentMethod.self, from: jsonData)
    
    // Verify key properties
    XCTAssertEqual(paymentMethod.id, "pm-12345")
    XCTAssertEqual(paymentMethod.customerId, "cust-789")
    XCTAssertEqual(paymentMethod.merchantId, "merch-456")
    XCTAssertEqual(paymentMethod.userId, "user-123")
    XCTAssertEqual(paymentMethod.nickname, "Business Credit Card")
    XCTAssertEqual(paymentMethod.isDefault, 1)
    XCTAssertEqual(paymentMethod.method, .card)
    XCTAssertEqual(paymentMethod.binType, "credit")
    XCTAssertEqual(paymentMethod.personName, "John Smith")
    XCTAssertEqual(paymentMethod.cardType, "visa")
    XCTAssertEqual(paymentMethod.cardLastFour, "4242")
    XCTAssertEqual(paymentMethod.cardExpiry, "12/25")
    XCTAssertEqual(paymentMethod.bankName, "First National Bank")
    XCTAssertEqual(paymentMethod.bankHolderType, .personal)
    XCTAssertEqual(paymentMethod.bankType, .checking)
    XCTAssertEqual(paymentMethod.address1, "123 Main St")
    XCTAssertEqual(paymentMethod.address2, "Apt 4B")
    XCTAssertEqual(paymentMethod.addressCity, "San Francisco")
    XCTAssertEqual(paymentMethod.addressState, "CA")
    XCTAssertEqual(paymentMethod.addressZip, "94105")
    XCTAssertEqual(paymentMethod.addressCountry, "US")
    
    // Verify meta is parsed correctly
    XCTAssertNotNil(paymentMethod.meta)
    
    // Verify JSONCodable meta data if possible
    if case let .object(metaData) = paymentMethod.meta {
      XCTAssertEqual(metaData["note"], .string("Primary payment method"))
      XCTAssertEqual(metaData["internal_ref"], .string("REF-001"))
    } else {
      XCTFail("Meta should be an object")
    }
    
    // Verify dates
    let dateFormatter = ISO8601DateFormatter()
    XCTAssertEqual(paymentMethod.createdAt, dateFormatter.date(from: "2023-01-15T12:30:45Z"))
    XCTAssertEqual(paymentMethod.updatedAt, dateFormatter.date(from: "2023-02-10T09:15:22Z"))
    XCTAssertNil(paymentMethod.purgedAt)
  }
    
  // MARK: - Encoding Tests
  
  func testEncoding() throws {
    // Create a payment method
    let meta = JSONCodable.object([
      "note": .string("Primary payment method"),
      "internal_ref": .string("REF-001")
    ])
    
    let createdDate = ISO8601DateFormatter().date(from: "2023-01-15T12:30:45Z")!
    let updatedDate = ISO8601DateFormatter().date(from: "2023-02-10T09:15:22Z")!
    
    let paymentMethod = StaxPaymentMethod(
      id: "pm-12345",
      customerId: "cust-789",
      merchantId: "merch-456",
      userId: "user-123",
      nickname: "Business Credit Card",
      isDefault: 1,
      method: .card,
      meta: meta,
      binType: "credit",
      personName: "John Smith",
      cardType: "visa",
      cardLastFour: "4242",
      cardExpiry: "12/25",
      bankName: "First National Bank",
      bankType: .checking,
      bankHolderType: .personal,
      address1: "123 Main St",
      address2: "Apt 4B",
      addressCity: "San Francisco",
      addressState: "CA",
      addressZip: "94105",
      addressCountry: "US",
      createdAt: createdDate,
      updatedAt: updatedDate
    )
    
    // Encode to data
    let encoder = JSONEncoder()
    encoder.dateEncodingStrategy = .iso8601
    let data = try encoder.encode(paymentMethod)
    
    // Decode JSON to verify structure
    let json = try JSONSerialization.jsonObject(with: data) as! [String: Any]
    
    // Verify key mapping
    XCTAssertEqual(json["id"] as? String, "pm-12345")
    XCTAssertEqual(json["customer_id"] as? String, "cust-789")
    XCTAssertEqual(json["merchant_id"] as? String, "merch-456")
    XCTAssertEqual(json["user_id"] as? String, "user-123")
    XCTAssertEqual(json["nickname"] as? String, "Business Credit Card")
    XCTAssertEqual(json["is_default"] as? Int, 1)
    XCTAssertEqual(json["method"] as? String, "card")
    XCTAssertEqual(json["bin_type"] as? String, "credit")
    XCTAssertEqual(json["person_name"] as? String, "John Smith")
    XCTAssertEqual(json["card_type"] as? String, "visa")
    XCTAssertEqual(json["card_last_four"] as? String, "4242")
    XCTAssertEqual(json["card_exp"] as? String, "12/25")
    XCTAssertEqual(json["bank_note"] as? String, "First National Bank")
    XCTAssertEqual(json["bank_holder_type"] as? String, "personal")
    XCTAssertEqual(json["bank_type"] as? String, "checking")
    XCTAssertEqual(json["address_1"] as? String, "123 Main St")
    XCTAssertEqual(json["address_2"] as? String, "Apt 4B")
    XCTAssertEqual(json["address_city"] as? String, "San Francisco")
    XCTAssertEqual(json["address_state"] as? String, "CA")
    XCTAssertEqual(json["address_zip"] as? String, "94105")
    XCTAssertEqual(json["address_country"] as? String, "US")
  }
  
  // MARK: - Update Tests
  
  func testUpdating() {
    // Create initial payment method
    let paymentMethod = StaxPaymentMethod(
      id: "pm-12345",
      customerId: "cust-789",
      nickname: "Old Nickname",
      cardLastFour: "4242",
      cardExpiry: "12/25"
    )
    
    // Create update object
    var update = paymentMethod.updating()
    update.nickname = "New Business Card"
    update.isDefault = 1
    update.personName = "Jane Smith"
    update.address1 = "456 New St"
    update.addressCity = "Chicago"
    update.addressState = "IL"
    update.addressZip = "60601"
    
    // Apply changes
    let updatedPaymentMethod = update.apply()
    
    // Verify changes
    XCTAssertEqual(updatedPaymentMethod.nickname, "New Business Card") // This test will fail as nickname isn't included in changes array
    XCTAssertEqual(updatedPaymentMethod.isDefault, 1)
    XCTAssertEqual(updatedPaymentMethod.personName, "Jane Smith")
    XCTAssertEqual(updatedPaymentMethod.address1, "456 New St")
    XCTAssertEqual(updatedPaymentMethod.addressCity, "Chicago")
    XCTAssertEqual(updatedPaymentMethod.addressState, "IL")
    XCTAssertEqual(updatedPaymentMethod.addressZip, "60601")
    
    // Verify unchanged properties
    XCTAssertEqual(updatedPaymentMethod.id, "pm-12345")
    XCTAssertEqual(updatedPaymentMethod.customerId, "cust-789")
    XCTAssertEqual(updatedPaymentMethod.cardLastFour, "4242")
    XCTAssertEqual(updatedPaymentMethod.cardExpiry, "12/25")
  }
  
  func testModifiedFields() {
    // Create initial payment method
    let paymentMethod = StaxPaymentMethod(
      id: "pm-12345",
      customerId: "cust-789",
      cardLastFour: "4242"
    )
    
    // Create update object
    var update = paymentMethod.updating()
    update.isDefault = 1
    update.personName = "Jane Smith"
    
    // Check modified fields
    let modifiedFields = update.modifiedFields
    XCTAssertEqual(modifiedFields.count, 2)
    XCTAssertEqual(modifiedFields["isDefault"] as? Int, 1)
    XCTAssertEqual(modifiedFields["personName"] as? String, "Jane Smith")
    
    // Verify unmodified field isn't in the list
    XCTAssertNil(modifiedFields["cardLastFour"])
  }
  
  // MARK: - Bug Tests
  
  func testAddressInitializationBug() {
    // Create a payment method with addresses
    let paymentMethod = StaxPaymentMethod(
      address1: "123 Main St",
      address2: "Apt 4B"
    )
    
    // Create an update object
    let update = paymentMethod.updating()
    
    // Apply without changes
    let unchangedMethod = update.apply()
    
    // Verify address2 is not accidentally set to address1 value
    XCTAssertEqual(unchangedMethod.address1, "123 Main St")
    XCTAssertEqual(unchangedMethod.address2, "Apt 4B")
    
    // Now modify just address1
    var update2 = paymentMethod.updating()
    update2.address1 = "456 New St"
    
    // Apply changes
    let updatedMethod = update2.apply()
    
    // Verify address2 is still correct and not overwritten
    XCTAssertEqual(updatedMethod.address1, "456 New St")
    XCTAssertEqual(updatedMethod.address2, "Apt 4B")
  }
  
  // MARK: - Edge Cases
  
  func testNilValues() throws {
    // Decode JSON with null values
    let jsonWithNulls = """
    {
      "id": "pm-12345",
      "customer_id": "cust-789",
      "merchant_id": null,
      "user_id": null,
      "nickname": null,
      "is_default": null,
      "method": "card",
      "card_last_four": "4242",
      "card_exp": "12/25"
    }
    """
    
    let jsonData = jsonWithNulls.data(using: .utf8)!
    let decoder = JSONDecoder()
    let paymentMethod = try decoder.decode(StaxPaymentMethod.self, from: jsonData)
    
    // Verify properties
    XCTAssertEqual(paymentMethod.id, "pm-12345")
    XCTAssertEqual(paymentMethod.customerId, "cust-789")
    XCTAssertNil(paymentMethod.merchantId)
    XCTAssertNil(paymentMethod.userId)
    XCTAssertNil(paymentMethod.nickname)
    XCTAssertNil(paymentMethod.isDefault)
    XCTAssertEqual(paymentMethod.method, .card)
    XCTAssertEqual(paymentMethod.cardLastFour, "4242")
    XCTAssertEqual(paymentMethod.cardExpiry, "12/25")
  }
  
  func testRoundTripCoding() throws {
    // Create a payment method
    let paymentMethod = StaxPaymentMethod(
      id: "pm-12345",
      customerId: "cust-789",
      nickname: "Business Card",
      isDefault: 1,
      method: .card,
      cardType: "visa",
      cardLastFour: "4242",
      cardExpiry: "12/25"
    )
    
    // Encode to data
    let encoder = JSONEncoder()
    encoder.dateEncodingStrategy = .iso8601
    let data = try encoder.encode(paymentMethod)
    
    // Decode back to payment method
    let decoder = JSONDecoder()
    decoder.dateDecodingStrategy = .iso8601
    let decodedPaymentMethod = try decoder.decode(StaxPaymentMethod.self, from: data)
    
    // Verify properties match
    XCTAssertEqual(decodedPaymentMethod.id, paymentMethod.id)
    XCTAssertEqual(decodedPaymentMethod.customerId, paymentMethod.customerId)
    XCTAssertEqual(decodedPaymentMethod.nickname, paymentMethod.nickname)
    XCTAssertEqual(decodedPaymentMethod.isDefault, paymentMethod.isDefault)
    XCTAssertEqual(decodedPaymentMethod.method, paymentMethod.method)
    XCTAssertEqual(decodedPaymentMethod.cardType, paymentMethod.cardType)
    XCTAssertEqual(decodedPaymentMethod.cardLastFour, paymentMethod.cardLastFour)
    XCTAssertEqual(decodedPaymentMethod.cardExpiry, paymentMethod.cardExpiry)
  }
}
