/**
 * @author Creditcall Ltd
 * @brief Class to represent the current merchant account configuration.
 * @class MerchantData
 * @copyright Copyright Creditcall Limited 2010 - 2019.
 */

#import <Foundation/Foundation.h>
#import "Merchant.h"

@interface MerchantData : NSObject

/**
 * List of merchant accounts.
 * @see {@link Merchant}
 */
@property(nonatomic, strong, readonly)NSArray<Merchant *> *merchantInformation;

@end

