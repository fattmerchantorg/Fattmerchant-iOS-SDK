//
//  ANPLogger.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/03/20.
//  Copyright © 2020 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ANPLogger : NSObject

@property (nonatomic, copy) NSString *type;
@property (nonatomic) char identifier;
@property (nonatomic, strong) ANPLogConfigurationProperties *configuration;

- (instancetype)initWithType:(NSString *)type;
- (instancetype)initWithType:(NSString *)type configuration:(ANPLogConfigurationProperties *)config;
- (instancetype)initWithType:(NSString *)type identifier:(char)identifier;
- (instancetype)initWithType:(NSString *)type identifier:(char)identifier configuration:(ANPLogConfigurationProperties *)config;
- (void)applyConfiguration:(ANPLogConfigurationProperties *)configuration;

@end

NS_ASSUME_NONNULL_END
