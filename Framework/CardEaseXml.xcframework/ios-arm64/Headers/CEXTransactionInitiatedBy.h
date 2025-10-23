/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXTransactionInitiatedBy field for a given string value.
 * @copyright Copyright © 2019 CreditCall Ltd. All rights reserved.
 * @class CEXTransactionInitiatedByParser
 */

#import <Foundation/Foundation.h>

/// @file

/** @brief Represents the type of values for CEXTransactionInitiatedBy. **/
typedef enum
{
    TransactionInitiatedByUnknown,            ///< Unknown transaction initiator.
    TransactionInitiatedByCardHolder,         ///< The cardholder started the transaction.
    TransactionInitiatedByMerchant,           ///< The merchant started the transaction.
} CEXTransactionInitiatedBy;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXTransactionInitiatedBy.
 *
 * @property TransactionInitiatedByToString
 */
extern NSString *TransactionInitiatedByToString[3];

@interface CEXTransactionInitiatedByParser : NSObject

/**
 * @brief Returns the coresponding CEXTransactionInitiatedBy field for a given string value.
 *
 * @param code The string respresentation of an CEXTransactionInitiatedBy field.
 * @return The coresponding CEXTransactionInitiatedBy field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXTransactionInitiatedBy)parse:(NSString *)code;
@end
