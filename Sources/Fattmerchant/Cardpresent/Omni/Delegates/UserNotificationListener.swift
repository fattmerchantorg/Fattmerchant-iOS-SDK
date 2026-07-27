//
//  UserNotificationListener.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 2/24/21.
//  Copyright © 2021 Fattmerchant. All rights reserved.
//

import Foundation

/// Gets notified of all messages that need to be displayed to a user
///
/// For example, "Please insert/swipe your card"
public protocol UserNotificationDelegate: AnyObject {

  /// Called when a prompt is required to be shown to a customer and returns a UserNotification
  func onUserNotification(userNotification: UserNotification)
}
