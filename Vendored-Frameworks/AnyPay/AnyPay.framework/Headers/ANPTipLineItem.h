//
//  ANPTipLineItem.h
//  AnyPay
//
//  Created by Ankit Gupta on 21/04/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPSurchargeLineItem.h"

@protocol ANPTipLineItem;

@interface ANPTipLineItem : ANPSurchargeLineItem

- (instancetype)initWithName:(NSString *)name rate:(ANPAmount *)rate surchargeCalculationMethod:(ANPSurchargeCalculationMethod)surchargeCalculationMethod;
//- (instancetype)initWithName:(NSString *)name amount:(ANPAmount *)amount;

@end
