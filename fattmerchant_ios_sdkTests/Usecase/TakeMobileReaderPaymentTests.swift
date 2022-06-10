//
//  TakeMobileReaderTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 5/19/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class TakeMobileReaderPaymentTests: XCTestCase {

  var mockOmniApi: MockOmniApi = MockOmniApi()
  var mobileReaderDriverRepo: MobileReaderDriverRepository!
  var invoiceRepo: InvoiceRepository!
  var customerRepo: CustomerRepository!
  var paymentMethodRepo: PaymentMethodRepository!
  var transactionRepo: TransactionRepository!

  override func setUp() {
    invoiceRepo = InvoiceRepository(omniApi: mockOmniApi)
    customerRepo = CustomerRepository(omniApi: mockOmniApi)
    paymentMethodRepo = PaymentMethodRepository(omniApi: mockOmniApi)
    transactionRepo = TransactionRepository(omniApi: mockOmniApi)
    mobileReaderDriverRepo = MobileReaderDriverRepository()
  }

  func testCanTakeMobileReaderPayment() {
    let transactionCompleted = expectation(description: "Transaction was successfully completed")

    let transactionRequest = TransactionRequest(amount: Amount(cents: 1), tokenize: true)
    TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    ).start(completion: { completedTransaction in
      transactionCompleted.fulfill()
    }) { exception in
      XCTFail("Transaction failed")
    }

    wait(for: [transactionCompleted], timeout: 10.0)
  }

  func testCreateTransactionIsCreatedWithToken() {
    let transactionRequest = TransactionRequest(amount: Amount(cents: 1), tokenize: true)
    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    )

    // Verify that the externalId is put in the transaction
    let transactionHasExternalId = expectation(description: "Transaction has external id")

    let customer = Customer(fullName: "Some Guy")
    customer.id = "customer-id"

    let paymentMethod = PaymentMethod(customer: customer)
    paymentMethod.id = "payment-method-id"

    let invoice = Invoice()
    invoice.id = "invoice-id"

    var result = TransactionResult()
    result.externalId = "externalId"
    result.maskedPan = "41111111111111111"

    job.createTransaction(result: result,
                          driver: mobileReaderDriverRepo.driver,
                          paymentMethod: paymentMethod,
                          customer: customer,
                          invoice: invoice,
                          { exception in
                            XCTFail()
                          }, { transaction in
                            XCTAssertEqual(transaction.token, result.externalId)
                            transactionHasExternalId.fulfill()
                          })

    wait(for: [transactionHasExternalId], timeout: 10.0)
  }

  func testTransactionRequestWithInvalidIdFails() {
    let badInvoiceId = "notreal"
    let transactionRequest = TransactionRequest(amount: Amount(cents: 1), tokenize: true, invoiceId: badInvoiceId)
    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    )

    let expectedError = TakeMobileReaderPaymentException.invoiceNotFound
    let expectation = XCTestExpectation(description: "Transaction fails")

    job.start(completion: { _ in
      XCTFail("Transaction didn't fail")
    }) { error in
      XCTAssertEqual(error.message, expectedError.message)
      if case TakeMobileReaderPaymentException.invoiceNotFound = error {
        expectation.fulfill()
      } else {
        XCTFail("Wrong error thrown")
      }
    }

    wait(for: [expectation], timeout: 10.0)
  }

  func testTransactionRequestWithIdDoesNotCreateNewInvoice() {
    // Add one single invoice to the modelStore
    let invoiceId = UUID().uuidString
    let invoice = Invoice()
    invoice.id = invoiceId

    modelStore = [invoiceId: invoice]

    let transactionRequest = TransactionRequest(amount: Amount(cents: 1), tokenize: true, invoiceId: invoiceId)
    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    )
    
    let expectation = XCTestExpectation(description: "No other invoices are in the modelStore")

    job.start(completion: { transaction in
      XCTAssertEqual(modelStore.values.filter { $0 is Invoice }.count, 1)
      XCTAssertEqual(transaction.invoiceId, invoiceId)
      expectation.fulfill()
    }) { error in
      XCTFail("Transaction failed")
    }

    wait(for: [expectation], timeout: 10.0)
  }
  
  func testTransactionRequestWithCatalogItems() {
    // Create a list of catalog items
    var requestedItems = [CatalogItem]()
    
    let testItem1 = CatalogItem(id: "fakeid1", item: "TestItem1", details: "Test item number one", quantity: 1, price: 0.1, isPercentage: true)
    let testItem2 = CatalogItem(id: "fakeid2", item: "TestItem2", details: "Test item number two", quantity: 1, price: 0.5)
    let testItem3 = CatalogItem(id: "fakeid3", item: "TestItem2", details: "Test item number three", quantity: 3, price: 0.2)
    
    requestedItems.append(testItem1)
    requestedItems.append(testItem2)
    requestedItems.append(testItem3)
    
    let transactionRequest = TransactionRequest(amount: Amount(cents: 8), lineItems: requestedItems)
    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    )
    
    let expectation = XCTestExpectation(description: "Result of transaction has catalog items that match the requested catalog items")
  
    job.start(completion: { transaction in
      if let lineItems = transaction.getLineItems() {
          XCTAssertTrue(lineItems.allSatisfy({ item in
            requestedItems.contains { requestedItem in
              item.id == requestedItem.id
            }
          }))
      } else {
        XCTFail("Failed to get catalog items")
      }
      expectation.fulfill()
    }) { error in
      XCTFail("Transaction failed")
    }
    
    wait(for: [expectation], timeout: 10.0)
  }

  func testTransactionRequestWithExtraMeta() {
    var transactionRequest = TransactionRequest(amount: Amount(cents: 1))
    transactionRequest.subtotal = 0.01
    transactionRequest.tax = 0
    transactionRequest.tip = 0
    transactionRequest.memo = "This transaction is so great!"
    transactionRequest.reference = "1478"

    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    )

    let expectation = XCTestExpectation(description: "Result of transaction has the expected meta data")

      job.start(completion: { transaction in
        XCTAssertEqual(transaction.meta?["subtotal"] as Double?, 0.01)
        XCTAssertEqual(transaction.meta?["tax"] as Double?, 0)
        XCTAssertEqual(transaction.meta?["tip"] as Double?, 0)
        XCTAssertEqual(transaction.meta?["memo"] as String?, "This transaction is so great!")
        XCTAssertEqual(transaction.meta?["reference"] as String?, "1478")
        expectation.fulfill()
      }) { error in
        XCTFail("Transaction failed")
      }

      wait(for: [expectation], timeout: 10.0)
  }

  func testTransactionGetsSetToNotRefundableIfOmniCantPerformRefund() {
    // Set the driver to understand that omni cannot perform the refund. This is the deciding factor in whether or not
    // the transaction gets is_refundable and is_voidable set to false
    MockDriver.omniRefundsSupported = false

    let transactionRequest = TransactionRequest(amount: Amount(cents: 2))

    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    )

    let expectation = XCTestExpectation(description: "Transaction is not refundable or voidable")
    job.start(completion: { transaction in
      XCTAssertEqual(transaction.isRefundable, false)
      XCTAssertEqual(transaction.isRefundable, false)
      expectation.fulfill()
    }) { error in
      XCTFail("Transaction failed")
    }

    wait(for: [expectation], timeout: 3.0)
  }

  func testTransactionDoesNotGetSetToNotRefundableIfOmniCanPerformRefund() {
    // Set the driver to understand that omni cannot perform the refund. This is the deciding factor in whether or not
    // the transaction gets is_refundable and is_voidable set to false
    MockDriver.omniRefundsSupported = true

    let transactionRequest = TransactionRequest(amount: Amount(cents: 2))

    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    )

    let expectation = XCTestExpectation(description: "Transaction is not refundable or voidable")
    job.start(completion: { transaction in
      XCTAssertEqual(transaction.isRefundable, nil)
      XCTAssertEqual(transaction.isRefundable, nil)
      expectation.fulfill()
    }) { error in
      XCTFail("Transaction failed")
    }

    wait(for: [expectation], timeout: 3.0)
  }

  func testInvoiceMetaCreationContainsProperFields() {
    // Setup the transaction request that has all of the important data
    var transactionRequest = TransactionRequest(amount: Amount(cents: 1))
    transactionRequest.subtotal = 0.01
    transactionRequest.tax = 2
    transactionRequest.tip = 0
    transactionRequest.memo = "This transaction is so great!"
    transactionRequest.reference = "1478"
    transactionRequest.poNumber = "988"
    transactionRequest.shippingAmount = 33.43

    // Create the meta
    let meta = TakeMobileReaderPayment.createInvoiceMeta(from: transactionRequest)!

    // Verify fields
    XCTAssertEqual(meta["memo"], transactionRequest.memo)
    XCTAssertEqual(meta["reference"], transactionRequest.reference)
    XCTAssertEqual(meta["poNumber"], transactionRequest.poNumber)
    XCTAssertEqual(meta["shippingAmount"], transactionRequest.shippingAmount)
    XCTAssertEqual(meta["tip"], transactionRequest.tip)
    XCTAssertEqual(meta["subtotal"], transactionRequest.subtotal)
    XCTAssertEqual(meta["tax"], transactionRequest.tax)
  }

  func testTransactionRequestWithInsufficientAmountFails() {
    let fakeBankAccount = BankAccount(personName: "fakePerson", bankType: "fakeType", bankHolderType: .personal, bankAccount: "fakeBank", bankRouting: "fakeRoute", addressZip: "fakeAddress")
    let transactionRequest = TransactionRequest(amount: Amount(dollars: 1000.0), bankAccount: fakeBankAccount)
    let job = TakeMobileReaderPayment(
      mobileReaderDriverRepository: mobileReaderDriverRepo,
      invoiceRepository: invoiceRepo,
      customerRepository: customerRepo,
      paymentMethodRepository: paymentMethodRepo,
      transactionRepository: transactionRepo,
      request: transactionRequest,
      signatureProvider: nil,
      transactionUpdateDelegate: nil,
      userNotificationDelegate: nil
    )

    let expectedError = XCTestError(description: "Insufficient funds")

    job.start(completion: { _ in
      XCTFail("Transaction didn't fail")
    }) { error in
      XCTAssertEqual(error.message, expectedError.message)
      if expectedError.message = error.message {
        expectedError.fulfill()
      } else {
        XCTFail("Wrong error thrown")
      }
    }

    wait(for: [expectedError], timeout: 10.0)
  }
}
