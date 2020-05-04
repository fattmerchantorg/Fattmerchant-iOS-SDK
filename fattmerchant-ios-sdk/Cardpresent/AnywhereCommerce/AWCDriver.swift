//
//  AWCDriver.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 5/4/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import AnyPay

/// A `MobileReaderDriver` that controls AnywhereCommerce mobile readers
class AWCDriver: MobileReaderDriver {

  fileprivate var testGatewayUrl = "https://testpayments.anywherecommerce.com/merchant"
  fileprivate var gatewayUrl = "https://payments.anywherecommerce.com/merchant"
  fileprivate var testTerminalId = "2994002"
  fileprivate var testTerminalKey = "password"

  /// Allows us to interact with AnywhereCommerce
  fileprivate var anyPay: AnyPay?

  /// The place where the transactions take place
  static var source: String = "AWC"

  /// Attempts to initialize the AnyPay SDK
  /// - Parameters:
  ///   - args: a Dictionary containing all necessary data to initialize the sdk
  ///   - completion: a block to run once initialization is complete. The block will receive the value 'true' if initialization was successful, false otherwise
  func initialize(args: [String: Any], completion: @escaping (Bool) -> Void) {
    guard
      let merchant = args["merchant"] as? Merchant,
      let worldnetSecret = merchant.emvTerminalSecret(),
      let worldnetTerminalId = merchant.emvTerminalId()
      else {
        completion(false)
        return
    }

    // Initialize the AnyPay object. This will allow us to interact with AnyPay later on
    let anyPay = AnyPay.initialise()
    self.anyPay = anyPay

    // Configure the endpoint object
    guard let endpoint = anyPay.terminal.endpoint as? ANPWorldnetEndpoint else {
      return
    }
    endpoint.worldnetTerminalID = worldnetTerminalId
    endpoint.worldnetSecret = worldnetSecret
    endpoint.gatewayUrl = gatewayUrl

    // Authenticate
    anyPay.terminal.endpoint.authenticateTerminal { (authenticated, _) in
      var _ = authenticated
      completion(authenticated)
    }
  }

  func isReadyToTakePayment(completion: (Bool) -> Void) {
    fatalError("Not implemented")
  }

  func searchForReaders(args: [String: Any], completion: @escaping ([MobileReader]) -> Void) {
    fatalError("Not implemented")
  }

  func connect(reader: MobileReader, completion: @escaping (Bool) -> Void) {
    fatalError("Not implemented")
  }

  func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    fatalError("Not implemented")
  }

  func performTransaction(with request: TransactionRequest, completion: @escaping (TransactionResult) -> Void) {
    fatalError("Not implemented")
  }

  func refund(transaction: Transaction, refundAmount: Amount?, completion: @escaping (TransactionResult) -> Void, error: @escaping (OmniException) -> Void) {
    fatalError("Not implemented")
  }

  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    fatalError("Not implemented")
  }

}
