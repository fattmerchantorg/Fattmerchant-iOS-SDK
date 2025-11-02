//
//  MobileReaderConnectionStatusDelegate.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 6/20/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Receives notifications of reader connection events such as when a reader is connecting or updating
public protocol MobileReaderConnectionStatusDelegate: AnyObject {

  /// Called when `MobileReader` has a new `MobileReaderConnectionStatus`
  /// - Parameters:
  ///   - status: The new `MobileReaderConnectionStatus` of the `reader`
  func mobileReaderConnectionStatusUpdate(status: MobileReaderConnectionStatus)
}
