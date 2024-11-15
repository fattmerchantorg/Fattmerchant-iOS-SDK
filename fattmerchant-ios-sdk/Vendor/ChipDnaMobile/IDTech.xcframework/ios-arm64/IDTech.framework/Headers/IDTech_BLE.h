//
//  IDTech_BLE.h
//  IDTech
//
//  Created by Randy Palermo on 5/7/24.
//  Copyright © 2024 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "IDTCommon.h"
#import <CoreBluetooth/CoreBluetooth.h>


NS_ASSUME_NONNULL_BEGIN
typedef enum{
    BLE_MSG_SCAN_STARTING,
    BLE_MSG_DEVICE_FOUND,
    BLE_MSG_VERIFYING_CONNECTION,
    BLE_MSG_SCAN_COMPLETE,
    BLE_MSG_SCAN_TIMEOUT,
    BLE_MSG_VERIFY_TIMEOUT
}BLE_MSG;

@interface IDTech_BLE : NSObject
@property (copy, nullable) NSString* friendlyName;  //!< BLE device friendly name
@property (copy, nullable) NSString* identifier;  //!< BLE Device Identifer - Takes priority over friently name
@property int bleScanTimeout;  //!< Timeout for BLE Scanning
@property IDT_DEVICE_Types device;        //!< Device Type.
@property (copy, nullable) NSArray<CBUUID *> *spsServiceFilters;        //!< UUID BLE Device Filters
@property int validationTimeout;        //!< Timeout waiting for device communication to validate

@end

NS_ASSUME_NONNULL_END
