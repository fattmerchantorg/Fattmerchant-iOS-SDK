/*
 * @author Creditcall Ltd
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXXmlWriter
 */
#import <Foundation/Foundation.h>

#import "CEXXmlEncoding.h"

@interface CEXXmlWriter : NSObject
{
	BOOL isNodeEmpty;
	BOOL isTagOpen;
	NSMutableArray *tags;
	NSMutableString *writer;
    CEXXmlEncoding encoding;
}

@property CEXXmlEncoding encoding;

-(CEXXmlWriter *)init;
-(CEXXmlWriter *)initWithEncoding:(CEXXmlEncoding)encoding;

-(NSString *)close;
-(void)writeAttribute:(NSString *)name value:(NSString *)value;
-(void)writeComment:(NSString *)comment;
-(void)writeElement:(NSString *)name text:(NSString *)text;
-(void)writeEndDocument;
-(void)writeEndElement;
-(void)writeStartDocument:(BOOL)standalone;
-(void)writeStartElement:(NSString *)name;
-(void)writeStartElement:(NSString *)name namespace:(NSString*)namespace;
-(void)writeString:(NSString *)text;

@end
