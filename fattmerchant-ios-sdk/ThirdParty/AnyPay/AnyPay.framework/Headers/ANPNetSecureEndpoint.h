//
//  ANPNetSecureEndpoint.h
//  AnyPay
//
//  Created by Ankit Gupta on 07/08/19.
//  Copyright © 2019 Dan McCann. All rights reserved.
//

#import "ANPWorldnetEndpoint.h"

@interface ANPNetSecureEndpoint : ANPWorldnetEndpoint

@property (nonatomic, copy) NSString *username;
@property (nonatomic, copy) NSString *password;
@property (nonatomic, copy) NSString *merchantID;
@property (nonatomic, copy) NSString *providerApiUrl;

- (void)shadowTransaction:(ANPTransaction *)transaction resultHandler:(void (^)(BOOL, ANPMeaningfulError * _Nullable))resultHandler;

@end
