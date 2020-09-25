//
//  ANPTransactionStatuses.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ANPSharedEnums.h"

@interface ANPTransactionStatuses : NSObject

+ (ANPTransactionStatus)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPTransactionStatus)transactionStatus;

+ (BOOL)checkTransactionApprovalStatusFor:(ANPTransactionStatus)status;
+ (BOOL)checkTransactionDeclinedStatusFor:(ANPTransactionStatus)status;

@end
