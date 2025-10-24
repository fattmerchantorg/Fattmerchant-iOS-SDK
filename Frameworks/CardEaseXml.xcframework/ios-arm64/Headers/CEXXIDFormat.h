/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXXIDFormat field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXXIDFormatParser
 */
#import <Foundation/Foundation.h>

/// @file

/** @brief The format of the XID property. **/
typedef enum
{
    /**
     * @brief The format of the value data is ASCII. 
     *
     * For example, =XYZ
     **/
    XIDFormatAscii,
    
    /**
     * @brief The format of the value data is ASCII Hex.
     *
     * For example, FF00.
     **/
    XIDFormatAsciiHex,
    
    /**
     * @brief The format of the value data is Base64.
     **/
    XIDFormatBase64,
    
} CEXXIDFormat;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXXIDFormat.
 *
 * @property XIDFormatToString
 **/
extern NSString *XIDFormatToString[3];

@interface CEXXIDFormatParser : NSObject

/**
 * @brief Returns the coresponding CEXXIDFormat field for a given string value.
 *
 * @param code The string respresentation of an CEXXIDFormat field.
 * @return The coresponding CEXXIDFormat field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 **/
+(CEXXIDFormat) parse : (NSString *) code;
@end
