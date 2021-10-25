//
//  ANPPaymentStatuses.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ANPPaymentStatus) {
    ANPPaymentStatus_UNKNOWN = 0,
    ANPPaymentStatus_INCOMPLETE,
    ANPPaymentStatus_PENDING,
    ANPPaymentStatus_COMPLETED,
    ANPPaymentStatus_PARTIALLY_REFUNDED,
    ANPPaymentStatus_REFUNDED,
    ANPPaymentStatus_VOIDED
};

@interface ANPPaymentStatuses : NSObject

+ (ANPPaymentStatus)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPPaymentStatus)paymentStatus;

@end
