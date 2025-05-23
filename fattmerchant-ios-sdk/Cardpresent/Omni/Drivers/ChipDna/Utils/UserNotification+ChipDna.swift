#if targetEnvironment(simulator)
#else

extension UserNotification {

  /// Makes an Omni TransactionUpdate from a ChipDna TransactionUpdate string
  init?(chipDnaUserNotification: String) {
    switch chipDnaUserNotification {
      case "UserNotificationTryCardAgain": self = UserNotification.TryCardAgain
      case "UserNotificationChipReadErrorApplicationNotSupportedPleaseRetry": self = UserNotification.ChipReadErrorApplicationNotSupportedPleaseRetry
      case "UserNotificationICCFallforward": self = UserNotification.FallforwardInsertCard
      case "UserNotificationICCMSRFallforward": self = UserNotification.FallforwardInsertSwipeCard
      case "UserNotificationMSRFallback": self = UserNotification.FallbackSwipeCard
      case "UserNotificationMSRFallforward": self = UserNotification.FallforwardSwipeCard
      case "UserNotificationPresentOnlyOneCard": self = UserNotification.PresentOneCardOnly
      case "UserNotificationReferToDevice": self = UserNotification.ReferToDevice
      default: return nil
    }
  }
}

#endif
