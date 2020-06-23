//
//  ANPUser.h
//  AnyPay
//
//  Created by Ankit Gupta on 24/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ANPCloudPOSTerminalUser.h"

@class ANPMeaningfulError;
@interface ANPUser : ANPCloudPOSTerminalUser

@property (nonatomic, copy) NSString * _Nullable authenticationToken;
@property (nonatomic, copy) NSDate * _Nullable authenticationExpiryDate;

- (nullable instancetype)init;
- (nullable instancetype)initWithUsername:(nonnull NSString *)username password:(nonnull NSString *)password NS_DESIGNATED_INITIALIZER;

//- (void)authenticate:(void (^_Nullable)(BOOL, ANPMeaningfulError * _Nullable))completionHandler ;

@end
