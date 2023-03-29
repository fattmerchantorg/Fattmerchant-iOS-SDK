//
//  StaxTest.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/31/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class StaxTest: XCTestCase {

  var stax: Stax = Stax()
  let staxApi = MockStaxApi()
  let hostedPaymentsToken = "faketoken"

  override func setUp() {
    // Mark Stax as initialized so we don't have to run through the initialization. There is a test for the initialization
    stax.mobileReaderDriversInitialized = true

    // Give stax the mock api
    stax.staxApi = staxApi

    // Give stax a Merchant
    stax.merchant = Merchant()
    stax.merchant?.hostedPaymentsToken = hostedPaymentsToken

    // Give stax the mock repositories
    stax.mobileReaderDriverRepository = MobileReaderDriverRepository()
    stax.mobileReaderDriverRepository.driver = MockDriver()
    stax.invoiceRepository = InvoiceRepository(staxApi: staxApi)
    stax.transactionRepository = TransactionRepository(staxApi: staxApi)
    stax.paymentMethodRepository = MockPaymentMethodRepository(staxApi: staxApi)
    stax.customerRepository = CustomerRepository(staxApi: staxApi)
  }

  func testCanGetAvailableReaders() {
    let readersAreAvailable = XCTestExpectation(description: "There is at least one mobile reader that is available")

    stax.getAvailableReaders(
      completion: { (availableReaders) in
        readersAreAvailable.fulfill()
    }) { error in
      XCTFail(error.message)
    }

    wait(for: [readersAreAvailable], timeout: 10.0)
  }

  func testCanConnectMobileReader() {
    let connectedReader = XCTestExpectation(description: "Connected the mobile reader")

    stax.getAvailableReaders(completion: { availableReaders in
      let chosenReader = availableReaders.first!
      self.stax.connect(reader: chosenReader, completion: { reader in
        connectedReader.fulfill()
      }, error: { _ in
        XCTFail("Could not connect reader")
      })
    }) { (error) in
      XCTFail("Could not get available readers")
    }

    wait(for: [connectedReader], timeout: 10.0)
  }

  func testThrowsReaderConnectionError() {
    let errorThrown = XCTestExpectation(description: "Connected the mobile reader")
    stax.mobileReaderDriverRepository.driver.shouldConnect = false

    stax.getAvailableReaders(completion: { availableReaders in
      let chosenReader = availableReaders.first!
      self.stax.connect(reader: chosenReader, completion: { _ in
        XCTFail("Connected reader, but was expecting error")
      }, error: { exception in

        if case ConnectMobileReaderException.couldNotConnectMobileReader(reader: _) = exception {
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
    stax.getConnectedReader(completion: { connectedReader in
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
    stax.mobileReaderDriverRepository.driver = driver
    stax.getConnectedReader(completion: { connectedReader in
      XCTAssertNil(connectedReader)
      connectedReaderNil.fulfill()
    }) { error in
      XCTFail()
    }

    wait(for: [connectedReaderNil], timeout: 3.0)
  }

  func testCantGetConnectedMobileReaderIfUninitialized() {
    stax.mobileReaderDriversInitialized = false
    let errorThrown = XCTestExpectation(description: "Error was thrown")
    let expectedError = StaxGeneralException.uninitialized

    stax.getConnectedReader(completion: { connectedReader in
      XCTFail()
    }) { error in
      XCTAssertEqual(error as! StaxGeneralException, expectedError)
      XCTAssertNotNil(error.detail)
      errorThrown.fulfill()
    }

    wait(for: [errorThrown], timeout: 3.0)
  }

  func testCanDisconnectMobileReader() {
    let reader = MobileReader(name: "Reader", serialNumber: "1234")
    stax.mobileReaderDriverRepository.driver.reader = reader
    stax.mobileReaderDriverRepository.driver.familiarSerialNumbers.append(reader.serialNumber!)

    let expectation = XCTestExpectation(description: "Mobile reader is no longer connected")
    stax.disconnect(reader: reader, completion: { disconnected in
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

    let customer = Customer(firstName: "Joe", lastName: "Tester")
    customer.id = "customerid"
    let paymentMethod = PaymentMethod(customer: customer)
    paymentMethod.id = "12345"
    paymentMethod.customerId = "customerid"
    paymentMethod.merchantId = "merchantid"

    staxApi.stub("post", "/webpayment/\(hostedPaymentsToken)/tokenize", body: data, response: .success(paymentMethod))

    stax.pay(transactionRequest: transactionRequest, completion: { (completedTransaction) in
      transactionWasCompleted.fulfill()
    }) { error in
      XCTFail("Transaction failed")
    }

    wait(for: [transactionWasCompleted], timeout: 10.0)
  }

  func testCanNoTakeTransactionIfMobileReaderNotReady() {
    let transactionDoesntStart = XCTestExpectation()

    // Simulate busy mobile reader
    stax.mobileReaderDriverRepository.driver.readyToTakePayment = false

    // Begin the transaction and wait for it to fail
    stax.takeMobileReaderTransaction(
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
    let staxSelf = StaxSelf()
    staxSelf.merchant = merchant
    (stax.staxApi as? MockStaxApi)?.staxSelf = staxSelf

    let mockApi = stax.staxApi as! MockStaxApi

    let awcDetails = AWCDetails(terminalId: "someterminalid", terminalSecret: "terminalsecret")
    let mobileReaderDetails = MobileReaderDetails()
    mobileReaderDetails.anywhereCommerce = awcDetails
    mockApi.stub("get", "/team/gateway/hardware/mobile", body: nil, response: .success(mobileReaderDetails))

    let initialized = expectation(description: "Stax gets initialized")
    
    stax.initialize(
      params: Stax.InitParams(appId: "123", apiKey: "123"),
      completion: {
        initialized.fulfill()
    }) { (error) in
      XCTFail()
    }

    wait(for: [initialized], timeout: 10.0)
  }

  func testCanInitializeWithMobileReaderSettingsInMerchantOptions() {
    // Mock the self object
    let merchant = Merchant()
    merchant.id = "generated_merchant_id_123"
    let staxSelf = StaxSelf()
    staxSelf.merchant = merchant
    (stax.staxApi as? MockStaxApi)?.staxSelf = staxSelf
    let mockApi = stax.staxApi as! MockStaxApi

    // Make the stax api NOT return the mobile reader settings.
    // This will still make the http call succeed, but it will not provide the necessary info to initialize
    let mobileReaderDetails = MobileReaderDetails()
    mockApi.stub("get", "/team/gateway/hardware/mobile", body: nil, response: .success(mobileReaderDetails))

    // Give the merchant the mobile reader creds. This will be the fallback since the gateways call will not
    // return the details
    merchant.options = ["emv_password": "somepassword"].jsonValue()!

    let initialized = expectation(description: "Stax gets initialized")

    stax.initialize(
      params: Stax.InitParams(appId: "123", apiKey: "123"),
      completion: {
        initialized.fulfill()
    }) { (error) in
      XCTFail()
    }

    wait(for: [initialized], timeout: 3.0)
  }

  func testShouldNotThrowErrorWhenMobileReaderSettingsMissing() {
    // Mock the self object
    let merchant = Merchant()
    merchant.id = "generated_merchant_id_123"
    let staxSelf = StaxSelf()
    staxSelf.merchant = merchant
    (stax.staxApi as? MockStaxApi)?.staxSelf = staxSelf
    let mockApi = stax.staxApi as! MockStaxApi

    // Make the stax api NOT return the mobile reader settings.
    // This will still make the http call succeed & initialize the API, but it will not provide the necessary info to init mobile readers
    let mobileReaderDetails = MobileReaderDetails()
    mockApi.stub("get", "/team/gateway/hardware/mobile", body: nil, response: .success(mobileReaderDetails))

    let errorThrown = expectation(description: "Stax throws error")
    let expectedError = StaxInitializeException.missingMobileReaderCredentials

    stax.initialize(
      params: Stax.InitParams(appId: "123", apiKey: "123"),
      completion: {
        errorThrown.fulfill()
    }) { (error) in
      XCTFail()
      errorThrown.fulfill()
    }

    wait(for: [errorThrown], timeout: 3.0)
  }

  /// Tests that the isInitialized function truly reflects initialization status
  func testIsInitialized() {
    stax.mobileReaderDriversInitialized = false
    XCTAssertEqual(stax.isInitialized, false)

    stax.mobileReaderDriversInitialized = true
    XCTAssertEqual(stax.isInitialized, true)
  }

  /// Tests cases where stax initialization fails
  func testFailedInitialization() {
    // Mock the self object, but leave out the merchant
    let merchant = Merchant()
    merchant.id = "generated_merchant_id_123"
    let staxSelf = StaxSelf()
    (stax.staxApi as? MockStaxApi)?.staxSelf = staxSelf

    let missingMerchant = expectation(description: "Couldn't get merchant")
    let expectedError = StaxNetworkingException.couldNotGetMerchantDetails

    stax.initialize(
      params: Stax.InitParams(appId: "123", apiKey: "123"),
      completion: {
        XCTFail()
    }) { error in
      XCTAssertEqual(error as! StaxNetworkingException, expectedError)
      XCTAssertNotNil(error.detail)
      missingMerchant.fulfill()
    }

    wait(for: [missingMerchant], timeout: 10.0)
  }

  /// Tests cases where stax initialization fails
  func testFailedInitializationIfDetailsAreMissing() {
    let missingMerchant = expectation(description: "Couldnt initialize")
    var initParams = Stax.InitParams(appId: "", apiKey: "")
    initParams.appId = nil

    let expectedError = StaxInitializeException.missingInitializationDetails

    stax.initialize(
      params: initParams,
      completion: {
        XCTFail()
    }) { error in
      XCTAssertEqual(error as! StaxInitializeException, expectedError)
      XCTAssertNotNil(error.detail)
      missingMerchant.fulfill()
    }

    wait(for: [missingMerchant], timeout: 10.0)
  }

  func testUninitializedStaxThrowsError() {
    let stax = Stax()

    func errorBlock(expectation: XCTestExpectation) -> (StaxException) -> Void {
      return { exception in
        switch exception {
        case StaxGeneralException.uninitialized:
          XCTAssertNotNil(exception.detail)
          expectation.fulfill()
        default:
          XCTFail("Stax expected to throw an uninitialized exception, but didn't")
        }
      }
    }

    // Test that connect reader fails
    let connectReaderFails = XCTestExpectation(description: "Stax throws an uninitialized error")
    stax.connect(reader: MobileReader(name: "reader"), completion: { _ in
      XCTFail("Stax expected to throw an uninitialized exception, but didn't")
    }, error: { _ in
      connectReaderFails.fulfill()
    })

    // Test that get available readers fails
    let getAvailableReadersFails = XCTestExpectation(description: "Stax throws an uninitialized error")
    stax.getAvailableReaders(completion: { _ in
      XCTFail("Stax expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: getAvailableReadersFails))

    // Test that get mobile reader transactions fails
    let getMobileReaderTransactionsFails = XCTestExpectation(description: "Stax throws an uninitialized error")
    stax.getMobileReaderTransactions(completion: { _ in
      XCTFail("Stax expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: getMobileReaderTransactionsFails))

    // Test that refund fails
    let refundMobileReaderFails = XCTestExpectation(description: "Stax throws an uninitialized error")
    stax.refundMobileReaderTransaction(transaction: Transaction(), completion: { _ in
      XCTFail("Stax expected to throw an uninitialized exception, but didn't")
    }, error: errorBlock(expectation: refundMobileReaderFails))

    // Test that mobile reader transaction fails
    let mobileReaderTransactionFails = XCTestExpectation(description: "Stax throws an uninitialized error")
    stax.takeMobileReaderTransaction(request: TransactionRequest(amount: Amount(cents: 10)), completion: { _ in
        XCTFail("Stax expected to throw an uninitialized exception, but didn't")
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
