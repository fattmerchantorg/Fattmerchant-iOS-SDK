//
//  ChipDnaUtils.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/17/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

#if targetEnvironment(simulator)

#else

extension MobileReader {
    static func from(pinPad: ChipDnaDriver.SelectablePinPad) -> MobileReader {
        let reader = MobileReader(name: pinPad.name)
        reader.connectionType = pinPad.connectionType
        return reader
    }

    static func from(deviceStatus: DeviceStatus) -> MobileReader {
        return MobileReader(name: deviceStatus.name,
                            firmwareVersion: deviceStatus.firmwareVersion,
                            make: deviceStatus.make,
                            model: deviceStatus.model,
                            serialNumber: deviceStatus.serialNumber)
    }
}

extension TransactionUpdate {

    /// Makes an Stax TransactionUpdate from a ChipDna TransactionUpdate string
    init?(chipDnaTransactionUpdate: String) {
        switch chipDnaTransactionUpdate {

        case TransactionUpdateCardEntryPrompted:
            self = .PromptInsertSwipeCard

        case TransactionUpdateCardSwiped:
            self = .CardSwiped

        case TransactionUpdateSmartcardInserted:
            self = .CardInserted

        case TransactionUpdateCardSwipeError:
            self = .CardSwipeError

        case TransactionUpdateSmartcardRemovePrompted:
            self = .PromptRemoveCard

        case TransactionUpdateSmartcardRemoved:
            self = .CardRemoved

        case TransactionUpdateOnlineAuthorisation:
            self = .Authorizing

        case TransactionUpdateAmountConfirmationStarted:
            self = .PromptConfirmAmountOnReader

        default:
            return nil
        }
    }
}

extension UserNotification {

    /// Makes an Stax TransactionUpdate from a ChipDna TransactionUpdate string
    init?(chipDnaUserNotification: String) {
        switch chipDnaUserNotification {

        case UserNotificationTryCardAgain:
            self = UserNotification.TryCardAgain

        case UserNotificationChipReadErrorApplicationNotSupportedPleaseRetry:
            self = UserNotification.ChipReadErrorApplicationNotSupportedPleaseRetry

        case UserNotificationICCFallforward:
            self = UserNotification.FallforwardInsertCard

        case UserNotificationICCMSRFallforward:
            self = UserNotification.FallforwardInsertSwipeCard

        case UserNotificationMSRFallback:
            self = UserNotification.FallbackSwipeCard

        case UserNotificationMSRFallforward:
            self = UserNotification.FallforwardSwipeCard

        case UserNotificationPresentOnlyOneCard:
            self = UserNotification.PresentOneCardOnly

        case UserNotificationReferToDevice:
            self = UserNotification.ReferToDevice

        default:
            return nil
        }
    }
}

extension MobileReaderConnectionStatus {

    /// Initializes a `MobileReaderConnectionStatus` object from the given ChipDnaConfigurationUpdate
    ///
    /// ChipDna hands us a ton of configuration updates during the mobile reader connection process. The ones we care
    /// about are:
    /// * Connecting
    /// * Permorming Tms Update
    /// * Updating Pinpad Firmware
    /// * Rebooting
    /// * Registering
    ///
    /// When any of these events are passed into this initializer, then the corresponding [MobileReaderConnectionStatus]
    /// is created. When any other event is passed into this initializer, then the initialization fails and `nil` is
    /// returned.
    ///
    /// - Note: Some [MobileReaderConnectionStatus] correspond to several ChipDna statuses. For example,
    ///  `CCValueRegistering` and `CCValuePerformingTmsUpdate` both map to `MobileReaderConnectionStatus.updating`
    ///
    /// - Parameter chipDnaConfigurationUpdate: a string representation of the configuration update from ChipDna. This
    /// is obtained by grabbing the value of `CCParamConfigurationUpdate`
    init?(chipDnaConfigurationUpdate configUpdate: String) {
        switch configUpdate {
        case CCValueRegistering, CCValueConnecting:
            self = .connecting

        case CCValuePerformingTmsUpdate:
            self = .updating_configuration

        case CCValueUpdatingPinPadFirmware:
            self = .updating_firmware

        case CCValueRebootingPinPad:
            self = .rebooting

        default:
            return nil
        }
    }

    init?(chipDnaDeviceStatus: DeviceStatus) {
        switch chipDnaDeviceStatus.deviceStatus {
        case .disconnected:
            self = .disconnected
        default:
            return nil
        }
    }
}

extension CCParameters {

    /// The param value to make NMI add a customer to the customer vault
    ///
    /// This should be used with the CCParamCustomerVaultCommand and passed into startTransaction()
    var ParamValueAddCustomer: String {
        return "add-customer"
    }

    subscript(key: String) -> String? {
        get {
            guard
                let allKeys = allKeys() as? [String],
                allKeys.contains(key)
            else { return nil }

            return value(forKey: key)
        }
        set {
            setValue(newValue, forKey: key)
        }

    }

    convenience init(transactionRequest: TransactionRequest) {
        self.init()
        self[CCParamAmount] = transactionRequest.amount.centsString()
        self[CCParamAmountType] = CCValueAmountTypeActual
        self[CCParamCurrency] = "USD"
        self[CCParamUserReference] = generateChipDnaTransactionUserReference()
        self[CCParamPaymentMethod] = CCValueCard
        self[CCParamTransactionType] = CCValueSale

        if transactionRequest.tokenize {
            self[CCParamCustomerVaultCommand] = ParamValueAddCustomer
        }
    }

}

#endif
