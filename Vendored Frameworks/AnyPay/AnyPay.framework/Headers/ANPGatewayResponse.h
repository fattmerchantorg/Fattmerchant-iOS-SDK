//
//  ANPGatewayResponse.h
//  AnyPay
//
//  Created by Ankit Gupta on 22/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AnyPayModel.h"

@interface ANPGatewayResponse : AnyPayModel

@property (nonatomic, strong) NSMutableArray *emvTagListArray;
@property (nonatomic, copy) NSString *approvalCode;
@property (nonatomic, copy) NSString *authorizedAmount;
@property (nonatomic, copy) NSString *cvvResponseCode;
@property (nonatomic, copy) NSString *currency;
@property (nonatomic, copy) NSString *uniqueRef;
@property (nonatomic, copy) NSString *avsResponseCode;
@property (nonatomic, strong) NSDate *transactionTime;
@property (nonatomic, copy) NSString *statusDescription;
@property (nonatomic, copy) NSString *code;
@property (nonatomic, copy) NSString *cardHolderName;
@property (nonatomic, copy) NSString *cardNumber;
@property (nonatomic, copy) NSString *cardType;
@property (nonatomic, copy) NSString *expiryDate;
@property (nonatomic) BOOL didPerformReversal;
@property (nonatomic, copy) NSString *secureCardReference;
@property (nonatomic, copy) NSString *secureCardMerchantRef;
@property (nonatomic, copy) NSString *cardExpiryMonth;
@property (nonatomic, copy) NSString *cardExpiryYear;
@property (nonatomic, copy) NSString *maskedPAN;

@property (nonatomic, strong) NSObject *transactionResponse;

//Raw JSON response from gateway
@property (nonatomic, strong) NSDictionary *rawResponse;

- (instancetype)initWithResponse:(id)response;

- (BOOL)isApproved;

- (id)getEmvPayload;

- (NSDictionary *)responseJson;

@end
