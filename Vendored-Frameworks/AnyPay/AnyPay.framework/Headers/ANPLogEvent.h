//
//  ANPLoggingConfiguration.h
//  AnyPay
//
//  Created by Ankit Gupta on 12/09/19.
//  Copyright © 2019 Dan McCann. All rights reserved.
//

@interface ANPLogEvent : NSObject

@property (nonatomic, copy) NSString *type;
@property (nonatomic) char loggerIdentifier;
@property (nonatomic, copy) NSString *context;
@property (nonatomic, strong) NSDictionary *parameters;

- (instancetype)initWithType:(NSString *)type context:(NSString *)context;
- (instancetype)initWithIdentifier:(char)identifier type:(NSString *)type context:(NSString *)context parameters:(NSDictionary *)params;

//@property (nonatomic, strong) NSNumber *skipSequentialDuplicates;
//@property (nonatomic, strong) NSNumber *realtimeLoggingEnabled;
//@property (nonatomic, strong) NSNumber *remoteLoggingEnabled;

@end
