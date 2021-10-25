//
//  ANPWorldnetTerminal.h
//  AnyPay
//
//  Created by Ankit Gupta on 15/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayTerminal.h"

@class ANPWorldnetTerminalSettings;
@interface ANPWorldnetTerminal : AnyPayTerminal

@property (nonatomic, copy, readonly) NSString * _Nullable terminalID;
@property (nonatomic, copy, readonly) NSString * _Nullable terminalKey;
@property (nonatomic, strong, readonly) ANPWorldnetTerminalSettings * _Nullable settings;

- (nonnull instancetype)initWithTerminalID:(nonnull NSString *)terminalID terminalKey:(nonnull NSString *)terminalKey gatewayURL:(nullable NSString *)gatewayURL NS_DESIGNATED_INITIALIZER;

- (void)fetchTransactions:(nonnull NSNumber *)page orderID:(nullable NSString *)orderID fromDate:(nullable NSString *)fromDate toDate:(nullable NSString *)toDate responseHandler:(void (^_Nullable)(NSArray<AnyPayTransaction *> *_Nullable))responseHandler;
@end
