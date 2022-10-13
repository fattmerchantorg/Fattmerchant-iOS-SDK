//
//  ANPTerminalResponse.h
//  AnyPay
//
//  Created by Ankit Gupta on 21/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ANPMeaningfulMessage;
@interface ANPTerminalResponse : NSObject

@property (nonatomic) BOOL approved;
@property (nonatomic) BOOL isOffline;
@property (nonatomic, copy) NSString *batchData;
@property (nonatomic, copy) NSString *batchDataKsn;
@property (nonatomic, copy) NSString *cryptogram;
@property (nonatomic, copy) NSString *status;
@property (nonatomic, strong) ANPMeaningfulMessage *detail;

@end
