#if !targetEnvironment(simulator)
import Foundation

class ChipDnaDriver: NSObject, MobileReaderDriver, TapDriver {
    static var isStaxRefundsSupported: Bool = true
    static var source: String = "NMI"

    /// The ChipDna init params passed in the `initialize` function.
    private static var initializationArgs: ChipDnaInitializationArgs?

    /// Set by the auto-dispose branch in `initialize` to signal that the next
    /// `connectToTap` is running on a merchant-switched session and must
    /// force a full TMS re-download. Consumed + cleared by `connectToTap`.
    /// Stays `false` on first init and on same-merchant disconnect→reinit.
    ///
    /// TMS = Terminal Management System: NMI's server-side provisioning
    /// service that pushes per-merchant configuration (EMV kernels, limits,
    /// TapToMobilePOIIdentifier, merchant display name) down into
    /// ChipDnaMobile's local encrypted database. The cache survives
    /// `dispose(nil)`, which is why we need to force a refresh on
    /// merchant switch.
    fileprivate static var tmsRefreshPending: Bool = false

    var familiarSerialNumbers: [String] = []

    weak var tapConnectionStatusDelegate: (any TapConnectionStatusDelegate)?
    weak var mobileReaderConnectionStatusDelegate:
        MobileReaderConnectionStatusDelegate?

    /// A block to run after self deserializes a list of SelectablePinPads from the result of ChipDna availablePinPads
    fileprivate var onAvailablePinPadsCallback: (([SelectablePinPad]) -> Void)?

    /// Runs after `connectAndConfigure` finishes.
    fileprivate var onConnectAndConfigureCallback: ((MobileReader?) -> Void)?

    /// Runs after Tap `connectAndConfigure` finishes. Propagates success/failure.
    fileprivate var onTapConnectAndConfigureCallback: ((Bool, OmniException?) -> Void)?

    /// Gets the connected MobileReader
    /// - Note: This is blocking, and will fail silently if no reader is found
    /// - Returns: The connected mobile reader, if any
    internal static func getConnectedReader() -> MobileReader? {
        guard
            let status = ChipDnaMobile.sharedInstance()?.getStatus(nil),
            let deviceStatusXml = status[CCParamDeviceStatus],
            let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(
                deviceStatusXml
            ),
            deviceStatus.deviceStatus == DeviceStatusEnum.connected
        else {
            return nil
        }

        return MobileReader.from(status: deviceStatus)
    }

    /// Attempts to initialize the ChipDNA SDK
    /// - Parameter args:A `ChipDnaInitializationArgs` object that contains the required security keys.
    /// - Parameter completion: A `(Bool) -> Void` callback containing whether the initialization was successful.
    func initialize(
        args: MobileReaderDriverInitializationArgs,
        completion: @escaping (Bool) -> Void
    ) {
        guard let args = args as? ChipDnaInitializationArgs,
            !args.keys.securityKey.isEmpty
        else {
            ChipDnaMobile.dispose(nil)
            completion(false)
            return
        }

        // Dispose prior state on re-init so merchant switches rebind
        // ChipDnaMobile to the new merchant's Tap attestation —
        // `setProperties(newApiKey)` alone does not clear the cached link.
        // Remove target-action registrations first: `dispose(nil)` is not
        // documented to clear class-level target lists, so callbacks
        // from a prior merchant's failed/cancelled connectAndConfigure
        // can leak across dispose and fire on a later merchant's
        // re-attestation.
        if ChipDnaMobile.isInitialized() {
            ChipDnaMobile.removeConnectAndConfigureFinishedTarget(self)
            ChipDnaMobile.removeConfigurationUpdateTarget(self)
            ChipDnaMobile.removeDeviceUpdateTarget(self)
            ChipDnaMobile.removeAvailablePinPadsTarget(self)
            let disposeResult = ChipDnaMobile.dispose(nil)
            if disposeResult?[CCParamResult] != CCValueTrue {
                print("[ChipDnaDriver.initialize] dispose returned non-success: \(String(describing: disposeResult))")
            }
            // Merchant switch: the next TTP connect must force a full TMS
            // re-download so ChipDnaMobile rotates the cached
            // TapToMobilePOIIdentifier + merchant display name to the new
            // merchant's values. `dispose` + `setProperties(newApiKey)`
            // alone do not invalidate the TMS cache.
            ChipDnaDriver.tmsRefreshPending = true
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
        properties.setValue(args.environment.baseUrlString(), forKey: CCParamEnvironment)
        let setPropertiesResult = ChipDnaMobile.sharedInstance()?.setProperties(
            properties
        )
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

        if ChipDnaMobile.isDeviceConnected
            && ChipDnaMobile.isTerminalStatusEnabled
        {
            completion(true)
        } else {
            completion(false)
        }
    }

    /// Searches for available `MobileReader` devices to connect to.
    /// - Parameter args: A `ChipDnaSearchArgs` object that contains the allowed search types.
    /// - Parameter completion: A `[MobileReader] -> Void` callback that contains the found `MobileReader` objects.
    func searchForReaders(
        args: MobileReaderDriverSearchArgs?,
        completion: @escaping ([MobileReader]) -> Void
    ) {
        // TODO: Allow scans for only USB, BLE, or BT based on args

        // Scan everything for 5 seconds
        let params = CCParameters()
        params[CCParamBLEScanTime] = "5"

        // Set the callback to contain this function's completion parameter
        onAvailablePinPadsCallback = { availablePinPads in
            let readers = availablePinPads.map({ MobileReader.from(pinPad: $0) }
            )
            completion(readers)
        }

        // Remove extraneous listeners, and search for available pin pads.
        ChipDnaMobile.removeAvailablePinPadsTarget(self)
        ChipDnaMobile.addAvailablePinPadsTarget(
            self,
            action: #selector(onAvailablePinPads(parameters:))
        )
        ChipDnaMobile.sharedInstance()?.getAvailablePinPads(params)
    }

    /// Connects to a specific `MobileReader` devices to connect to.
    /// - Parameter args: A `ChipDnaSearchArgs` object that contains the allowed search types.
    /// - Parameter completion: A `[MobileReader] -> Void` callback that contains the found `MobileReader` objects.
    func connect(
        reader: MobileReader,
        completion: @escaping (MobileReader?) -> Void
    ) {
        let requestParams = CCParameters()
        requestParams[CCParamPinPadName] = reader.name
        requestParams[CCParamPinPadConnectionType] =
            reader.connectionType ?? CCValueBLE

        onConnectAndConfigureCallback = { connectedReader in
            let _ = ChipDnaMobile.sharedInstance().getStatus(nil)
            if let connectedReader = connectedReader,
                let serial = connectedReader.serialNumber
            {
                self.familiarSerialNumbers.append(serial)
            }
            completion(connectedReader)
        }

        if reader.name.uppercased().hasPrefix("IDTECH") {
            requestParams.setValue(CCValueTrue, forKey: CCParamApplyFirmwareUpdate)
        } else {
            requestParams.setValue(CCValueTrue, forKey: CCParamForceTmsUpdate)
        }

        ChipDnaMobile.sharedInstance()?.setProperties(requestParams)
        ChipDnaMobile.addConnectAndConfigureFinishedTarget(
            self,
            action: #selector(onConnectAndConfigure(parameters:))
        )
        ChipDnaMobile.addConfigurationUpdateTarget(
            self,
            action: #selector(onConfigurationUpdate(parameters:))
        )
        ChipDnaMobile.addDeviceUpdateTarget(
            self,
            action: #selector(onDeviceUpdate(parameters:))
        )
       if !reader.name.uppercased().hasPrefix("IDTECH") {
            ChipDnaDriver.dumpRequestParams(requestParams, label: "BBPOS:\(reader.name)")
        }
        ChipDnaMobile.sharedInstance()?.connectAndConfigure(requestParams)
    }

    /// Logs every key/value in a CCParameters object before it's handed to
    /// ChipDnaMobile.connectAndConfigure. Use to diagnose whether a flag like
    /// CCParamForceTmsUpdate is actually being set on a given connect path.
    fileprivate static func dumpRequestParams(_ params: CCParameters, label: String) {
        let keys = (params.allKeys() as? [String]) ?? []
        let dump = keys.map { key in
            let value = params.value(forKey: key) ?? "nil"
            return "\(key)=\(value)"
        }.joined(separator: ", ")
        print("[ChipDnaDriver.\(label)] connectAndConfigure params: { \(dump) }")
    }

    /// Connects to Tap to Pay via ChipDna and propagates success/failure.
    func connectToTap(
        completion: @escaping (Bool, OmniException?) -> Void
    ) {
        let requestParams = CCParameters()
        requestParams.setValue(CCValueTrue, forKey: CCParamTapToMobilePOI)
        requestParams.setValue(CCValueFalse, forKey: CCParamPaymentDevicePOI)

        // Only force a full TMS re-download when we know a merchant switch
        // occurred (flag set by the auto-dispose branch in `initialize`).
        // Without this, dispose + setProperties(newApiKey) leaves the prior
        // merchant's TMS cache in place — so the attestation token handed
        // to Apple carries the old POI, and the PassKit sheet and the
        // ISO 8583 Card Acceptor Name on the cardholder's statement both
        // reflect the previous merchant even though NMI routes funds to
        // the new one. Skip the refresh on same-merchant reconnects to
        // avoid an unnecessary NMI round-trip.
        let forceTmsRefresh = ChipDnaDriver.tmsRefreshPending
        ChipDnaDriver.tmsRefreshPending = false
        if forceTmsRefresh {
            requestParams.setValue(CCValueTrue, forKey: CCParamFullTmsUpdate)
        }

        // Diagnostic: capture POI identifier + merchant display name before
        // connectAndConfigure so we can compare against the post-connect
        // values in onTapConnectAndConfigure and verify whether the refresh
        // actually rotated them.
        let beforeStatus = ChipDnaMobile.sharedInstance()?.getStatus(nil)
        let beforePOI = beforeStatus?[CCParamTapToMobilePOIIdentifier] ?? "nil"
        let beforeName = beforeStatus?[CCParamMerchantDisplayName] ?? "nil"
        print("[ChipDnaDriver.connectToTap] BEFORE connectAndConfigure forceTmsRefresh=\(forceTmsRefresh) POI=\(beforePOI) name=\(beforeName)")

        onTapConnectAndConfigureCallback = { success, error in
            completion(success, error)
        }

        ChipDnaMobile.addConnectAndConfigureFinishedTarget(
            self,
            action: #selector(onTapConnectAndConfigure(parameters:))
        )

        ChipDnaMobile.addConfigurationUpdateTarget(
            self,
            action: #selector(onTapConfigurationUpdate(parameters:))
        )

        ChipDnaMobile.sharedInstance()?.connectAndConfigure(requestParams)
    }

    /// Gets the connected MobileReader
    /// - Parameter completion: A `(MobileReader?) -> Void` callback that contains the `MobileReader` if connected; `nil` if not.
    /// - Parameter error: A `(OmniException) -> Void` callback that contains an `OmniException` if one occurred.
    func getConnectedReader(
        completion: (MobileReader?) -> Void,
        error: @escaping (OmniException) -> Void
    ) {
        if !ChipDnaMobile.isInitialized() {
            error(OmniGeneralException.uninitialized)
        }

        completion(ChipDnaDriver.getConnectedReader())
    }

    /// Attempts to disconnect the actively connected `MobileReader`
    /// - Parameter completion: A `(Bool) -> Void` callback containing the result of the disconnection.
    /// - Parameter error: A `(OmniException) -> Void` callback containing an `OmniException` if one occured.
    func disconnect(
        completion: @escaping (Bool) -> Void,
        error: @escaping (OmniException) -> Void
    ) {
        if !ChipDnaMobile.isInitialized() {
            error(OmniGeneralException.uninitialized)
        }

        // Re-initializing the ChipDnaMobile SDK disconnects everything, so that works.
        // Remove target-action registrations first — same rationale as
        // the auto-dispose guard in `initialize`.
        ChipDnaMobile.removeConnectAndConfigureFinishedTarget(self)
        ChipDnaMobile.removeConfigurationUpdateTarget(self)
        ChipDnaMobile.removeDeviceUpdateTarget(self)
        ChipDnaMobile.removeAvailablePinPadsTarget(self)
        let disposeResult = ChipDnaMobile.dispose(nil)
        if disposeResult?[CCParamResult] != CCValueTrue {
            print("[ChipDnaDriver.disconnect] dispose returned non-success: \(String(describing: disposeResult))")
        }
        initialize(
            args: ChipDnaDriver.initializationArgs!,
            completion: completion
        )
    }

    func performTransaction(
        with request: TransactionRequest,
        signatureProvider: SignatureProviding?,
        transactionUpdateDelegate: TransactionUpdateDelegate?,
        completion: @escaping (TransactionResult) -> Void
    ) {
        performTransaction(
            with: request,
            signatureProvider: signatureProvider,
            transactionUpdateDelegate: transactionUpdateDelegate,
            userNotificationDelegate: nil,
            completion: completion
        )
    }

    func performTransaction(
        with request: TransactionRequest,
        signatureProvider: SignatureProviding?,
        transactionUpdateDelegate: TransactionUpdateDelegate?,
        userNotificationDelegate: UserNotificationDelegate?,
        completion: @escaping (TransactionResult) -> Void
    ) {
        let requestParams = CCParameters(transactionRequest: request)

        // Create a fresh transaction listener for this transaction to prevent callback accumulation
        let transactionListener = ChipDnaTransactionListener()
        
        // Store card details for enriching the transaction result
        var additionalCardDetails: CCParameters?

        transactionListener.onFinished = { result in

            // For Tap to Pay: check if transaction was terminated before card details
            // If only errors are present (no transaction result), this is an early termination
            let hasTransactionResult = result[CCParamTransactionResult] != nil
            let hasErrors = result[CCParamErrors] != nil && !(result[CCParamErrors]?.isEmpty ?? true)
            
            // Early termination case: transaction finished before card details
            if !hasTransactionResult && hasErrors {
                var transactionResult = TransactionResult()
                transactionResult.source = Self.source
                transactionResult.request = request
                transactionResult.success = false
                
                if let errors = result[CCParamErrors] {
                    transactionResult.message = self.parseTapTransactionErrorMessage(errors: errors)
                }
                
                // Clean up and return early termination result
                transactionListener.detachFromChipDna()
                completion(transactionResult)
                return
            }
            
            // Normal transaction completion (after card details or regular flow)
            let success = result[CCParamTransactionResult] == CCValueApproved
            let receiptData = ChipDnaMobileSerializer.deserializeReceiptData(
                result[CCParamReceiptData]
            )

            var transactionResult = TransactionResult()
            transactionResult.source = Self.source
            transactionResult.request = request
            transactionResult.success = success
            transactionResult.maskedPan = result[CCParamMaskedPan]
            transactionResult.cardHolderFirstName =
                result[CCParamCardHolderFirstName]
            transactionResult.cardHolderLastName =
                result[CCParamCardHolderLastName]
            transactionResult.authCode = result[CCParamAuthCode]
            transactionResult.cardType = result[CCParamCardSchemeId]?
                .lowercased()
            transactionResult.userReference = result[CCParamUserReference]
            transactionResult.localId = result[CCParamCardEaseReference]
            transactionResult.externalId = result[CCParamTransactionId]
            transactionResult.transactionSource =
                receiptData?[kCCReceiptFieldTransactionSource]?.value
            
            // Parse Tap to Pay transaction errors if transaction failed
            if !success && hasErrors, let errors = result[CCParamErrors] {
                transactionResult.message = self.parseTapTransactionErrorMessage(errors: errors)
            }

            if let token = result[CCParamCustomerVaultId] {
                transactionResult.paymentToken = "nmi_\(token)"
            }
            
            // Enrich with additional card details if available (from tap to pay)
            // Card details callback fires BEFORE transaction finished for successful transactions
            if let cardDetails = additionalCardDetails {
                if transactionResult.maskedPan == nil {
                    transactionResult.maskedPan = cardDetails[CCParamMaskedPan]
                }
                
                // Get card type from card details if not already present
                if transactionResult.cardType == nil {
                    transactionResult.cardType = cardDetails[CCParamCardSchemeId]?.lowercased()
                }
                
                // Get expiry date from card details if not already present
                if let expiryDate = cardDetails[CCParamExpiryDate] {
                    // Convert YYMM format to MMYY format
                    if expiryDate.count == 4 {
                        let yy = String(expiryDate.prefix(2))
                        let mm = String(expiryDate.suffix(2))
                        transactionResult.cardExpiration = "\(mm)\(yy)"
                    }
                }
            }

            // Get more details about the transaction since ChipDna doesn't get everything
            TransactionGateway.getTransactionCcExpiration(
                securityKey: Self.initializationArgs!.keys.securityKey,
                transactionId: result[CCParamTransactionId] ?? ""
            ) { ccExpiration in
                // Only override if we don't have expiration from card details
                if transactionResult.cardExpiration == nil {
                    transactionResult.cardExpiration = ccExpiration
                }
                // Clean up this transaction listener once we're done
                transactionListener.detachFromChipDna()
                completion(transactionResult)
            }
        }
        
        // Set up card details callback for tap to pay
        transactionListener.onCardDetailsReceived = { cardDetails in
            // Store card details for later enrichment
            additionalCardDetails = cardDetails
        }

        transactionListener.bindToChipDna(
            signatureProvider: signatureProvider,
            transactionUpdateDelegate: transactionUpdateDelegate,
            userNotificationDelegate: userNotificationDelegate
        )

        ChipDnaMobile.sharedInstance()?.startTransaction(requestParams)
    }

    func capture(transaction: Transaction, completion: @escaping (Bool) -> Void)
    {
        guard let transactionMeta = transaction.meta,
            let userRef: String = transactionMeta["nmiUserRef"]
        else {
            completion(false)
            return
        }

        let params = CCParameters()
        params[CCParamUserReference] = userRef
        DispatchQueue.global(qos: .userInitiated).async {
            guard
                let result = ChipDnaMobile.sharedInstance()?.confirmTransaction(
                    params
                )
            else {
                return completion(false)
            }

            completion(result[CCParamTransactionResult] == CCValueApproved)
        }
    }

    func capture(
        _ transaction: StaxTransaction,
        completion: @escaping (Bool) -> Void
    ) {
        guard let meta = transaction.meta,
            let user: String = meta.string(at: "nmiUserRef")
        else {
            completion(false)
            return
        }

        let params = CCParameters()
        params[CCParamUserReference] = user
        DispatchQueue.global(qos: .userInitiated).async {
            guard
                let result = ChipDnaMobile.sharedInstance()?.confirmTransaction(
                    params
                )
            else {
                return completion(false)
            }

            completion(result[CCParamTransactionResult] == CCValueApproved)
        }
    }

    func void(
        transactionResult: TransactionResult,
        completion: @escaping (Bool) -> Void
    ) {
        guard let userRef = transactionResult.userReference else {
            completion(false)
            return
        }

        let params = CCParameters()
        params[CCParamUserReference] = userRef
        DispatchQueue.global(qos: .userInitiated).async {
            guard
                let result = ChipDnaMobile.sharedInstance()?.voidTransaction(
                    params
                )
            else {
                return completion(false)
            }

            completion(result[CCParamTransactionResult] == CCValueApproved)
        }
    }

    func cancelCurrentTransaction(
        completion: @escaping (Bool) -> Void,
        error: @escaping (OmniException) -> Void
    ) {
        if let result = ChipDnaMobile.sharedInstance()?.terminateTransaction(
            nil
        ) {
            if let success = result[CCParamResult], success == CCValueTrue {
                completion(true)
            } else {

                // Check if ChipDna is IDLE. If it is, there is no transaction to cancel
                if let status = ChipDnaMobile.sharedInstance()?.getStatus(nil),
                    status[CCParamChipDnaStatus] == "IDLE"
                {
                    error(
                        CancelCurrentTransactionException.noTransactionToCancel
                    )
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
    func refund(
        transaction: Transaction,
        refundAmount: Amount?,
        completion: @escaping (TransactionResult) -> Void,
        error: @escaping (OmniException) -> Void
    ) {
        // Get card ease reference. This is what we use to reference the transaction within NMI
        guard let cardEaseReference = transaction.cardEaseReference else {
            error(
                RefundException.transactionNotRefundable(
                    details: "Could not find user reference"
                )
            )
            return
        }

        // Get the amount to refund from the transaction
        guard let amountDollars = refundAmount?.dollars() ?? transaction.total
        else {
            error(
                RefundException.transactionNotRefundable(
                    details: "Could not find amount to refund"
                )
            )
            return
        }

        // Create the params for the 3rd-party refund
        let refundRequestParams = CCParameters()
        refundRequestParams[CCParamUserReference] =
            generateChipDnaTransactionUserReference()
        refundRequestParams[CCParamCardEaseReference] = cardEaseReference
        refundRequestParams[CCParamAmount] = Amount(dollars: amountDollars)
            .centsString()
        refundRequestParams[CCParamCurrency] = "USD"

        // Do the 3rd party refund
        guard
            let result = ChipDnaMobile.sharedInstance()?
                .linkedRefundTransaction(refundRequestParams)
        else {
            error(
                RefundException.transactionNotRefundable(
                    details: "Error while performing refund"
                )
            )
            return
        }

        let receiptData = ChipDnaMobileSerializer.deserializeReceiptData(
            result[CCParamReceiptData]
        )

        // Check status
        if result[CCParamErrors] != nil {
            error(
                RefundException.errorRefunding(
                    details: "Error while performing refund"
                )
            )
        } else {
            var transactionResult = TransactionResult()
            transactionResult.source = Self.source
            transactionResult.success = true
            transactionResult.transactionType = "refund"
            transactionResult.amount = refundAmount
            transactionResult.transactionSource =
                receiptData?["TRANSACTION_SOURCE"]?.value
            completion(transactionResult)
        }
    }

    fileprivate func deserializeAvailablePinPads(pinPadsXml: String)
        -> [SelectablePinPad]?
    {
        var availablePinPadsList: [SelectablePinPad]

        guard
            let availablePinPadsDict =
                ChipDnaMobileSerializer.deserializeAvailablePinPadsString(
                    pinPadsXml
                ) as? [String: Any]
        else {
            return nil
        }

        // Create empty list of pinpads
        availablePinPadsList = []

        // Add Bluetooth Low Energy devices
        let bleDevices = (availablePinPadsDict[CCValueBLE] as? [String])?.map {
            pinPadName in
            SelectablePinPad(name: pinPadName, connectionType: CCValueBLE)
        }
        availablePinPadsList.append(contentsOf: bleDevices ?? [])

        // Add Bluetooth devices
        let btDevices = (availablePinPadsDict[CCValueBluetooth] as? [String])?
            .map { pinPadName in
                SelectablePinPad(
                    name: pinPadName,
                    connectionType: CCValueBluetooth
                )
            }
        availablePinPadsList.append(contentsOf: btDevices ?? [])

        // USB
        let usbDevices =
            (availablePinPadsDict[CCValueLightningUsb] as? [String])?.map {
                pinPadName in
                SelectablePinPad(
                    name: pinPadName,
                    connectionType: CCValueLightningUsb
                )
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

        guard let onAvailablePinPadsCallback = onAvailablePinPadsCallback else {
            return
        }

        // Attempt deserialization
        guard
            let availablePinPadsXml = parameters[CCParamAvailablePinPads],
            let pinPads = deserializeAvailablePinPads(
                pinPadsXml: availablePinPadsXml
            )
        else {
            onAvailablePinPadsCallback([])
            return
        }

        onAvailablePinPadsCallback(pinPads)
    }

    @objc func onConnectAndConfigure(parameters: CCParameters) {
       
        ChipDnaMobile.removeConnectAndConfigureFinishedTarget(self)

        guard let onConnectAndConfigureCallback = onConnectAndConfigureCallback
        else { return }
        if parameters[CCParamResult] != CCValueTrue {
            onConnectAndConfigureCallback(nil)
            return
        }

        // Figure out the reader details and pass them along
        onConnectAndConfigureCallback(ChipDnaDriver.getConnectedReader())
    }

    @objc func onTapConnectAndConfigure(parameters: CCParameters) {
        ChipDnaMobile.removeConnectAndConfigureFinishedTarget(self)

        guard let onTapConnectAndConfigureCallback =
            onTapConnectAndConfigureCallback
        else {
            return
        }

        let result = parameters[CCParamResult]
        if result == CCValueTrue {
            tapConnectionStatusDelegate?.tapConnectionStatusUpdate(
                status: .connected
            )
            onTapConnectAndConfigureCallback(true, nil)
        } else {
            // Parse error codes and map to specific exceptions
            let exception = parseTapConnectionError(parameters: parameters)
            onTapConnectAndConfigureCallback(false, exception)
        }

        // Diagnostic: pairs with the BEFORE log in `connectToTap`. If the POI
        // identifier here matches the BEFORE value after a merchant switch,
        // the TMS refresh didn't rotate what Apple's attestation sees.
        let afterStatus = ChipDnaMobile.sharedInstance()?.getStatus(nil)
        let afterPOI = afterStatus?[CCParamTapToMobilePOIIdentifier] ?? "nil"
        let afterName = afterStatus?[CCParamMerchantDisplayName] ?? "nil"
        print("[ChipDnaDriver.onTapConnectAndConfigure] AFTER connectAndConfigure result=\(result ?? "nil") POI=\(afterPOI) name=\(afterName)")
    }
    
    /// Parses ChipDNA error parameters and returns appropriate ConnectTapException
    private func parseTapConnectionError(parameters: CCParameters) -> ConnectTapException {
        guard let errors = parameters[CCParamErrors], !errors.isEmpty else {
            return .couldNotConnectToTap(detail: nil)
        }
        
        let errorString = errors.lowercased()
        
        // Connect and Configure errors
        if errorString.contains("taptomobilenotsupported") || errorString.contains("tap to mobile not supported") {
            return .tapToMobileNotSupported
        }
        if errorString.contains("nopiselected") || errorString.contains("no poi selected") {
            return .noPoiSelected
        }
        if errorString.contains("locationpermissionsnotgranted") || errorString.contains("location permissions") {
            return .locationPermissionsNotGranted
        }
        if errorString.contains("applicationupdaterequired") || errorString.contains("application update") {
            return .applicationUpdateRequired
        }
        
        // Connect and Configure Finished event errors
        if errorString.contains("countrycodeinvalid") || errorString.contains("country code") {
            return .countryCodeInvalid
        }
        if errorString.contains("attestationfailed") || errorString.contains("attestation") {
            return .attestationFailed
        }
        if errorString.contains("accesstokenexpired") || errorString.contains("token expired") {
            return .accessTokenExpired
        }
        if errorString.contains("teamidentifiermissing") || errorString.contains("team identifier") {
            return .teamIdentifierMissing
        }
        if errorString.contains("invalidappleaccount") || errorString.contains("apple account") {
            return .invalidAppleAccount
        }
        if errorString.contains("missingbundleidentifier") || errorString.contains("bundle identifier") {
            return .missingBundleIdentifier
        }
        if errorString.contains("currentcountrynotallowed") || errorString.contains("country not allowed") {
            return .currentCountryNotAllowed
        }
        if errorString.contains("nolocationfound") || errorString.contains("no location") {
            return .noLocationFound
        }
        
        // Default case with error details
        return .couldNotConnectToTap(detail: errors)
    }
    
    /// Parses Tap to Pay transaction errors and returns user-friendly message
    private func parseTapTransactionErrorMessage(errors: String) -> String {
        let errorString = errors.lowercased()
        
        // Start Transaction errors
        if errorString.contains("transactionpoinotconnected") || errorString.contains("poi not connected") {
            return "Tap to Pay is not connected"
        }
        if errorString.contains("transactionpoiinvalid") || errorString.contains("poi invalid") {
            return "Invalid transaction point of interaction"
        }
        if errorString.contains("autoconfirmrequired") || errorString.contains("auto confirm") {
            return "Auto-confirmation is required for this transaction"
        }
        if errorString.contains("tipamountinvalid") || errorString.contains("tip amount invalid") {
            return "Invalid tip amount format"
        }
        if errorString.contains("tipamountnotallowed") || errorString.contains("tip amount not allowed") {
            return "Merchant tipping is not supported for this device"
        }
        if errorString.contains("merchanttippingnotsupported") || errorString.contains("tipping not supported") {
            return "Tipping is not supported with the configured processor"
        }
        
        // Transaction Finished event errors
        if errorString.contains("taptomobiletransactionterminated") || errorString.contains("transaction terminated") {
            return "Transaction was terminated by Tap to Pay"
        }
        if errorString.contains("taptomobilesessionclosed") || errorString.contains("session closed") {
            return "Tap to Pay session is no longer available"
        }
        
        // Return original error if no specific match
        return errors
    }

    @objc func onConfigurationUpdate(parameters: CCParameters) {
        if let str = parameters[CCParamConfigurationUpdate],
            let status = MobileReaderConnectionStatus(
                chipDnaConfigurationUpdate: str
            )
        {
            mobileReaderConnectionStatusDelegate?
                .mobileReaderConnectionStatusUpdate(status: status)
        }
    }

    @objc func onTapConfigurationUpdate(parameters: CCParameters) {
        // Handle configuration update status
        if let str = parameters[CCParamConfigurationUpdate],
            let status = TapConnectionStatus(
                chipDnaConfigurationUpdate: str
            )
        {
            tapConnectionStatusDelegate?.tapConnectionStatusUpdate(
                status: status
            )
        }
        
        // Handle configuration percentage (0-100)
        // This is received after CCValueUpdatingTapToMobileConfig
        if let percentageStr = parameters[CCParamTapToMobileConfigurationPercentage],
            let percentage = Int(percentageStr),
            percentage >= 0 && percentage <= 100
        {
            tapConnectionStatusDelegate?.tapConnectionConfigurationPercentage(
                percentage: percentage
            )
        }
    }

    @objc func onDeviceUpdate(parameters: CCParameters) {
        if let deviceStatusXml = parameters[CCParamDeviceStatusUpdate],
            let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(
                deviceStatusXml
            ),
            let status = MobileReaderConnectionStatus(
                chipDnaDeviceStatus: deviceStatus
            )
        {
            mobileReaderConnectionStatusDelegate?
                .mobileReaderConnectionStatusUpdate(status: status)
        }
    }
    
    /// Requests card details from the payment device (for tap to pay)
    /// - Parameter completion: A callback containing the card details parameters if successful
    func requestCardDetails(completion: @escaping (CCParameters?) -> Void) {
        let cardDetailsListener = ChipDnaTransactionListener()
        
        cardDetailsListener.onCardDetailsReceived = { cardDetails in
            cardDetailsListener.detachFromChipDna()
            completion(cardDetails)
        }
        
        cardDetailsListener.bindToChipDna()
        
        let params = CCParameters()
        let result = ChipDnaMobile.sharedInstance()?.getCardDetails(params)
        
        // If the call failed immediately, clean up and return nil
        if result?[CCParamResult] != CCValueTrue {
            cardDetailsListener.detachFromChipDna()
            completion(nil)
        }
    }
}
#endif
