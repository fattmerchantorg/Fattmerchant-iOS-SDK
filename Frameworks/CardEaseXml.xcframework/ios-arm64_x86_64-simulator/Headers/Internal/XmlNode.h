//
//  XmlNode.h
//  CardEaseXmlClient
//
//  Created by Ricky Lewis on 08/07/2020.
//  Copyright © 2020 CreditCall Ltd. All rights reserved.
//

@interface XmlNode : NSObject
@property (nonatomic, strong)NSString *elementName;
@property (nonatomic, strong)NSString *elementText;
@property (nonatomic, strong)XmlNode *parent;
@property (nonatomic, strong)NSMutableArray *children;
@property (nonatomic, strong)NSDictionary<NSString*, NSString*> *attributes;

-(instancetype)initWithTag:(NSString*)tag attributes:(NSDictionary<NSString *,NSString *> *)attributes;
-(void)addChild:(XmlNode*)child;
-(void)addChildren:(NSArray*)children;
-(XmlNode*)getChildWithElementName:(NSString*)name;
-(NSArray*)getChildrenWithElementName:(NSString*)name;
-(NSString*)xmlStringValue:(BOOL)startDocument;
-(NSString*)getInnerXmlString;
-(void)replaceXmlNodeContentsWithXmlNode:(XmlNode*)xmlNode;
@end
