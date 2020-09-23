//
//  ANPTransmissionStatuses.h
//  AnyPay
//
//  Created by Ankit Gupta on 12/06/20.
//  Copyright © 2020 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ANPTransmissionStatus) {
    ANPTransmissionStatus_UNSENT = 0,
    ANPTransmissionStatus_SENDING,
    ANPTransmissionStatus_SENT,
    ANPTransmissionStatus_RECEIVED,
    ANPTransmissionStatus_COMPLETED
};

@interface ANPTransmissionStatuses : NSObject

+ (ANPTransmissionStatus)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPTransmissionStatus)transmissionStatus;

@end

NS_ASSUME_NONNULL_END
