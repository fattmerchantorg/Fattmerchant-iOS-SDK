/**
 * @author Creditcall Ltd
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 * @file TransactionTypeIDEnum.h
 */
#ifndef TransactionTypeID_h
#define TransactionTypeID_h

/**
 * @typedef TransactionTypeIDValue
 *
 * The possible values returned for TransactionTypeIDEnum.
 */
typedef enum
{
    TransactionTypeIDEnumAuth                        = 0,     /**< (0)     Authorisation  */
    TransactionTypeIDEnumLinkedRefund                = 1,     /**< (1)     Linked Refund */
    TransactionTypeIDEnumAccountVerification         = 3,     /**< (3)     Account Verification */
    TransactionTypeIDEnumOfflineAuthorisation        = 4,     /**< (4)     Offline Authorisation  */
    TransactionTypeIDEnumVoiceReferral               = 5,     /**< (5)     Voice Referral */
    TransactionTypeIDEnumStandaloneRefund            = 6,     /**< (6)     Standalone Refund */
    TransactionTypeIDEnumDeferredAuth                = 30     /**< (30)    Deferred Authorization */
} TransactionTypeIDValue;

@interface TransactionTypeIDEnum : NSObject

@end

#endif /* TransactionTypeID_h */
