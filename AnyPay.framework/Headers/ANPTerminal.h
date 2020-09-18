//
//  ANPTerminal.h
//  AnyPay
//
//  Created by Ankit Gupta on 15/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPUser.h"
#import "ANPMeaningfulError.h"
#import "AnyPayEndpoint.h"

@class JSONModel;
@protocol ANPTerminal <NSObject>

@property (nonatomic, strong) AnyPayEndpoint * _Nonnull endpoint;

@optional
@property (nonatomic, strong) ANPUser * _Nullable user;
@property (nonatomic, strong) NSDate * _Nullable authenticationExpiryDate;

@required
- (nullable instancetype)initWithEndpoint:(nonnull id <ANPEndpoint>)endpoint;

//- (nullable instancetype)init;
//
//+ (nullable instancetype)new;

- (void)authenticate:(void (^_Nonnull)(BOOL authenticated, ANPMeaningfulError *_Nullable))completionHandler;

@end
