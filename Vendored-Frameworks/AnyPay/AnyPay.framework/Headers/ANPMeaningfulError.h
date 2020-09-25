//
//  ANPMeaningfulError.h
//  AnyPay
//
//  Created by Ankit Gupta on 16/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ANPMeaningfulMessage.h"

@interface ANPMeaningfulError : ANPMeaningfulMessage

@property (nonatomic) BOOL isFatal;
@property (nonatomic) NSInteger code;
@property (nonatomic, copy, readonly) NSString *domain;
@property (nonatomic) BOOL isTimeoutOrNetworkError;

- (instancetype)initWithSource:(id)source code:(NSInteger)code message:(NSString *)message detail:(NSString *)detail;
- (instancetype)initWithSource:(id)source code:(NSInteger)code message:(NSString *)message detail:(NSString *)detail title:(NSString *)title;

+ (instancetype)errorWithNSError:(NSError *)error;

- (NSString *)stringValue;

@end
