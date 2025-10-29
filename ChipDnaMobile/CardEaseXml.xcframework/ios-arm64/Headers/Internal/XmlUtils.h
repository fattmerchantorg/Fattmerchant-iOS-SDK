//
//  XmlUtils.h
//  CardEaseXmlClientTests
//
//  Created by Tom Wilson on 11/01/2019.
//  Copyright © 2019 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XmlNode.h"

@interface XmlUtils : NSObject
+(XmlNode*) getNodeListFromXml:(NSString *)xml;
+(NSString*)getFieldInXml:(NSString*)xmlStr fieldName:(NSString*)fieldName;
+(NSError*)replaceLastErrorWithDomain:(NSString *)domain code:(NSInteger)code userInfo:(NSDictionary *)dict;
@end
