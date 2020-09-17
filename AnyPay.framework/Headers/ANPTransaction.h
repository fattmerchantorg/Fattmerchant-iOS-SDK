//
//  ANPTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "IAnyPayTransaction.h"
#import "ANPCardholderVerificationMethods.h"
#import "ANPTransactionTypes.h"
#import "ANPPaymentStatuses.h"
#import "ANPTransactionStatuses.h"
#import "ANPPaymentMethods.h"
#import "ANPTransactionWorkflow.h"
#import "ANPGatewayResponse.h"
#import "ANPTipLineItem.h"
#import "ANPTaxLineItem.h"
#import "ANPFeeLineItem.h"
#import "AnyPayTransactionBase.h"
#import "ANPBatchStatuses.h"
#import "ANPTransmissionStatuses.h"

@class ANPAmount;
@interface ANPTransaction : AnyPayTransactionBase<IAnyPayTransaction>

@property (nonatomic, strong) ANPTransactionWorkflow *transactionWorkflow;
@property (nonatomic, copy) NSString *internalID;
@property (nonatomic, copy) NSString *externalID;
@property (nonatomic, copy) NSString *refTransactionID;
@property (nonatomic, copy) NSString *paymentID;
@property (nonatomic, strong) NSDate *transactionTime;
@property (nonatomic) ANPTransactionType transactionType;
@property (nonatomic) ANPPaymentMethod paymentMethod;
@property (nonatomic) ANPTransactionStatus currentStatus;
@property (nonatomic) ANPTransactionStatus pendingStatus;
@property (nonatomic) ANPPaymentStatus paymentStatus;
@property (nonatomic) ANPBatchStatus batchStatus;
@property (nonatomic, copy) NSString *currency;
@property (nonatomic, copy) NSString *user;
@property (nonatomic, strong) ANPAmount *totalAmount;
@property (nonatomic, strong) ANPAmount *subtotal;
@property (nonatomic, strong) ANPAmount *tax;
@property (nonatomic, strong) ANPAmount *fee;
@property (nonatomic, strong) ANPAmount *tip;
@property (nonatomic, strong, readonly) NSArray<ANPTaxLineItem *> <ANPTaxLineItem> *taxItems;
@property (nonatomic, strong, readonly) NSArray<ANPTipLineItem *> <ANPTipLineItem> *tipItems;
@property (nonatomic, strong, readonly) NSArray<ANPFeeLineItem *> <ANPFeeLineItem> *feeItems;
@property (nonatomic, copy) NSString *notes;
@property (nonatomic, copy) NSString *responseText;
@property (nonatomic, copy) NSString *approvalCode;
@property (nonatomic, copy) NSString *customerEmail;
@property (nonatomic, copy) NSString *customerPhone;
@property (nonatomic, copy) NSString *invoiceNumber;
@property (nonatomic, strong) ANPAmount *approvedAmount;
@property (nonatomic, strong) ANPAmount *amountAvailableToRefund;
@property (nonatomic, strong) ANPGatewayResponse *gatewayResponse;
@property (nonatomic, getter=isApproved) BOOL approved;
@property (nonatomic, strong, readonly) NSDictionary<NSString *, id> *customProperties;
@property (nonatomic) ANPTransmissionStatus transmissionStatus;
@property (nonatomic, strong, readonly) NSArray<NSString *> *warnings;

- (instancetype)initWithType:(ANPTransactionType)type;

- (void)addTaxItem:(ANPTaxLineItem *)taxItem;
- (ANPTaxLineItem *)getTaxItem:(NSString *)name;
- (void)removeTaxItem:(NSString *)name;

- (void)addTipItem:(ANPTipLineItem *)tipItem;
- (ANPTipLineItem *)getTipItem:(NSString *)name;
- (void)removeTipItem:(NSString *)name;

- (void)addFeeItem:(ANPFeeLineItem *)feeItem;
- (ANPFeeLineItem *)getFeeItem:(NSString *)name;
- (void)removeFeeItem:(NSString *)name;

- (ANPAmount *)calculateAmounts;
- (ANPAmount *)calculateTaxAmount;
- (ANPAmount *)calculateTipAmount;
- (ANPAmount *)calculateFeeAmount;
- (BOOL)validateCalculatedAmount;

- (BOOL)isCancelled;
- (BOOL)isSuspended;
- (BOOL)isFinalized;
- (id<IAnyPayTransaction>)createReversal;

- (void)setOnSignatureRequired:(void (^)(void))signatureRequiredHandler;

- (void)persist;

- (void)addCustomProperty:(NSString *)key value:(id)value;
- (void)removeCustomProperty:(NSString *)key;

- (void)addWarning:(NSString *)warning;

@end
