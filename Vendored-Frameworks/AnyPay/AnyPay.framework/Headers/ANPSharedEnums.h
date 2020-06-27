//
//  ANPSharedEnums.h
//  AnyPay
//
//  Created by Ankit Gupta on 25/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#ifndef ANPSharedEnums_h
#define ANPSharedEnums_h

typedef NS_ENUM(NSUInteger, ANPTransactionStatus) {
    ANPTransactionStatus_UNKNOWN = 0,
    ANPTransactionStatus_OPEN,
    ANPTransactionStatus_PENDING,
    ANPTransactionStatus_PRE_AUTHORIZED,
    ANPTransactionStatus_AUTHORIZED,
    ANPTransactionStatus_DECLINED,
    ANPTransactionStatus_PARTIALLY_APPROVED,
    ANPTransactionStatus_APPROVED,
    ANPTransactionStatus_SUSPENDED,
    ANPTransactionStatus_CANCELLED,
    ANPTransactionStatus_FINALIZED,
    ANPTransactionStatus_REVERSED,
    ANPTransactionStatus_DELETED,
    ANPTransactionStatus_FAILED,
    ANPTransactionStatus_QUEUED,
    ANPTransactionStatus_PROCESSING
};

typedef NS_ENUM(NSUInteger, ANPTransactionType) {
    ANPTransactionType_REVERSEAUTH = 0,
    ANPTransactionType_AUTHONLY,
    ANPTransactionType_CAPTURE,
    ANPTransactionType_VOID,
    ANPTransactionType_SALE,
    ANPTransactionType_REFUND,
    ANPTransactionType_ADMIN
};

#endif /* ANPSharedEnums_h */
