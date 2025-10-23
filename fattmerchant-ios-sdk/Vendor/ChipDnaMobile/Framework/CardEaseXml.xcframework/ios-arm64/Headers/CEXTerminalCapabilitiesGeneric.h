//
//  CEXTerminalCapabilitiesGeneric.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CEXTerminalCapabilitiesGeneric : NSObject

@property (nonatomic, strong)NSString *attendedType;
@property (nonatomic, strong)NSString *deviceType;
@property (nonatomic, strong)NSString *mobilePos;
@property (nonatomic, strong)NSString *premises;

@end

NS_ASSUME_NONNULL_END
