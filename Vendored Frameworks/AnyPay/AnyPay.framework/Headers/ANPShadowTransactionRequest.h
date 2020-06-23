//
//  ANPShadowTransactionRequest.h
//  AnyPay
//
//  Created by Ankit Gupta on 06/08/19.
//  Copyright © 2019 Dan McCann. All rights reserved.
//

#import "AnyPayTransaction.h"

@class ANPNetSecurePortfolio;
@interface ANPShadowTransactionRequest : AnyPayTransaction

@property (nonatomic, strong) ANPNetSecurePortfolio *$shadow;

@end
