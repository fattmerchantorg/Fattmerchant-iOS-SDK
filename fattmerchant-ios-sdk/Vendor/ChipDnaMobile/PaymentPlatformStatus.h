/**
 * @author Creditcall Ltd
 * @brief The status info about ChipDNA's connection to the payment platform.
 * @class PaymentPlatformStatus
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

#import <Foundation/Foundation.h>

/**
 * @typedef PaymentPlatformState
 *
 * Payment platform connection state.
 */
typedef NS_ENUM(NSUInteger, PaymentPlatformState) {
    PaymentPlatformStateUnavailable, /**< Server connection unsuccessful. */
    PaymentPlatformStateAvailable /**< Server connection successful. */
};

@interface PaymentPlatformStatus : NSObject

/**
 * Payment platform connection state.
 */
@property (nonatomic, readonly)PaymentPlatformState state;

/**
 * The datetime format for {@link PaymentPlatformStatus#localDateTime localDateTime}
 */
@property (nonatomic, strong, readonly)NSString *localDateTimeFormat;

/**
 * The payment platform local date time.
 */
@property (nonatomic, strong, readonly)NSString *localDateTime;

/**
 * Errors returned during the connection test.
 */
@property (nonatomic, strong, readonly)NSArray *errors;

/**
 * The date time format for {@link PaymentPlatformStatus#platformDateTime platformDateTime}.
 */
@property (nonatomic, strong, readonly)NSString *platformDateTimeFormat;

/**
 * The date time when payment platform status was checked.
 */
@property (nonatomic, strong, readonly)NSString *platformDateTime;


@end
