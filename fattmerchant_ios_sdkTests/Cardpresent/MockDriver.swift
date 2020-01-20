//
//  MockDriver.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/18/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class MockDriver: MobileReaderDriver {

  var reader = MockMobileReader(name: "Mock Reader")

  func isReadyToTakePayment(completion: (Bool) -> Void) {
    completion(true)
  }

  func initialize(args: [String : Any], completion: (Bool) -> Void) {
    completion(true)
  }

  func searchForReaders(args: [String : Any], completion: @escaping ([MobileReader]) -> Void) {
    completion([reader])
  }

  func connect(reader: MobileReader, completion: @escaping (Bool) -> Void) {
    completion(true)
  }

  func performTransaction(with request: TransactionRequest, completion: @escaping (TransactionResult) -> Void) {
    let transactionResult = TransactionResult(
      request: request,
      success: true,
      maskedPan: "411111111234",
      cardHolderFirstName: "William",
      cardHolderLastName: "Holder",
      authCode: "abc123",
      transactionType: "charge",
      amount: request.amount,
      cardType: "visa",
      userReference: "cdm-123123"
    )

    completion(transactionResult)
  }

  func refund(transaction: Transaction, completion: @escaping (TransactionResult) -> Void) {
    let transactionResult = TransactionResult(
      request: nil,
      success: true,
      maskedPan: "411111111234",
      cardHolderFirstName: "William",
      cardHolderLastName: "Holder",
      authCode: "def456",
      transactionType: "refund",
      amount: Amount(cents: 5),
      cardType: "visa",
      userReference: "cdm-123123"
    )

    completion(transactionResult)
  }

}
