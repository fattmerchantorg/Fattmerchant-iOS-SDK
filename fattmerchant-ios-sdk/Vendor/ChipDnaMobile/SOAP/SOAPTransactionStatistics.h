/**
 * @author Creditcall Ltd
 * @brief This class contains the transaction statistics.
 * @class SOAPTransactionStatistics
 * @copyright Copyright Creditcall Limited 2010 - 2017.
 */

#import <Foundation/Foundation.h>
#import "SOAPEnums.h"

@interface SOAPTransactionStatistics : NSObject

/**
 * Gets the result ID of the transaction statistics.
 *
 * This property is read-only.
 *
 * @return The result ID of the transaction statistics.
 */
@property (readonly) ResultID resultID;

/**
 * Gets the state ID of the transaction statistics.
 *
 * This property is read-only.
 *
 * @return The state ID of the transaction statistics.
 */
@property (readonly) StateID stateID;

/**
 * Gets the transaction type ID of the transaction statistics.
 *
 * This property is read-only.
 *
 * @return The transaction type ID of the transaction statistics.
 */
@property (readonly) TransactionTypeID transactionTypeID;

/**
 * Gets the currency alpha code of the transaction statistics, as defined in ISO 4217.
 *
 * This property is read-only.
 *
 * @return The currency alpha code of the transaction statistics.
 */
@property (readonly) NSString *currencyAlpha;

/**
 * Gets the currency numeric code of the transaction statistics, as defined in ISO 4217.
 *
 * This property is read-only.
 *
 * @return The currency numeric code of the transaction statistics.
 */
@property (readonly) NSNumber *currencyCode;

/**
 * Gets the volume for group of the transaction statistics,
 * indicating the total number of the transactions.
 *
 * This property is read-only.
 *
 * @return The volume for group of the transaction statistics.
 */
@property (readonly) NSNumber *volumeForGroup;

/**
 * Gets the amount for group of the transaction statistics, 
 * indicating the total amount for all transactions.
 *
 * This property is read-only.
 *
 * @return The amount for group of the transaction statistics.
 */
@property (readonly) NSNumber *amountForGroup;

/**
 *  Create a SOAPTransactionStatistics instance with the specified details.
 *
 *  @param resultID The result ID of the transaction statistics.
 *  @param stateID The state ID of the transaction statistics.
 *  @param transactionTypeID The transaction type ID of the transaction statistics.
 *  @param currencyAlpha The currency alpha code of the transaction statistics.
 *  @param currencyCode The currency numeric code of the transaction statistics.
 *  @param volumeForGroup The volume for the group of the transaction statistics.
 *  @param amountForGroup The amount for the group of the transaction statistics.
 *
 *  @return A SOAPTransactionStatistics instance.
 */
-(id)initWithResultID:(ResultID)resultID stateID:(StateID)stateID transactionTypeID:(TransactionTypeID)transactionTypeID currencyAlpha:(NSString *)currencyAlpha currencyCode:(NSNumber *)currencyCode volumeForGroup:(NSNumber *)volumeForGroup amountForGroup:(NSNumber *)amountForGroup;
@end
