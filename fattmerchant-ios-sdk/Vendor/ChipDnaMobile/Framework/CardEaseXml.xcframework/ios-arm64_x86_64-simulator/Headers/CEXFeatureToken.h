/**
* @author NMI
* @brief A Feature Token is a JSON Web Token which when provided is used to enable a custom or restricted feature / function.
* @copyright Copyright NMI 2010 - 2020.
* @class CEXFeatureToken
*/
#import <Foundation/Foundation.h>

@interface CEXFeatureToken : NSObject

/**
 * @brief The value of the FeatureToken
 * @property value
 **/
@property (strong) NSString *value;

/**
 * @brief Creates a new FeatureToken with no values
 *
 * The value defaults to nil.
 *
 * @return The initialised CEXFeatureToken instance.
 **/
-(CEXFeatureToken *)init;

/**
 * @brief Creates a new FeatureToken with the specified value.
 *
 * @param theValue The value of the FeatureToken.
 *
 * @return The initialised CEXFeatureToken instance.
 **/
-(CEXFeatureToken *)initWithValue:(NSString *)theValue;

@end
