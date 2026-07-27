//
//  MobileReader.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// A Mobile Reader that can take a payment
final public class MobileReader: CustomStringConvertible, Sendable {

  /// The name of this mobile reader
  /// For example, "Miura 186"
  public let name: String

  /// The firmware version of the mobile reader
  public let firmwareVersion: String?

  /// The make of the mobile reader
  public let make: String?

  /// The model of the mobile reader
  public let model: String?

  /// The serail number of the mobile reader
  public let serialNumber: String?

  /// The way that the iOS device connects to the reader. Bluetooth, BLE, etc
  public let connectionType: String?

  /// Initialize a MobileReader by name
  ///
  /// This name must match the real name of the mobile reader, or mobile reader operations will not work
  /// - Parameter name: The name of the mobile reader
  public init(name: String,
              firmwareVersion: String? = nil,
              make: String? = nil,
              model: String? = nil,
              serialNumber: String? = nil) {
    self.name = name
    self.firmwareVersion = firmwareVersion
    self.make = make
    self.model = model
    self.serialNumber = serialNumber
    self.connectionType = nil
  }

  /// Initialize a MobileReader by name
  ///
  /// This name must match the real name of the mobile reader, or mobile reader operations will not work
  /// - Parameter name: The name of the mobile reader
  public init(name: String,
              firmwareVersion: String? = nil,
              make: String? = nil,
              model: String? = nil,
              serialNumber: String? = nil,
              connectionType: String? = nil) {
    self.name = name
    self.firmwareVersion = firmwareVersion
    self.make = make
    self.model = model
    self.serialNumber = serialNumber
    self.connectionType = connectionType
  }

  public var description: String { return self.name }
}
