//
//  ANPWorldnetTerminalSettings.h
//  AnyPay
//
//  Created by Ankit Gupta on 26/03/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ANPTaxLineItem, ANPTipLineItem;
@interface ANPWorldnetTerminalSettings : NSObject

@property (nonatomic, strong) NSArray<ANPTaxLineItem *> *taxRates;
@property (nonatomic, strong) NSArray<ANPTipLineItem *> *tipRates;
@property (nonatomic) BOOL allowUnreferencedTransactions;
@property (nonatomic) BOOL allowTax;
@property (nonatomic) BOOL allowTip;
@property (nonatomic) BOOL allowKeyedTransactions;
@property (nonatomic) NSString *currency;

@end
