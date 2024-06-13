//
//  IDTUtility.h
//  IDTech
//
//  Created by Randy Palermo on 5/9/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//
#import "IDTMSRData.h"

@interface IDTUtility : NSObject

+ (NSBundle *)frameworkBundle;
+(NSString*) configGroupListing:(NSData*) data;
+(NSString*) dataToPrintableString:(NSData*)data;
+(NSDictionary*) processTLVUnencryptedGroup:(NSData*)param;
+(NSString*) getErrorString:(int)code;
+ (NSString*) hexToString:(NSData*)data startByte:(int)start length:(int)len;
+ (NSString*) dataToString:(NSData*)data startByte:(int)start length:(int)len;
+ (NSString *) getSubString:(NSString*)str startPosition:(int)start length:(int)len;
+ (unsigned int) hex2int:(NSString*)hex;
+ (NSData *)hexToData:(NSString*)str ;
+ (NSString*) dataToString:(NSData*)data;
+ (NSData*) extendedHextoHex:(NSData*)data;
+ (NSData*) stringToData:(NSString *)str;
+ (NSString*) hexToString:(NSString*)str;
+(NSString*) dataToHexString:(NSData*)data;
+(NSData*) encodeTags:(NSMutableDictionary*)tags;
+(NSMutableDictionary*) makeStringDict:(NSMutableDictionary*)tags;
+(NSMutableDictionary*) makeDataDict:(NSMutableDictionary*)tags;
+(NSDictionary*) processTLV:(NSData*)param;
+(NSDictionary*) TLVtoDICT:(NSData*)param;
+(NSDictionary*) TLVtoDICT_HEX_ASCII:(NSData*)param;
+(NSData*) DICTotTLV:(NSDictionary*)tags;
+ (NSString *) stringByPaddingTheLeftToLength:(NSString*)orig newLen:(NSUInteger)newLength withString:(NSString *) padString;
+ (NSString*) padLeftZeros:(NSString *)orig Length:(int)len;
+ (NSString*) padRightZeros:(NSString *)orig Length:(int)len;
+(NSData*) shortToDataReverse:(short)val;
+(NSData*) shortToData:(short)val;
+(NSData*) longToData:(long)val;
+(NSData*) byteToData:(Byte)val;
+(NSData*) computeSHA1:(NSData*) data;
+(NSDictionary*) combineDictionaries:(NSDictionary*)source dest:(NSDictionary*)dest overwrite:(BOOL)overwrite;
+(void)pause:(float)sec;
+(NSDictionary*) processTLVUnencrypted:(NSData*)param;
+(NSDictionary*) processTLVUnencryptedStringValue:(NSData*)param;
+(NSString*) dtv:(double)amt;
+(void) retrieveCTLSMessage:(Byte)scheme lang:(int)lang messageID:(Byte)messageID line1:(NSString**)line1 line2:(NSString**)line2;
+(NSMutableDictionary*) updateTags_Old_To_New:(NSDictionary*)tags;
+(NSString*) tagToString:(NSString*)tag value:(NSData*)value;
+ (NSString*) printDictionary:(NSDictionary<NSString*,NSData*>*)dict;
+ (NSString*) msrToKB:(NSData*)data;
+(IDTMSRData*) parseEnhancedEncryptedMSRFormat:(NSData*)data;
+(NSString*) getHashSha256:(NSString*)input;
+(NSString*) extractVersionString:(NSString*)str;
+(BOOL) version1Later2:(NSString*)v1 compare:(NSString*)v2;
+(BOOL) version1Earlier2:(NSString*)v1 compare:(NSString*)v2;
+(NSData*) decryptDUKPT:(NSData*)data BDK:(NSData*)BDK KSN:(NSData*)KSN;

@end
