//
//  ANPCloudPOSTerminal.h
//  AnyPay
//
//  Created by Ankit Gupta on 13/09/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPCloudPOSEntity.h"

@class ANPCloudPOSTerminalUser;
@interface ANPCloudPOSTerminal : ANPCloudPOSEntity

@property (nonatomic, copy) NSString *activationCode;
@property (nonatomic, copy) ANPCloudPOSTerminalUser *operator;
@property (nonatomic, copy) NSString *sessionKey;
@property (nonatomic, copy) NSString *sessionKeyExpiry;
@property (nonatomic, copy) NSString *sessionKeySalt;
@property (nonatomic, copy) NSString *secretKey;
@property (nonatomic, copy) NSString *status;

@end
