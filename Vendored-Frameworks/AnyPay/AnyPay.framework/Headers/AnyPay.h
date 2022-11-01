//
//  AnyPay.h
//  AnyPay
//
//  Created by Ankit Gupta on 10/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

//! Project version number for AnyPay.
FOUNDATION_EXPORT double AnyPayVersionNumber;

//! Project version string for AnyPay.
FOUNDATION_EXPORT const unsigned char AnyPayVersionString[];

#import "ANPTerminal.h"
#import "ANPWorldnetEndpoint.h"
#import "ANPWorldnetTerminal.h"
#import "ANPCardReaderController.h"
#import "ANPWorldnetTransaction.h"
#import "ANPWorldnetReferenceTransaction.h"
#import "ANPDatabase.h"
#import "ANPWorldnetUser.h"
#import "ANPBBPOSOTACredential.h"
#import "ANPEndpoint.h"
#import "AnyPayEndpoint.h"
#import "ANPTerminals.h"
#import "ANPKeyedTransactionWorkflow.h"
#import "ANPWorldnetTerminalSettings.h"
#import "ANPTaxLineItem.h"
#import "ANPTipLineItem.h"
#import "ANPTipAdjustmentLineItem.h"
#import "ANPFeeLineItem.h"
#import "ANPSurchargeLineItem.h"
#import "ANPLineItem.h"
#import "ANPSignature.h"
#import "ANPDrawPath.h"
#import "ANPSignatureView.h"
#import "ANPBBPOSOTADeviceConfiguration.h"
#import "AnyPayTransaction.h"
#import "AnyPayCardReaders.h"
#import "ANPConfiguration.h"
#import "ANPCloudPOSTerminalUser.h"
#import "ANPShadowTransactionRequest.h"
#import "ANPCloudPOSTerminalMessage.h"
#import "ANPTerminalNotActivatedException.h"
#import "ANPLogStream.h"
#import "ANPLogger.h"
#import "ANPLogConfigurationProperties.h"
#import "ANPHousekeeper.h"

//CloudPOS related
#import "ANPCloudAPI.h"
#import "ANPCloudPosTerminalMessageQueue.h"
#import "ANPCloudPosTerminalConnectionStatus.h"

//ProPay related
#import "ANPProPayTerminal.h"
#import "ANPProPayEndpoint.h"
#import "ANPProPayTransaction.h"
#import "ANPProPayReferenceTransaction.h"

//PriorityPayments related
#import "ANPPriorityPaymentsTerminal.h"
#import "ANPPriorityPaymentsEndpoint.h"
#import "ANPPriorityPaymentsTransaction.h"
#import "ANPPriorityPaymentsReferenceTransaction.h"

//NetSecure related
#import "ANPNetSecureEndpoint.h"


//--------------------------------------------------------//
                /*  ... AnyPay ... */
//------------------------------------------------------//

@interface AnyPay : NSObject

@property (nonnull, nonatomic, strong) id<ANPTerminal> terminal;

+ (nonnull instancetype)initialise;
+ (nonnull instancetype)initialise:(NSString *_Nullable)flavor;
+ (nullable instancetype)initialiseWithConfiguration:(nonnull ANPConfiguration *)configuration;
+ (nullable instancetype)initialiseWithConfigurationFile:(nonnull NSString *)terminalFilePath;
+ (void)getSupportKey:(NSString *)passphrase completionHandler:(void (^)(NSString *))completionHandler;
+ (void)getSupportKey:(NSString *)passphrase descriptor:(NSString *)descriptor completionHandler:(void (^)(NSString *))completionHandler;
+ (NSString *)getSavedSupportKey;
+ (void)setSupportKeyPassphrase:(NSString *_Nonnull)passphrase completionHandler:(void (^_Nullable)(BOOL))completionHandler;
+ (nullable instancetype)initializeWithTerminal:(nullable id<ANPTerminal>) terminal __deprecated;

+ (nullable instancetype)init NS_UNAVAILABLE;

+ (nullable instancetype)new NS_UNAVAILABLE;

+ (BOOL)isTerminalActivated;
+ (BOOL)clearTerminalState;

+ (void)addBlockedBIN:(NSString *)binToBlock;

#pragma mark - Configuration
+ (void)setDebugLogsEnabled:(BOOL)enabled __deprecated;

#pragma mark - Information
+ (nonnull NSString *)currentVersion;
+ (NSString *)bbposVersion;
+ (BOOL)isNetworkAvailable;

- (nullable NSString *)getLogs __deprecated;
- (void)clearLogs __deprecated;

@end
