//
//  ANPSurchargeLineItem.h
//  AnyPay
//
//  Created by Ankit Gupta on 21/04/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPLineItem.h"
#import "ANPSurchargeCalculationMethods.h"

@interface ANPSurchargeLineItem : ANPLineItem

@property (nonatomic, strong) ANPAmount *rate;
@property (nonatomic, strong) NSString *displayName;
@property (nonatomic) ANPSurchargeCalculationMethod surchargeCalculationMethod;

- (instancetype)initWithDisplayName:(NSString *)name type:(NSString *)type rate:(ANPAmount *)rate surchargeCalculationMethod:(ANPSurchargeCalculationMethod)surchargeCalculationMethod;
- (ANPAmount *)calculateOn:(ANPAmount *)onAmount;

@end
