/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXIccTagValueType field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXIccTagValueTypeParser
 */

#import <Foundation/Foundation.h>

/// @file

/** @brief Represents the type of value data held in an EMV ICC tag. **/
typedef enum
{
	IccTagValueTypeEmpty,       ///< The format is empty/unspecified.
	IccTagValueTypeAsciiHex,    ///< The format of the ICC value data is ASCII Hex. For example, FF00.
	IccTagValueTypeString       ///< The format of the ICC value data is a String. For example, REQ01.
} CEXIccTagValueType;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXIccTagValueType.
 *
 * @property IccTagValueTypeToString
 */
extern NSString *IccTagValueTypeToString[3];

@interface CEXIccTagValueTypeParser : NSObject

/**
 * @brief Returns the coresponding CEXIccTagValueType field for a given string value.
 *
 * @param code The string respresentation of an CEXIccTagValueType field.
 * @return The coresponding CEXIccTagValueType field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXIccTagValueType)parse:(NSString *)code;
@end
