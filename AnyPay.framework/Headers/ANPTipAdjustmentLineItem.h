//
//  ANPTipAdjustmentLineItem.h
//  AnyPay
//
//  Created by Ankit Gupta on 14/05/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPTipLineItem.h"
#import "ANPSurchargeCalculationMethods.h"

@class ANPAmount;
@interface ANPTipAdjustmentLineItem : ANPTipLineItem

- (instancetype)initWithRate:(ANPAmount *)rate surchargeCalculationMethod:(ANPSurchargeCalculationMethod)surchargeCalculationMethod;
//- (instancetype)initWithAmount:(ANPAmount *)amount;

@end
