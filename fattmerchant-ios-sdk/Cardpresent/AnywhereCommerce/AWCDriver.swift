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

  /// The transaction currently underway
  fileprivate var currentTransaction: AnyPayTransaction?

  /// The place where the transactions take place
  static var source: String = "AWC"

  var familiarSerialNumbers: [String] = []

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
      // If we were unable to authenticate with AWC, then clear out the anyPay instance
      if authenticated == false {
        self.anyPay = nil
      }

      completion(authenticated)
    }
  }

  func isInitialized(completion: @escaping (Bool) -> Void) {
    completion(anyPay != nil)
  }

  func isReadyToTakePayment(completion: (Bool) -> Void) {
    // Make sure the reader is connected
    guard let connectedReader = ANPCardReaderController.shared().connectedReader else {
      return completion(false)
    }

    // Make sure reader is idle
    guard connectedReader.connectionStatus == ANPCardReaderConnectionStatus.CONNECTED else {
      return completion(false)
    }

    // Make sure there is no transaction running
    guard currentTransaction == nil else {
      return completion(false)
    }

    completion(true)
  }

  func searchForReaders(args: [String: Any], completion: @escaping ([MobileReader]) -> Void) {

    ANPCardReaderController.shared().subscribe { (cardReader: AnyPayCardReader?) in
      if let serial = cardReader?.serialNumber {
        self.familiarSerialNumbers.append(serial)
      }

      completion([MobileReader.from(anyPayCardReader: cardReader!)])
    }

    ANPCardReaderController.shared().connectBluetoothReader { (devices: [ANPBluetoothDevice]?) in
      var d = devices

    }
  }

  func connect(reader: MobileReader, completion: @escaping (Bool) -> Void) {
    if ANPCardReaderController.shared().isReaderConnected() &&
      ANPCardReaderController.shared().connectedReader?.serialNumber == reader.serialNumber {
      completion(true)
      return
    }

    guard let serialNumber = reader.serialNumber else {
      completion(false)
      return
    }

    ANPCardReaderController.shared().connectToBluetoothReader(withSerial: serialNumber)
  }

  func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    fatalError("Not implemented")
  }

  func performTransaction(with request: TransactionRequest, completion: @escaping (TransactionResult) -> Void) {
    // Create AnyPay Transaction
    let transaction = AnyPayTransaction(type: .SALE)
    transaction?.currency = "USD"
    transaction?.totalAmount = ANPAmount(string: request.amount.dollarsString())

    transaction?.onSignatureRequired = {
      print("Creating dummy signature")
      transaction?.signature = ANPSignature()
      transaction?.proceed()
    }

    transaction?.execute({ (transactionStatus, error) in
      print(transactionStatus)
      let result = TransactionResult(transaction!)
      completion(result)
    }, cardReaderEvent: { message in
      print(message?.message)
    })
    
  }

  func refund(transaction: Transaction, refundAmount: Amount?, completion: @escaping (TransactionResult) -> Void, error: @escaping (OmniException) -> Void) {
    let refund = AnyPayTransaction(type: .REFUND)
    refund?.externalID = transaction.awcExternalId()
    refund?.totalAmount = ANPAmount(string: refundAmount?.dollarsString())

    refund?.execute({ (transactionStatus, error) in
      print(transactionStatus)
      let result = TransactionResult(refund!)
      completion(result)
    }, cardReaderEvent: { message in
      print(message?.message)
    })
  }

  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    guard let reader = ANPCardReaderController.shared().connectedReader else {
      return completion(nil)
    }
    completion(MobileReader.from(anyPayCardReader: reader))
  }

}
