/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXXmlEncoding field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXXmlEncodingParser
 */
#import <Foundation/Foundation.h>

/// @file

/** @brief The XML encoding used in communication with the CardEaseXML server. **/
typedef enum
{
    /**
     * @brief The encoding is unknown/unspecified.
     **/
	XmlEncodingEmpty,

    /**
     * @brief The XML will be encoded using US ASCII.
     **/
    XmlEncodingUsAscii,
    
    /**
     * @brief The XML will be encoded using UTF16.
     **/
	XmlEncodingUtf16,
    
    /**
     * @brief The XML will be encoded using UTF8.
     **/
	XmlEncodingUtf8
    
} CEXXmlEncoding;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXXmlEncoding.
 *
 * @property XmlEncodingToString
 **/
extern NSString *XmlEncodingToString[4];

@interface CEXXmlEncodingParser : NSObject

/**
 * @brief Returns the coresponding CEXXmlEncoding field for a given string value.
 *
 * @param code The string respresentation of an CEXXmlEncoding field.
 * @return The coresponding CEXXmlEncoding field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXXmlEncoding)parse:(NSString *)code;
+ (NSStringEncoding) getNativeEncoding:(CEXXmlEncoding)encoding;

@end
