/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXThreeDSecureCardHolderEnrolled field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXThreeDSecureCardHolderEnrolledParser
 */
#import <Foundation/Foundation.h>

/// @file

/** @brief The result returned from a 3-D Secure enrollment checking. **/
typedef enum
{
    ThreeDSecureCardHolderEnrolledEmpty,    ///< Internal use to indicate no CardHolder Enrollment is known.
    ThreeDSecureCardHolderEnrolledNo,       ///< The card holder is not enrolled.
    ThreeDSecureCardHolderEnrolledNone,     ///< The 3-D Secure enrollment check did not return anything.
    ThreeDSecureCardHolderEnrolledUnknown,  ///< The 3-D Secure enrollment check returned "unknown" or "unable".
    ThreeDSecureCardHolderEnrolledYes,      ///< The card holder is enrolled.
} CEXThreeDSecureCardHolderEnrolled;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXThreeDSecureCardHolderEnrolled.
 *
 * @property ThreeDSecureCardHolderEnrolledToString
 */
extern NSString *ThreeDSecureCardHolderEnrolledToString[5];

@interface CEXThreeDSecureCardHolderEnrolledParser : NSObject

/**
 * @brief Returns the coresponding CEXThreeDSecureCardHolderEnrolled field for a given string value.
 *
 * @param code The string respresentation of an CEXThreeDSecureCardHolderEnrolled field.
 * @return The coresponding CEXThreeDSecureCardHolderEnrolled field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXThreeDSecureCardHolderEnrolled)parse:(NSString *)code;
@end
