#if targetEnvironment(simulator)
#else

import Foundation

class ChipDnaDriver: NSObject, MobileReaderDriver {
  static var isStaxRefundsSupported: Bool = true
  static var source: String = "NMI"
  
  /// The ChipDna init params passed in the `initialize` function.
  private static var initializationArgs: ChipDnaInitializationArgs?

  var familiarSerialNumbers: [String] = []

  weak var mobileReaderConnectionStatusDelegate: MobileReaderConnectionStatusDelegate?

  /// Listens to the transaction events of ChipDna
  fileprivate var chipDnaTransactionListener = ChipDnaTransactionListener()

  /// A block to run after self deserializes a list of SelectablePinPads from the result of ChipDna availablePinPads
  fileprivate var onAvailablePinPadsCallback: (([SelectablePinPad]) -> Void)?

  /// Runs after `connectAndConfigure` finishes.
  fileprivate var onConnectAndConfigureCallback: ((MobileReader?) -> Void)?
  
  /// Gets the connected MobileReader
  /// - Note: This is blocking, and will fail silently if no reader is found
  /// - Returns: The connected mobile reader, if any
  internal static func getConnectedReader() -> MobileReader? {
    guard
      let status = ChipDnaMobile.sharedInstance()?.getStatus(nil),
      let deviceStatusXml = status[CCParamDeviceStatus],
      let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(deviceStatusXml),
      deviceStatus.deviceStatus == DeviceStatusEnum.connected else {
      return nil
    }

    return MobileReader.from(status: deviceStatus)
  }
  
  /// Attempts to initialize the ChipDNA SDK
  /// - Parameter args:A `ChipDnaInitializationArgs` object that contains the required security keys.
  /// - Parameter completion: A `(Bool) -> Void` callback containing whether the initialization was successful.
  func initialize(args: MobileReaderDriverInitializationArgs, completion: @escaping (Bool) -> Void) {
    guard let args = args as? ChipDnaInitializationArgs, !args.keys.securityKey.isEmpty else {
      ChipDnaMobile.dispose(nil)
      completion(false)
      return
    }
    
    ChipDnaDriver.initializationArgs = args

    // Initialize the ChipDna SDK
    let parameters = CCParameters()
    parameters.setValue("password", forKey: CCParamPassword)
    parameters.setValue(CCValueTrue, forKey: CCParamAutoConfirm)
    if ChipDnaMobile.initialize(parameters)?[CCParamResult] != CCValueTrue {
      ChipDnaMobile.dispose(nil)
      completion(false)
      return
    }

    // Set ChipDna for production with merchant's API key
    let properties = CCParameters()
    properties.setValue(args.keys.securityKey, forKey: CCParamApiKey)
    properties.setValue(args.appId, forKey: CCParamApplicationIdentifier)
    properties.setValue(CCValueEnvironmentLive, forKey: CCParamEnvironment)
    let setPropertiesResult = ChipDnaMobile.sharedInstance()?.setProperties(properties)
    if setPropertiesResult?[CCParamResult] != CCValueTrue {
      ChipDnaMobile.dispose(nil)
      completion(false)
      return
    }

    // Initialized
    completion(true)
  }

  /// Returns `true` if the ChipDna SDK is initialized.
  /// - Parameter completion: A `(Bool) -> Void` callback containing whether the ChipDna SDK is initialized.
  func isInitialized(completion: @escaping (Bool) -> Void) {
    completion(ChipDnaMobile.isInitialized())
  }

  /// Returns `true` if ChipDna is ready to take a payment
  /// - Parameter completion: a `(Bool) -> Void` callback that returns `true` if ChipDna is ready to take payment
  func isReadyToTakePayment(completion: (Bool) -> Void) {
    // If ChipDna is not initialized, return `false`
    if !ChipDnaMobile.isInitialized() {
      completion(false)
      return
    }

    if ChipDnaMobile.isDeviceConnected && ChipDnaMobile.isTerminalStatusEnabled {
      completion(true)
    } else {
      completion(false)
    }
  }

  /// Searches for available `MobileReader` devices to connect to.
  /// - Parameter args: A `ChipDnaSearchArgs` object that contains the allowed search types.
  /// - Parameter completion: A `[MobileReader] -> Void` callback that contains the found `MobileReader` objects.
  func searchForReaders(args: MobileReaderDriverSearchArgs?, completion: @escaping ([MobileReader]) -> Void) {
    // TODO: Allow scans for only USB, BLE, or BT based on args
    
    // Scan everything for 5 seconds
    let params = CCParameters()
    params[CCParamBLEScanTime] = "5"

    // Set the callback to contain this function's completion parameter
    onAvailablePinPadsCallback = { availablePinPads in
      let readers = availablePinPads.map({ MobileReader.from(pinPad: $0) })
      completion(readers)
    }
    
    // Remove extraneous listeners, and search for available pin pads.
    ChipDnaMobile.removeAvailablePinPadsTarget(self)
    ChipDnaMobile.addAvailablePinPadsTarget(self, action: #selector(onAvailablePinPads(parameters:)))
    ChipDnaMobile.sharedInstance()?.getAvailablePinPads(params)
  }

  /// Connects to a specific `MobileReader` devices to connect to.
  /// - Parameter args: A `ChipDnaSearchArgs` object that contains the allowed search types.
  /// - Parameter completion: A `[MobileReader] -> Void` callback that contains the found `MobileReader` objects.
  func connect(reader: MobileReader, completion: @escaping (MobileReader?) -> Void) {
    let requestParams = CCParameters()
    requestParams[CCParamPinPadName] = reader.name
    requestParams[CCParamPinPadConnectionType] = reader.connectionType ?? CCValueBLE
    
    onConnectAndConfigureCallback = { connectedReader in
      let _ = ChipDnaMobile.sharedInstance().getStatus(nil)
      if let connectedReader = connectedReader, let serial = connectedReader.serialNumber {
        self.familiarSerialNumbers.append(serial)
      }
      completion(connectedReader)
    }
    
    ChipDnaMobile.sharedInstance()?.setProperties(requestParams)
    ChipDnaMobile.addConnectAndConfigureFinishedTarget(self, action: #selector(onConnectAndConfigure(parameters:)))
    ChipDnaMobile.addConfigurationUpdateTarget(self, action: #selector(onConfigurationUpdate(parameters:)))
    ChipDnaMobile.addDeviceUpdateTarget(self, action: #selector(onDeviceUpdate(parameters:)))
    ChipDnaMobile.sharedInstance()?.connectAndConfigure(nil)
  }

  /// Gets the connected MobileReader
  /// - Parameter completion: A `(MobileReader?) -> Void` callback that contains the `MobileReader` if connected; `nil` if not.
  /// - Parameter error: A `(OmniException) -> Void` callback that contains an `OmniException` if one occurred.
  func getConnectedReader(completion: (MobileReader?) -> Void, error: @escaping (OmniException) -> Void) {
    if !ChipDnaMobile.isInitialized() {
      error(OmniGeneralException.uninitialized)
    }

    completion(ChipDnaDriver.getConnectedReader())
  }

  /// Attempts to disconnect the actively connected `MobileReader`
  /// - Parameter completion: A `(Bool) -> Void` callback containing the result of the disconnection.
  /// - Parameter error: A `(OmniException) -> Void` callback containing an `OmniException` if one occured.
  func disconnect(completion: @escaping (Bool) -> Void, error: @escaping (OmniException) -> Void) {
    if !ChipDnaMobile.isInitialized() {
      error(OmniGeneralException.uninitialized)
    }

    // Re-initializing the ChipDnaMobile SDK disconnects everything, so that works.
    ChipDnaMobile.dispose(nil)
    initialize(args: ChipDnaDriver.initializationArgs!, completion: completion)
  }

  func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, completion: @escaping (TransactionResult) -> Void) {
    performTransaction(with: request,
                       signatureProvider: signatureProvider,
                       transactionUpdateDelegate: transactionUpdateDelegate,
                       userNotificationDelegate: nil,
                       completion: completion)
  }

  func performTransaction(with request: TransactionRequest,
                          signatureProvider: SignatureProviding?,
                          transactionUpdateDelegate: TransactionUpdateDelegate?,
                          userNotificationDelegate: UserNotificationDelegate?,
                          completion: @escaping (TransactionResult) -> Void) {
    let requestParams = CCParameters(transactionRequest: request)

    chipDnaTransactionListener.detachFromChipDna()

    chipDnaTransactionListener.onFinished = { result in

      let success = result[CCParamTransactionResult] == CCValueApproved
      let receiptData = ChipDnaMobileSerializer.deserializeReceiptData(result[CCParamReceiptData])

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
      transactionResult.transactionSource = receiptData?[kCCReceiptFieldTransactionSource]?.value

      if let token = result[CCParamCustomerVaultId] {
        transactionResult.paymentToken = "nmi_\(token)"
      }

      // Get more details about the transaction since ChipDna doesn't get everything
      TransactionGateway.getTransactionCcExpiration(
        securityKey: Self.initializationArgs!.keys.securityKey,
        transactionId: result[CCParamTransactionId] ?? ""
      ) { ccExpiration in
        transactionResult.cardExpiration = ccExpiration
        completion(transactionResult)
      }
    }

    chipDnaTransactionListener.bindToChipDna(signatureProvider: signatureProvider,
                                             transactionUpdateDelegate: transactionUpdateDelegate,
                                             userNotificationDelegate: userNotificationDelegate)

    ChipDnaMobile.sharedInstance()?.startTransaction(requestParams)
  }

  func capture(transaction: Transaction, completion: @escaping (Bool) -> Void) {
    guard let transactionMeta = transaction.meta, let userRef: String = transactionMeta["nmiUserRef"] else {
      completion(false)
      return
    }

    let params = CCParameters()
    params[CCParamUserReference] = userRef
    DispatchQueue.global(qos: .userInitiated).async {
      guard let result = ChipDnaMobile.sharedInstance()?.confirmTransaction(params) else {
        return completion(false)
      }

      completion(result[CCParamTransactionResult] == CCValueApproved)
    }
  }
  
  func capture(_ transaction: StaxTransaction, completion: @escaping (Bool) -> Void) {
    guard let meta = transaction.meta, let user: String = meta.string(at: "nmiUserRef") else {
      completion(false)
      return
    }

    let params = CCParameters()
    params[CCParamUserReference] = user
    DispatchQueue.global(qos: .userInitiated).async {
      guard let result = ChipDnaMobile.sharedInstance()?.confirmTransaction(params) else {
        return completion(false)
      }

      completion(result[CCParamTransactionResult] == CCValueApproved)
    }
  }

  func void(transactionResult: TransactionResult, completion: @escaping (Bool) -> Void) {
    guard let userRef = transactionResult.userReference else {
      completion(false)
      return
    }

    let params = CCParameters()
    params[CCParamUserReference] = userRef
    DispatchQueue.global(qos: .userInitiated).async {
      guard let result = ChipDnaMobile.sharedInstance()?.voidTransaction(params) else {
        return completion(false)
      }

      completion(result[CCParamTransactionResult] == CCValueApproved)
    }
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
    guard let cardEaseReference = transaction.cardEaseReference else {
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

    let receiptData = ChipDnaMobileSerializer.deserializeReceiptData(result[CCParamReceiptData])

    // Check status
    if result[CCParamErrors] != nil {
      error(RefundException.errorRefunding(details: "Error while performing refund"))
    } else {
      var transactionResult = TransactionResult()
      transactionResult.source = Self.source
      transactionResult.success = true
      transactionResult.transactionType = "refund"
      transactionResult.amount = refundAmount
      transactionResult.transactionSource = receiptData?["TRANSACTION_SOURCE"]?.value
      completion(transactionResult)
    }
  }

  fileprivate func deserializeAvailablePinPads(pinPadsXml: String) -> [SelectablePinPad]? {
    var availablePinPadsList: [SelectablePinPad]

    guard
      let availablePinPadsDict = ChipDnaMobileSerializer.deserializeAvailablePinPadsString(pinPadsXml) as? [String: Any] else {
      return nil
    }

    // Create empty list of pinpads
    availablePinPadsList = []

    // Add Bluetooth Low Energy devices
    let bleDevices = (availablePinPadsDict[CCValueBLE] as? [String])?.map { pinPadName in
      SelectablePinPad(name: pinPadName, connectionType: CCValueBLE)
    }
    availablePinPadsList.append(contentsOf: bleDevices ?? [])

    // Add Bluetooth devices
    let btDevices = (availablePinPadsDict[CCValueBluetooth] as? [String])?.map { pinPadName in
      SelectablePinPad(name: pinPadName, connectionType: CCValueBluetooth)
    }
    availablePinPadsList.append(contentsOf: btDevices ?? [])
    
    // USB
    let usbDevices = (availablePinPadsDict[CCValueLightningUsb] as? [String])?.map { pinPadName in
      SelectablePinPad(name: pinPadName, connectionType: CCValueLightningUsb)
    }
    availablePinPadsList.append(contentsOf: usbDevices ?? [])

    // Known PinPad Filter
    var known: [SelectablePinPad] = []
    for pad in availablePinPadsList {
      if isKnownPinPad(pad.name) {
        known.append(pad)
      }
    }

    return known
  }
  
  private func isKnownPinPad(_ pad: String) -> Bool {
    let known = ["IDTECH", "CHB"]
    for pre in known {
      if pad.uppercased().hasPrefix(pre) {
        return true
      }
    }
    return false
  }

  // MARK: - ChipDna Listeners

  @objc public func onAvailablePinPads(parameters: CCParameters) {
    ChipDnaMobile.removeAvailablePinPadsTarget(self)

    guard let onAvailablePinPadsCallback = onAvailablePinPadsCallback else { return }

    // Attempt deserialization
    guard
      let availablePinPadsXml = parameters[CCParamAvailablePinPads],
      let pinPads = deserializeAvailablePinPads(pinPadsXml: availablePinPadsXml)
    else {
      onAvailablePinPadsCallback([])
      return
    }

    onAvailablePinPadsCallback(pinPads)
  }

  @objc func onConnectAndConfigure(parameters: CCParameters) {
    ChipDnaMobile.removeConnectAndConfigureFinishedTarget(self)

    guard let onConnectAndConfigureCallback = onConnectAndConfigureCallback else { return }
    if parameters[CCParamResult] != CCValueTrue {
      onConnectAndConfigureCallback(nil)
    }

    // Figure out the reader details and pass them along
    onConnectAndConfigureCallback(ChipDnaDriver.getConnectedReader())
  }

  @objc func onConfigurationUpdate(parameters: CCParameters) {
    if let str = parameters[CCParamConfigurationUpdate], let status = MobileReaderConnectionStatus(chipDnaConfigurationUpdate: str) {
      mobileReaderConnectionStatusDelegate?.mobileReaderConnectionStatusUpdate(status: status)
    }
  }

  @objc func onDeviceUpdate(parameters: CCParameters) {
    if
      let deviceStatusXml = parameters[CCParamDeviceStatusUpdate],
      let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(deviceStatusXml),
      let status = MobileReaderConnectionStatus(chipDnaDeviceStatus: deviceStatus)
    {
      mobileReaderConnectionStatusDelegate?.mobileReaderConnectionStatusUpdate(status: status)
    }
  }

}

#endif
