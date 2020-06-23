//
//  ANPBBPOSOTADeviceConfiguration.h
//  AnyPay
//
//  Created by Ankit Gupta on 19/10/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ANPBBPOSOTADeviceConfiguration : NSObject

@property (nonatomic, copy) NSString *firmwareVersion;
@property (nonatomic, copy) NSString *deviceSettingVersion;
@property (nonatomic, copy) NSString *terminalSettingVersion;

@end

NS_ASSUME_NONNULL_END
