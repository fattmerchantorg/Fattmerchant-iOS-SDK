/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXThreeDSecureTransactionStatus field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXThreeDSecureTransactionStatusParser
 */
#import <Foundation/Foundation.h>

/// @file

/** @brief The result returned from a 3-D Secure authentication. **/
typedef enum
{
    /**
     * @brief Internal use to indicate no Transaction Status is known.
     **/
    ThreeDSecureTransactionStatusEmpty,
    
    /**
     * @brief The 3-D Secure authentication returned "attempted".
     *
     * All associated ECI, CAVV/AAV and XID data should also be sent.
     **/
    ThreeDSecureTransactionStatusAttempted,

    /**
     * @brief The 3-D Secure authentication returned "failure".
     **/
    ThreeDSecureTransactionStatusFailed,
    
    /**
     * @brief The 3-D Secure authentication did not return a value.
     **/
    ThreeDSecureTransactionStatusNone,
    
    /**
     * @brief The 3-D Secure authentication returned "success".
     *
     * All associated ECI, CAVV/AAV and XID data should also be sent.
     **/
    ThreeDSecureTransactionStatusSuccessful,
    
    /**
     * @brief The 3-D Secure authentication returned "unknown" or "unable".
     **/
    ThreeDSecureTransactionStatusUnknown,
    
} CEXThreeDSecureTransactionStatus;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXThreeDSecureTransactionStatus.
 *
 * @property ThreeDSecureTransactionStatusToString
 */
extern NSString *ThreeDSecureTransactionStatusToString[7];

@interface CEXThreeDSecureTransactionStatusParser : NSObject

/**
 * @brief Returns the coresponding CEXThreeDSecureTransactionStatus field for a given string value.
 *
 * @param code The string respresentation of an CEXThreeDSecureTransactionStatus field.
 * @return The coresponding CEXThreeDSecureTransactionStatus field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXThreeDSecureTransactionStatus)parse:(NSString *)code;
@end
