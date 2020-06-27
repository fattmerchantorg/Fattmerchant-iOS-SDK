//
//  ANPAmount.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ANPAmount : AnyPayModel

@property (nonatomic, readonly) NSString *stringValue;
@property (nonatomic, readonly) double doubleValue;
@property (nonatomic, readonly) NSDecimalNumber *decimalValue;

@property (nonatomic) BOOL isPercentageType;

- (instancetype)initWithDecimal:(NSDecimalNumber *)amount NS_DESIGNATED_INITIALIZER;

+ (instancetype)amountWithString:(NSString *)amountString;
+ (instancetype)amountWithDouble:(double)amount;
+ (instancetype)amountWithDecimal:(NSDecimalNumber *)amount;
+ (instancetype)amountWithNSNumber:(NSNumber *)amount;
+ (instancetype)zero;

+ (instancetype)taxAmountWithDouble:(double)amount;

- (ANPAmount *)add:(ANPAmount *)amountToAdd;
- (ANPAmount *)subtract:(ANPAmount *)amountToSubtract;

- (BOOL)greaterThanZero;
+ (NSString *)amountInCents:(NSDecimalNumber *)decimalValue;

- (BOOL)isEquivalentTo:(ANPAmount *)amount;

#pragma mark - Display Helpers

- (NSString *)ISOCodeString:(NSString *)currency;
- (NSString *)localizedString;
- (NSString *)percentStringWithDecimalPlaces:(NSUInteger)decimalPlaces;

@end
