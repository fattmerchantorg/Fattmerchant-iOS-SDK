//
//  ANPWorldnetTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPEmvTransaction.h"

@class ANPSignature, ANPTipAdjustmentLineItem;

__deprecated_msg("This class is deprecated. Use AnyPayTransaction instead. Refer Sample code on how to use AnyPayTransaction")
@interface ANPWorldnetTransaction : ANPEmvTransaction

- (void)sendReceiptToEmail:(NSString * _Nullable)email phone:(NSString * _Nullable)phone resultHandler:(void (^_Nullable)(BOOL sent, ANPMeaningfulError * _Nullable))resultHandler;

- (void)updateWithTipAdjustment:(nonnull ANPTipAdjustmentLineItem *)tipAdjustment resultHandler:(void (^_Nullable)(BOOL submitted, ANPMeaningfulError * _Nullable))resultHandler;
 
- (void)updateWithSignature:(ANPSignature *_Nullable)signature resultHandler:(void (^_Nullable)(BOOL sent, ANPMeaningfulError * _Nullable))resultHandler;

@end
