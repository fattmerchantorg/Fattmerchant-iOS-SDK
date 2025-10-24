/**
 * @author Creditcall Ltd
 * @brief A class used to hold currency data.
 * @copyright Copyright Creditcall Limited 2010 - 2016.
 * @details some responses from the platform may contain currency data.
 * @class CEXCurrencyData
 */

/// @file

@interface CEXCurrencyData : NSObject

/**
 * @brief The currency numeric code
 * @property numericCode
 **/
@property (strong) NSString* numericCode;

/**
 * @brief The currency symbol
 * @property symbol
 **/
@property (strong) NSString* symbol;

/**
 * @brief The currency char code
 * @property charCode
 **/
@property (strong) NSString* charCode;

/**
 * @brief The currency exponent
 * @property exponent
 **/
@property NSUInteger exponent;


/**
 * @brief Creates a new CurrencyData object.
 *
 * @param theNumericCode The email address
 * @param theSymbol The type of the email address
 * @param theCharCode The email address
 * @param theExponent The type of the email address
 * @return An initialised CEXEmailAddress instance.
 **/
- (CEXCurrencyData*) initWithNumericCode:(NSString *) theNumericCode theSymbol:(NSString *) theSymbol  theCharCode:(NSString *) theCharCode theExponent:(NSUInteger) theExponent;

@end