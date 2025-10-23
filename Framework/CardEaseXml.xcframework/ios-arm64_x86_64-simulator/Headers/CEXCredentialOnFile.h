/**
 * @author Creditcall Ltd
 * @brief Holds Credential-on-File information.
 * @copyright Copyright © 2019 CreditCall Ltd. All rights reserved.
 * @class CEXCredentialOnFile
 */
#import <Foundation/Foundation.h>
#import "CEXTransactionInitiatedBy.h"
#import "CEXTransactionReason.h"
#import "CEXTransactionFirstStore.h"

@interface CEXCredentialOnFile : NSObject

/**
 * @brief  Who initiated the transaction.
 * @property initiatedBy
 **/
@property CEXTransactionInitiatedBy initiatedBy;

/**
 * @brief  The reason for the transaction.  Must be specified if the transaction was intiated by the merchant.
 * @property reason
 **/
@property CEXTransactionReason reason;

/**
 * @brief  The CardEaseReference of the initial transaction.  Must be supplied when using CardReference and CardHash, otherwise leave as null.
 * @property cardEaseReference
 **/
@property NSString * cardEaseReference;

/**
 * @brief Optional flag to indicate whether this transaction is from a stored card. This is needed when using your own card store and sending card details in each transaction instead of a token.
 * @property firstStore
 **/
@property CEXTransactionFirstStore firstStore;

/**
 * @brief Creates a new Credential-On-File instance
 *
 * @param initiatedBy Who initiated the transaction.
 * @param reason The reason for the transaction.  Must be specified if the transaction was intiated by the merchant.
 * @param cardEaseReference The CardEaseReference of the initial transaction.  Must be supplied when using CardReference and CardHash, otherwise leave as null.
 * @param firstStore Optional flag to indicate whether this transaction is from a stored card. This is needed when using your own card store and sending card details in each transaction instead of a token.
 *
 * @return An initialised CEXCredentialOnFile instance.
 **/
-(CEXCredentialOnFile*) initWithInitiatedBy:(CEXTransactionInitiatedBy) initiatedBy
                                     reason:(CEXTransactionReason) reason
                                     cardEaseReference:(NSString*)cardEaseReference
                                     firstStore:(CEXTransactionFirstStore) firstStore;

/**
 * @brief Creates a new Credential-On-File instance
 *
 * @param initiatedBy Who initiated the transaction.
 * @param reason The reason for the transaction.  Must be specified if the transaction was intiated by the merchant.
 * @param cardEaseReference The CardEaseReference of the initial transaction.  Must be supplied when using CardReference and CardHash, otherwise leave as null.
 *
 * @return An initialised CEXCredentialOnFile instance.
 **/
-(CEXCredentialOnFile*) initWithInitiatedBy:(CEXTransactionInitiatedBy) initiatedBy
                                     reason:(CEXTransactionReason) reason
                                     cardEaseReference:(NSString*)cardEaseReference;

/**
 * @brief Creates a new Credential-On-File instance
 *
 * @param initiatedBy Who initiated the transaction.
 * @param reason The reason for the transaction.  Must be specified if the transaction was intiated by the merchant.
 *
 * @return An initialised CEXCredentialOnFile instance.
 **/
-(CEXCredentialOnFile*) initWithInitiatedBy:(CEXTransactionInitiatedBy) initiatedBy
                                     reason:(CEXTransactionReason) reason;
@end

