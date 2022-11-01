//
//  ANPCloudPosTerminalMessageQueue.h
//  AnyPay
//
//  Created by Ankit Gupta on 02/08/20.
//  Copyright © 2020 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ANPCloudPosTerminalConnectionStatus.h"

NS_ASSUME_NONNULL_BEGIN

@interface ANPCloudPosTerminalMessageQueue : NSObject

@property (nonatomic) ANPCloudPosTerminalConnectionStatus connectionStatus;
@property (nonatomic, copy) void (^onConnectionStatusChanged)(ANPCloudPosTerminalConnectionStatus status);

+ (instancetype)sharedInstance;
//- (void)startPolling;
//- (void)stopPolling;
- (void)start;
- (void)stop;
- (void)updateMessageOnCloud:(ANPCloudPOSTerminalMessage *)message;
- (void)subscribeToMessagesOfType:(NSString *)messageType subscriber:(void (^ _Nonnull)(ANPCloudPOSTerminalMessage *message))subscriber;
- (void)unsubscribeToMessagesOfType:(NSString *)messageType subscriber:(void (^ _Nonnull)(ANPCloudPOSTerminalMessage *message))subscriber;

@end

NS_ASSUME_NONNULL_END
