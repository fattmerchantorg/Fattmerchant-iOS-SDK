
/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXTransactionFirstStore field for a given string value.
 * @copyright Copyright © 2021 CreditCall Ltd. All rights reserved.
 * @class CEXTransactionFirstStoreParser
 */


#import <Foundation/Foundation.h>

/**
 * @brief Optional indicator as to whether this transaction is from a stored card. This is needed when using your own card store and sending card details in each transaction instead of a token.
 *
 * Represents the type of values for CEXTransactionFirstStore.
 */
typedef enum
{
    TransactionFirstStoreTrue,          ///< Used to override the default behaviour and indicate that this is a first store transaction, rather than determine the indicator based upon the way the card details are provided.
    TransactionFirstStoreFalse,         ///< Used to override the default behaviour and indicate that this is not a first store transaction, rather than determine the indicator based upon the way the card details are provided.
    TransactionFirstStoreDefault,       ///< TransactionFirstStore will, by default, be determined based on whether card details are provided. When tokenized card details are provided (CardHash and CardReference) it is assumed that this is not a first store transaction. When                                                                                    tokenized card details are not provided, but instead a PAN, etc. it is  assumed that this is a first store transaction.
} CEXTransactionFirstStore;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXTransactionFirstStore.
 *
 * @property TransactionFirstStoreToString
 */

extern NSString *TransactionFirstStoreToString[3];

@interface CEXTransactionFirstStoreParser : NSObject

/**
 * @brief Returns the coresponding CEXTransactionFirstStore field for a given string value.
 *
 * @param code The string respresentation of an CEXTransactionFirstStore field.
 * @return The coresponding CEXTransactionFirstStore field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */

+(CEXTransactionFirstStore)parse:(NSString *)code;
@end
