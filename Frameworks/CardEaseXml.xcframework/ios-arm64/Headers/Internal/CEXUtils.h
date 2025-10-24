//
//  CEXUtils.h
//  CardEaseXmlClient
//
//  Created by Jacob Durbin on 22/04/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

@interface CEXUtils : NSObject
+(NSString *)bytesToHex:(NSData*)stringData;
+(NSString *)getPaddingForBytes:(int)binaryLength;
+(NSString *)generateZeroPaddingForLength:(int)length;
+(NSString *)hexToAscii:(NSString *)hexString;
+(NSData *)hexToBytes:(NSString *)hex;
+(Byte)asciiToHex:(Byte)char1 char2:(Byte)char2;
@end


