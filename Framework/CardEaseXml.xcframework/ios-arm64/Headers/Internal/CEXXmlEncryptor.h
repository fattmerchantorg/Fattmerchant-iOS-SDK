//
//  CEXXmlEncryptor.h
//  CardEaseXmlClient
//
//  Created by Ricky Lewis on 22/06/2020.
//  Copyright © 2020 CreditCall Ltd. All rights reserved.
//

#import "XmlNode.h"

@interface CEXXmlEncryptor : NSObject

-(instancetype)initWithCertificate:(SecCertificateRef)certificate;
-(BOOL)encryptElement:(XmlNode*)elementNode;
@end
