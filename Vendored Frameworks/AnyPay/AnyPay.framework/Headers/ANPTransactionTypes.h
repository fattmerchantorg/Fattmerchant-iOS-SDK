//
//  ANPTransactionTypes.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ANPSharedEnums.h"

@interface ANPTransactionTypes : NSObject

+ (ANPTransactionType)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPTransactionType)transactionType;

@end
