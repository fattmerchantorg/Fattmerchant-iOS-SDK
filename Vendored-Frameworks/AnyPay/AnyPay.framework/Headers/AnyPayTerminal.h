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

NS_ASSUME_NONNULL_BEGIN

@class AnyPayTransaction, ANPCloudPOSTerminalMessage;
@interface AnyPayTerminal : ANPCloudPOSTerminal<ANPTerminal>

//@property (nonatomic, copy) NSString *cloudPOSURL;

+ (instancetype)getInstance;
+ (instancetype)getInstanceWithConfigurationFile:(NSString *)terminalFilePath;
+ (instancetype)initialize:(AnyPayEndpoint *)endpoint;
+ (instancetype)initializeFromCloudWithActivationCode:(NSString *)activationCode activationKey:(NSString *)activationKey completionHandler:(void (^)(BOOL, ANPMeaningfulError * _Nullable error))completionHandler;
+ (instancetype)defaultTerminalFromJsonFile;

+ (AnyPayTerminal *)restoreState;

- (void)saveState;
+ (BOOL)clearSavedState;
- (void)clearSavedState;
- (void)overwriteConfiguration:(AnyPayTerminal *)terminal;

- (BOOL)isActivated;
+ (BOOL)isActivated;

- (nonnull instancetype)initWithEndpoint:(nonnull id <ANPEndpoint>)endpoint __deprecated;

#pragma mark Private Methods

+ (instancetype)sharedInstance;
+ (instancetype)sharedInstanceWithConfigurationFile:(NSString *)terminalFilePath;

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

- (void)processDeferredTransactions:(void (^)(NSString *, id))eventHandler notificationHandler:(void (^)(NSString *transactionId, BOOL success, ANPMeaningfulError * _Nullable error))notificationHandler;

- (void)logout;

@end

NS_ASSUME_NONNULL_END
