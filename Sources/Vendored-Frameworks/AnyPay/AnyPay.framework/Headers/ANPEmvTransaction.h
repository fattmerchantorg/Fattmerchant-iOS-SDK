//
//  ANPEmvTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPSwipeTransaction.h"
#import "ANPTerminalResponse.h"

@interface ANPEmvTransaction : ANPSwipeTransaction

@property (nonatomic, strong) ANPTerminalResponse *terminalResponse;
@property (nonatomic, copy) NSString *onlineTransactionEmvPayload;
@property (nonatomic, copy) NSString *offlineTransactionEmvPayload;
@property (nonatomic, copy) NSString *selectedApplication;
@property (nonatomic, strong) NSArray *applications;

- (void)applyTerminalResponse:(ANPTerminalResponse *)terminalResponse;

@end
