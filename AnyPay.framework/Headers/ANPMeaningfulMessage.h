//
//  ANPMeaningfulMessage.h
//  AnyPay
//
//  Created by Ankit Gupta on 16/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ANPMeaningfulMessage : NSObject

@property (nonatomic, strong) id source;
@property (nonatomic, copy) NSString *message;
@property (nonatomic, copy) NSString *detail;
@property (nonatomic, copy) NSString *suggestedAction;
@property (nonatomic, copy) NSString *title;

- (instancetype)initWithSource:(id)source message:(NSString *)message detail:(NSString *)detail;
- (instancetype)initWithSource:(id)source message:(NSString *)message detail:(NSString *)detail title:(NSString *)title;

- (NSString *)stringValue;
- (NSDictionary *)toDictionary;

@end
