//
//  ANPBBPOSDevice.h
//  AnyPay
//
//  Created by Ankit Gupta on 17/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayCardReader.h"

@interface ANPBBPOSDevice : AnyPayCardReader

@property (nonatomic, readonly) NSDictionary *deviceInfo;

- (instancetype)initWithDeviceInfo:(NSDictionary *)deviceInfo NS_DESIGNATED_INITIALIZER;

@end
