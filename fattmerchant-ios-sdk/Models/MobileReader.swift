//
//  MobileReader.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// A Mobile Reader that can take a payment
public class MobileReader: CustomStringConvertible {

  /// The name of this mobile reader
  /// For example, "Miura 186"
  public var name: String

  /// The firmware version of the mobile reader
  public var firmwareVersion: String?

  /// The make of the mobile reader
  public var make: String?

  /// The model of the mobile reader
  public var model: String?

  /// The serail number of the mobile reader
  public var serialNumber: String?

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
  }

  public var description: String { return self.name }
}
