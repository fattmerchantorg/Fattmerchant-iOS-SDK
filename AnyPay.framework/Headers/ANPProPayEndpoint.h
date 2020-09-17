//
//  ANPProPayEndpoint.h
//  AnyPay
//
//  Created by Ankit Gupta on 12/07/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayEndpoint.h"

@interface ANPProPayEndpoint : AnyPayEndpoint

@property (nonatomic, copy) NSString *integrationServerURI;
@property (nonatomic, copy) NSString *x509Certificate;
@property (nonatomic, copy) NSString *certStr;
@property (nonatomic, copy) NSString *terminalID;
@property (nonatomic, copy) NSString *accountNumber;

+ (nonnull instancetype)sharedInstance;
- (void)initializeWithTerminalID:(nonnull NSString *)terminalID accountNumber:(nonnull NSString *)accountNumber gatewayURL:(nullable NSString *)gatewayURL;
- (void)validateConfiguration:(void (^)(BOOL, ANPMeaningfulError * _Nullable))completionHandler;

- (void)setSessionToken:(NSString *)sessionToken;
- (void)getSessionId:(void (^)(NSString *, ANPMeaningfulError * _Nullable))completionHandler;

- (void)refreshSession:(void (^)(BOOL, ANPMeaningfulError * _Nullable))completionHandler;

- (void)cancelTransaction:(ANPTransaction *)transaction;

- (void)fetchTransactions:(void (^)(NSArray *))responseHandler;
- (void)fetchTransactionWithExternalID:(NSString *)externalID responseHandler:(void (^)(NSDictionary *))responseHandler;

@end
