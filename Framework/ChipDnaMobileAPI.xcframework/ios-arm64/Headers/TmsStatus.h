/**
 * @author Creditcall Ltd
 * @brief This class represents the current status of TMS updates.
 * @class TmsStatus
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */


#import <Foundation/Foundation.h>

@interface TmsStatus : NSObject

/**
 * Date format for {@link TmsStatus#lastConfigUpdateDateTime lastConfigUpdateDateTime}.
 */
@property (nonatomic, strong, readonly)NSString *dateTimeFormat;

/**
 * The time and date of the last TMS config update.
 */
@property (nonatomic, strong, readonly)NSString *lastConfigUpdateDateTime;

/**
 * The number of days until a TMS config update must take place.
 */
@property (nonatomic, strong, readonly)NSString *daysUntilConfigUpdateRequired;

/**
 * The time and date when config update is required.
 */
@property (nonatomic, strong, readonly)NSString *requiredConfigUpdateDateTime;

@end
