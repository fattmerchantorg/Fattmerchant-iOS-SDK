//
//  ANPWorldnetEndpoint.h
//  AnyPay
//
//  Created by Ankit Gupta on 17/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayEndpoint.h"

NS_ASSUME_NONNULL_BEGIN

@class ANPTransaction, ANPWorldnetGatewayResponse, ANPWorldnetTerminalSettings, ANPEmvTransaction, AnyPayTransaction, ANPWorldnetUser;
@interface ANPWorldnetEndpoint : AnyPayEndpoint

@property (nonatomic, copy) NSString *worldnetSecret;
@property (nonatomic, copy) NSString *worldnetTerminalID;
@property (nonatomic, strong, readonly) ANPWorldnetTerminalSettings * _Nullable terminalSettings;

+ (nonnull instancetype)sharedInstance;
- (void)initializeWithTerminalID:(nonnull NSString *)terminalID terminalKey:(nonnull NSString *)terminalKey gatewayURL:(nullable NSString *)gatewayURL;
- (void)validateConfiguration:(void (^)(BOOL, ANPMeaningfulError * _Nullable))completionHandler;

//- (void)authenticateUser:(ANPWorldnetUser *)user completionHandler:(void (^)(BOOL, ANPMeaningfulError * _Nullable))completionHandler;
- (void)authenticateUser:(ANPWorldnetUser *)user view:(NSObject *)view completionHandler:(void (^)(BOOL, ANPMeaningfulError * _Nullable))completionHandler;

- (void)fetchTransactions:(nonnull NSNumber *)page orderID:(nullable NSString *)orderID fromDate:(nullable NSString *)fromDate toDate:(nullable NSString *)toDate responseHandler:(void (^_Nullable)(NSArray<AnyPayTransaction *> *_Nullable))responseHandler;

- (void)sendReceiptForTransaction:(ANPEmvTransaction *)transaction toEmail:(NSString *)email phone:(NSString *)phone resultHandler:(void (^)(BOOL sent, ANPMeaningfulError *))resultHandler;

- (void)cancelTransaction:(ANPTransaction *)transaction;

#pragma mark - Updates
- (void)submitTipAdjustment:(ANPTipLineItem *)tip forTransaction:(ANPTransaction *)transaction resultHandler:(void (^)(BOOL submitted, ANPMeaningfulError *))resultHandler;
- (void)submitSignature:(ANPSignature *)signature forTransaction:(ANPTransaction *)transaction resultHandler:(void (^)(BOOL submitted, ANPMeaningfulError *))resultHandler;

NS_ASSUME_NONNULL_END

@end
