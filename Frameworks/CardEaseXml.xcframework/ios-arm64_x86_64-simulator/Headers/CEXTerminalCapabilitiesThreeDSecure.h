//
//  CEXTerminalCapabilitiesThreeDSecure.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CEXTerminalCapabilitiesThreeDSecure : NSObject

@property (nonatomic, strong)NSString *threeDSecureVersion1;
@property (nonatomic, strong)NSString *threeDSecureVersion2;
@property (nonatomic, strong)NSString *visa;
@property (nonatomic, strong)NSString *masterCard;
@property (nonatomic, strong)NSString *amex;

@end

NS_ASSUME_NONNULL_END
