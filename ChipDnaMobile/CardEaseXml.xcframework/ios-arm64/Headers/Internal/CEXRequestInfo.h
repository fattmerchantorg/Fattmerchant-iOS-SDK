//
//  CEXRequestInfo.h
//  CardEaseXmlClient
//
//  Created by Sam Sutton on 22/02/2017.
//  Copyright © 2017 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CEXRequestType.h"

@interface CEXRequestInfo : NSObject

@property (nonatomic, readonly) NSString *terminalID;
@property (nonatomic, readonly) NSString *cardEaseReference;
@property (nonatomic, readonly) CEXRequestType requestType;
@property (nonatomic, readonly) NSString *apiKey;

-(instancetype)initWithTerminalID:(NSString *)terminalID cardEaseReference:(NSString *)cardEaseReference requestType:(CEXRequestType)requestType apiKey:(NSString *)apiKey;

@end
