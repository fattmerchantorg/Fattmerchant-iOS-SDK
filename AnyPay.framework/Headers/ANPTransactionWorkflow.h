//
//  ANPTransactionWorkflow.h
//  AnyPay
//
//  Created by Ankit Gupta on 18/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ANPTransaction;
@interface ANPTransactionWorkflow : NSObject

@property (nonatomic, strong) ANPTransaction *transaction;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)initWithTransaction:(ANPTransaction *)transaction NS_DESIGNATED_INITIALIZER;

- (void)execute:(void (^)(BOOL, ANPMeaningfulError *))resultHandler cardReaderEvent:(void (^)(ANPMeaningfulMessage *))readerEventHandler;
- (void)execute:(void (^)(BOOL, ANPMeaningfulError *))resultHandler;

- (void)cancelTransaction;

@end
