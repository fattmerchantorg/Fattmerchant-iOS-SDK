/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXIAVFormat field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXIAVFormatParser
 */
#import <Foundation/Foundation.h>

/// @file

/** 
 * @brief The format of the IAV property. 
 **/
typedef enum
{
    IAVFormatAsciiHex, ///< The format of the value data is ASCII Hex. For example, FF00.
    IAVFormatBase64,   ///< The format of the value data is Base64.
} CEXIAVFormat;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXIAVFormat.
 *
 * @property IAVFormatToString
 */
extern NSString *IAVFormatToString[2];

@interface CEXIAVFormatParser : NSObject

/**
 * @brief Returns the coresponding CEXIAVFormat field for a given string value.
 *
 * @param code The string respresentation of an CEXIAVFormat field.
 * @return The coresponding CEXIAVFormat field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXIAVFormat)parse:(NSString *)code;
@end