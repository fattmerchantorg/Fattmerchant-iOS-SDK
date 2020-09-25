//
//  ANPReferenceTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 22/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPEmvTransaction.h"

@interface ANPReferenceTransaction : ANPEmvTransaction

@property (nonatomic, copy) NSString *refTransactionInternalId;
@property (nonatomic, strong) NSDate *refTransactionTime;

@end
