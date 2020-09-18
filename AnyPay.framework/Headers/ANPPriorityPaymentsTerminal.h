//
//  ANPPriorityPaymentsTerminal.h
//  AnyPay
//
//  Created by Ankit Gupta on 31/07/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayTerminal.h"

@interface ANPPriorityPaymentsTerminal : AnyPayTerminal

@property (nonatomic, copy, readonly) NSString * _Nullable consumerKey;
@property (nonatomic, copy, readonly) NSString * _Nullable consumerSecret;
@property (nonatomic, copy, readonly) NSString * _Nullable merchantID;

- (nonnull instancetype)initWithConsumerKey:(nonnull NSString *)consumerKey consumerSecret:(nonnull NSString *)consumerSecret merchantID:(nonnull NSString *)merchantID gatewayURL:(nullable NSString *)gatewayURL NS_DESIGNATED_INITIALIZER;

- (void)fetchTransactions:(void (^)(NSArray * _Nullable))responseHandler;

@end
