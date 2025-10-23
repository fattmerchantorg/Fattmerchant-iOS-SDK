extension ChipDnaMobile {
  /// Returns `true` if a device is actively connected to the ChipDnaMobile SDK
  internal static var isDeviceConnected: Bool {
    guard
      let status = ChipDnaMobile.sharedInstance().getStatus(nil),
      let deviceStatusXml = status[CCParamDeviceStatus],
      let deviceStatus = ChipDnaMobileSerializer.deserializeDeviceStatus(deviceStatusXml),
      deviceStatus.deviceStatus == DeviceStatusEnum.connected
    else {
      return false
    }
    
    return true
  }
  
  /// Returns `true` is a device is actively connected and enabled
  internal static var isTerminalStatusEnabled: Bool {
    guard
      let status = ChipDnaMobile.sharedInstance().getStatus(nil),
      let terminalStatusXml = status[CCParamTerminalStatus],
      let terminalStatus = ChipDnaMobileSerializer.deserializeTerminalStatus(terminalStatusXml),
      terminalStatus.enabled
    else {
      return false
    }
    
    return true
  }
}
