//
//  CEXTerminalCapabilitiesCardInput.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CEXTerminalCapabilitiesCardInput : NSObject

@property (nonatomic, strong)NSString *keyed;
@property (nonatomic, strong)NSString *magstripe;
@property (nonatomic, strong)NSString *contactEMV;
@property (nonatomic, strong)NSString *contactlessEMV;

@end

NS_ASSUME_NONNULL_END
