//
//  ANPCloudPOS.h
//  AnyPay
//
//  Created by Ankit Gupta on 13/09/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ANPMeaningfulError, AnyPayTransaction, ANPCloudPOSTerminalMessage, ANPShadowTransactionRequest, ANPCloudPosSupportKey;
@interface ANPCloudAPI : NSObject

//@property (nonatomic, copy) NSString *cloudPOSURL;
@property (nonatomic, copy) NSString *apiBaseUrl;
@property (nonatomic, copy) NSString *logApiBaseUrl;

+ (instancetype)getInstance;
+ (void)setFlavor:(NSString *)flavor;
- (void)setFlavor:(NSString *)flavor;
- (NSString *)getFlavor;
- (void)modulateFlavor:(NSString *)flavor;
+ (NSString *)getFlavor:(NSString *)url;

- (void)activateTerminal:(AnyPayTerminal *)terminal completionHandler:(void (^)(AnyPayTerminal *terminal, ANPMeaningfulError * _Nullable))completionHandler;
- (void)getTransactions:(AnyPayTerminal *)terminal forReader:(NSString * _Nullable)connectedReader completionHandler:(void (^)(AnyPayTransaction *transaction, ANPMeaningfulError * _Nullable))completionHandler;
- (void)getTransactions:(AnyPayTerminal *)terminal completionHandler:(void (^)(AnyPayTransaction *transaction, ANPMeaningfulError * _Nullable))completionHandler;
- (void)updateTransaction:(AnyPayTransaction *)transaction terminal:(AnyPayTerminal *)terminal completionHandler:(void (^)(AnyPayTransaction *transaction, ANPMeaningfulError * _Nullable))completionHandler;
- (void)acceptTransaction:(AnyPayTransaction *)transaction terminal:(AnyPayTerminal *)terminal completionHandler:(void (^)(BOOL success, ANPMeaningfulError * _Nullable))completionHandler;
- (void)submitDeviceInfo:(NSDictionary *)readerInfo terminal:(AnyPayTerminal *)terminal completionHandler:(void (^)(BOOL success, ANPMeaningfulError * _Nullable))completionHandler;
- (void)shadowTransaction:(ANPShadowTransactionRequest *)shadowTransaction terminal:(AnyPayTerminal *)terminal shadowApiUrl:(NSString *)url completionHandler:(void (^)(BOOL success, ANPMeaningfulError * _Nullable))completionHandler;

- (void)getMessages:(AnyPayTerminal *)terminal completionHandler:(void (^)(ANPCloudPOSTerminalMessage *message, ANPMeaningfulError * _Nullable))completionHandler;
- (void)updateMessage:(ANPCloudPOSTerminalMessage *)message terminal:(AnyPayTerminal *)terminal completionHandler:(void (^)(ANPMeaningfulError * _Nullable))completionHandler;
- (void)getMessages:(AnyPayTerminal *)terminal forReader:(NSString *)connectedReader completionHandler:(void (^)(ANPCloudPOSTerminalMessage *message, ANPMeaningfulError * _Nullable))completionHandler;

- (void)getTransactionWithID:(NSString *)uuid terminal:(AnyPayTerminal *)terminal completionHandler:(void (^)(AnyPayTransaction *transaction, ANPMeaningfulError * _Nullable))completionHandler;

- (void)streamToLog:(NSString *)log terminal:(AnyPayTerminal *)terminal realtime:(BOOL)realtime completionHandler:(void (^)(BOOL success, ANPMeaningfulError * _Nullable))completionHandler;

- (id)getMessages:(AnyPayTerminal *)terminal forReader:(NSString *)connectedReader;
//- (void)getDeviceIdAssignedTerminal:(AnyPayTerminal *)terminal deviceId:(NSString *)deviceId completionHandler:(void (^)(ANPCloudPosSupportKey *, ANPMeaningfulError * _Nullable))completionHandler;

- (void)getSupportKey:(NSString * _Nullable)passphrase completionHandler:(void (^)(NSString *, ANPMeaningfulError * _Nullable))completionHandler;
- (void)getSupportKey:(NSString *)passphrase descriptor:(NSString *)descriptor completionHandler:(void (^)(NSString *, ANPMeaningfulError * _Nullable))completionHandler;
- (void)addSupportKeyTerminal:(void (^)(ANPMeaningfulError * _Nullable))completionHandler;
- (NSString *)getSavedSupportKey;

- (void)recordTransaction:(NSDictionary *)tx completionHandler:(void (^)(ANPMeaningfulError * _Nullable))completionHandler;

#pragma mark Private Methods
+ (instancetype)sharedInstance;

@end

NS_ASSUME_NONNULL_END
