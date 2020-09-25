//
//  ANPProPayTerminal.h
//  AnyPay
//
//  Created by Ankit Gupta on 12/07/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayTerminal.h"

@interface ANPProPayTerminal : AnyPayTerminal

@property (nonatomic, copy, readonly) NSString * _Nullable terminalID;
@property (nonatomic, copy, readonly) NSString * _Nullable accountNumber;

- (nonnull instancetype)initWithTerminalID:(nonnull NSString *)terminalID accountNumber:(nonnull NSString *)accountNumber gatewayURL:(nullable NSString *)gatewayURL NS_DESIGNATED_INITIALIZER;
- (void)setIntegrationServerURI:(NSString *)uri; //Defaults to https://epay.propay.com/API/PropayAPI.aspx. Required for Get Session Token api
- (void)setCertStr:(NSString *)certStr;
- (void)setX509Certificate:(NSString *)x509Cert;
- (void)refreshSession:(void (^)(BOOL, ANPMeaningfulError *))completionHandler;

- (void)fetchTransactions:(void (^)(NSArray * _Nullable))responseHandler;
- (void)fetchTransaction:(NSString *)externalID responseHandler:(void (^)(NSDictionary * _Nullable))responseHandler;

@end
