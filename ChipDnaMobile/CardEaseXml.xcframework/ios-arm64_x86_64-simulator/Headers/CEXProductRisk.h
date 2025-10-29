/**
 * @author Creditcall Ltd
 * @brief parser which returns the coresponding CEXProductRisk field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXProductRiskParser
 */
#import <Foundation/Foundation.h>

/// @file

/**
 * @brief Represents the risk that a particular product holds. 
 *
 * For example a high value product may have a higher risk, and a low value
 * product a lower risk.
 **/
typedef enum
{
    ProductRiskEmpty,       ///< The product risk is empty/unspecified.
    ProductRiskVeryLow,     ///< The product has very low risk.
    ProductRiskLow,         ///< The product has low risk.
    ProductRiskMedium,      ///< The product has medium risk.
    ProductRiskHigh,        ///< The product has high risk.
    ProductRiskVeryHigh,    ///< The product has very high risk.
} CEXProductRisk;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXProductRisk.
 *
 * @property ProductRiskToString
 */
extern NSString *ProductRiskToString[6];

@interface CEXProductRiskParser : NSObject

/**
 * @brief Returns the coresponding CEXProductRisk field for a given string value.
 *
 * @param code The string respresentation of a CEXProductRisk field.
 * @return The coresponding CEXProductRisk field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXProductRisk)parse:(NSString *)code;    
@end
