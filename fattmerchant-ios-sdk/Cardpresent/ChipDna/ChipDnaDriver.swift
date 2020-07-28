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

  static var omniRefundsSupported: Bool = true

  enum PinPadManufacturer: String {
    case Miura, BBPOS
  }

  weak var mobileReaderConnectionStatusDelegate: MobileReaderConnectionStatusDelegate?

  /// Listens to the transaction events of ChipDna
  fileprivate var chipDnaTransactionListener = ChipDnaTransactionListener()

  /// A block to run after self deserializes a list of SelectablePinPads from the result of ChipDna availablePinPads
  fileprivate var didFindAvailablePinPads: (([SelectablePinPad]) -> Void)?

  /// A block to run after self receives the connect and configure callback from ChipDna
  fileprivate var didConnectAndConfigure: ((MobileReader?) -> Void)?

  var familiarSerialNumbers: [String] = []

  /// A key used to communicate with TransactionGateway
  fileprivate var securityKey: String = ""

  /// This is the data that we will need in order to initialize ChipDna again if something happens at runtime.
  ///
  /// For example, if the user wants to disconnect a reader, we have to use the ChipDnaMobile.dispose() method. This
  /// method uninitializes the SDK and we have to initialize again if we want to reconnect a reader. When we want to
  /// reconnect, we use these args
  fileprivate var initArgs: [String: Any] = [:]

  /// Attempts to initialize the ChipDNA SDK
  ///
  /// - Parameters:
  ///   - args: a Dictionary containing the necessary things to initialize the sdk.
  ///   - completion: a block to run once initialization is complete. The block will receive the value 'true' if initialization was successful, false otherwise
  func initialize(args: [String: Any], completion: (Bool) -> Void) {
    guard
      let appId = args["appId"] as? String,
      let nmiDetails = args["nmi"] as? NMIDetails,
      !apiKey.isEmpty
      else {
        completion(false)
        return
    }
    
    let apiKey = nmiDetails.security_key

    // Store the apiKey and the init args for later use
    securityKey = apiKey
    initArgs = args

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
  ///   - completion: a block to run once the MobileReader is connected. If successfully connected, the block will
  ///   receive the `MobileReader`. Otherwise, it will receive nil
  func connect(reader: MobileReader, completion: @escaping (MobileReader?) -> Void) {
    let requestParams = CCParameters()
    requestParams[CCParamPinPadName] = reader.name
    requestParams[CCParamPinPadConnectionType] = CCValueBluetooth
    ChipDnaMobile.sharedInstance()?.setProperties(requestParams)
    ChipDnaMobile.addConnectAndConfigureFinishedTarget(self, action: #selector(onConnectAndConfigure(parameters:)))
    didConnectAndConfigure = { connectedReader in
      if let connectedReader = connectedReader, let serial = connectedReader.serialNumber {
        self.familiarSerialNumbers.append(serial)
      }
      completion(connectedReader)
    }
    ChipDnaMobile.addConfigurationUpdateTarget(self, action: #selector(onConfigurationUpdate(parameters:)))
    ChipDnaMobile.addDeviceUpdateTarget(self, action: #selector(onDeviceUpdate(parameters:)))
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

    completion(ChipDnaDriver.connectedReader())
  }

  /// Gets the connected MobileReader
  /// - Note: This is blocking, and will fail silently if no reader is found
  /// - Returns: The connected mobile reader, if any
  internal static func connectedReader() -> MobileReader? {
    guard
      let status = ChipDnaMobile.sharedInstance()?.getStatus(nil),
      let deviceStatusXml = status[CCParamDeviceStatus],
      let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(deviceStatusXml),
      deviceStatus.deviceStatus == DeviceStatusEnum.connected else {
      return nil
    }

    return MobileReader.from(deviceStatus: deviceStatus)
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
    initialize(args: initArgs, completion: completion)
  }

  func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, completion: @escaping (TransactionResult) -> Void) {
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

      // Get more details about the transaction since ChipDna doesn't get everything
      TransactionGateway.getTransactionCcExpiration(securityKey: self.securityKey,
                                                    transactionId: result[CCParamTransactionId] ?? "") { ccExpiration in
        transactionResult.cardExpiration = ccExpiration
        completion(transactionResult)
      }
    }

    chipDnaTransactionListener.bindToChipDna(signatureProvider: signatureProvider, transactionUpdateDelegate: transactionUpdateDelegate)

    ChipDnaMobile.sharedInstance()?.startTransaction(requestParams)
  }

  func cancelCurrentTransaction(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    if let result = ChipDnaMobile.sharedInstance()?.terminateTransaction(nil) {
      if let success = result[CCParamResult], success == CCValueTrue {
        completion(true)
      } else {

        // Check if ChipDna is IDLE. If it is, there is no transaction to cancel
        if let status = ChipDnaMobile.sharedInstance()?.getStatus(nil), status[CCParamChipDnaStatus] == "IDLE" {
          error(CancelCurrentTransactionException.noTransactionToCancel)
        } else {
          error(CancelCurrentTransactionException.unknown)
        }
      }
    } else {
      fatalError()
    }
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

    // Make sure someone is listening to the callback
    guard let didConnectAndConfigure = didConnectAndConfigure else {
      return
    }

    // If no reader was connected, pass nil
    if (parameters[CCParamResult] != CCValueTrue) {
      didConnectAndConfigure(nil)
    }

    // Figure out the reader details and pass them along
    didConnectAndConfigure(ChipDnaDriver.connectedReader())
  }

  @objc func onConfigurationUpdate(parameters: CCParameters) {
    if
      let str = parameters[CCParamConfigurationUpdate],
      let status = MobileReaderConnectionStatus(chipDnaConfigurationUpdate: str) {
      mobileReaderConnectionStatusDelegate?.mobileReaderConnectionStatusUpdate(status: status)
    }
  }

  @objc func onDeviceUpdate(parameters: CCParameters) {
    if
      let deviceStatusXml = parameters[CCParamDeviceStatusUpdate],
      let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(deviceStatusXml),
      let status = MobileReaderConnectionStatus(chipDnaDeviceStatus: deviceStatus) {
      mobileReaderConnectionStatusDelegate?.mobileReaderConnectionStatusUpdate(status: status)
    }
  }

}

#endif
