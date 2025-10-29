//
//  X509CertificateData.h
//  CardEaseXmlClient
//
//  Created by Jacob Durbin on 16/04/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import "ASN1ParsingUtils.h"
#import "CEXUtils.h"
#import <Foundation/Foundation.h>

@interface X509CertificateData : NSObject

@property (nonatomic, strong)NSString *issuerName;
@property (nonatomic, strong)NSDate *startDate;
@property (nonatomic, strong)NSDate *expiryDate;

-(X509CertificateData *)init;
-(X509CertificateData *)initWithData:(NSData *)data;

-(X509CertificateData *)parseASN1DataFromArray:(NSArray *)data;
@end
