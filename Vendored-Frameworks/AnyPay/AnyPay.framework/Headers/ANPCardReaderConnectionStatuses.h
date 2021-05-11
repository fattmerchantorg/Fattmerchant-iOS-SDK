//
//  ANPCardReaderConnectionStatuses.h
//  AnyPay
//
//  Created by Ankit Gupta on 17/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, ANPCardReaderConnectionStatus) {
    ANPCardReaderConnectionStatusUNKNOWN = 0,
    ANPCardReaderConnectionStatusCONNECTING = 1,
    ANPCardReaderConnectionStatusCONNECTED,
    ANPCardReaderConnectionStatusDISCONNECTING,
    ANPCardReaderConnectionStatusDISCONNECTED
};

@interface ANPCardReaderConnectionStatuses : NSObject

+ (ANPCardReaderConnectionStatus)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPCardReaderConnectionStatus)status;

@end
