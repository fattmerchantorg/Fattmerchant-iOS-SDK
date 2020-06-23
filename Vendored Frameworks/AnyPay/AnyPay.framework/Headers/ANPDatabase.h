//
//  ANPDatabase.h
//  AnyPay
//
//  Created by Ankit Gupta on 24/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IAnyPayTransaction.h"

@interface ANPDatabase : NSObject

+ (id<IAnyPayTransaction>)getTransactionWithId:(NSString *)ID;
+ (NSArray<id<IAnyPayTransaction>> *)getAllTransactions;
+ (NSArray *)getTransactionWithPredicate:(NSPredicate *)predicate;
+ (NSArray *)getTransactionWithPredicate:(NSPredicate *)predicate sortDescriptor:(NSSortDescriptor *)sortDescriptor;
+ (void)deleteAllTransactions:(NSError **)error;
+ (void)deleteTransactionWithID:(NSString *)ID error:(NSError * __autoreleasing *)error;

@end
