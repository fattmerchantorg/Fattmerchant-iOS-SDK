//
//  ANPCloudPOSTerminalUser.h
//  AnyPay
//
//  Created by Ankit Gupta on 13/09/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPCloudPOSEntity.h"

@interface ANPCloudPOSTerminalUser : ANPCloudPOSEntity

@property (nonatomic, copy) NSString<Optional> *username;
@property (nonatomic, copy) NSString<Optional> *password;
@property (nonatomic, copy) NSString<Optional> *status;
@property (nonatomic, strong) NSString<Optional> *lastKnownActive;

@end
