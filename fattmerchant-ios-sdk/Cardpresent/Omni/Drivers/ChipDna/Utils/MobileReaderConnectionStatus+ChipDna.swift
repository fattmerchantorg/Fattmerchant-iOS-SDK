#if targetEnvironment(simulator)
#else

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
      case CCValueRegistering, CCValueConnecting: self = .connecting
      case CCValuePerformingTmsUpdate: self = .updating_configuration
      case CCValueUpdatingPinPadFirmware: self = .updating_firmware
      case CCValueRebootingPinPad: self = .rebooting
      default: return nil
    }
  }

  init?(chipDnaDeviceStatus: DeviceStatus) {
    switch chipDnaDeviceStatus.deviceStatus {
      case .disconnected: self = .disconnected
      default: return nil
    }
  }
}

#endif
