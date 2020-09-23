//
//  ANPLineItem.h
//  AnyPay
//
//  Created by Ankit Gupta on 21/04/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPAmount.h"

@interface ANPLineItem : AnyPayModel

@property (nonatomic) double ID;
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *type;
@property (nonatomic, strong) ANPAmount *amount;

- (instancetype)initWithName:(NSString *)name type:(NSString *)type amount:(ANPAmount *)amount;

@end
