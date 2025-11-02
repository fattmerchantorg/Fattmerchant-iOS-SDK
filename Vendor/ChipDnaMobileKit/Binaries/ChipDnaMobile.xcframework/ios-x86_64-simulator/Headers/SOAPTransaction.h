/**
 * @author Creditcall Ltd
 * @brief This class contains the details of the transaction.
 * @class SOAPTransaction
 * @copyright Copyright Creditcall Limited 2010 - 2017.
 */

#import <Foundation/Foundation.h>

@interface SOAPTransaction : NSObject

/**
 * Gets the GUID of the transaction.
 * 
 * This property is read-only.
 * 
 * @return The GUID of the transaction.
 */
@property (readonly) NSString *guid;

/**
 * Gets the UTC date and time of the transaction.
 *
 * It is in the "dd/MM/yyyy HH:mm:ss" format.
 *
 * This property is read-only.
 *
 * @return The UTC date and time of the transaction.
 */
@property (readonly) NSString *dateAndTimeUtc;

/**
 * Gets the currency alpha code of the transaction, as defined in ISO 4217.
 *
 * This property is read-only.
 *
 * @return The currency alpha code of the transaction.
 */
@property (readonly) NSString *currencyAlpha;

/**
 * Gets the currency numeric code of the transaction, as defined in ISO 4217.
 *
 * This property is read-only.
 *
 * @return The currency numeric code of the transaction.
 */
@property (readonly) NSNumber *currencyCode;

/**
 * Gets the amount of the transaction, in xx.xx format.
 * Refund amounts are in the format -xx.xx.
 *
 * This property is read-only.
 *
 * @return The amount of the transaction.
 */
@property (readonly) NSNumber *amount;

/**
 * Gets the first six and the last four digits of the card used for transaction.
 *
 * This property is read-only.
 *
 * @return The first six and the last four digits of the card.
 */
@property (readonly) NSString *cardDigits;

/**
 * Gets the local date and time of the transaction.
 *
 * It is in the "dd/MM/yyyy HH:mm:ss" format.
 *
 * This property is read-only.
 *
 * @return The local date and time of the transaction.
 */
@property (readonly) NSString *dateAndTimeLocal;

/**
 *  Creates a SOAPTransaction instance with the specified details.
 *
 *  @param GUID GUID of the transaction.
 *  @param dateAndTimeUTC The UTC date and time of the transaction.
 *  @param currencyAlpha The currency alpha code of the transaction.
 *  @param currencyCode The currency numeric code of the transaction.
 *  @param amount The amount of the transaction.
 *  @param cardDigits First six and last four digits of the card used for the transaction.
 *  @param dateAndTimeLocal The local date and time of the transaction.
 *
 *  @return A SOAPTransaction instance.
 */
-(id)initWithGUID:(NSString *)GUID dateAndTimeUTC:(NSString *)dateAndTimeUTC currencyAlpha:(NSString *)currencyAlpha currencyCode:(NSNumber *)currencyCode amount:(NSNumber *)amount cardDigits:(NSString *)cardDigits dateAndTimeLocal:(NSString *)dateAndTimeLocal;

@end
