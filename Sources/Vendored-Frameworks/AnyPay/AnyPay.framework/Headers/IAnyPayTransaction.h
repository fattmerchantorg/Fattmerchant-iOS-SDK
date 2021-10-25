//
//  AnyPayTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ANPMeaningfulError.h"
#import "ANPAmount.h"
#import "ANPGatewayResponse.h"
#import "ANPTransactionTypes.h"
#import "CryptoCoding.h"

@protocol IAnyPayTransaction <NSObject, CryptoCoding>

@optional
- (void)setTotalAmount:(ANPAmount *_Nonnull)totalAmount;
- (void)setSubtotal:(ANPAmount *_Nonnull)subtotal;

//For Card Transactions
- (void)execute:(void (^_Nonnull)(ANPTransactionStatus status, ANPMeaningfulError *_Nullable error))resultHandler cardReaderEvent:(void (^_Nonnull)(ANPMeaningfulMessage *_Nullable message))readerEventHandler;

//For Keyed Transactions
- (void)execute:(void (^_Nonnull)(ANPTransactionStatus status, ANPMeaningfulError *_Nullable error))resultHandler;
- (void)proceed;
- (void)cancel;
- (void)applyGatewayResponse:(ANPGatewayResponse *_Nonnull)gatewayResponse;
- (void)persist;

@end
