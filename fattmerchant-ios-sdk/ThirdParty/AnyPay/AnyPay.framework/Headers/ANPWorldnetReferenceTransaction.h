//
//  ANPWorldnetReferenceTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 22/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPReferenceTransaction.h"

@interface ANPWorldnetReferenceTransaction : ANPReferenceTransaction

- (void)sendReceiptToEmail:(NSString * _Nullable)email phone:(NSString * _Nullable)phone resultHandler:(void (^ _Nullable)(BOOL sent, ANPMeaningfulError * _Nullable))resultHandler;

@end
