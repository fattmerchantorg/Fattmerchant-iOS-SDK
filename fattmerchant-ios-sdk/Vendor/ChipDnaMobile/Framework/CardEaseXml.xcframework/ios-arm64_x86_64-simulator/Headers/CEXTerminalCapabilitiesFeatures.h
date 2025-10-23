//
//  CEXTerminalCapabilitiesFeatures.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CEXTerminalCapabilitiesFeatures : NSObject

@property (nonatomic, strong)NSString *cardCapture;
@property (nonatomic, strong)NSString *dcc;
@property (nonatomic, strong)NSString *multiCurrency;
@property (nonatomic, strong)NSString *partialAuth;
@property (nonatomic, strong)NSString *partialReversal;
@property (nonatomic, strong)NSString *voiceReferral;
@property (nonatomic, strong)NSString *onDeviceTipping;
@property (nonatomic, strong)NSString *endOfDayTipping;
@property (nonatomic, strong)NSString *digitalSignature;
@property (nonatomic, strong)NSString *screen;
@property (nonatomic, strong)NSString *printer;
@property (nonatomic, strong)NSString *receipt;

@end

NS_ASSUME_NONNULL_END
