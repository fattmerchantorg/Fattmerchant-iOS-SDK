#if targetEnvironment(simulator)
#else

extension MobileReader {
  
  static func from(pinPad: SelectablePinPad) -> MobileReader {
    let reader = MobileReader(name: pinPad.name)
    reader.connectionType = pinPad.connectionType
    return reader
  }

  static func from(status: DeviceStatus) -> MobileReader {
    return MobileReader(
      name: status.name,
      firmwareVersion: status.firmwareVersion,
      make: status.make,
      model: status.model,
      serialNumber: status.serialNumber
    )
  }
}

#endif
