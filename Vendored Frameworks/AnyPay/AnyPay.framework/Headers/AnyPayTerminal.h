//
//  AnyPayTerminal.h
//  AnyPay
//
//  Created by Ankit Gupta on 10/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ANPTerminal.h"
#import "ANPCloudPOSTerminal.h"

@class AnyPayTransaction, ANPCloudPOSTerminalMessage;
@interface AnyPayTerminal : ANPCloudPOSTerminal<ANPTerminal>

@property (nonatomic, copy) NSString *cloudPOSURL;

+ (instancetype)sharedInstance;
+ (instancetype)sharedInstanceWithConfiguration:(NSString *)terminalFilePath;

- (void)saveState;
+ (BOOL)clearSavedState;
- (void)clearSavedState;

- (BOOL)isActivated;
+ (BOOL)isActivated;

- (nonnull instancetype)initWithEndpoint:(nonnull id <ANPEndpoint>)endpoint;
//- (void)assignTerminalID:(nonnull NSString *)terminalID terminalKey:(nonnull NSString *)terminalKey gatewayURL:(nullable NSString *)gatewayURL;

- (void)applyLoggingConfiguration;

- (void)activateWithActivationCode:(NSString *_Nonnull)activationCode activationKey:(NSString *)activationKey completionHandler:(void (^_Nonnull)(BOOL, ANPMeaningfulError * _Nullable error))completionHandler;
- (void)getQueuedTransaction:(void (^)(AnyPayTransaction *transaction, ANPMeaningfulError * _Nullable error))completionHandler;
- (void)getQueuedTransactionForReader:(NSString *)readerSerialNumber completionHandler:(void (^)(AnyPayTransaction *transaction, ANPMeaningfulError * _Nullable error))completionHandler;
- (void)updateTransaction:(AnyPayTransaction *)transaction completionHandler:(void (^)(AnyPayTransaction *transaction, ANPMeaningfulError * _Nullable error))completionHandler;
- (void)acceptTransaction:(AnyPayTransaction *)transaction completionHandler:(void (^)(BOOL success, ANPMeaningfulError * _Nullable error))completionHandler;
- (void)submitReaderInfo:(NSDictionary *)deviceInfo completionHandler:(void (^)(BOOL success, ANPMeaningfulError * _Nullable error))completionHandler;
- (void)getQueuedMessage:(void (^)(ANPCloudPOSTerminalMessage *message, ANPMeaningfulError * _Nullable error))completionHandler;
- (void)getQueuedMessageForReader:(NSString *)readerSerialNumber completionHandler:(void (^)(ANPCloudPOSTerminalMessage *message, ANPMeaningfulError * _Nullable error))completionHandler;
- (void)getTransactionWithUUID:(NSString *)uuid completionHandler:(void (^)(AnyPayTransaction *transaction, ANPMeaningfulError * _Nullable error))completionHandler;

@end
