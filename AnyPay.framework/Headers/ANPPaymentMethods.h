//
//  ANPPaymentMethods.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ANPPaymentMethod) {
    ANPPaymentMethod_UNSPECIFIED = 0,
    ANPPaymentMethod_DEBIT,
    ANPPaymentMethod_CREDIT,
    ANPPaymentMethod_CASH,
    ANPPaymentMethod_CHECK,
    ANPPaymentMethod_CRYPTO,
    ANPPaymentMethod_STORED_VALUE
};

@interface ANPPaymentMethods : NSObject

+ (ANPPaymentMethod)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPPaymentMethod)paymentMethod;

@end
