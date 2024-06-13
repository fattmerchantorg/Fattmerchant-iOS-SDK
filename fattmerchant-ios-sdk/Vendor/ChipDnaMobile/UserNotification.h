/**
 * @author Creditcall Ltd
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 * @file UserNotification.h
 */

#import <Foundation/Foundation.h>

/**
 * @typedef UserNotification
 *
 * The possible notifications that can occur during a standalone (sale or refund) transaction process and be presented to the user.
 */
typedef NSString* UserNotification;

extern UserNotification const UserNotificationReferToDevice; /**< Indicates referral to the device. */

extern UserNotification const UserNotificationChipReadErrorApplicationNotSupportedPleaseRetry; /**< Indicates there has been an issue with the ICC chip and the user should retry. */

extern UserNotification const UserNotificationPresentOnlyOneCard; /**< Indicates that only one card should be presented. */

extern UserNotification const UserNotificationMSRFallback; /**< Indicates that fallback to Swipe has occurred. */

extern UserNotification const UserNotificationMSRFallforward; /**< Indicates that fallforward from Contactless to Swipe has occurred. */

extern UserNotification const UserNotificationICCFallforward; /**< Indicates that fallforward from Contactless to Insert has occurred. */

extern UserNotification const UserNotificationICCMSRFallforward; /**< Indicates that fallforward from Contactless to Insert/Swipe has occurred. */

extern UserNotification const UserNotificationTryCardAgain; /**< Inidicates that the card should be tried again. */
