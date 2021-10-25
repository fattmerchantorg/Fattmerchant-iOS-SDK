//
//  ANPCardTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPTransaction.h"
#import "ANPCardReaderInterfaces.h"
#import "ANPSignature.h"
#import "ANPAccountType.h"

@class AnyPayCardReader, ANPCardReaderInterfaces;
@interface ANPCardTransaction : ANPTransaction

@property (nonatomic, strong) AnyPayCardReader *cardReader;
@property (nonatomic, strong) ANPSignature *signature;
@property (nonatomic) ANPCardReaderInterface cardInterface;

#pragma mark - Card Information
@property (nonatomic, copy) NSString *cardExpiryMonth;
@property (nonatomic, copy) NSString *cardExpiryYear;
@property (nonatomic, copy) NSString *maskedPAN;
@property (nonatomic, copy) NSString *cardType;
@property (nonatomic, copy) NSString *cardHolderName;
@property (nonatomic, copy) NSString *CVV2;
@property (nonatomic, copy) NSString *address;
@property (nonatomic, copy) NSString *postalCode;
@property (nonatomic, copy) NSString *cardNumber;
@property (nonatomic, copy) NSString *EPB;
@property (nonatomic, copy) NSString *EPBKsn;
@property (nonatomic) ANPCardholderVerificationMethod cardholderVerificationMethod;
@property (nonatomic) ANPAccountType accountType;

//Set for signature callback
@property (nonatomic, copy) void (^onSignatureRequired)(void);
@property (nonatomic, copy) void (^onAccountSelectionRequired)(NSArray<NSString *> *);

@end
