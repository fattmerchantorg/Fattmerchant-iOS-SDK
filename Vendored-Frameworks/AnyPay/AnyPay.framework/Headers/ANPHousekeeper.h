//
//  ANPHousekeeper.h
//  AnyPay
//
//  Created by Ankit Gupta on 10/06/20.
//  Copyright © 2020 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ANPHousekeeper : NSObject

+ (instancetype)sharedInstance;
- (void)queueForVoid:(NSString *)transactionID;
- (void)queueForUpdate:(NSString *)transactionID;

@end

NS_ASSUME_NONNULL_END
