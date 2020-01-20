//
//  ChipDnaDriver.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 12/9/19.
//  Copyright © 2019 Fattmerchant. All rights reserved.
//

import Foundation

class ChipDnaDriver: NSObject, MobileReaderDriver {

  struct SelectablePinPad {
    var name: String
    var connectionType: String
  }

  /// Listens to the transaction events of ChipDna
  fileprivate var chipDnaTransactionListener = ChipDnaTransactionListener()

  /// A block to run after self deserializes a list of SelectablePinPads from the result of ChipDna availablePinPads
  fileprivate var didFindAvailablePinPads: (([SelectablePinPad]) -> Void)?

  /// A block to run after self receives the connect and configure callback from ChipDna
  fileprivate var didConnectAndConfigure: ((Bool) -> Void)?

  /// Attempts to initialize the ChipDNA SDK
  ///
  /// - Parameters:
  ///   - args: a Dictionary containing the necessary things to initialize the sdk.
  ///   - completion: a block to run once initialization is complete. The block will receive the value 'true' if initialization was successful, false otherwise
  func initialize(args: [String: Any], completion: (Bool) -> Void) {
    print("inside init chipdnadriver")
    guard
      let appId = args["appId"] as? String,
      let merchant = args["merchant"] as? Merchant,
      let apiKey = merchant.emvPassword()
      else {
        print("couldn' get past guard in initialize")
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

    ChipDnaMobile.sharedInstance()?.getAvailablePinPads(nil) //TODO: Listen to the result and handle the error
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
    didConnectAndConfigure = completion //TODO: Make this weak?
    ChipDnaMobile.sharedInstance()?.connectAndConfigure(requestParams)
  }

  func performTransaction(with request: TransactionRequest, completion: @escaping (TransactionResult) -> Void) {
    let requestParams = CCParameters(transactionRequest: request)

    chipDnaTransactionListener.detachFromChipDna()

    chipDnaTransactionListener.onFinished = { result in
      // Check for errors
      //TODO: Implement this
//      if let error = result[CCParamError] else {
//
//      }

      let success = result[CCParamTransactionResult] == CCValueDeclined

      var transactionResult = TransactionResult()
      transactionResult.request = request
      transactionResult.success = success
      transactionResult.maskedPan = result[CCParamMaskedPan]
      transactionResult.cardHolderFirstName = result[CCParamCardHolderFirstName]
      transactionResult.cardHolderLastName = result[CCParamCardHolderLastName]
      transactionResult.authCode = result[CCParamAuthCode]
      transactionResult.cardType = result[CCParamCardSchemeId]?.lowercased()
      transactionResult.userReference = result[CCParamTransactionResult]

      completion(transactionResult)
      return
    }

    chipDnaTransactionListener.bindToChipDna()
  }

  func refund(transaction: Transaction, completion: (TransactionResult) -> Void) {
    //TODO: Implement this
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
