//
//  CEXClient+Internal.h
//  CardEaseXmlClient
//
//  Created by Sam Sutton on 24/03/2017.
//  Copyright © 2017 CreditCall Ltd. All rights reserved.
//

#import "CEXClient.h"
#import "CEXKevlarState.h"
#import "CEXRequestInfo.h"

@interface CEXClient ()

-(void)performRequest:(NSString *)requestXml terminalID:(NSString *)terminalID cardEaseReference:(NSString *)cardEaseReference operationToken:(NSString *)operationToken operationGUID:(NSString *)operationGUID requestType:(CEXRequestType)requestType url:(CEXServerUrl *)url apiKey:(NSString *)apiKey;
-(void)performHead:(NSURL *)uri terminalID:(NSString *)terminalID cardEaseReference:(NSString *)cardEaseReference operationToken:(NSString **)operationToken operationGUID:(NSString **)operationGUID apiKey:(NSString *)apiKey;
-(BOOL)performPost:(NSURL *)uri terminalID:(NSString *)terminalID cardEaseReference:(NSString *)cardEaseReference operationToken:(NSString *)operationToken apiKey:(NSString *)apiKey;
-(void)didReceiveData:(NSData *)data;
-(NSString *)getResponseXML;
-(NSError *)getLastError;
-(void)didReceiveResponse:(NSURLResponse *)urlResponse;
-(void)didReceiveError:(NSError*) error;
-(void)resetClient;
-(NSError *)parseRequestInfo:(NSString *)requestXML;
-(CEXKevlarState)getKevlarState;
-(CEXRequestInfo *)getRequestInfo;
+(NSString *)kevlarRecoveryAction:(CEXRequestType)requestType response:(CEXResponse *)response state:(CEXKevlarState)state;
@end
