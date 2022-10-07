/**
 * @author Creditcall Ltd
 * @brief The class contains the jailbreak check.
 * @class JailbreakChecker
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

#import <Foundation/Foundation.h>

@interface JailbreakChecker : NSObject

/**
 * Checks whether the device is jailbroken.
 * @note Always returns NO on simulator.
 * @return YES if the device is jailbroken, otherwise NO.
 */
+ (BOOL)isDeviceJailbroken;

@end
