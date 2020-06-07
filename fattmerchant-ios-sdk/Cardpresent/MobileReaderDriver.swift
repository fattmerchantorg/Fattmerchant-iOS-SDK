//
//  MobileReaderDriver.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum MobileReaderDriverException: OmniException {
  case couldNotInitialize(detail: String?)

  static var mess: String = "Mobile reader driver error"

  var detail: String? {
    switch self {
    case .couldNotInitialize(let d):
      return d ?? "Could not initialize mobile reader driver"
    }
  }

}

protocol MobileReaderDriver {

  func isReadyToTakePayment(completion: (Bool) -> Void)

  func initialize(args: [String: Any], completion: (Bool) -> Void)

  func searchForReaders(args: [String: Any], completion: @escaping ([MobileReader]) -> Void)

  func connect(reader: MobileReader, completion: @escaping (Bool) -> Void)

  func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, completion: @escaping (TransactionResult) -> Void)

  func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void)

  func refund(transaction: Transaction, refundAmount: Amount?, completion: @escaping (TransactionResult) -> Void, error: @escaping (OmniException) -> Void)

  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void)
}
