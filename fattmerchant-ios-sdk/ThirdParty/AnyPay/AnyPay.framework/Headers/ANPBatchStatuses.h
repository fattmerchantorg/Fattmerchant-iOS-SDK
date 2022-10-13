//
//  ANPBatchStatuses.h
//  AnyPay
//
//  Created by Ankit Gupta on 22/12/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ANPBatchStatus) {
    ANPBatchStatus_UNKNOWN = 0,
    ANPBatchStatus_OPEN,
    ANPBatchStatus_CLOSED
};

@interface ANPBatchStatuses : NSObject

+ (ANPBatchStatus)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPBatchStatus)paymentMethod;

@end

NS_ASSUME_NONNULL_END
