//
//  ANPLogStream.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/03/20.
//  Copyright © 2020 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 *  Log levels are used to filter out logs. Used together with flags.
 */
typedef NS_ENUM(NSUInteger, ANPLogLevel){
    
    /**
     *  Error logs only
     */
    ANPLogLevelError     = 1,
    
    /**
     *  Error and warning logs
     */
    ANPLogLevelWarning   = 2,
    
    /**
     *  Error, warning and info logs
     */
    ANPLogLevelInfo      = 3,
    
    /**
     *  Error, warning, info and debug logs
     */
    ANPLogLevelDebug     = 4,
    
    /**
     *  Error, warning, info, debug and verbose logs
     */
    ANPLogLevelVerbose   = 5,
    
};

#define ANP_LOG_MACRO(logStream, cdn, lvl, fnct, frmt, ...) \
[logStream log:lvl condition:cdn function:fnct file:__FILE__ line:__LINE__ format:(frmt), ## __VA_ARGS__];

#define ANP_LOG_MACRO_WITH_PARAMS(logStream, cdn, lvl, fnct, msg, frmt) \
[logStream log:lvl function:fnct file:__FILE__ line:__LINE__ message:msg params:frmt];

#define ANPLogError(logStream, frmt, ...)   ANP_LOG_MACRO(logStream, YES, ANPLogLevelError, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define ANPLogWarn(logStream, frmt, ...)    ANP_LOG_MACRO(logStream, YES, ANPLogLevelWarning, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define ANPLogInfo(logStream, frmt, ...)    ANP_LOG_MACRO(logStream, YES, ANPLogLevelInfo, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define ANPLogDebug(logStream, frmt, ...)   ANP_LOG_MACRO(logStream, YES, ANPLogLevelDebug, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define ANPLogVerbose(logStream, frmt, ...) ANP_LOG_MACRO(logStream, YES, ANPLogLevelVerbose, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)

#define ANPLogErrorIf(logStream, condtion, frmt, ...)   ANP_LOG_MACRO(logStream, condtion, ANPLogLevelError, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define ANPLogWarnIf(logStream, condtion, frmt, ...)    ANP_LOG_MACRO(logStream, condtion, ANPLogLevelWarning, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define ANPLogInfoIf(logStream, condtion, frmt, ...)    ANP_LOG_MACRO(logStream, condtion, ANPLogLevelInfo, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define ANPLogDebugIf(logStream, condtion, frmt, ...)   ANP_LOG_MACRO(logStream, condtion, ANPLogLevelDebug, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)
#define ANPLogVerboseIf(logStream, condtion, frmt, ...) ANP_LOG_MACRO(logStream, condtion, ANPLogLevelVerbose, __PRETTY_FUNCTION__, frmt, ##__VA_ARGS__)

#define ANPLogErrorWithParams(logStream, message, params)   ANP_LOG_MACRO_WITH_PARAMS(logStream, YES, ANPLogLevelError, __PRETTY_FUNCTION__, message, params)
#define ANPLogWarnWithParams(logStream, message, params)    ANP_LOG_MACRO_WITH_PARAMS(logStream, YES, ANPLogLevelWarning, __PRETTY_FUNCTION__, message, params)
#define ANPLogInfoWithParams(logStream, message, params)    ANP_LOG_MACRO_WITH_PARAMS(logStream, YES, ANPLogLevelInfo, __PRETTY_FUNCTION__, message, params)
#define ANPLogDebugWithParams(logStream, message, params)   ANP_LOG_MACRO_WITH_PARAMS(logStream, YES, ANPLogLevelDebug, __PRETTY_FUNCTION__, message, params)
#define ANPLogVerboseWithParams(logStream, message, params) ANP_LOG_MACRO_WITH_PARAMS(logStream, YES, ANPLogLevelVerbose, __PRETTY_FUNCTION__, message, params)


NS_ASSUME_NONNULL_BEGIN

@class ANPLogger, ANPHTTPSession;
@interface ANPLogStream : NSObject

@property (nonatomic, copy) NSString *context;
@property (nonatomic, copy) NSString *type;

+ (instancetype)withContext:(NSString *)context;
+ (instancetype)withContext:(NSString *)context type:(NSString *)type;
- (void)log:(ANPLogLevel)level condition:(BOOL)condition function:(const char *)function file:(const char *)file line:(NSUInteger)line format:(NSString *)format, ...;
- (void)log:(ANPLogLevel)level function:(const char *)function file:(const char *)file line:(NSUInteger)line message:( NSString * _Nullable )message params:(NSDictionary *)params;
+ (BOOL)addLogger:(ANPLogger *)logger;
+ (BOOL)removeLogger:(char)identifier;

+ (void)addEventProperty:(NSString *)key value:(id)value;
+ (void)removeEventProperty:(NSString *)key;

- (void)trace;
- (void)trace:(NSString *)message;
- (void)traceIf:(BOOL)condition message:(NSString *)message;
- (void)debug:(NSString *)message;
- (void)debugIf:(BOOL)condition message:(NSString *)message;
- (void)info:(NSString *)message;
- (void)infoIf:(BOOL)condition message:(NSString *)message;
- (void)warn:(NSString *)message;
- (void)warnIf:(BOOL)condition message:(NSString *)message;
- (void)error:(NSString *)message;
- (void)errorIf:(BOOL)condition message:(NSString *)message;
- (void)errorWithParams:(NSDictionary *)parameters;
- (void)logWithParams:(NSDictionary *)parameters;
- (void)recordHTTPSession:(ANPHTTPSession *)session;

+ (void)applyConfiguration:(AnyPayTerminal *)terminal;

+ (ANPLogger *)getLogger:(NSString *)type;

+ (NSString *)getFileLogsAsString;

+ (BOOL)isConfigurationApplied;

@end

NS_ASSUME_NONNULL_END
