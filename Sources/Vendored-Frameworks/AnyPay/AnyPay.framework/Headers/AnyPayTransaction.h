//
//  AnyPayTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 14/09/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPReferenceTransaction.h"

NS_ASSUME_NONNULL_BEGIN

@interface AnyPayTransaction : ANPReferenceTransaction

@property (nonatomic, copy) NSString *origin; //Who created the transaction - Cloud server or app

- (void)sendReceiptToEmail:(NSString *)email phone:(NSString *)phone resultHandler:(void (^)(BOOL sent, ANPMeaningfulError * _Nullable))resultHandler;
- (void)updateWithSignature:(ANPSignature *)signature resultHandler:(void (^)(BOOL sent, ANPMeaningfulError * _Nullable))resultHandler;
- (instancetype)createCapture;

@end

NS_ASSUME_NONNULL_END
