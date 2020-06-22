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
    return MobileReader(name: pinPad.name)
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

  /// Makes an Omni TransactionUpdate from a ChipDna TransactionUpdate string
  init?(chipDnaTransactionUpdate: String) {
    switch chipDnaTransactionUpdate {

    case TransactionUpdateCardEntryPrompted:
      guard
        let reader = ChipDnaDriver.connectedReader(),
        let makeString = reader.make,
        let make = ChipDnaDriver.PinPadManufacturer(rawValue: makeString) else {
          return nil
      }

      switch make {
      case .Miura:
        self = .PromptInsertSwipeCard
      default:
        self = .PromptSwipeCard
      }

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
    case CCValueConnecting:
      self = .connecting

    case CCValueRegistering, CCValuePerformingTmsUpdate, CCValueUpdatingPinPadFirmware:
      self = .updating

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
    self[CCParamAutoConfirm] = CCValueTrue
    self[CCParamTransactionType] = CCValueSale

    if transactionRequest.tokenize {
      self[CCParamCustomerVaultCommand] = ParamValueAddCustomer
    }
  }

}

#endif
