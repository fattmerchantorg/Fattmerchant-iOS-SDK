//
//  ANPCardReaderInterfaces.h
//  AnyPay
//
//  Created by Ankit Gupta on 17/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ANPCardReaderInterface) {
    ANPCardReaderInterfaceSwipe = 0,
    ANPCardReaderInterfaceInsert = 1,
    ANPCardReaderInterfaceTap,
    ANPCardReaderInterfaceSwipeOrInsert,
    ANPCardReaderInterfaceSwipeOrTap,
    ANPCardReaderInterfaceSwipeOrInsertOrTap,
    ANPCardReaderInterfaceInsertOrTap,
    ANPCardReaderInterfaceManualPanEntry,
    ANPCardReaderInterfaceQRCode
};

@interface ANPCardReaderInterfaces : NSObject

+ (ANPCardReaderInterface)enumValue:(NSString *)stringValue;
+ (NSString *)valueOf:(ANPCardReaderInterface)cardInterface;

@end
