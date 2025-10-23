/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXAmountUnit field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXAmountUnitParser
 */
#import <Foundation/Foundation.h>

/// @file

/**
 * @brief The AmountUnit is used to describe the units with which an amount in a request
 *        is supplied to the CardEase platform.
 *
 * For example, 1.23 GBP can be specified as 1.23 Major or 123 Minor.
 **/
typedef enum
{
	AmountUnitEmpty, ///< The amount unit is empty/unspecified.
	AmountUnitMajor, ///< The amount is in major units.
	AmountUnitMinor, ///< The amount is in minor units.
} CEXAmountUnit;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXAmountUnit.
 *
 * @property AmountUnitToString
 */
extern NSString *AmountUnitToString[3];

@interface CEXAmountUnitParser : NSObject

/**
 * @brief Returns the coresponding CEXAmountUnit field for a given
 *        string value.
 *
 * @param code The string respresentation of a CEXAmountUnit field.
 * @return The coresponding CEXAmountUnit field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXAmountUnit)parse:(NSString *)code;
@end
