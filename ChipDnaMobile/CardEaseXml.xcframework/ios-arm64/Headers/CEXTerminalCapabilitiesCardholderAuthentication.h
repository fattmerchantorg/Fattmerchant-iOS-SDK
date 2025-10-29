//
//  CEXTerminalCapabilitiesCardholderAuthentication.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CEXTerminalCapabilitiesCardholderAuthentication : NSObject

@property (nonatomic, strong)NSString *signature;
@property (nonatomic, strong)NSString *onlinePin;
@property (nonatomic, strong)NSString *pinRetry;
@property (nonatomic, strong)NSString *offlinePin;
@property (nonatomic, strong)NSString *pinBypass;
@property (nonatomic, strong)NSString *cdcvm;

@end

NS_ASSUME_NONNULL_END
