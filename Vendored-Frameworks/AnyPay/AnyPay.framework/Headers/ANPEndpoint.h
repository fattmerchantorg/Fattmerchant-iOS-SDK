//
//  ANPEndpoint.h
//  AnyPay
//
//  Created by Ankit Gupta on 15/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ANPTerminal.h"
#import "ANPMeaningfulError.h"
#import "ANPGatewayResponse.h"
#import "ANPTransaction.h"
#import "ANPSignature.h"
#import "ANPTipLineItem.h"

@protocol ANPEndpoint <NSObject>

- (void)submitTransaction:(ANPTransaction *_Nullable)transaction resultHandler:(void (^ _Nullable)(ANPGatewayResponse * _Nullable response, ANPMeaningfulError * _Nullable))resultHandler;

@optional
- (void)authenticateTerminal:(void (^_Nonnull)(BOOL authenticated, ANPMeaningfulError *_Nullable))completionHandler;
- (void)submitTipAdjustment:(ANPTipLineItem *)tip forTransaction:(ANPTransaction *)transaction resultHandler:(void (^)(BOOL submitted, ANPMeaningfulError * _Nullable))resultHandler;
- (void)submitSignature:(ANPSignature *)signature forTransaction:(ANPTransaction *)transaction resultHandler:(void (^)(BOOL submitted, ANPMeaningfulError * _Nullable))resultHandler;
- (void)cancelTransaction:(ANPTransaction *)transaction;
- (void)sendReceiptForTransaction:(ANPTransaction *)transaction toEmail:(NSString *)email phone:(NSString *)phone resultHandler:(void (^)(BOOL sent, ANPMeaningfulError * _Nullable))resultHandler;

@end
