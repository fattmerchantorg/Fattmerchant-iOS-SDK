//
//  AWCDriver.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 5/4/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import AnyPay
import CoreBluetooth

#if targetEnvironment(simulator)

#else

/// A `MobileReaderDriver` that controls AnywhereCommerce mobile readers
class AWCDriver: NSObject, MobileReaderDriver, CBCentralManagerDelegate {

  /// Manages the search of BT devices
  fileprivate var btCentralManager: CBCentralManager?

  /// Holds a set of the bluetooth devices that have been discovered for the purpose of BT connection
  ///
  /// - Note: This set is cleared every time a new BT search is conducted
  fileprivate var discoveredBluetoothDeviceSerialNumbers: Set<String> = []

  /// A test endpoint for AWC
  fileprivate var testGatewayUrl = "https://testpayments.anywherecommerce.com/merchant"

  /// The live endpoint for AWC
  fileprivate var gatewayUrl = "https://payments.anywherecommerce.com/merchant"

  /// A test terminalId for AWC
  fileprivate var testTerminalId = "2994002"

  /// A test TerminalKey for AWC
  fileprivate var testTerminalKey = "password"

  /// Allows us to interact with AnywhereCommerce
  fileprivate var anyPay: AnyPay?

  /// The transaction currently underway
  fileprivate var currentTransaction: AnyPayTransaction?

  /// Listens to transaction update events
  weak var transactionUpdateDelegate: TransactionUpdateDelegate?

  /// Listens to mobile reader status update events
  weak var mobileReaderConnectionStatusDelegate: MobileReaderConnectionStatusDelegate?

  /// The place where the transactions take place
  static var source: String = "AWC"

  static var omniRefundsSupported: Bool = false

  var familiarSerialNumbers: [String] = []

  /// Attempts to initialize the AnyPay SDK
  /// - Parameters:
  ///   - args: a Dictionary containing all necessary data to initialize the sdk
  ///   - completion: a block to run once initialization is complete. The block will receive the value 'true' if initialization was successful, false otherwise
  func initialize(args: [String: Any], completion: @escaping (Bool) -> Void) {
    guard
      let awcDetails = args["awc"] as? AWCDetails
      else {
        completion(false)
        return
    }
    
    let worldnetSecret = awcDetails.terminal_secret
    let worldnetTerminalId = awcDetails.terminal_id

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
      } else {
        self.startBtService()
        self.onCardReaderDisconnected = {
          self.mobileReaderConnectionStatusDelegate?.mobileReaderConnectionStatusUpdate(status: .disconnected)
        }
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
    // Start searching
    guard startBtDeviceSearch() else {
      return completion([])
    }

    // After some time, stop searching and report the results
    let deadline = DispatchTime.now() + DispatchTimeInterval.seconds(3)
    DispatchQueue.global(qos: .userInitiated).asyncAfter(deadline: deadline) {
      self.stopBtDeviceSearch()
      let readers = self.discoveredBluetoothDeviceSerialNumbers.map {
        MobileReader(name: $0, serialNumber: $0)
      }
      completion(readers)
    }
  }

  func connect(reader: MobileReader, completion: @escaping (MobileReader?) -> Void) {
    // If the reader we want to connect is already connect it, just return it
    if let connectedReader = connectedReader(), connectedReader.serialNumber == reader.serialNumber {
      completion(connectedReader)
      return
    }

    // Can't connect to a reader without a serial number
    guard let serialNumber = reader.serialNumber else {
      completion(nil)
      return
    }

    // Subscribe to events
    onCardReaderConnected = { reader in
      if let reader = reader {
        self.familiarSerialNumbers.append(serialNumber)
        completion(MobileReader.from(anyPayCardReader: reader))
      } else {
        completion(nil)
      }
    }

    onCardReaderConnectionFailed = { _ in
      completion(nil)
    }

    // Request that AWC connect the reader
    mobileReaderConnectionStatusDelegate?.mobileReaderConnectionStatusUpdate(status: .connecting)
    ANPCardReaderController.shared().connectToBluetoothReader(withSerial: serialNumber)
  }

  func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    ANPCardReaderController.shared().disconnectReader()
    completion(true)
  }

  func cancelCurrentTransaction(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    guard let transaction = currentTransaction else {
      return error(CancelCurrentTransactionException.noTransactionToCancel)
    }
    transaction.cancel()
    completion(transaction.isCancelled())
  }

  func performTransaction(with request: TransactionRequest,
                          signatureProvider: SignatureProviding?,
                          transactionUpdateDelegate: TransactionUpdateDelegate?,
                          completion: @escaping (TransactionResult) -> Void) {
    // Create AnyPay Transaction
    let transaction = AnyPayTransaction(type: .SALE)
    transaction?.currency = "USD"
    transaction?.totalAmount = ANPAmount(string: request.amount.dollarsString())

    // Register block for capturing signature
    transaction?.onSignatureRequired = {
      transactionUpdateDelegate?.onTransactionUpdate(transactionUpdate: .PromptProvideSignature)

      if let signatureProvider = signatureProvider {
        signatureProvider.signatureRequired(completion: { signature in
          transactionUpdateDelegate?.onTransactionUpdate(transactionUpdate: .SignatureProvided)
          do {
            let anpSignature = try ANPSignature(data: signature.data(using: .utf8))
            transaction?.signature = anpSignature
          } catch {
            transaction?.signature = ANPSignature()
          }
          transaction?.proceed()
        })
      } else {
        transaction?.signature = ANPSignature()
        transaction?.proceed()
      }
    }

    currentTransaction = transaction
    transaction?.execute({ (transactionStatus, error) in
      self.currentTransaction = nil
      let result = TransactionResult(transaction!)
      completion(result)
    }, cardReaderEvent: { message in
      guard let message = message else {
        return
      }
      print(message.message)
      if let transactionUpdate = TransactionUpdate(anpMeaningfulMessage: message) {
        transactionUpdateDelegate?.onTransactionUpdate(transactionUpdate: transactionUpdate)
      }
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
    })
  }

  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    guard let reader = ANPCardReaderController.shared().connectedReader else {
      return completion(nil)
    }
    completion(MobileReader.from(anyPayCardReader: reader))
  }

  /// Gets the connected MobileReader
  /// - Note: This is blocking, and will fail silently if no reader is found
  /// - Returns: The connected mobile reader, if any
  internal func connectedReader() -> MobileReader? {
    guard let reader = ANPCardReaderController.shared().connectedReader else {
      return nil
    }

    return MobileReader.from(anyPayCardReader: reader)
  }

  // MARK: AWC Events
  var onCardReaderConnected: ((AnyPayCardReader?) -> Void)? {
    willSet(newValue) {
      if let onCardReaderConnected = onCardReaderConnected {
        ANPCardReaderController.shared().unsubscribe(onCardReaderConnected: onCardReaderConnected)
      }

      if let newValue = newValue {
        ANPCardReaderController.shared().subscribe(onCardReaderConnected: newValue)
      }
    }
  }

  var onCardReaderDisconnected: (() -> Void)? {
    willSet(newValue) {
      if let onCardReaderDisconnected = onCardReaderDisconnected {
        ANPCardReaderController.shared().unsubscribe(onCardReaderDisConnected: onCardReaderDisconnected)
      }

      if let newValue = newValue {
        ANPCardReaderController.shared().subscribe(onCardReaderDisConnected: newValue)
      }
    }
  }

  var onCardReaderConnectionFailed: ((ANPMeaningfulError?) -> Void)? {
    willSet(newValue) {
      if let onCardReaderConnectionFailed = onCardReaderConnectionFailed {
        ANPCardReaderController.shared().unsubscribe(onCardReaderConnectionFailed: onCardReaderConnectionFailed)
      }

      if let newValue = newValue {
        ANPCardReaderController.shared().subscribe(onCardReaderConnectionFailed: newValue)
      }
    }
  }

  // MARK: CoreBluetooth Central Manager Delegate

  /// Does the necessary steps to start search of BT devices
  func startBtService() {
    btCentralManager = CBCentralManager(delegate: self, queue: nil)
  }

  /// Begins searching for AWC BT Devices
  ///
  /// The serial numbers of the BT devices will be inserted into `discoveredBluetoothDeviceSerailNumbers` as they
  /// are found.
  ///
  /// - Returns: True if started searching. False otherwise
  func startBtDeviceSearch() -> Bool {
    guard let centralManager = btCentralManager else {
      return false
    }
    discoveredBluetoothDeviceSerialNumbers = []
    centralManager.scanForPeripherals(withServices: nil, options: nil)
    return true
  }

  /// Stops searching for AWC BT Devices
  func stopBtDeviceSearch() {
    btCentralManager?.stopScan()
  }

  func centralManagerDidUpdateState(_ central: CBCentralManager) {
    switch central.state {
    case .unknown:
      print("central.state is .unknown")
    case .resetting:
      print("central.state is .resetting")
    case .unsupported:
      print("central.state is .unsupported")
    case .unauthorized:
      print("central.state is .unauthorized")
    case .poweredOff:
      print("central.state is .poweredOff")
    case .poweredOn:
      print("central.state is .poweredOn")
    }
  }

  func centralManager(_ central: CBCentralManager,
                      didDiscover peripheral: CBPeripheral,
                      advertisementData: [String : Any],
                      rssi RSSI: NSNumber) {
    guard let serial = peripheral.name, serial.contains("CHB") == true else { return }
    self.discoveredBluetoothDeviceSerialNumbers.insert(serial)
  }


}

#endif
