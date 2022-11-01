//
//  ANPCardholderVerificationMethod.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ANPCardholderVerificationMethod) {
    ANPCardholderVerificationMethod_UNKNOWN = 0,
    ANPCardholderVerificationMethod_NONE,
    ANPCardholderVerificationMethod_PIN,
    ANPCardholderVerificationMethod_SIGNATURE,
    ANPCardholderVerificationMethod_BIOMETRIC,
    ANPCardholderVerificationMethod_VOICE,
    ANPCardholderVerificationMethod_OTHER
};

@interface ANPCardholderVerificationMethods : NSObject

+ (ANPCardholderVerificationMethod)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPCardholderVerificationMethod)cvm;
+ (ANPCardholderVerificationMethod)cvmFromTag9f34:(NSString *) tag9f34Value;
+ (NSString *)cvmBitsFromTag9f34:(NSString *)tag9f34Value;

@end
