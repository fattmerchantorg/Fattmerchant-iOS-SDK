import XCTest

final class StaxInvoiceTests: XCTestCase {
    
    // MARK: - Test Data
    
    let invoiceJsonString = """
    {
      "id": "invoice-123",
      "user_id": "user-456",
      "customer_id": "customer-789",
      "merchant_id": "merchant-101",
      "payment_method_id": "pm-202",
      "schedule_id": "schedule-303",
      "balance_due": 150.50,
      "is_merchant_present": true,
      "is_webpayment": false,
      "payment_attempt_failed": false,
      "payment_attempt_message": null,
      "status": "draft",
      "total": 150.50,
      "total_paid": 0.00,
      "url": "https://pay.example.com/invoices/invoice-123",
      "meta": {
        "notes": "Priority customer",
        "ref_num": "PO-12345"
      },
      "due_at": "2023-04-15T00:00:00Z",
      "sent_at": null,
      "paid_at": null,
      "viewed_at": null,
      "created_at": "2023-03-15T14:30:45Z",
      "updated_at": "2023-03-15T14:30:45Z"
    }
    """
    
    // MARK: - Initialization Tests
    
    func testInitialization() {
      // Create an invoice with the initializer
      let invoice = StaxInvoice(
        id: "invoice-123",
        userId: "user-456",
        customerId: "customer-789",
        merchantId: "merchant-101",
        status: .draft,
        total: 150.50
      )
      
      // Verify properties
      XCTAssertEqual(invoice.id, "invoice-123")
      XCTAssertEqual(invoice.userId, "user-456")
      XCTAssertEqual(invoice.customerId, "customer-789")
      XCTAssertEqual(invoice.merchantId, "merchant-101")
      XCTAssertEqual(invoice.status, .draft)
      XCTAssertEqual(invoice.total, 150.50)
      
      // Optional properties should be nil
      XCTAssertNil(invoice.paymentMethodId)
      XCTAssertNil(invoice.balanceDue)
      XCTAssertNil(invoice.dueAt)
      XCTAssertNil(invoice.meta)
    }
    
    // MARK: - Decoding Tests
    
    func testDecoding() throws {
      // Decode from the JSON string
      let jsonData = invoiceJsonString.data(using: .utf8)!
      
      // Configure decoder
      let decoder = JSONDecoder()
      decoder.dateDecodingStrategy = .iso8601
      
      // Decode invoice
      let invoice = try decoder.decode(StaxInvoice.self, from: jsonData)
      
      // Verify key properties
      XCTAssertEqual(invoice.id, "invoice-123")
      XCTAssertEqual(invoice.userId, "user-456")
      XCTAssertEqual(invoice.customerId, "customer-789")
      XCTAssertEqual(invoice.merchantId, "merchant-101")
      XCTAssertEqual(invoice.paymentMethodId, "pm-202")
      XCTAssertEqual(invoice.scheduleId, "schedule-303")
      XCTAssertEqual(invoice.balanceDue, 150.50)
      XCTAssertEqual(invoice.isMerchantPresent, true)
      XCTAssertEqual(invoice.isWebpayment, false)
      XCTAssertEqual(invoice.hasPaymentAttemptFailed, false)
      XCTAssertNil(invoice.paymentAttemptMessage)
      XCTAssertEqual(invoice.status, .draft)
      XCTAssertEqual(invoice.total, 150.50)
      XCTAssertEqual(invoice.totalPaid, 0.00)
      XCTAssertEqual(invoice.url, "https://pay.example.com/invoices/invoice-123")
      
      // Verify meta is parsed correctly
      XCTAssertNotNil(invoice.meta)
      
      // Verify dates
      let dateFormatter = ISO8601DateFormatter()
      XCTAssertEqual(invoice.dueAt, dateFormatter.date(from: "2023-04-15T00:00:00Z"))
      XCTAssertNil(invoice.sentAt)
      XCTAssertNil(invoice.paidAt)
      XCTAssertNil(invoice.viewedAt)
      XCTAssertEqual(invoice.createdAt, dateFormatter.date(from: "2023-03-15T14:30:45Z"))
      XCTAssertEqual(invoice.updatedAt, dateFormatter.date(from: "2023-03-15T14:30:45Z"))
    }
    
    // MARK: - Encoding Tests
    
    func testEncoding() throws {
      // Create an invoice
      let dueDate = ISO8601DateFormatter().date(from: "2023-04-15T00:00:00Z")!
      let createdDate = ISO8601DateFormatter().date(from: "2023-03-15T14:30:45Z")!
      
      let metaData = JSONCodable.object([
        "notes": .string("Priority customer"),
        "ref_num": .string("PO-12345")
      ])
      
      let invoice = StaxInvoice(
        id: "invoice-123",
        userId: "user-456",
        customerId: "customer-789",
        merchantId: "merchant-101",
        paymentMethodId: "pm-202",
        scheduleId: "schedule-303",
        balanceDue: 150.50,
        isMerchantPresent: true,
        isWebpayment: false,
        hasPaymentAttemptFailed: false,
        total: 150.50,
        totalPaid: 0.00,
        url: "https://pay.example.com/invoices/invoice-123",
        meta: metaData,
        dueAt: dueDate,
        createdAt: createdDate,
        updatedAt: createdDate
      )
      
      // Encode to data
      let encoder = JSONEncoder()
      encoder.dateEncodingStrategy = .iso8601
      let data = try encoder.encode(invoice)
      
      // Decode JSON to verify structure
      let json = try JSONSerialization.jsonObject(with: data) as! [String: Any]
      
      // Verify key mapping
      XCTAssertEqual(json["id"] as? String, "invoice-123")
      XCTAssertEqual(json["user_id"] as? String, "user-456")
      XCTAssertEqual(json["customer_id"] as? String, "customer-789")
      XCTAssertEqual(json["merchant_id"] as? String, "merchant-101")
      XCTAssertEqual(json["payment_method_id"] as? String, "pm-202")
      XCTAssertEqual(json["schedule_id"] as? String, "schedule-303")
      XCTAssertEqual(json["balance_due"] as? Double, 150.50)
      XCTAssertEqual(json["is_merchant_present"] as? Bool, true)
      XCTAssertEqual(json["is_webpayment"] as? Bool, false)
      XCTAssertEqual(json["payment_attempt_failed"] as? Bool, false)
      XCTAssertEqual(json["total"] as? Double, 150.50)
      XCTAssertEqual(json["total_paid"] as? Double, 0.00)
      XCTAssertEqual(json["url"] as? String, "https://pay.example.com/invoices/invoice-123")
    }
    
    // MARK: - Update Tests
    
    func testUpdating() {
      // Create initial invoice
      let invoice = StaxInvoice(
        id: "invoice-123",
        customerId: "customer-789",
        status: .draft,
        total: 100.00
      )
      
      // Create update object
      var update = invoice.updating()
      
      // Modify properties
      update.total = 150.50
      update.status = StaxInvoiceStatus.sent
      update.dueAt = ISO8601DateFormatter().date(from: "2023-04-15T00:00:00Z")
      
      // Apply changes
      let updatedInvoice = update.apply()
      
      // Verify changes
      XCTAssertEqual(updatedInvoice.total, 150.50)
      XCTAssertEqual(updatedInvoice.status, .sent)
      XCTAssertEqual(updatedInvoice.dueAt, ISO8601DateFormatter().date(from: "2023-04-15T00:00:00Z"))
      
      // Verify unchanged properties
      XCTAssertEqual(updatedInvoice.id, "invoice-123")
      XCTAssertEqual(updatedInvoice.customerId, "customer-789")
    }
    
    func testModifiedFields() {
      // Create initial invoice
      let invoice = StaxInvoice(
        id: "invoice-123",
        customerId: "customer-789",
        status: .draft,
        total: 100.00
      )
      
      // Create update object
      var update = invoice.updating()
      
      // Modify properties
      update.total = 150.50
      update.status = "sent"
      
      // Check modified fields
      let modifiedFields = update.modifiedFields
      XCTAssertEqual(modifiedFields.count, 2)
      XCTAssertEqual(modifiedFields["total"] as? Double, 150.50)
      XCTAssertEqual(modifiedFields["status"] as? String, "sent")
      
      // Verify unmodified field isn't in the list
      XCTAssertNil(modifiedFields["customerId"])
    }
    
    // MARK: - Edge Cases
    
    func testNilValues() throws {
      // Decode JSON with null values
      let jsonWithNulls = """
      {
        "id": "invoice-123",
        "user_id": null,
        "customer_id": "customer-789",
        "merchant_id": null,
        "payment_method_id": null,
        "status": "draft",
        "total": 150.50,
        "created_at": null
      }
      """
      
      let jsonData = jsonWithNulls.data(using: .utf8)!
      let decoder = JSONDecoder()
      let invoice = try decoder.decode(StaxInvoice.self, from: jsonData)
      
      // Verify properties
      XCTAssertEqual(invoice.id, "invoice-123")
      XCTAssertNil(invoice.userId)
      XCTAssertEqual(invoice.customerId, "customer-789")
      XCTAssertNil(invoice.merchantId)
      XCTAssertNil(invoice.paymentMethodId)
      XCTAssertEqual(invoice.status, .draft)
      XCTAssertEqual(invoice.total, 150.50)
      XCTAssertNil(invoice.createdAt)
    }
    
    func testRoundTripCoding() throws {
      // Create an invoice
      let invoice = StaxInvoice(
        id: "invoice-123",
        userId: "user-456",
        customerId: "customer-789",
        status: .draft,
        total: 150.50,
        dueAt: ISO8601DateFormatter().date(from: "2023-04-15T00:00:00Z")
      )
      
      // Encode to data
      let encoder = JSONEncoder()
      encoder.dateEncodingStrategy = .iso8601
      let data = try encoder.encode(invoice)
      
      // Decode back to invoice
      let decoder = JSONDecoder()
      decoder.dateDecodingStrategy = .iso8601
      let decodedInvoice = try decoder.decode(StaxInvoice.self, from: data)
      
      // Verify properties match
      XCTAssertEqual(decodedInvoice.id, invoice.id)
      XCTAssertEqual(decodedInvoice.userId, invoice.userId)
      XCTAssertEqual(decodedInvoice.customerId, invoice.customerId)
      XCTAssertEqual(decodedInvoice.status, invoice.status)
      XCTAssertEqual(decodedInvoice.total, invoice.total)
      XCTAssertEqual(decodedInvoice.dueAt, invoice.dueAt)
    }
}
