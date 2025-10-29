/**
 * @author Creditcall Ltd
 * @brief ChipDna Mobile currency interface that is used to represent the currency and its readable attributes.
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 * @protocol Currency
 */

#import <Foundation/Foundation.h>

@interface Currency : NSObject
/**
 * Gets the numeric code for the currency.
 *
 * @return The currency numeric code.
 */
@property (nonatomic, readonly)NSString *numericCode;

/**
 * Gets the symbol for the currency.
 *
 * @return The currency symbol.
 */
@property (nonatomic, readonly)NSString *symbol;

/**
 * Gets the character code for the currency.
 *
 * @return The currency character code
 */
@property (nonatomic, readonly)NSString *charCode;

/**
 * Gets the exponent value for the currency.
 *
 * @return The exponent
 */
@property (nonatomic, readonly)NSInteger exponent;

/**
 * Get the currency object for the 3 character code.
 *
 * @return The {@link Currency} object for a given code. If not found nil is returned.
 */
+(Currency *)currencyForCharCode:(NSString *)charcode;
@end
