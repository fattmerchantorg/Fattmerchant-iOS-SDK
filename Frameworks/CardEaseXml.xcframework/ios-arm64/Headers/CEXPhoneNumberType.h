/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXPhoneNumberType field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXPhoneNumberTypeParser
 */
#import <Foundation/Foundation.h>

/// @file

/** @brief Represents the type of phone number. **/
typedef enum
{
    PhoneNumberTypeEmpty,       ///< An empty phone number.
    PhoneNumberTypeHome,        ///< A home phone number.
    PhoneNumberTypeMobile,      ///< A mobile phone number.
    PhoneNumberTypeOther,       ///< A phone number that does not match any other category.
    PhoneNumberTypeWork,        ///< A work phone number.
    PhoneNumberTypeUnknown,     ///< A phone number with an unknown type.
} CEXPhoneNumberType;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXPhoneNumberType.
 *
 * @property PhoneNumberTypeToString
 */
extern NSString *PhoneNumberTypeToString[6];

@interface CEXPhoneNumberTypeParser : NSObject

/**
 * @brief Returns the coresponding CEXPhoneNumberType field for a given string value.
 *
 * @param code The string respresentation of a CEXPhoneNumberType field.
 * @return The coresponding CEXPhoneNumberType field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXPhoneNumberType)parse:(NSString *)code;
@end
