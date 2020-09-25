//
//  ANPCloudPOSTerminalUser.h
//  AnyPay
//
//  Created by Ankit Gupta on 13/09/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPCloudPOSEntity.h"

@interface ANPCloudPOSTerminalUser : ANPCloudPOSEntity

@property (nonatomic, copy) NSString *username;
@property (nonatomic, copy) NSString *password;
@property (nonatomic, copy) NSString *status;
@property (nonatomic, strong) NSString *lastKnownActive;

@end
