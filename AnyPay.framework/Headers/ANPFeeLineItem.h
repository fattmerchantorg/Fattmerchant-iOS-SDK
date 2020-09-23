//
//  ANPFeeLineItem.h
//  AnyPay
//
//  Created by Ankit Gupta on 11/05/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPSurchargeLineItem.h"

@protocol ANPFeeLineItem;

@interface ANPFeeLineItem : ANPSurchargeLineItem

- (instancetype)initWithName:(NSString *)name rate:(ANPAmount *)rate surchargeCalculationMethod:(ANPSurchargeCalculationMethod)surchargeCalculationMethod;
//- (instancetype)initWithName:(NSString *)name amount:(ANPAmount *)amount;

@end
