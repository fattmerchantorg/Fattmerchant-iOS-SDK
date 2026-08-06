/**
 * @author Creditcall Ltd
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 * @file FirmwareUpdates.h
 */

#import <Foundation/Foundation.h>

/**
 * @typedef FirmwareUpdate
 *
 * The possible updates that can occur during a firmware update process.
 */

typedef NSString* FirmwareUpdate;
extern FirmwareUpdate const FirmwareUpdateStarting;                                 /**< Indicates the firmware update process has started. */
extern FirmwareUpdate const FirmwareUpdateDownloadingFirmwareToDevice;              /**< Indicates the firmware has started being downloaded to the device. */
extern FirmwareUpdate const FirmwareUpdateApplyingFirmwareToDevice;                 /**< Indicates the firmware has started being applied to the device. */
extern FirmwareUpdate const FirmwareUpdateComplete;                                 /**< Indicates the firmware update process is complete. */
