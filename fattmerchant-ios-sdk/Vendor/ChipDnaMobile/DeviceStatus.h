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
 * Revision of currently connected PIN pad.
 */
@property (nonatomic, strong, readonly)NSString *pinPadRevision;

/**
 * Operating system version of the connected PIN pad.
 */
@property (nonatomic, strong, readonly)NSString *pinPadOsVersion;

/**
 * Kernel version of the connected PIN pad.
 */
@property (nonatomic, strong, readonly)NSString *pinPadKernelVersion;

/**
 * Battery charge percentage of the currently connected device.
 */
@property (nonatomic, readonly)NSInteger batteryChargePercentage;

/**
 * Charging status of the currently connected device.
 * Returns "Not Charging", "Charging" or "Fully Charged".
 */
@property (nonatomic, strong, readonly)NSString *batteryChargingStatus;

/**
 * Date and time for the last battery charging status update of the currently connected device.
 * Returned using the local date, time, and time zone of the mobile device.
 * Format specified by batteryChargingStatusUpdateDateTimeFormat.
 */
@property (nonatomic, strong, readonly)NSString *batteryChargingStatusUpdateDateTime;

/**
 * Date and time format used for batteryChargingStatusUpdateDateTime.
 * Returns "dd/MM/yyyy HH:mm:ss".
 */
@property (nonatomic, strong, readonly)NSString *batteryChargingStatusUpdateDateTimeFormat;

/**
 * Convert string constant into {@link DeviseStatus#DeviceStatusEnum DeviceStatusEnum}.
 */
+(DeviceStatusEnum)getDeviceStatusEnum:(NSString *)deviceStatusStr;

/**
 * Convert {@link DeviseStatus#DeviceStatusEnum DeviceStatusEnum} into string constant.
 */
+(NSString *)getDeviceStatusString:(DeviceStatusEnum)deviceStatusEnum;


@end
