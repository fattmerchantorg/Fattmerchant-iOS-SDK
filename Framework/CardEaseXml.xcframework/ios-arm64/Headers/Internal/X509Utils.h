//
//  X509Utils.h
//  CardEaseXmlClient
//
//  Created by Ricky Lewis on 06/07/2020.
//  Copyright © 2020 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)

@interface X509Utils : NSObject
+(NSData*)getCertificateSerialNumber:(SecCertificateRef)certificate;
+(NSString*)getCertificateGetIssuerName:(SecCertificateRef)certificate;
@end
