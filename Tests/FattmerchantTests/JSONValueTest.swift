//
//  JSONValueTest.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/14/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import XCTest

class JsonValueTests: XCTestCase {

  var jsonDecoder: JSONDecoder {
    let decoder = JSONDecoder()
    decoder.keyDecodingStrategy = .convertFromSnakeCase
    return decoder
  }

  var jsonEncoder: JSONEncoder {
    let encoder = JSONEncoder()
    encoder.keyEncodingStrategy = .convertToSnakeCase
    return encoder
  }

  func testPaymentMethodJson() {
    let jsonData = paymentMethodJson.data(using: .utf8)!
    let paymentMethod = try? jsonDecoder.decode(PaymentMethod.self, from: jsonData)
    XCTAssertNotNil(paymentMethod, "Parsing payment method failed")
    XCTAssertNotNil(paymentMethod?.id, "Parsed payment method doesn't have id")
  }

  func testTransactionJson() {
    let jsonData = transactionjson.data(using: .utf8)!
    let transaction = try? jsonDecoder.decode(Transaction.self, from: jsonData)
    XCTAssertNotNil(transaction, "Parsing transaction failed")
    XCTAssertNotNil(transaction?.id, "Parsed transaction doesn't have id")
  }

  func testDecodeCustomerJson() {
    let jsonData = customerJson.data(using: .utf8)!
    let customer = try? jsonDecoder.decode(Customer.self, from: jsonData)
    XCTAssertNotNil(customer, "Parsing customer failed")
    XCTAssertNotNil(customer?.id, "Parsed customer doesn't have id")
  }

  func testDecodeInvoiceJson() {
    let jsonData = invoiceJson.data(using: .utf8)!
      let invoice = try? jsonDecoder.decode(Invoice.self, from: jsonData)
      XCTAssertNotNil(invoice, "Parsing invoice failed")
      XCTAssertNotNil(invoice?.id, "Parsed invoice doesn't have id")
  }

  func testDecodeMerchantJson() {
    let jsonData = merchantJson.data(using: .utf8)!
    let merchant = try? jsonDecoder.decode(Merchant.self, from: jsonData)
    XCTAssertNotNil(merchant, "Parsing merchant failed")
    XCTAssertNotNil(merchant?.id, "Parsed merchant doesn't have id")
  }

  func testEncodeMerchantJson() {
    // First decode the merchant from JSON
    let jsonData = merchantJson.data(using: .utf8)!
    guard let merchant = try? jsonDecoder.decode(Merchant.self, from: jsonData) else {
      XCTFail("Couldn't decode merchant")
      return
    }

    // Then encode again using JSONEncoder
    let encodedMerchant = try? jsonEncoder.encode(merchant)
    XCTAssertNotNil(encodedMerchant, "Could not encode merchant")

    // Decode once again
    let decodedMerchant = try? jsonDecoder.decode(Merchant.self, from: encodedMerchant!)

    // Verify that merchants are equal
    XCTAssertEqual(merchant.id, decodedMerchant?.id)
    XCTAssertEqual(merchant.options, merchant.options)
  }

}
