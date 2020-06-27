//
//  ANPSurchargeCalculationMethods.h
//  AnyPay
//
//  Created by Ankit Gupta on 11/05/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ANPSurchargeCalculationMethod) {
    ANPSurchargeCalculationMethod_UNSPECIFIED = 0,
    ANPSurchargeCalculationMethod_FLAT_RATE,
    ANPSurchargeCalculationMethod_PERCENTAGE_SUBTOTAL,
    ANPSurchargeCalculationMethod_PERCENTAGE_TOTAL
};

@interface ANPSurchargeCalculationMethods : NSObject

+ (ANPSurchargeCalculationMethod)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPSurchargeCalculationMethod)paymentMethod;
+ (NSDictionary<NSString *, NSString *> *)calculationMethods;

@end
