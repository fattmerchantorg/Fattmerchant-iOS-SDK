//
//  ChipDnaDriver.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 12/9/19.
//  Copyright © 2019 Fattmerchant. All rights reserved.
//

import Foundation

#if targetEnvironment(simulator)
  
#else
class ChipDnaDriver: NSObject, MobileReaderDriver {

  struct SelectablePinPad {
    var name: String
    var connectionType: String
  }

  /// The place where the transactions take place
  static var source: String = "NMI"

  /// Listens to the transaction events of ChipDna
  fileprivate var chipDnaTransactionListener = ChipDnaTransactionListener()

  /// A block to run after self deserializes a list of SelectablePinPads from the result of ChipDna availablePinPads
  fileprivate var didFindAvailablePinPads: (([SelectablePinPad]) -> Void)?

  /// A block to run after self receives the connect and configure callback from ChipDna
  fileprivate var didConnectAndConfigure: ((Bool) -> Void)?

  var familiarSerialNumbers: [String] = []

  /// Attempts to initialize the ChipDNA SDK
  ///
  /// - Parameters:
  ///   - args: a Dictionary containing the necessary things to initialize the sdk.
  ///   - completion: a block to run once initialization is complete. The block will receive the value 'true' if initialization was successful, false otherwise
  func initialize(args: [String: Any], completion: (Bool) -> Void) {
    guard
      let appId = args["appId"] as? String,
      let merchant = args["merchant"] as? Merchant,
      let apiKey = merchant.emvPassword(),
      !apiKey.isEmpty
      else {
        completion(false)
        return
    }

    // Initialize
    let parameters = CCParameters()
    parameters.setValue("password", forKey: CCParamPassword)
    parameters.setValue(CCValueTrue, forKey: CCParamAutoConfirm)
    if ChipDnaMobile.initialize(parameters)?[CCParamResult] != CCValueTrue {
      completion(false)
      return
    }

    // Set creds
    let properties = CCParameters()
    properties.setValue(apiKey, forKey: CCParamApiKey)
    properties.setValue(appId, forKey: CCParamApplicationIdentifier)
    properties.setValue(CCValueEnvironmentLive, forKey: CCParamEnvironment)
    let setPropertiesResult = ChipDnaMobile.sharedInstance()?.setProperties(properties)
    if setPropertiesResult?[CCParamResult] != CCValueTrue {
      completion(false)
      return
    }

    completion(true)
  }

  func isInitialized(completion: @escaping (Bool) -> Void) {
    completion(ChipDnaMobile.isInitialized())
  }

  /// Checks if ChipDna is ready to take a payment
  ///
  /// - Parameters:
  ///   - completion: a block to execute once this operation is complete. Receives `true` if ChipDna is ready to take payment, `false` otherwise
  func isReadyToTakePayment(completion: (Bool) -> Void) {
    // ChipDna must be initialized
    if !ChipDnaMobile.isInitialized() {
      completion(false)
    }

    // Ensure device is connected and terminal is enabled
    guard
      let status = ChipDnaMobile.sharedInstance()?.getStatus(nil),
      let deviceStatusXml = status[CCParamDeviceStatus],
      let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(deviceStatusXml),
      deviceStatus.deviceStatus == DeviceStatusEnum.connected,

      let terminalStatusXml = status[CCParamTerminalStatus],
      let terminalStatus = ChipDnaMobileSerializer.deserializeTerminalStatus(terminalStatusXml),
      terminalStatus.enabled

      else {
      completion(false)
      return
    }

    completion(true)
  }

  /// Searches for available mobile readers
  func searchForReaders(args: [String: Any], completion: @escaping ([MobileReader]) -> Void) {
    // Set the connection type to BT
    let params = CCParameters()
    params[CCParamPinPadConnectionType] = CCValueBluetooth

    ChipDnaMobile.removeAvailablePinPadsTarget(self)
    ChipDnaMobile.addAvailablePinPadsTarget(self, action: #selector(onAvailablePinPads(parameters:)))
    didFindAvailablePinPads = { pinPads in
      completion(pinPads.map({ MobileReader.from(pinPad: $0) }))
    }

    ChipDnaMobile.sharedInstance()?.getAvailablePinPads(nil)
  }

  /// Connects the mobile reader and prepares it for use
  ///
  /// - Parameters:
  ///   - reader: a MobileReader to connect
  ///   - completion: a block to run once the MobileReader is connected. If successfully connected, the block will receive true. Otherwise, it will receive false
  func connect(reader: MobileReader, completion: @escaping (Bool) -> Void) {
    let requestParams = CCParameters()
    requestParams[CCParamPinPadName] = reader.name
    requestParams[CCParamPinPadConnectionType] = CCValueBluetooth
    ChipDnaMobile.sharedInstance()?.setProperties(requestParams)
    ChipDnaMobile.addConnectAndConfigureFinishedTarget(self, action: #selector(onConnectAndConfigure(parameters:)))
    didConnectAndConfigure = { connected in
      if connected, let serial = reader.serialNumber {
        self.familiarSerialNumbers.append(serial)
      }
      completion(connected)
    }
    ChipDnaMobile.sharedInstance()?.connectAndConfigure(requestParams)
  }

  /// Gets the connected MobileReader
  /// - Parameters:
  ///   - completion: the connected MobileReader, if any
  ///   - error: a block to run if anything goes wrong during the operation
  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    // ChipDna must be initialized
    if !ChipDnaMobile.isInitialized() {
      error(OmniGeneralException.uninitialized)
    }

    guard
      let status = ChipDnaMobile.sharedInstance()?.getStatus(nil),
      let deviceStatusXml = status[CCParamDeviceStatus],
      let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(deviceStatusXml),
      deviceStatus.deviceStatus == DeviceStatusEnum.connected else {
      completion(nil)
      return
    }

    completion(MobileReader.from(deviceStatus: deviceStatus))
  }

  /// Attempts to disconnect a connected MobileReader
  /// - Parameters:
  ///   - reader: the MobileReader that is to be disconnected
  ///   - completion: a block to run once done. if disconnected, this receives true
  ///   - error: a block to run in anything goes wrong during the operation
  func disconnect(reader: MobileReader, completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    // ChipDna must be initialized
    if !ChipDnaMobile.isInitialized() {
      error(OmniGeneralException.uninitialized)
    }
    
    ChipDnaMobile.dispose(nil)
    completion(true)
  }

  func performTransaction(with request: TransactionRequest, completion: @escaping (TransactionResult) -> Void) {
    let requestParams = CCParameters(transactionRequest: request)

    chipDnaTransactionListener.detachFromChipDna()

    chipDnaTransactionListener.onFinished = { result in

      let success = result[CCParamTransactionResult] == CCValueApproved

      var transactionResult = TransactionResult()
      transactionResult.source = Self.source
      transactionResult.request = request
      transactionResult.success = success
      transactionResult.maskedPan = result[CCParamMaskedPan]
      transactionResult.cardHolderFirstName = result[CCParamCardHolderFirstName]
      transactionResult.cardHolderLastName = result[CCParamCardHolderLastName]
      transactionResult.authCode = result[CCParamAuthCode]
      transactionResult.cardType = result[CCParamCardSchemeId]?.lowercased()
      transactionResult.userReference = result[CCParamUserReference]
      transactionResult.localId = result[CCParamCardEaseReference]
      transactionResult.externalId = result[CCParamTransactionId]

      if let token = result[CCParamCustomerVaultId] {
        transactionResult.paymentToken = "nmi_\(token)"
      }

      completion(transactionResult)
      return
    }

    chipDnaTransactionListener.bindToChipDna()

    ChipDnaMobile.sharedInstance()?.startTransaction(requestParams)
  }

  /// Attempts a refund in ChipDna
  ///
  /// This will attempt to refund the full amount for the Transaction.
  /// - Parameters:
  ///   - transaction: the Transaction you intend to refund
  ///   - completion: A block to run after the refund is complete
  ///   - refundAmount: The amount to be refunded. If nil is passed, the remaining amount will be refunded
  ///   - error: A block to run in case an error occurs
  func refund(transaction: Transaction, refundAmount: Amount?, completion: @escaping (TransactionResult) -> Void, error: @escaping (OmniException) -> Void) {
    // Get card ease reference. This is what we use to reference the transaction within NMI
    guard let cardEaseReference = extractCardEaseReference(from: transaction) else {
      error(RefundException.transactionNotRefundable(details: "Could not find user reference"))
      return
    }

    // Get the amount to refund from the transaction
    guard let amountDollars = refundAmount?.dollars() ?? transaction.total else {
      error(RefundException.transactionNotRefundable(details: "Could not find amount to refund"))
      return
    }

    // Create the params for the 3rd-party refund
    let refundRequestParams = CCParameters()
    refundRequestParams[CCParamUserReference] = generateChipDnaTransactionUserReference()
    refundRequestParams[CCParamCardEaseReference] = cardEaseReference
    refundRequestParams[CCParamAmount] = Amount(dollars: amountDollars).centsString()
    refundRequestParams[CCParamCurrency] = "USD"

    // Do the 3rd party refund
    guard let result = ChipDnaMobile.sharedInstance()?.linkedRefundTransaction(refundRequestParams) else {
      error(RefundException.transactionNotRefundable(details: "Error while performing refund"))
      return
    }

    // Check status
    if result[CCParamError] != nil {
      error(RefundException.errorRefunding(details: "Error while performing refund"))
    } else {
      var transactionResult = TransactionResult()
      transactionResult.source = Self.source
      transactionResult.success = true
      transactionResult.transactionType = "refund"
      transactionResult.amount = refundAmount
      completion(transactionResult)
    }
  }

  fileprivate func refund2(transaction: Transaction, completion: @escaping (TransactionResult) -> Void, error: @escaping (OmniException) -> Void) {

    let request = TransactionRequest(amount: Amount(dollars: transaction.total!))

    let requestParams = CCParameters(transactionRequest: request)
    requestParams[CCParamTransactionType] = CCValueRefund
    requestParams[CCParamSaleReference] = extractUserReference(from: transaction)!

    chipDnaTransactionListener.detachFromChipDna()

    chipDnaTransactionListener.onFinished = { result in

      let success = result[CCParamTransactionResult] == CCValueApproved

      var transactionResult = TransactionResult()
      transactionResult.source = Self.source
      transactionResult.request = request
      transactionResult.success = success
      transactionResult.maskedPan = result[CCParamMaskedPan]
      transactionResult.cardHolderFirstName = result[CCParamCardHolderFirstName]
      transactionResult.cardHolderLastName = result[CCParamCardHolderLastName]
      transactionResult.authCode = result[CCParamAuthCode]
      transactionResult.cardType = result[CCParamCardSchemeId]?.lowercased()
      transactionResult.userReference = result[CCParamUserReference]
      transactionResult.transactionType = "refund"

      completion(transactionResult)
      return
    }

    chipDnaTransactionListener.bindToChipDna()

    ChipDnaMobile.sharedInstance()?.startTransaction(requestParams)
  }

  fileprivate func extractCardEaseReference(from transaction: Transaction) -> String? {
    return transaction.meta?["cardEaseReference"]
  }

  fileprivate func extractUserReference(from transaction: Transaction) -> String? {
    return transaction.meta?["nmiUserRef"]
  }

  fileprivate func deserializeAvailablePinPads(pinPadsXml: String) -> [SelectablePinPad]? {
    var availablePinPadsList: [SelectablePinPad]?

    guard
      let availablePinPadsDict = ChipDnaMobileSerializer.deserializeAvailablePinPadsString(pinPadsXml) as? [String: Any],
      let btPinPads = availablePinPadsDict[CCValueBluetooth] as? [String] else {
      return nil
    }

    availablePinPadsList = btPinPads.map { pinPadName in
      SelectablePinPad(name: pinPadName, connectionType: CCValueBluetooth)
    }

    return availablePinPadsList
  }

  // MARK: - ChipDna Listeners

  @objc public func onAvailablePinPads(parameters: CCParameters) {
    ChipDnaMobile.removeAvailablePinPadsTarget(self)

    // Make sure that there is a job for this to do after serialization
    guard let completion = didFindAvailablePinPads else {
      return
    }

    // Attempt deserialization
    guard
      let availablePinPadsXml = parameters[CCParamAvailablePinPads],
      let pinPads = deserializeAvailablePinPads(pinPadsXml: availablePinPadsXml)
    else {
      completion([])
      return
    }

    completion(pinPads)
  }

  @objc func onConnectAndConfigure(parameters: CCParameters) {
    ChipDnaMobile.removeConnectAndConfigureFinishedTarget(self)
    didConnectAndConfigure?(parameters[CCParamResult] == CCValueTrue)
  }

}

#endif
