/**
 * @author Creditcall Ltd
 * @brief The CEXAmountType.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXAmountType
 */
#import <Foundation/Foundation.h>

/**
 * @brief The AmountType is used to indicate if the amount is an actual or an estimate amount.
 *
 * For amount specified as Actual it will be expected that amount will not change between Auth and Confirm.
 **/
typedef enum
{
    /**
     * @brief Not known if amount is actual or estimate.
     **/
    AmountTypeUnknown,
    
    /**
     * @brief The amount is an actual final amount.
     **/
    AmountTypeActual,
    
    /**
     * @brief The amount is an estimated final amount.
     **/
    AmountTypeEstimate,
    
} CEXAmountType;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXAmountType.
 *
 * @property AmountTypeToString
 **/
extern NSString *AmountTypeToString[3];

@interface CEXAmountTypeParser : NSObject

/**
 * @brief Returns the coresponding CEXAmountType field for a given string value.
 *
 * @param code The string respresentation of an CEXAmountType field.
 * @return The coresponding CEXAmountType field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXAmountType)parse:(NSString *)code;
@end
