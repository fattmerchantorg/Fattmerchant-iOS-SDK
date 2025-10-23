//
//  CEXTerminalCapabilitiesContactless.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CEXParameterKeys.h"

NS_ASSUME_NONNULL_BEGIN

@interface CEXTerminalCapabilitiesContactless : NSObject

@property (nonatomic, strong)NSString *fallForward;
@property (nonatomic, strong)NSString *singleTap;
@property (nonatomic, strong)NSString *magstripe;

@end

NS_ASSUME_NONNULL_END
