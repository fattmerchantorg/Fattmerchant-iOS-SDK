/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXResultCode field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXResultCodeParser
 */
#import <Foundation/Foundation.h>

/// @file

/** 
 * @brief The result code that can be obtained from the CardEase platform when it processes a CardEaseXML request.
 **/
typedef enum
{
    /**
     * @brief No result code.
     **/
	ResultCodeEmpty,
    
    /** 
     * @brief The requested transaction was approved.
     **/
	ResultCodeApproved,
    
    /** 
     * @brief The requested transaction was declined.
     **/
	ResultCodeDeclined,

    /**
     * @brief The requested transaction has been referred.
     *
     * The operator must call the bank, otherwise the transaction is considered
     * declined.
     */
	ResultCodeVoiceReferralRequired,    
    
    /**
     * @brief The requested transaction was partially approved.
     *
     * The approved amount is contained in {@link CEXResponse::amountOnlineApproved}.
     **/
    ResultCodePartialApproval,
    
    /**
     * @brief The requested test transaction was successful.
     **/
	ResultCodeTestOk,
    
} CEXResultCode;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXResultCode.
 *
 * @property ResultCodeToString
 */
extern NSString *ResultCodeToString[6];

@interface CEXResultCodeParser : NSObject

/**
 * @brief Returns the coresponding CEXResultCode field for a given string value.
 *
 * @param code The string respresentation of a CEXResultCode field.
 * @return The coresponding CEXResultCode field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXResultCode)parse:(NSString *)code;
@end
