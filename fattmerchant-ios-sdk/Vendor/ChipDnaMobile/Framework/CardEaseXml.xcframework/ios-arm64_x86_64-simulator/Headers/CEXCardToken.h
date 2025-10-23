/**
* @author NMI
* @brief A Card Token is a surrogate value for a PAN (Primary Account Number). The algorithm used to generate the token will
*  determine the size, format and whether the token is reversible or not.
* @copyright Copyright NMI 2010 - 2020.
* @class CEXCardToken
*/

#import <Foundation/Foundation.h>

@interface CEXCardToken : NSObject

/**
 * @brief The CardToken algorithm.
 * @property algorithm
 **/
@property (strong) NSString *algorithm;

/**
 * @brief The CardToken key.
 * @property key
 */
@property (strong) NSString *key;

/**
 * @brief The value of the CardToken
 * @property value
 */
@property (strong) NSString *value;

/**
 * @brief Creates a new CardToken with no values
 *
 * The algorithm defaults to nil.
 * The key defaults to nil.
 * The value defaults to nil.
 *
 * @return The initialised CEXCardToken instance.
 */
-(CEXCardToken *)init;

/**
 * @brief Creates a new CardToken with the specified algorithm, key and value.
 *
 * @param theAlgorithm  The algorithm of the CardToken.
 * @param theKey The type of the CardToken.
 * @param theValue The value of the CardToken.
 *
 * @return The initialised CEXCardToken instance.
 */
-(CEXCardToken *)initWithAlgorithm:(NSString *)theAlgorithm key:(NSString *)theKey value:(NSString *)theValue;

@end
