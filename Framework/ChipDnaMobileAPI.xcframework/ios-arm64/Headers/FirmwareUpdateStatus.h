/**
 * @author Creditcall Ltd
 * @brief This class represents the current status of firmware updates.
 * @class FirmwareUpdateStatus
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */


#import <Foundation/Foundation.h>

@interface FirmwareUpdateStatus : NSObject

/**
 * Date format for {@link FirmwareUpdateStatus#requiredFirmwareUpdateDateTime requiredFirmwareUpdateDateTime}.
 */
@property (nonatomic, strong, readonly)NSString *dateTimeFormat;

/**
 * The number of days until a firmware update must take place.
 */
@property (nonatomic, strong, readonly)NSString *daysUntilFirmwareUpdateRequired;

/**
 * The time and date when firmware update is required.
 */
@property (nonatomic, strong, readonly)NSString *requiredFirmwareUpdateDateTime;

/**
 * List of firmware files requiring to be updated.
 */
@property(nonatomic, strong, readonly)NSArray<NSString *> *filesRequiringUpdate;

@end
