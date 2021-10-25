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

  static var source: String { get }

  /// True when the Omni API can perfrom the refund
  ///
  /// Some `MobileReaderDriver`s, like NMI, support a deeper integration with Omni, such that Omni can facilitate the
  /// void/refund. This allows the SDK to relieve itself of the responsibility of having to perform the refund directly
  /// with the vendor (NMI), via the vendored SDK (ChipDNA).
  ///
  /// Other `MobileReaderDriver`s, like AWC, do not support this integration. For that reason, the SDK must perform
  /// the void/refund directly with AWC via the AWC sdk.
  ///
  /// - Returns: True when Omni should handle the void/refund logic. False when the driver itself should handle the
  /// refund directly with the vendor.
  static var omniRefundsSupported: Bool { get }

  /// A list of serial numbers that this driver has previously connected to
  var familiarSerialNumbers: [String] { get set }

  var mobileReaderConnectionStatusDelegate: MobileReaderConnectionStatusDelegate? { get set }

  func isReadyToTakePayment(completion: (Bool) -> Void)

  func initialize(args: [String: Any], completion: @escaping (Bool) -> Void)

  /// Checks if the receiver has been initialized
  /// - Parameter completion: Receives true when MobileReaderDriver is initialized. False otherwise
  func isInitialized(completion: @escaping (Bool) -> Void)

  func searchForReaders(args: [String: Any], completion: @escaping ([MobileReader]) -> Void)

  /// Connects the given MobileReader
  /// - Parameters:
  ///   - reader: The `MobileReader` to connect
  ///   - completion: A block to call once finished. Receives the connected `MobileReader` if connected. Otherwise, nil
  func connect(reader: MobileReader, completion: @escaping (MobileReader?) -> Void)

  func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, completion: @escaping (TransactionResult) -> Void)

  func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, userNotificationDelegate: UserNotificationDelegate?, completion: @escaping (TransactionResult) -> Void)

  func capture(transaction: Transaction, completion: @escaping (Bool) -> Void)

  func cancelCurrentTransaction(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void)

  func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void)

  func refund(transaction: Transaction, refundAmount: Amount?, completion: @escaping (TransactionResult) -> Void, error: @escaping (OmniException) -> Void)

  func void(transactionResult: TransactionResult, completion: @escaping (Bool) -> Void)

  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void)
}

extension MobileReaderDriver {

  func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, completion: @escaping (TransactionResult) -> Void) {
    performTransaction(with: request,
                       signatureProvider: signatureProvider,
                       transactionUpdateDelegate: transactionUpdateDelegate,
                       userNotificationDelegate: nil,
                       completion: completion)
  }

  func capture(transaction: Transaction, completion: @escaping (Bool) -> Void) {
    print("MobileReaderDriver#capture not implemented")
    completion(true)
  }

  func void(transactionResult: TransactionResult, completion: @escaping (Bool) -> Void) {
    print("MobileReaderDriver#void not implemented")
    completion(true)
  }

}
