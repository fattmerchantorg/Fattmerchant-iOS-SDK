//
//  ANPBBPOSOTACredential.h
//  AnyPay
//
//  Created by Ankit Gupta on 08/03/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ANPBBPOSOTACredential : NSObject

@property (nonatomic, copy) NSString *otaServerURL;
@property (nonatomic, copy) NSString *vendorID;
@property (nonatomic, copy) NSString *vendorSecret;
@property (nonatomic, copy) NSString *appID;
@property (nonatomic, copy) NSString *appSecret;

@end
