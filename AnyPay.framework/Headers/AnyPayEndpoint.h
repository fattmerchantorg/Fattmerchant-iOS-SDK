//
//  AnyPayEndpoint.h
//  AnyPay
//
//  Created by Ankit Gupta on 11/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPEndpoint.h"
#import "AnyPayModel.h"

@interface AnyPayEndpoint : AnyPayModel<ANPEndpoint>

@property (nonatomic, copy) NSString * _Nullable gatewayUrl;
@property (nonatomic, copy) NSString * _Nullable provider;
@property (nonatomic, copy) NSString * _Nullable flavor;
@property (nonatomic) BOOL debug;
@property (nonatomic, copy) NSString * _Nullable serverDateFormat;
@property (nonatomic, strong) NSDictionary * _Nullable logging;

- (instancetype _Nonnull )initWithGatewayURL:(NSString *_Nonnull)url;

@end
