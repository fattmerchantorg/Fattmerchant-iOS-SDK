//
//  MobileReaderConnectionStatus.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 6/20/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// The status of a `MobileReader`
public enum MobileReaderConnectionStatus: String {
  /// The reader has been found by the iOS device and is currently being connected
  case connecting

  /// The reader is connected
  case connected

  /// The reader is disconnected
  case disconnected

  /// The reader is performing an update
  ///
  /// - Note: This might be a long-running operation
  case updating

  /// The reader is performing a reboot
  case rebooting
}
