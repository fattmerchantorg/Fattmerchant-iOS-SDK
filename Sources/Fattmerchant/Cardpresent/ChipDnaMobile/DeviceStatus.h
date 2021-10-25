/**
 * @author Creditcall Ltd
 * @brief This class represents the current status of a device connected to the SDK.
 * @class DeviceStatus
 * @copyright Copyright Creditcall Limited 2010 - 2017.
 */

#import <Foundation/Foundation.h>

/**
 * @typedef DeviceStatusEnum
 *  Enum defining the current connection status of a device.
 */
typedef NS_ENUM(NSUInteger, DeviceStatusEnum) {
    DeviceStatusUnknown, /**< The device connection is currently unknown. */
    DeviceStatusDisconnected, /**< The device is currently disconnected. */
    DeviceStatusConnected, /**< The device is currently connected and has been successfully configured. */
    DeviceStatusConnectedUnconfigured /**< The device is currently connected, however has not been configured. */
};

@interface DeviceStatus : NSObject

/**
 * The current connection status of the device.
 */
@property (nonatomic, readonly)DeviceStatusEnum deviceStatus;

/**
 * If filled the error why the device is currently unavailable.
 */
@property (nonatomic, strong, readonly)NSString *availablityError;

/**
 * NSDictionary containing key value pairs with information about the availablilityError.
 */
@property (nonatomic, strong, readonly)NSDictionary *errorInformation;

/**
 * PIN pad name of currently connected device.
 */
@property (nonatomic, strong, readonly)NSString *name;

/**
 * Make of the currently connected device.
 */
@property (nonatomic, strong, readonly)NSString *make;

/**
 * Model of the currently connected device.
 */
@property (nonatomic, strong, readonly)NSString *model;

/**
 * Firmware name of the currently connected device.
 */
@property (nonatomic, strong, readonly)NSString *firmwareName;

/**
 * Firmware version of the currently connected device.
 */
@property (nonatomic, strong, readonly)NSString *firmwareVersion;

/**
 * Serial number of currently connected PIN pad.
 */
@property (nonatomic, strong, readonly)NSString *serialNumber;

/**
 * Convert string constant into {@link DeviseStatus#DeviceStatusEnum DeviceStatusEnum}.
 */
+(DeviceStatusEnum)getDeviceStatusEnum:(NSString *)deviceStatusStr;

/**
 * Convert {@link DeviseStatus#DeviceStatusEnum DeviceStatusEnum} into string constant.
 */
+(NSString *)getDeviceStatusString:(DeviceStatusEnum)deviceStatusEnum;


@end
