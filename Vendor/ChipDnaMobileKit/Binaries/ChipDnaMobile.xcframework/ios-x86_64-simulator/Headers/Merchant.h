/**
 * @author Creditcall Ltd
 * @brief Class representing information about a merchant.
 * @class Merchant
 * @copyright Copyright Creditcall Limited 2010 - 2019.
 */

#import <Foundation/Foundation.h>
#import "CCCardSchemeIdEnum.h"
#import "Currency.h"

@interface Merchant : NSObject

/**
 * The card schemes enabled for this merchant.
 */
@property(nonatomic, strong, readonly)NSArray<NSNumber *> *cardSchemes;

/**
 * The transaction types enabled for this merchant.
 */
@property(nonatomic, strong, readonly)NSArray<NSNumber *> *transactionTypes;

/**
 * The currencies enabled for this merchant.
 */
@property(nonatomic, strong, readonly)NSArray<Currency *> *currencies;

/**
 * The mercahnt name.
 */
@property(nonatomic, strong, readonly)NSString *name;

/**
 * The merchant number used for processing.
 */
@property(nonatomic, strong, readonly)NSString *number;

/**
 * The merchant category code. Format is IOS 18245.
 */
@property(nonatomic, strong, readonly)NSString *categoryCode;

@end

