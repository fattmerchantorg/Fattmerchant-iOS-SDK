//
//  UserNotification.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 2/24/21.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Represents a user notification
///
/// This object will provide information about events that prompt the user.
public struct UserNotification {

  /// Describes what the transaction status is
  ///
  /// Example: "Prompt User Fallback Swipe Card"
  public let value: String

  /// A message that you can show to the end user
  ///
  /// Example: "Please swipe your card."
  public let userFriendlyMessage: String?

  public init(_ value: String, _ userFriendlyMessage: String? = nil) {
    self.value = value
    self.userFriendlyMessage = userFriendlyMessage
  }
  /// Indicates referral to the device
  public static let ReferToDevice = UserNotification("Prompt User Refer To Device", "Please check your device.")

  /// Indicates there has been an issue with the ICC chip and the user should retry
  public static let ChipReadErrorApplicationNotSupportedPleaseRetry = UserNotification("Prompt User Error Application Not Supported",
          "There was an error with processing the chip card. Please try again.")

  /// Indicates that only one card should be presented
  public static let PresentOneCardOnly = UserNotification("Prompt User Present One Card Only", "Please only present one card.")

  /// Indicates that fallback to Swipe has occurred
  public static let FallbackSwipeCard = UserNotification("Prompt User Fallback Swipe Card", "Please swipe your card.")

  /// Indicates that fallforward to Swipe has occurred
  public static let FallforwardSwipeCard = UserNotification("Prompt User Fallforward Swipe Card", "Please swipe your card.")

  /// Indicates that fallforward to Insert has occurred
  public static let FallforwardInsertCard = UserNotification("Prompt User Fallforward Insert Card", "Please insert your card.")

  /// Indicates that fallforward to Insert/Swipe has occurred
  public static let FallforwardInsertSwipeCard = UserNotification("Prompt User Fallforward Insert Swipe Card", "Please insert or swipe your card.")

  /// Indicates that the card should be tried again
  public static let TryCardAgain = UserNotification( "Prompt User Try Card Again","Please try your card again.")

}
