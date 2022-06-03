//
//  MockDriver.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/18/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class MockDriver: MobileReaderDriver {

  weak var mobileReaderConnectionStatusDelegate: MobileReaderConnectionStatusDelegate?

  var currentlyConnectedReader: MobileReader?

  var reader: MobileReader? = MobileReader(name: "Reader",
                            firmwareVersion: "FakeFirmwareVersion1",
                            make: "FakeMakeR1",
                            model: "FakeModelR1",
                            serialNumber: "FakeSerialNumber")

  var reader2: MobileReader? = MobileReader(name: "Reader 2",
                                           firmwareVersion: "fwv2",
                                           make: "FakeMakeR2",
                                           model: "FakeModelR2",
                                           serialNumber: "TJCJX23UWLE9Y69J")

  var reader3: MobileReader? = MobileReader(name: "Reader 3",
                                           firmwareVersion: "fwv3",
                                           make: "FakeMakeR3",
                                           model: "FakeModelR3",
                                           serialNumber: "GDES4RB9K4QPGWWW")

  var reader4: MobileReader? = MobileReader(name: "Reader 4",
                                           firmwareVersion: "fwv4",
                                           make: "FakeMakeR4",
                                           model: "FakeModelR4",
                                           serialNumber: "7ZP3Y2JM5G227U9E")

  /// Set this to false to simulate a busy mobile reader
  var readyToTakePayment = true

  var familiarSerialNumbers: [String] = []

  var isInitialized: Bool = true
  var shouldConnect: Bool = true

  static var source: String = "MOCKSOURCE"

  static var omniRefundsSupported: Bool = false

  func isReadyToTakePayment(completion: (Bool) -> Void) {
    completion(readyToTakePayment)
  }

  func initialize(args: [String: Any], completion: (Bool) -> Void) {
    if let nmiArgs = args["nmi"] as? NMIDetails {
      return completion(!nmiArgs.securityKey.isEmpty)
    } else if let awcArgs = args["awc"] as? AWCDetails {
      return completion(!awcArgs.terminalId.isEmpty && !awcArgs.terminalSecret.isEmpty)
    } else {
      return completion(false)
    }
  }

  func isInitialized(completion: @escaping (Bool) -> Void) {
    completion(isInitialized)
  }

  func searchForReaders(args: [String: Any], completion: @escaping ([MobileReader]) -> Void) {
    // We have multiple cases for readers either 0, 1, many
    let listOfReaders: [MobileReader] = [reader!, reader2!, reader3!, reader4!]
    guard let readersNeeded = args["readersNeeded"] as? Int else {
      return completion([listOfReaders.first!])
    }
    if readersNeeded <= 0 {
      return completion([])
    } else if readersNeeded == 1 {
      return completion([listOfReaders.first!])
    } else {
      return completion(listOfReaders)
    }
  }

  func connect(reader: MobileReader, completion: @escaping (MobileReader?) -> Void) {
    guard shouldConnect else {
      return completion(nil)
    }

    if let serial = reader.serialNumber {
      familiarSerialNumbers.append(serial)
    }
    currentlyConnectedReader = reader
    completion(reader)
  }

  func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    currentlyConnectedReader = nil
    completion(true)
  }

  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    if let connectedReader = currentlyConnectedReader {
      completion(connectedReader)
      return
    }
    completion(nil)
  }

  func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, userNotificationDelegate: UserNotificationDelegate?, completion: @escaping (TransactionResult) -> Void) {
    performTransaction(with: request,
                       signatureProvider: signatureProvider,
                       transactionUpdateDelegate: transactionUpdateDelegate,
                       completion: completion)
  }

  func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, completion: @escaping (TransactionResult) -> Void) {
    var transactionResult = TransactionResult()
    transactionResult.request = request
    transactionResult.success = true
    transactionResult.maskedPan = "411111111234"
    transactionResult.cardHolderFirstName = "William"
    transactionResult.cardHolderLastName = "Holder"
    transactionResult.authCode = "abc123"
    transactionResult.transactionType = "charge"
    transactionResult.amount = request.amount
    transactionResult.cardType = "visa"
    transactionResult.userReference = "cdm-123123"

    completion(transactionResult)
  }

  func cancelCurrentTransaction(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    completion(true)
  }

  func refund(transaction: Transaction, completion: @escaping (TransactionResult) -> Void, error: @escaping (OmniException) -> Void) {
    refund(transaction: transaction, refundAmount: nil, completion: completion, error: error)
  }

  func refund(transaction: Transaction, refundAmount: Amount?, completion: @escaping (TransactionResult) -> Void, error: @escaping (OmniException) -> Void) {
    var transactionResult = TransactionResult()
    transactionResult.request = nil
    transactionResult.success = true
    transactionResult.maskedPan = "411111111234"
    transactionResult.cardHolderFirstName = "William"
    transactionResult.cardHolderLastName = "Holder"
    transactionResult.authCode = "def456"
    transactionResult.transactionType = "refund"
    transactionResult.amount = Amount(cents: 5)
    transactionResult.cardType = "visa"
    transactionResult.userReference = "cdm-123123"
    transactionResult.transactionSource = nil

    completion(transactionResult)
  }

  func refund(transaction: Transaction, completion: @escaping (TransactionResult) -> Void) {
    var transactionResult = TransactionResult()
    transactionResult.request = nil
    transactionResult.success = true
    transactionResult.maskedPan = "411111111234"
    transactionResult.cardHolderFirstName = "William"
    transactionResult.cardHolderLastName = "Holder"
    transactionResult.authCode = "def456"
    transactionResult.transactionType = "refund"
    transactionResult.amount = Amount(cents: 5)
    transactionResult.cardType = "visa"
    transactionResult.userReference = "cdm-123123"
    transactionResult.transactionSource = nil

    completion(transactionResult)
  }

}
