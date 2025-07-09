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

  var reader: MobileReader? = MobileReader(name: "MockedReader-0000",
                            firmwareVersion: "MockFirmwareVersion-0000",
                            make: "MockMake-Manufacturer",
                            model: "MockModel-MD000",
                            serialNumber: "MockSerialNumber-0000-0000")

  /// Set this to false to simulate a busy mobile reader
  var readyToTakePayment = true

  var familiarSerialNumbers: [String] = []

  var isInitialized: Bool = true
  var shouldConnect: Bool = true

  static var source: String = "MOCKSOURCE"

  static var isStaxRefundsSupported: Bool = false

  func isReadyToTakePayment(completion: (Bool) -> Void) {
    completion(readyToTakePayment)
  }

  func initialize(args: MobileReaderDriverInitializationArgs, completion: (Bool) -> Void) {
    if let args = args as? ChipDnaInitializationArgs {
      completion(args.keys.securityKey.isEmpty)
      return
    }
    
    completion(false)
  }

  func isInitialized(completion: @escaping (Bool) -> Void) {
    completion(isInitialized)
  }

  func searchForReaders(args: MobileReaderDriverSearchArgs?, completion: @escaping ([MobileReader]) -> Void) {
    completion([reader!])
  }

  func connect(reader: MobileReader, completion: @escaping (MobileReader?) -> Void) {
    guard shouldConnect else {
      return completion(nil)
    }

    if let serial = reader.serialNumber {
      familiarSerialNumbers.append(serial)
    }

    completion(reader)
  }

  func disconnect(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    completion(true)
  }

  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    completion(reader)
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
