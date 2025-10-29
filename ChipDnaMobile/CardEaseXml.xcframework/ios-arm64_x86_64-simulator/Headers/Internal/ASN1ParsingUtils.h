//
//  ASN1ParsingUtils.h
//  CardEaseXmlClient
//
//  Created by Jacob Durbin on 15/04/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ASN1ParsingUtils : NSObject
+(NSArray *)createHexArrayFromString:(NSString *)hexString;
+(NSArray *)parseASN1ArrayFromData:(NSData *)rawDataArray parsingOctet:(BOOL)octet parsingBitString:(BOOL)bitString;
@end
