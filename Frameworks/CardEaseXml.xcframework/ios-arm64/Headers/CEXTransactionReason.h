/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXTransactionReason field for a given string value.
 * @copyright Copyright © 2019 CreditCall Ltd. All rights reserved.
 * @class CEXTransactionReasonParser
 */

#import <Foundation/Foundation.h>

/// @file

/** @brief Represents the type of values for CEXTransactionReason. **/
typedef enum
{
    TransactionReasonEmpty,             ///< No reason supplied.
    TransactionReasonUnscheduled,       ///< The transaction is unscheduled.
    TransactionReasonInstallment,       ///< The transaction is part of an installment.
    TransactionReasonIncremental,       ///< The transaction is part of an incremental payment plan.
    TransactionReasonResubmission,      ///< The transaction is being resubmitted.
    TransactionReasonDelayedCharge,     ///< The charge was delayed.
    TransactionReasonReAuth,            ///< The transaction is a reauthorization.
    TransactionReasonNoShow,            ///< The cardholder did not show up.
} CEXTransactionReason;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXTransactionReason.
 *
 * @property TransactionReasonToString
 */
extern NSString *TransactionReasonToString[9];

@interface CEXTransactionReasonParser : NSObject

/**
 * @brief Returns the coresponding CEXTransactionReason field for a given string value.
 *
 * @param code The string respresentation of an CEXTransactionReason field.
 * @return The coresponding CEXTransactionReason field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXTransactionReason)parse:(NSString *)code;
@end
