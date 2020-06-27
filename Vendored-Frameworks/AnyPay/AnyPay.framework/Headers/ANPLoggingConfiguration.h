//
//  ANPLoggingConfiguration.h
//  AnyPay
//
//  Created by Ankit Gupta on 12/09/19.
//  Copyright © 2019 Dan McCann. All rights reserved.
//

#import "AnyPayModel.h"

@interface ANPLoggingConfiguration : AnyPayModel

@property (nonatomic, copy) NSString *logLevel;
@property (nonatomic, strong) NSNumber *skipSequentialDuplicates;
@property (nonatomic, strong) NSNumber *realtimeLoggingEnabled;
@property (nonatomic, strong) NSNumber *remoteLoggingEnabled;

@end
