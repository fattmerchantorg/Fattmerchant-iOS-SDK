//
//  TapConnectionStatusDelegate.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 11/5/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

import Foundation

public protocol TapConnectionStatusDelegate: AnyObject {
  func tapConnectionStatusUpdate(status: TapConnectionStatus)
  
  /// Called when the Tap to Pay configuration percentage changes (0-100)
  /// This is fired during the configuration update process to show progress
  /// - Parameter percentage: An integer between 0 and 100 indicating configuration progress
  func tapConnectionConfigurationPercentage(percentage: Int)
}
