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
  public var name: String

  /// Initialize a MobileReader by name
  ///
  /// This name must match the real name of the mobile reader, or mobile reader operations will not work
  /// - Parameter name: The name of the mobile reader
  public init(name: String) {
    self.name = name
  }

  public var description: String { return self.name }
}
