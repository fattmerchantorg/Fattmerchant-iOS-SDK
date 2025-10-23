/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXVoidReason field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXVoidReasonParser
 */
#import <Foundation/Foundation.h>

/// @file

/** 
 * @brief The reason for which a void request is being made.
 *
 * This must be specified for a void request to be valid.
 **/
typedef enum
{
    /**
     * @brief Internal use to indicate no void reason is unknown.
     **/
	VoidReasonEmpty,
    
    /**
     * @brief A communication failure has occured.
     **/
	VoidReasonCommunicationFailure,
    
    /**
     * @brief The terminal failed to print a receipt.
     **/
	VoidReasonPrintFailure,
    
    /**
     * @brief A reset or power failure has occurred.
     **/
	VoidReasonResetOrPowerFailure,
    
    /**
     * @brief A transaction has failed to complete.
     **/
	VoidReasonTransactionFailure,
    
    /**
     * @brief The terminal failed to vend the product.
     **/
	VoidReasonVendFailure,
    
} CEXVoidReason;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXVoidReason.
 *
 * @property VoidReasonToString
 **/
extern NSString *VoidReasonToString[6];

@interface CEXVoidReasonParser : NSObject

/**
 * @brief Returns the coresponding CEXVoidReason field for a given string value.
 *
 * @param code The string respresentation of an CEXVoidReason field.
 * @return The coresponding CEXVoidReason field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXVoidReason)parse:(NSString *)code;
@end
