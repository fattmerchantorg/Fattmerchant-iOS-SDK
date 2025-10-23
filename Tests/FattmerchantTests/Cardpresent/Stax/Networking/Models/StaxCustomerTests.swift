import XCTest

final class StaxCustomerTests: XCTestCase {
  
  // MARK: - Test Data
  
  let customerJsonString = """
  {
    "id": "89609cf9-92ba-4829-b115-e753ad67c68e",
    "firstname": "hello",
    "lastname": "bob",
    "company": "",
    "email": "",
    "cc_emails": null,
    "cc_sms": null,
    "phone": "",
    "address_1": "",
    "address_2": "",
    "address_city": "",
    "address_state": "",
    "address_zip": "",
    "address_country": "",
    "notes": null,
    "reference": "",
    "options": null,
    "created_at": "2020-01-09 19:29:49",
    "updated_at": "2020-01-09 19:29:49",
    "deleted_at": null,
    "allow_invoice_credit_card_payments": true,
    "gravatar": false,
    "parent_merge": null,
    "child_merges": []
  }
  """
  
  let validCustomerData: [String: Any?] = [
    "id": "customer-123",
    "firstname": "John",
    "lastname": "Doe",
    "company": "Acme Inc",
    "email": "john.doe@example.com",
    "cc_emails": ["support@example.com"],
    "cc_sms": ["+15551234567"],
    "phone": "+15559876543",
    "address_1": "123 Main St",
    "address_2": "Suite 100",
    "address_city": "San Francisco",
    "address_state": "CA",
    "address_zip": "94105",
    "address_country": "US",
    "notes": "VIP Customer",
    "reference": "REF-123",
    "allow_invoice_credit_card_payments": true,
    "has_address": true,
    "parent_merge": nil,
    "child_merges": ["customer-456"],
    "created_at": "2023-01-01T12:00:00Z",
    "updated_at": "2023-01-02T12:00:00Z"
  ]
  
  // MARK: - Encoding Tests
  
  func testEncoding() throws {
    // Create a customer instance
    let customer = StaxCustomer(
      firstname: "John",
      lastname: "Doe",
      company: "Acme Inc",
      email: "john.doe@example.com",
      ccEmails: ["support@example.com"],
      phone: "+15559876543",
      ccSms: ["+15551234567"],
      address1: "123 Main St",
      address2: "Suite 100",
      addressCity: "San Francisco",
      addressState: "CA",
      addressZip: "94105",
      addressCountry: "US",
      notes: "VIP Customer",
      reference: "REF-123",
      allowInvoiceCreditCardPayments: true
    )
      
    // Encode to data
    let encoder = JSONEncoder()
    encoder.dateEncodingStrategy = .iso8601
    let data = try encoder.encode(customer)
    
    // Decode JSON to verify structure
    let json = try JSONSerialization.jsonObject(with: data) as! [String: Any]
    
    // Verify key mapping
    XCTAssertEqual(json["firstname"] as? String, "John")
    XCTAssertEqual(json["lastname"] as? String, "Doe")
    XCTAssertEqual(json["email"] as? String, "john.doe@example.com")
    XCTAssertEqual(json["cc_emails"] as? [String], ["support@example.com"])
    XCTAssertEqual(json["address_1"] as? String, "123 Main St")
    XCTAssertEqual(json["address_zip"] as? String, "94105")
    XCTAssertEqual(json["allow_invoice_credit_card_payments"] as? Bool, true)
  }
  
  // MARK: - Decoding Tests
  
  func testDecodingFromJsonString() throws {
    // Decode from the provided JSON string
    let jsonData = customerJsonString.data(using: .utf8)!
    
    // Create date formatter for the specific format
    let dateFormatter = DateFormatter()
    dateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss"
    
    // Configure decoder
    let decoder = JSONDecoder()
    decoder.dateDecodingStrategy = .formatted(dateFormatter)
    
    // Decode customer
    let customer = try decoder.decode(StaxCustomer.self, from: jsonData)
    
    // Verify key properties
    XCTAssertEqual(customer.id, "89609cf9-92ba-4829-b115-e753ad67c68e")
    XCTAssertEqual(customer.firstname, "hello")
    XCTAssertEqual(customer.lastname, "bob")
    XCTAssertEqual(customer.company, "")
    XCTAssertEqual(customer.email, "")
    XCTAssertNil(customer.ccEmails)
    XCTAssertNil(customer.ccSms)
    XCTAssertEqual(customer.allowInvoiceCreditCardPayments, true)
    XCTAssertEqual(customer.childMerges, [])
    
    // Verify dates
    let expectedCreatedAt = dateFormatter.date(from: "2020-01-09 19:29:49")
    let expectedUpdatedAt = dateFormatter.date(from: "2020-01-09 19:29:49")
    XCTAssertEqual(customer.createdAt, expectedCreatedAt)
    XCTAssertEqual(customer.updatedAt, expectedUpdatedAt)
  }
  
  func testDecoding() throws {
    // Create JSON data
    let jsonData = try JSONSerialization.data(withJSONObject: validCustomerData)
    
    // Decode customer
    let decoder = JSONDecoder()
    decoder.dateDecodingStrategy = .iso8601
    let customer = try decoder.decode(StaxCustomer.self, from: jsonData)
    
    // Verify all properties were decoded correctly
    XCTAssertEqual(customer.id, "customer-123")
    XCTAssertEqual(customer.firstname, "John")
    XCTAssertEqual(customer.lastname, "Doe")
    XCTAssertEqual(customer.company, "Acme Inc")
    XCTAssertEqual(customer.email, "john.doe@example.com")
    XCTAssertEqual(customer.ccEmails, ["support@example.com"])
    XCTAssertEqual(customer.ccSms, ["+15551234567"])
    XCTAssertEqual(customer.phone, "+15559876543")
    XCTAssertEqual(customer.address1, "123 Main St")
    XCTAssertEqual(customer.address2, "Suite 100")
    XCTAssertEqual(customer.addressCity, "San Francisco")
    XCTAssertEqual(customer.addressState, "CA")
    XCTAssertEqual(customer.addressZip, "94105")
    XCTAssertEqual(customer.addressCountry, "US")
    XCTAssertEqual(customer.notes, "VIP Customer")
    XCTAssertEqual(customer.reference, "REF-123")
    XCTAssertEqual(customer.allowInvoiceCreditCardPayments, true)
    XCTAssertEqual(customer.hasAddress, true)
    XCTAssertEqual(customer.childMerges, ["customer-456"])
    
    // Verify dates
    let dateFormatter = ISO8601DateFormatter()
    XCTAssertEqual(customer.createdAt, dateFormatter.date(from: "2023-01-01T12:00:00Z"))
    XCTAssertEqual(customer.updatedAt, dateFormatter.date(from: "2023-01-02T12:00:00Z"))
  }
  
  // MARK: - Validation Tests
  
  func testMinimumRequirementValidation() throws {
    // Test with only firstname
    let firstnameOnly = StaxCustomer(firstname: "John")
    XCTAssertNotNil(firstnameOnly)
    
    // Test with only lastname
    let lastnameOnly = StaxCustomer(lastname: "Doe")
    XCTAssertNotNil(lastnameOnly)
    
    // Test with only email
    let emailOnly = StaxCustomer(email: "test@example.com")
    XCTAssertNotNil(emailOnly)
    
    // Test with only company
    let companyOnly = StaxCustomer(company: "Acme Inc")
    XCTAssertNotNil(companyOnly)
    
    // Test with completely empty data (this is valid at init time but would fail API validation)
    let emptyCustomer = StaxCustomer()
    XCTAssertNotNil(emptyCustomer)
  }
  
  // MARK: - Update Tests
  
  func testUpdating() {
    // Create initial customer
    let customer = StaxCustomer(
      firstname: "John",
      lastname: "Doe",
      email: "john.doe@example.com"
    )
    
    // Create update object
    var update = customer.updating()
    
    // Modify properties
    update.firstname = "Jane"
    update.phone = "+15551234567"
    update.allowInvoiceCreditCardPayments = false
    
    // Apply changes
    let updatedCustomer = update.apply()
    
    // Verify changes
    XCTAssertEqual(updatedCustomer.firstname, "Jane")
    XCTAssertEqual(updatedCustomer.lastname, "Doe") // Unchanged
    XCTAssertEqual(updatedCustomer.email, "john.doe@example.com") // Unchanged
    XCTAssertEqual(updatedCustomer.phone, "+15551234567") // New value
    XCTAssertEqual(updatedCustomer.allowInvoiceCreditCardPayments, false) // New value
  }
  
  func testModifiedFields() {
    // Create initial customer
    let customer = StaxCustomer(firstname: "John")
    
    // Create update object
    var update = customer.updating()
    
    // Modify properties
    update.firstname = "Jane"
    update.lastname = "Doe"
    
    // Check modified fields
    let modifiedFields = update.modifiedFields
    XCTAssertEqual(modifiedFields.count, 2)
    XCTAssertEqual(modifiedFields["firstname"] as? String, "Jane")
    XCTAssertEqual(modifiedFields["lastname"] as? String, "Doe")
  }
  
  // MARK: - Edge Cases
  
  func testNilValues() throws {
    var data = validCustomerData
    data["firstname"] = nil
    data["lastname"] = nil
    
    // The customer should still decode with nil values
    let jsonData = try JSONSerialization.data(withJSONObject: data)
    let decoder = JSONDecoder()
    decoder.dateDecodingStrategy = .iso8601
    let customer = try decoder.decode(StaxCustomer.self, from: jsonData)
    
    XCTAssertNil(customer.firstname)
    XCTAssertNil(customer.lastname)
    XCTAssertNotNil(customer.email) // Should still have email
  }
  
  func testRoundTripCoding() throws {
    // Create a customer
    let customer = StaxCustomer(
      firstname: "John",
      lastname: "Doe",
      email: "john.doe@example.com"
    )
    
    // Encode to data
    let encoder = JSONEncoder()
    let data = try encoder.encode(customer)
    
    // Decode back to customer
    let decoder = JSONDecoder()
    let decodedCustomer = try decoder.decode(StaxCustomer.self, from: data)
    
    // Verify properties match
    XCTAssertEqual(decodedCustomer.firstname, customer.firstname)
    XCTAssertEqual(decodedCustomer.lastname, customer.lastname)
    XCTAssertEqual(decodedCustomer.email, customer.email)
  }
}
