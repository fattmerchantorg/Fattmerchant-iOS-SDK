//
//  UsbAccessoryDelegate.swift
//  fattmerchant-ios-sdk
//
//  Created by Thomas Vidas on 6/25/24.
//  Copyright © 2024 Fattmerchant. All rights reserved.
//

import Foundation

public protocol UsbAccessoryDelegate: class {

  /// Called when a something is plugged into the iPhone/iPad
  func onUsbAccessoryConnected()
  
  /// Called when a something is unplugged into the iPhone/iPad
  func onUsbAccessoryDisconnected()
}
