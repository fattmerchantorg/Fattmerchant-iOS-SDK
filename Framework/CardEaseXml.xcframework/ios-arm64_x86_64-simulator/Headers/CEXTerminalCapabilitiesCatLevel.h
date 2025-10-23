//
//  CEXTerminalCapabilitiesCatLevel.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CEXParameterKeys.h"

NS_ASSUME_NONNULL_BEGIN

@interface CEXTerminalCapabilitiesCatLevel : NSObject

extern NSString * const automatedDispensingMachine;
extern NSString * const selfService;

@property (nonatomic, strong)NSMutableDictionary<NSString *,NSString *> *catLevelDictionary;

@end

NS_ASSUME_NONNULL_END
