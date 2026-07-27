/**
 * @author Creditcall Ltd
 * @brief This class contains the details of the transaction.
 * @class SOAPExtendedTransaction
 * @copyright Copyright Creditcall Limited 2010 - 2017.
 */

#import "SOAPTransaction.h"
#import "SOAPEnums.h"

@interface SOAPExtendedTransaction : SOAPTransaction

/**
 * Gets the result ID of the transaction.
 *
 * This property is read-only.
 *
 * @return The result ID of the transaction.
 */
@property (readonly) ResultID resultID;

/**
 * Gets the state ID of the transaction.
 *
 * This property is read-only.
 *
 * @return The state ID of the transaction.
 */
@property (readonly) StateID stateID;

/**
 * Gets the settlement state ID of the transaction.
 *
 * This property is read-only.
 *
 * @return The settlement state ID of the transaction.
 */
@property (readonly) SettlementStateID settlementStateID;

/**
 * Gets the user reference of the transaction.
 *
 * This property is read-only.
 *
 * @return The user reference of the transaction.
 */
@property (readonly) NSString *userReference;

/**
 * Gets the card scheme ID of the transaction.
 *
 * This property is read-only.
 *
 * @return The card scheme ID of the transaction.
 */
@property (readonly) CardSchemeID cardSchemeID;

/**
 * Gets the card type ID of the transaction.
 *
 * This property is read-only.
 *
 * @return The card type ID of the transaction.
 */
@property (readonly) CardTypeID cardTypeID;

/**
 * Gets the transaction type ID of the transaction.
 *
 * This property is read-only.
 *
 * @return The transaction type ID of the transaction.
 */
@property (readonly) TransactionTypeID transactionTypeID;

/**
 * Gets the refunded amount of the transaction.
 *
 * This property is read-only.
 *
 * @return The refunded amount of the transaction.
 */
@property (readonly) NSNumber *amountRefunded;

/**
 * Gets the expiry date of the card used for the transaction.
 *
 * It is in the "yyyy-MM" format.
 *
 * This property is read-only.
 *
 * @return The expiry date of the card.
 */
@property (readonly) NSString *expiryDate;

/**
 * Indicates whether the transaction is refundable.
 *
 * This property is read-only.
 *
 * @return YES of the transaction is refundable, otherwise NO.
 */
@property (readonly) BOOL refundable;

/**
 * Indicates whether the transaction is voidable.
 *
 * This property is read-only.
 *
 * @return YES of the transaction is voidable, otherwise NO.
 */
@property (readonly) BOOL voidable;

/**
 *  Create a SOAPExtendedTransaction instance with the specified details.
 *
 *  @param GUID GUID of the transaction.
 *  @param dateAndTimeUTC The UTC date and time of the transaction.
 *  @param currencyAlpha The currency alpha code of the transaction.
 *  @param currencyCode The currency numeric code of the transaction.
 *  @param amount The amount of the transaction.
 *  @param cardDigits First six and last four digits of the card used for the transaction.
 *  @param resultID The result ID of the transaction.
 *  @param stateID The state ID of the transaction.
 *  @param settlementStateID The settlement state of the transaction.
 *  @param userReference The user reference of the transaction.
 *  @param cardSchemeID The card scheme ID of the transaction.
 *  @param cardTypeID The card type ID of the transaction.
 *  @param transactionTypeID The transaction type ID of the transaction.
 *  @param amountRefunded The amount refunded of the transaction.
 *  @param expiryDate The expiry date of the card used for the transaction.
 *  @param refundable Indicates whether the transaction is refundable.
 *  @param voidable Indicated whether the transaction is voidable.
 *  @param dateAndTimeLocal The local date and time of the transaction.
 *
 *  @return A SOAPExtendedTransaction instance.
 */
-(id)initWithGUID:(NSString *)GUID dateAndTimeUTC:(NSString *)dateAndTimeUTC currencyAlpha:(NSString *)currencyAlpha currencyCode:(NSNumber *)currencyCode amount:(NSNumber *)amount cardDigits:(NSString *)cardDigits resultID:(ResultID)resultID stateID:(StateID)stateID settlementStateID:(SettlementStateID)settlementStateID userReference:(NSString *)userReference cardSchemeID:(CardSchemeID)cardSchemeID cardTypeID:(CardTypeID)cardTypeID transactionTypeID:(TransactionTypeID)transactionTypeID amountRefunded:(NSNumber *)amountRefunded expiryDate:(NSString *)expiryDate refundable:(BOOL)refundable voidable:(BOOL)voidable dateAndTimeLocal:(NSString *)dateAndTimeLocal;

@end
