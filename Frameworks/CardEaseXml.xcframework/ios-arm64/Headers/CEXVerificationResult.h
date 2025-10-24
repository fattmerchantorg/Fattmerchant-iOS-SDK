/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXVerificationResult field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXVerificationResultParser
 */
#import <Foundation/Foundation.h>

/// @file

/** 
 * @brief The verification result that can be obtained from the CardEase platform
 *        when it verifies certain components during a CardEaseXML request.
 *
 * The components can include address, security code and zip/post code.
 **/
typedef enum
{
    /**
     * @brief The verification result is unknown.
     **/
	VerificationResultEmpty,
    
    /**
     * @brief The specified information matches the issuer records.
     **/
	VerificationResultMatched,
    
    /**
     * @brief The specified information was not checked against issuer records.
     **/
	VerificationResultNotChecked,

    /**
     * @brief The specified information did not match issuer records.
     **/
	VerificationResultNotMatched,
    
    /**
     * @brief The specified information was not supplied for checking.
     **/
	VerificationResultNotSupplied,

    /**
     * @brief The specified information partially matched the issuer records.
     **/
    VerificationResultPartialMatch,
    
} CEXVerificationResult;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXVerificationResult.
 *
 * @property VerificationResultToString
 * @see CEXVerificationResult
 */
extern NSString *VerificationResultToString[6];

@interface CEXVerificationResultParser : NSObject

/**
 * @brief Returns the coresponding CEXVerificationResult field for a given string value.
 *
 * @param code The string respresentation of a CEXVerificationResult field.
 * @return The coresponding CEXVerificationResult field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXVerificationResult)parse:(NSString *)code;
@end
