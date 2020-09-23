//
//  ANPKeyedTransactionWorkflow.h
//  AnyPay
//
//  Created by Ankit Gupta on 26/03/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPTransactionWorkflow.h"

@class ANPMeaningfulError;
@interface ANPKeyedTransactionWorkflow : ANPTransactionWorkflow

- (void)execute:(void (^)(BOOL, ANPMeaningfulError *))resultHandler;

@end
