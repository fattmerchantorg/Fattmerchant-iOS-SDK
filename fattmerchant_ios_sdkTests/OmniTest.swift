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
  let hostedPaymentsToken = "faketoken"

  override func setUp() {
    // Mark Omni as initialized so we don't have to run through the initialization. There is a test for the initialization
    omni.initialized = true

    // Give omni the mock api
    omni.omniApi = omniApi

    // Give omni a Merchant
    omni.merchant = Merchant()
    omni.merchant?.hostedPaymentsToken = hostedPaymentsToken

    // Give omni the mock repositories
    omni.mobileReaderDriverRepository = MobileReaderDriverRepository()
    omni.mobileReaderDriverRepository.driver = MockDriver()
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

  func testThrowsReaderConnectionError() {
    let errorThrown = XCTestExpectation(description: "Connected the mobile reader")
    omni.mobileReaderDriverRepository.driver.shouldConnect = false

    omni.getAvailableReaders(completion: { availableReaders in
      let chosenReader = availableReaders.first!
      self.omni.connect(reader: chosenReader, completion: { _ in
        XCTFail("Connected reader, but was expecting error")
      }, error: { exception in

        if case ConnectMobileReaderException.couldNotConnectMobileReader(reader: let _) = exception {
          errorThrown.fulfill()
        } else {
          XCTFail("Threw an error, but not the one we were expecting")
        }

      })
    }) { (error) in
      XCTFail("Could not get available readers")
    }

    wait(for: [errorThrown], timeout: 10.0)
  }

  func testCanGetConnectedMobileReader() {
    let readerFound = XCTestExpectation(description: "Reader was found")
    omni.getConnectedReader(completion: { connectedReader in
      readerFound.fulfill()
    }) { error in
      XCTFail("Didn't find the reader")
    }

    wait(for: [readerFound], timeout: 3.0)
  }

  func testCanNotGetConnectedMobileReaderIfNoneAvailable() {
    let connectedReaderNil = XCTestExpectation(description: "No connected reader found")
    let driver = MockDriver()
    driver.reader = nil
    omni.mobileReaderDriverRepository.driver = driver
    omni.getConnectedReader(completion: { connectedReader in
      XCTAssertNil(connectedReader)
      connectedReaderNil.fulfill()
    }) { error in
      XCTFail()
    }

    wait(for: [connectedReaderNil], timeout: 3.0)
  }

  func testCantGetConnectedMobileReaderIfUninitialized() {
    omni.initialized = false
    let errorThrown = XCTestExpectation(description: "Error was thrown")
    let expectedError = OmniGeneralException.uninitialized

    omni.getConnectedReader(completion: { connectedReader in
      XCTFail()
    }) { error in
      XCTAssertEqual(error as! OmniGeneralException, expectedError)
      XCTAssertNotNil(error.detail)
      errorThrown.fulfill()
    }

    wait(for: [errorThrown], timeout: 3.0)
  }

  func testCanDisconnectMobileReader() {
    let reader = MobileReader(name: "Reader", serialNumber: "1234")
    omni.mobileReaderDriverRepository.driver.reader = reader
    omni.mobileReaderDriverRepository.driver.familiarSerialNumbers.append(reader.serialNumber!)

    let expectation = XCTestExpectation(description: "Mobile reader is no longer connected")
    omni.disconnect(reader: reader, completion: { disconnected in
      XCTAssert(disconnected)
      expectation.fulfill()
    }) { error in
      XCTFail()
    }

    wait(for: [expectation], timeout: 10.0)
  }

  func testCanPayWithCreditCard() {
    let creditCard = CreditCard(personName: "Joe Tester", cardNumber: "4111111111111111", cardExp: "0230", addressZip: "32812")
    let transactionRequest = TransactionRequest(amount: Amount(cents: 10), tokenize: false, card: creditCard)
    let transactionWasCompleted = XCTestExpectation(description: "Transaction completed successfully")

    let data = "{\"card_number\":\"4111111111111111\",\"method\":\"card\",\"person_name\":\"Joe Tester\",\"card_exp\":\"0230\",\"address_zip\":\"32812\"}".data(using: .utf8)

    let paymentMethod = PaymentMethod()
    paymentMethod.id = "12345"
    paymentMethod.customerId = "customerid"
    paymentMethod.merchantId = "merchantid"

    omniApi.stub("post", "/webpayment/\(hostedPaymentsToken)/tokenize", body: data, response: .success(paymentMethod))

    omni.pay(transactionRequest: transactionRequest, completion: { (completedTransaction) in
      transactionWasCompleted.fulfill()
    }) { error in
      XCTFail("Transaction failed")
    }

    wait(for: [transactionWasCompleted], timeout: 10.0)
  }

  func testCanNotPayWithCreditCardWithoutMerchant() {
    omni.merchant = nil
    let creditCard = CreditCard(personName: "Joe Tester", cardNumber: "4111111111111111", cardExp: "0230", addressZip: "32812")
    let transactionRequest = TransactionRequest(amount: Amount(cents: 10), tokenize: false, card: creditCard)
    let transactionFailed = XCTestExpectation(description: "Transaction failed")
    let expectedError = OmniGeneralException.uninitialized

    omni.pay(transactionRequest: transactionRequest, completion: { _ in
      XCTFail("Transaction succeeded but shouldnt have")
    }) { error in
      XCTAssertEqual(error as! OmniGeneralException, expectedError)
      transactionFailed.fulfill()
    }

    wait(for: [transactionFailed], timeout: 10.0)
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
    let expectedError = OmniNetworkingException.couldNotGetMerchantDetails

    omni.initialize(
      params: Omni.InitParams(appId: "123", apiKey: "123"),
      completion: {
        XCTFail()
    }) { error in
      XCTAssertEqual(error as! OmniNetworkingException, expectedError)
      XCTAssertNotNil(error.detail)
      missingMerchant.fulfill()
    }

    wait(for: [missingMerchant], timeout: 10.0)
  }

  /// Tests cases where omni initialization fails
  func testFailedInitializationIfDetailsAreMissing() {
    let missingMerchant = expectation(description: "Couldnt initialize")
    var initParams = Omni.InitParams(appId: "", apiKey: "")
    initParams.appId = nil

    let expectedError = OmniInitializeException.missingInitializationDetails

    omni.initialize(
      params: initParams,
      completion: {
        XCTFail()
    }) { error in
      XCTAssertEqual(error as! OmniInitializeException, expectedError)
      XCTAssertNotNil(error.detail)
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
