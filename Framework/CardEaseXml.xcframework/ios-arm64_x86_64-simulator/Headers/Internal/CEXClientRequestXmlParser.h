//
//  CEXClientRequestXmlParser.h
//  CardEaseXmlClient
//
//  Created by Sam Sutton on 21/02/2017.
//  Copyright © 2017 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CEXRequestType.h"
#import "CEXXmlEncoding.h"

@class CEXClientRequestXmlParser;

@protocol CEXClientRequestXmlParserDelegate <NSObject>
-(void)parserFinishedParsing:(BOOL)success terminalID:(NSString *)terminalID cardEaseReference:(NSString *)cardEaseReference requestType:(CEXRequestType)requestType apiKey:(NSString *)apiKey;
@end

@interface CEXClientRequestXmlParser : NSObject <NSXMLParserDelegate>

@property (nonatomic, weak) id<CEXClientRequestXmlParserDelegate> delegate;

-(instancetype)initWithRequestXml:(NSString *)requestXml encoding:(CEXXmlEncoding)encoding;
-(void)parse;
-(NSError *)parsingErrors;

@end
