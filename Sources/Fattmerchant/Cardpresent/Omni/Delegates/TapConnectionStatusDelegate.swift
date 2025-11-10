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
}
