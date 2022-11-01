//
//  ANPLogConfigurationProperties.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/03/20.
//  Copyright © 2020 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ANPLogConfigurationProperties : AnyPayModel

@property (nonatomic) BOOL remoteLoggingEnabled;
@property (nonatomic) BOOL realtimeLoggingEnabled;
@property (nonatomic) int realtimeLoggingTimeoutInterval;
@property (nonatomic) BOOL batchingEnabled;
@property (nonatomic) BOOL logToFile;
@property (nonatomic) BOOL logToConsole;
@property (nonatomic) BOOL pciCompliant;
@property (nonatomic) int batchingInterval;
@property (nonatomic, copy) NSString *streamLogTo;
@property (nonatomic, copy) NSString *logLevel;

@property (nonatomic, strong) NSMutableDictionary *customFields;

@end

NS_ASSUME_NONNULL_END
