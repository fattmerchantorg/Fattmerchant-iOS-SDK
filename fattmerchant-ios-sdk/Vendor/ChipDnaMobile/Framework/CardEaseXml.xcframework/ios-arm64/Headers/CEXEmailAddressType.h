/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXEmailAddressType field for a given
 *        string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXEmailAddressTypeParser
 */
#import <Foundation/Foundation.h>

/// @file

/** @brief The types of email address. **/
typedef enum
{
    EmailAddressTypeEmpty,      ///< A unspecified/unknown email address type.
    EmailAddressTypeHome,       ///< A home email address.
    EmailAddressTypeOther,      ///< An email address that does not fit another category
    EmailAddressTypeWork,       ///< A work email address.
    EmailAddressTypeUnknown,    ///< An email address with an unknown type.
} CEXEmailAddressType;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXEmailAddressType.
 *
 * @property EmailAddressTypeToString
 */
extern NSString *EmailAddressTypeToString[5];

@interface CEXEmailAddressTypeParser : NSObject

/**
 * @brief Returns the coresponding CEXEmailAddressType field for a given string value.
 *
 * @param code The string respresentation of an CEXEmailAddressType field.
 * @return The coresponding CEXEmailAddressType field.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXEmailAddressType)parse:(NSString *)code;
@end
