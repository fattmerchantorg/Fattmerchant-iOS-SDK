//
//  OmniTest.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/31/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class OmniTest: XCTestCase {

  var omni: Omni = Omni()
  let omniApi = MockOmniApi()

  override func setUp() {
    // Mark Omni as initialized so we don't have to run through the initialization. There is a test for the initialization
    omni.initialized = true

    // Give omni the mock api
    omni.omniApi = omniApi

    // Give omni the mock repositories
    omni.mobileReaderDriverRepository = MobileReaderDriverRepository()
    omni.invoiceRepository = InvoiceRepository(omniApi: omniApi)
    omni.transactionRepository = TransactionRepository(omniApi: omniApi)
    omni.paymentMethodRepository = MockPaymentMethodRepository(omniApi: omniApi)
    omni.customerRepository = CustomerRepository(omniApi: omniApi)
  }

  func testCanGetAvailableReaders() {
    let readersAreAvailable = XCTestExpectation(description: "There is at least one mobile reader that is available")

    omni.getAvailableReaders(
      completion: { (availableReaders) in
        readersAreAvailable.fulfill()
    }) { error in
      XCTFail(error.message)
    }

    wait(for: [readersAreAvailable], timeout: 10.0)
  }

  func testCanConnectMobileReader() {
    let connectedReader = XCTestExpectation(description: "Connected the mobile reader")

    omni.getAvailableReaders(completion: { availableReaders in
      let chosenReader = availableReaders.first!
      self.omni.connect(reader: chosenReader, completion: { reader in
        connectedReader.fulfill()
      }) {
        XCTFail("Could not connect reader")
      }
    }) { (error) in
      XCTFail("Could not get available readers")
    }

    wait(for: [connectedReader], timeout: 10.0)
  }

  func testCanNoTakeTransactionIfMobileReaderNotReady() {
    let transactionDoesntStart = XCTestExpectation()

    // Simulate busy mobile reader
    omni.mobileReaderDriverRepository.driver.readyToTakePayment = false

    // Begin the transaction and wait for it to fail
    omni.takeMobileReaderTransaction(
      request: TransactionRequest(amount: Amount(cents: 10)),
      completion: { _ in
        XCTFail("Transaction went through with busy reader")
    },
      error: { error in
        XCTAssert(error is TakeMobileReaderPaymentException)
        transactionDoesntStart.fulfill()
    })

    wait(for: [
      transactionDoesntStart
    ], timeout: 10.0)
  }

  func testCanInitialize() {
    // Mock the self object
    let merchant = Merchant()
    merchant.id = "generated_merchant_id_123"
    let omniSelf = OmniSelf()
    omniSelf.merchant = merchant
    (omni.omniApi as? MockOmniApi)?.omniSelf = omniSelf

    let initialized = expectation(description: "Omni gets initialized")
    
    omni.initialize(
      params: Omni.InitParams(appId: "123", apiKey: "123"),
      completion: {
        initialized.fulfill()
    }) { (error) in
      XCTFail()
    }

    wait(for: [initialized], timeout: 10.0)
  }

  /// Tests that the isInitialized function truly reflects initialization status
  func testIsInitialized() {
    omni.initialized = false
    XCTAssertEqual(omni.isInitialized, false)

    omni.initialized = true
    XCTAssertEqual(omni.isInitialized, true)
  }

  /// Tests cases where omni initialization fails
  func testFailedInitialization() {
    // Mock the self object, but leave out the merchant
    let merchant = Merchant()
    merchant.id = "generated_merchant_id_123"
    let omniSelf = OmniSelf()
    (omni.omniApi as? MockOmniApi)?.omniSelf = omniSelf

    let missingMerchant = expectation(description: "Couldn't get merchant")

    omni.initialize(
      params: Omni.InitParams(appId: "123", apiKey: "123"),
      completion: {
        XCTFail()
    }) { error in
      missingMerchant.fulfill()
    }

    wait(for: [missingMerchant], timeout: 10.0)
  }

  func testUninitializedOmniThrowsError() {
    let omni = Omni()

    func errorBlock(expectation: XCTestExpectation) -> (OmniException) -> Void {
      return { exception in
        switch exception {
        case OmniGeneralException.uninitialized:
          XCTAssertNotNil(exception.detail)
          expectation.fulfill()
        default:
          XCTFail("Omni expected to throw an uninitialized exception, but didn't")
        }
      }
    }

    // Test that connect reader fails
    let connectReaderFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.connect(reader: MobileReaderOne(name: "reader"), completion: { _ in
      XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }, error: {
      connectReaderFails.fulfill()
    })

    // Test that get available readers fails
    let getAvailableReadersFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.getAvailableReaders(completion: { _ in
      XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: getAvailableReadersFails))

    // Test that get mobile reader transactions fails
    let getMobileReaderTransactionsFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.getMobileReaderTransactions(completion: { _ in
      XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: getMobileReaderTransactionsFails))

    // Test that refund fails
    let refundMobileReaderFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.refundMobileReaderTransaction(transaction: Transaction(), completion: { _ in
      XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: refundMobileReaderFails))

    // Test that mobile reader transaction fails
    let mobileReaderTransactionFails = XCTestExpectation(description: "Omni throws an uninitialized error")
    omni.takeMobileReaderTransaction(request: TransactionRequest(amount: Amount(cents: 10)), completion: { _ in
        XCTFail("Omni expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: mobileReaderTransactionFails))

    wait(for: [
      connectReaderFails,
      getAvailableReadersFails,
      getMobileReaderTransactionsFails,
      refundMobileReaderFails,
      mobileReaderTransactionFails
    ], timeout: 10)
  }

}
