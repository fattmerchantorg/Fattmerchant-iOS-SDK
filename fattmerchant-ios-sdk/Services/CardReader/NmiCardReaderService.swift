import Foundation

class NmiCardReaderService: NSObject, CardReaderService {
  public static let SOURCE = "NMI"
  
  public static var isStaxRefundSupported = true
  
  private var appId: String?
  private var securityKey: String?
  private var isInitSuccessful: Bool = false
  private var reader: CardReader? = nil
  
  private var onAvailablePinPadsContinuation: CheckedContinuation<[CardReader], Never>? = nil
  private var onConnectAndConfigureContinuation: CheckedContinuation<CardReader?, Never>? = nil
  private var onTransactionFinishedContinuation: CheckedContinuation<TransactionResult, Never>? = nil
  
  private weak var cardReaderConnectionStatusDelegate: CardReaderConnectionStatusDelegate?
  private weak var userNotificationDelegate: UserNotificationDelegate?
  private weak var transactionUpdateDelegate: TransactionUpdateDelegate?
  private var signatureProvider: SignatureProviding?
  
  fileprivate var nmiTransactionListener = NmiTransactionListener()
  fileprivate var currentTransactionRequest: TransactionRequest? = nil
  
  public func initialize(_ args: [String : Any]) throws -> Void {
    guard let nmiAppId = args["appId"] as? String else {
      ChipDnaMobile.dispose(nil)
      throw InitializeDriversException.noAppIdFound
    }
    
    guard let details = args["nmi"] as? NMIDetails else {
      ChipDnaMobile.dispose(nil)
      throw InitializeDriversException.incorrectMobileReaderSettings
    }
    
    if details.securityKey.isBlank() {
      ChipDnaMobile.dispose(nil)
      throw InitializeDriversException.incorrectMobileReaderSettings
    }
    
    securityKey = details.securityKey
    appId = nmiAppId

    // Initialize ChipDNA SDK
    let initializeParameters = CCParameters()
    initializeParameters.setValue("password", forKey: CCParamPassword)
    initializeParameters.setValue(CCValueTrue, forKey: CCParamAutoConfirm)
    
    let initializeResult = ChipDnaMobile.initialize(initializeParameters)
    if initializeResult?[CCParamResult] != CCValueTrue {
      ChipDnaMobile.dispose(nil)
      throw InitializeDriversException.incorrectMobileReaderSettings
    }

    // Set NMI gateway credentials
    let setPropertiesParameters = CCParameters()
    setPropertiesParameters.setValue(appId, forKey: CCParamApplicationIdentifier)
    setPropertiesParameters.setValue(securityKey, forKey: CCParamApiKey)
    setPropertiesParameters.setValue(CCValueEnvironmentLive, forKey: CCParamEnvironment)
    
    let setPropertiesResult = ChipDnaMobile.sharedInstance()?.setProperties(setPropertiesParameters)
    if setPropertiesResult?[CCParamResult] != CCValueTrue {
      ChipDnaMobile.dispose(nil)
      throw InitializeDriversException.incorrectMobileReaderSettings
    }
    
    // All good!
    isInitSuccessful = true
  }
  
  public func isInitialized() -> Bool {
    return ChipDnaMobile.isInitialized() && isInitSuccessful
  }
  
  public func searchForReaders(_ args: [String : Any]) async -> [CardReader] {
    let params = CCParameters()
    params[CCParamBLEScanTime] = "3"
    
    ChipDnaMobile.removeAvailablePinPadsTarget(self)
    ChipDnaMobile.addAvailablePinPadsTarget(self, action: #selector(onAvailablePinPads(parameters:)))
    return await withCheckedContinuation { continuation in
      onAvailablePinPadsContinuation = continuation
      ChipDnaMobile.sharedInstance()?.getAvailablePinPads(params)
    }
  }
  
  public func connectToReader(_ reader: CardReader) async -> CardReader? {
    let requestParams = CCParameters()
    requestParams[CCParamPinPadName] = reader.name
    requestParams[CCParamPinPadConnectionType] = reader.connection ?? CCValueBLE
    ChipDnaMobile.sharedInstance()?.setProperties(requestParams)
    ChipDnaMobile.addConnectAndConfigureFinishedTarget(self, action: #selector(onConnectAndConfigure(parameters:)))
    ChipDnaMobile.addConfigurationUpdateTarget(self, action: #selector(onConfigurationUpdate(parameters:)))
    ChipDnaMobile.addDeviceUpdateTarget(self, action: #selector(onDeviceUpdate(parameters:)))
    
    // Wrap in a .userInitiated Task for thread safety
    return await Task(priority: .utility){
      await withCheckedContinuation { continuation in
        onConnectAndConfigureContinuation = continuation
        ChipDnaMobile.sharedInstance()?.connectAndConfigure(nil)
      }
    }.value
  }
  
  public func getConnectedReader() async throws -> CardReader? {
    // Make sure the reader is connected, get the status, and ensure it
    guard
      let status = ChipDnaMobile.sharedInstance()?.getStatus(nil),
      let xml = status[CCParamDeviceStatus],
      let device = ChipDnaMobileSerializer.deserializeDeviceStatus(xml),
      device.deviceStatus == DeviceStatusEnum.connected
    else {
      return nil
    }

    var reader = CardReader()
    reader.name = device.name
    reader.make = device.make
    reader.model = device.model
    reader.serial = device.serialNumber
    reader.firmware = device.firmwareVersion
    
    return reader
  }
  
  public func disconnectFromReader() async throws -> Bool {
    // ChipDna must be initialized
    if !ChipDnaMobile.isInitialized() {
      throw StaxGeneralException.uninitialized
    }
    
    // And we should still have the security key
    guard let key = securityKey else {
      throw StaxGeneralException.uninitialized
    }

    // Re-initialize the ChipDNA SDK and return the result
    ChipDnaMobile.dispose(nil)
    try initialize([
      "appId": appId as Any,
      "nmi": NMIDetails(securityKey: key)
    ])
    
    return self.isInitialized()
  }
  
  public func performTransaction(_ request: TransactionRequest) async -> TransactionResult {
    let startTransactionParams = CCParameters.fromTransactionRequest(request)

    currentTransactionRequest = request

    nmiTransactionListener.removeAllListeners()
    nmiTransactionListener.onFinished = self.onTransactionFinished
    nmiTransactionListener.bindToNmiSdk(
      signatureProvider: self.signatureProvider,
      transactionUpdateDelegate: self.transactionUpdateDelegate,
      userNotificationDelegate: self.userNotificationDelegate
    )

    return await withCheckedContinuation { continuation in
      onTransactionFinishedContinuation = continuation
      ChipDnaMobile.sharedInstance()?.startTransaction(startTransactionParams)
    }
  }
  
  func captureTransaction(_ transaction: Transaction) async -> Bool {
    guard let meta = transaction.meta, let user: String = meta["nmiUserRef"] else {
      return false
    }

    let captureParams = CCParameters()
    captureParams[CCParamUserReference] = user
    
    return await withCheckedContinuation { continuation in
      guard let result = ChipDnaMobile.sharedInstance()?.confirmTransaction(captureParams) else {
        return continuation.resume(returning: false)
      }
      let isSuccess = result[CCParamTransactionResult] == CCValueApproved
      continuation.resume(returning: isSuccess)
    }
  }
  
  public func cancelActiveTransaction() async throws -> Bool {
    guard let result = ChipDnaMobile.sharedInstance()?.terminateTransaction(nil) else {
      return false
    }
    
    if let isSuccess = result[CCParamResult], isSuccess == CCValueTrue {
      return true
    }
    
    guard let status = ChipDnaMobile.sharedInstance()?.getStatus(nil) else {
      throw CancelCurrentTransactionException.noTransactionToCancel
    }

    if status[CCParamChipDnaStatus] == "IDLE" {
      throw CancelCurrentTransactionException.noTransactionToCancel
    } else {
      throw CancelCurrentTransactionException.unknown
    }
  }
  
  public func refundTransaction(_ transaction: Transaction, _ amount: Amount? = nil) throws -> TransactionResult {
    // Get card ease reference. This is what we use to reference the transaction within NMI
    guard let cardEaseReference: String = transaction.meta?["cardEaseReference"] else {
      throw RefundException.transactionNotRefundable(details: "Could not find user reference")
    }

    // Get the amount to refund from the transaction
    guard let amountDollars = amount?.dollars() ?? transaction.total else {
      throw RefundException.transactionNotRefundable(details: "Could not find amount to refund")
    }

    // Create the params for the 3rd-party refund
    let refundRequestParams = CCParameters()
    refundRequestParams[CCParamUserReference] = generateNmiTransactionUserReference()
    refundRequestParams[CCParamCardEaseReference] = cardEaseReference
    refundRequestParams[CCParamAmount] = Amount(dollars: amountDollars).centsString()
    refundRequestParams[CCParamCurrency] = "USD"

    // Do the 3rd party refund
    guard let refundStatus = ChipDnaMobile.sharedInstance()?.linkedRefundTransaction(refundRequestParams) else {
      throw RefundException.transactionNotRefundable(details: "Error while performing refund")
    }

    // Check if errors happened during
    if let _ = refundStatus[CCParamErrors] {
      throw RefundException.errorRefunding(details: "Error while performing refund")
    }
    
    let receipt = ChipDnaMobileSerializer.deserializeReceiptData(refundStatus[CCParamReceiptData])
    var result = TransactionResult()
    result.source = NmiCardReaderService.SOURCE
    result.success = true
    result.transactionType = "refund"
    result.amount = amount
    result.transactionSource = receipt?["TRANSACTION_SOURCE"]?.value
    return result
  }
  
  public func voidTransaction(_ result: TransactionResult) async -> Bool {
    guard let user = result.userReference else { return false }

    let voidParams = CCParameters()
    voidParams[CCParamUserReference] = user
    
    return await withCheckedContinuation { continuation in
      guard let result = ChipDnaMobile.sharedInstance()?.voidTransaction(voidParams) else {
        return continuation.resume(returning: false)
      }
      let isSuccess = result[CCParamTransactionResult] == CCValueApproved
      continuation.resume(returning: isSuccess)
    }
  }
  
  public func getCardReaderConnectionStatusDelegate() -> (any CardReaderConnectionStatusDelegate)? {
    return cardReaderConnectionStatusDelegate
  }

  public func setCardReaderConnectionStatusDelegate(_ delegate: CardReaderConnectionStatusDelegate?) -> Void {
    cardReaderConnectionStatusDelegate = delegate
  }
  
  public func getUserNotificationDelegate() -> (any UserNotificationDelegate)? {
    return userNotificationDelegate
  }
  
  public func setUserNotificationDelegate(_ delegate: (any UserNotificationDelegate)?) {
    userNotificationDelegate = delegate
  }
  
  public func getTransactionUpdateDelegate() -> (any TransactionUpdateDelegate)? {
    return transactionUpdateDelegate
  }
  
  public func setTransactionUpdateDelegate(_ delegate: (any TransactionUpdateDelegate)?) {
    transactionUpdateDelegate = delegate
  }
  
  public func getSignatureProvider() -> SignatureProviding? {
    return signatureProvider
  }
  
  public func setSignatureProvider(_ provider: SignatureProviding?) -> Void {
    signatureProvider = provider
  }
  
  @objc private func onAvailablePinPads(parameters: CCParameters) {
    ChipDnaMobile.removeAvailablePinPadsTarget(self)

    // No readers found, scrap what we've got and call the continuation
    guard let availablePinPadsXml = parameters[CCParamAvailablePinPads] else {
      onAvailablePinPadsContinuation?.resume(returning: [])
      onAvailablePinPadsContinuation = nil
      return
    }
    
    let output = deserializeAvailablePinPads(availablePinPadsXml)
    
    // Resume the async call from `searchForReaders`, remove the continuation, and return the readers
    onAvailablePinPadsContinuation?.resume(returning: output)
    onAvailablePinPadsContinuation = nil
  }
  
  @objc private func onConnectAndConfigure(parameters: CCParameters) {
    ChipDnaMobile.removeConnectAndConfigureFinishedTarget(self)

    // If no reader was connected, pass nil via the continuation
    if parameters[CCParamResult] != CCValueTrue {
      onConnectAndConfigureContinuation?.resume(returning: nil)
      onConnectAndConfigureContinuation = nil
      return
    }
    
    // It's not connected, so do the same as above
    guard
      let status = ChipDnaMobile.sharedInstance()?.getStatus(nil),
      let xml = status[CCParamDeviceStatus],
      let device = ChipDnaMobileSerializer.deserializeDeviceStatus(xml),
      device.deviceStatus == DeviceStatusEnum.connected 
    else {
      onConnectAndConfigureContinuation?.resume(returning: nil)
      onConnectAndConfigureContinuation = nil
      return
    }
    
    var reader = CardReader()
    reader.name = device.name
    reader.make = device.make
    reader.model = device.model
    reader.serial = device.serialNumber
    reader.firmware = device.firmwareVersion
    
    // Emit delegate event
    cardReaderConnectionStatusDelegate?.cardReaderConnectionStatusUpdate(status: .connected)
    
    // Save connected reader to service for quick access
    self.reader = reader
    
    // Return via continuation
    onConnectAndConfigureContinuation?.resume(returning: reader)
    onConnectAndConfigureContinuation = nil
  }
  
  @objc private func onTransactionFinished(parameters: CCParameters) {
    
    let isSuccess = parameters[CCParamTransactionResult] == CCValueApproved
    let receipt = ChipDnaMobileSerializer.deserializeReceiptData(parameters[CCParamReceiptData])

    Task {
      var result = TransactionResult()
      result.source = NmiCardReaderService.SOURCE
      result.request = currentTransactionRequest
      result.success = isSuccess
      result.maskedPan = parameters[CCParamMaskedPan]
      result.cardHolderFirstName = parameters[CCParamCardHolderFirstName]
      result.cardHolderLastName = parameters[CCParamCardHolderLastName]
      result.authCode = parameters[CCParamAuthCode]
      result.cardType = parameters[CCParamCardSchemeId]?.lowercased()
      result.userReference = parameters[CCParamUserReference]
      result.localId = parameters[CCParamCardEaseReference]
      result.externalId = parameters[CCParamTransactionId]
      result.transactionSource = receipt?[kCCReceiptFieldTransactionSource]?.value

      currentTransactionRequest = nil
      
      if let token = parameters[CCParamCustomerVaultId] {
        result.paymentToken = "nmi_\(token)"
      }
      
      let nmiTransactionId = parameters[CCParamTransactionId] ?? ""
      let expiry = try await self.getTransactionCardExpiry(id: nmiTransactionId)
      result.cardExpiration = expiry
      
      // Return via continuation
      onTransactionFinishedContinuation?.resume(returning: result)
      onTransactionFinishedContinuation = nil
    }
  }
  
  @objc private func onConfigurationUpdate(parameters: CCParameters) {
    let config = parameters[CCParamConfigurationUpdate] ?? ""
    if let status = CardReaderConnectionStatus(config) {
      cardReaderConnectionStatusDelegate?.cardReaderConnectionStatusUpdate(status: status)
    }
  }

  @objc private func onDeviceUpdate(parameters: CCParameters) {
    let xml = parameters[CCParamDeviceStatusUpdate]
    let deserialized = ChipDnaMobileSerializer.deserializeDeviceStatus(xml)
    if let status = CardReaderConnectionStatus(deserialized) {
      cardReaderConnectionStatusDelegate?.cardReaderConnectionStatusUpdate(status: status)
    }
  }

  private func createNmiParamsFromTransactionRequest(_ request: TransactionRequest) -> CCParameters {
    let output = CCParameters()
    output[CCParamAmount] = request.amount.centsString()
    output[CCParamAmountType] = CCValueAmountTypeActual
    output[CCParamCurrency] = "USD"
    output[CCParamUserReference] = generateNmiTransactionUserReference()
    output[CCParamPaymentMethod] = CCValueCard
    output[CCParamTransactionType] = CCValueSale
    if request.tokenize {
      output[CCParamCustomerVaultCommand] = "add-customer"
    }
    return output
  }
  
  private func generateNmiTransactionUserReference() -> String {
    let formatter = DateFormatter()
    formatter.locale = Locale(identifier: "en_US")
    formatter.setLocalizedDateFormatFromTemplate("yy-MM-dd-HH.mm.ss")
    return "CDM-\(formatter.string(from: Date()))"
  }
  
  /// Communicates with Transaction Gateway via the Direct Post API
  /// https://fattmerchant.transactiongateway.com/merchants/resources/integration/integration_portal.php
  /// Fetches the CCExpiration for a transaction from TransactionGateway
  /// - Parameters:
  ///   - id: the id of the Transaction
  private func getTransactionCardExpiry(id: String) async throws -> String? {
    let base = "https://secure.nmi.com/api/query.php"
    let path = "?security_key=\(securityKey ?? "")&transaction_id=\(id)"
    guard let url = URL(string: "\(base)\(path)") else {
      throw URLError(.badURL)
    }
    
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    
    let session = URLSession(configuration: URLSessionConfiguration.default)
    return await withCheckedContinuation { continuation in
      let task = session.dataTask(with: request) { (data, _, error) in
        guard error == nil, let data = data else {
          continuation.resume(returning: nil)
          return
        }
        
        let parser = NmiXmlParser()
        parser.parseExpirationDate(from: data, transactionId: id) { str in
          continuation.resume(returning: str)
        }
      }
      task.resume()
    }
  }
  
  private func deserializeAvailablePinPads(_ xml: String) -> [CardReader] {
    guard let dictionary = ChipDnaMobileSerializer.deserializeAvailablePinPadsString(xml) as? [String:Any] else {
      return []
    }

    let bluetoothLowEnergyPinPads: [CardReader] = (dictionary[CCValueBLE] as? [String])?.map { pad in
      var reader = CardReader()
      reader.name = pad
      reader.connection = CCValueBLE
      return reader
    } ?? []
    
    let bluetoothPinPads: [CardReader] = (dictionary[CCValueBluetooth] as? [String])?.map { pad in
      var reader = CardReader()
      reader.name = pad
      reader.connection = CCValueBluetooth
      return reader
    } ?? []
    
    let usbPinPads: [CardReader] = (dictionary[CCValueLightningUsb] as? [String])?.map { pad in
      var reader = CardReader()
      reader.name = pad
      reader.connection = CCValueLightningUsb
      return reader
    } ?? []
    
    return bluetoothLowEnergyPinPads + bluetoothPinPads + usbPinPads
  }
}
