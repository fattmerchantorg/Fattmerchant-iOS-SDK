import Foundation

public enum CardReaderConnectionStatus: String {
  case connecting
  case connected
  case disconnected
  case configuring
  case updating
  case rebooting
}

extension CardReaderConnectionStatus {
  init?(_ config: String) {
    switch config {
      case CCValueRegistering, CCValueConnecting: self = .connecting
      case CCValuePerformingTmsUpdate: self = .configuring
      case CCValueUpdatingPinPadFirmware: self = .updating
      case CCValueRebootingPinPad: self = .rebooting
      default: return nil
    }
  }
  
  init?(_ status: DeviceStatus?) {
    switch status?.deviceStatus {
      case .disconnected: self = .disconnected
      default: return nil
    }
  }
}
