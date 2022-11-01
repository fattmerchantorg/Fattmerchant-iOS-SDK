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
- (void)submitTipAdjustment:(ANPTipLineItem * _Nonnull)tip forTransaction:(ANPTransaction * _Nonnull)transaction resultHandler:(void (^_Nullable)(BOOL submitted, ANPMeaningfulError * _Nullable))resultHandler;
- (void)submitSignature:(ANPSignature * _Nonnull)signature forTransaction:(ANPTransaction * _Nonnull)transaction resultHandler:(void (^_Nullable)(BOOL submitted, ANPMeaningfulError * _Nullable))resultHandler;
- (void)cancelTransaction:(ANPTransaction * _Nonnull)transaction;
- (void)sendReceiptForTransaction:(ANPTransaction * _Nonnull)transaction toEmail:(NSString * _Nullable)email phone:(NSString * _Nullable)phone resultHandler:(void (^ _Nullable)(BOOL sent, ANPMeaningfulError * _Nullable))resultHandler;
- (NSDictionary *_Nullable)toSecureObject;
- (void)fetchTransactionWithInternalId:(NSString *)internalId responseHandler:(void (^)(ANPTransaction * _Nullable, ANPMeaningfulError *))responseHandler;

@end
