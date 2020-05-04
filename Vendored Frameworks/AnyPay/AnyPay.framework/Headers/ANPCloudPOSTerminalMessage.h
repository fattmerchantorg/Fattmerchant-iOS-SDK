//
//  ANPCloudPOSTerminalMessage.h
//  AnyPay
//
//  Created by Ankit Gupta on 17/01/19.
//  Copyright © 2019 Dan McCann. All rights reserved.
//

#import "AnyPayTransaction.h"
#import "ANPCloudPOSTerminalUser.h"

NS_ASSUME_NONNULL_BEGIN

@interface ANPCloudPOSTerminalMessage : ANPCloudPOSEntity

@property (nonatomic, copy) NSString *type;
@property (nonatomic, copy) NSString *message;
@property (nonatomic, copy) NSString *status;
@property (nonatomic, copy) NSString *reason;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *terminalUUID;
@property (nonatomic, copy) NSString *transactionUUID;
@property (nonatomic, strong) AnyPayTransaction *transaction;
@property (nonatomic, strong) AnyPayTerminal *terminal;
@property (nonatomic, strong) ANPCloudPOSTerminalUser *operator;

- (void)accept;
- (void)acceptAwaitingResponse;
- (void)reject;
- (void)fail:(ANPMeaningfulError *)error;
- (void)finished;

@end

NS_ASSUME_NONNULL_END
