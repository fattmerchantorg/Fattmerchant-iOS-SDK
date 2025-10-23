/**
 * @author Creditcall Ltd
 * @brief A class used to hold product information.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXProduct
 */
#import <Foundation/Foundation.h>
#import "CEXAmountUnit.h"
#import "CEXProductRisk.h"

/// @file

@interface CEXProduct : NSObject

/**
 * @brief The price of the product.
 * @property amount
 **/
@property (strong) NSString *amount;

/**
 * @brief The unit of the product price.
 * @property amountUnit
 * @see CEXAmountUnit
 **/
@property CEXAmountUnit amountUnit;

/**
 * @brief The category of the product.
 * @property category
 **/
@property (strong) NSString *category;

/**
 * @brief The code of the product.
 * @property code
 **/
@property (strong) NSString *code;

/**
 * @brief The currency code of the price.
 * @property currencyCode
 **/
@property (strong) NSString *currencyCode;

/**
 * @brief The description of the product.
 * @property description
 **/
@property (strong) NSString *description;

/**
 * @brief The name of the product.
 * @property name
 **/
@property (strong) NSString *name;

/**
 * @brief The quantity of the product.
 * @property quantity
 **/
@property (strong) NSString *quantity;

/**
 * @brief The risk of the product.
 * @property risk
 * @see CEXProductRisk
 **/
@property CEXProductRisk risk;

/**
 * @brief The type of the product.
 * @property type
 **/
@property (strong) NSString *type;

/**
 * @brief Creates a new product with the fields.
 *
 * @param theAmount The price of the product.
 * @param theAmountUnit The unit of the product price.
 * @param theCategory The category of the product.
 * @param theCode The code of the product.
 * @param theCurrencyCode The currency of the price.
 * @param theDescription The description of the product.
 * @param theName The name of the product.
 * @param theQuantity The quantity of the product.
 * @param theRisk The risk of the product.
 * @param theType The type of the product.
 *
 * @return An initialised CEXProduct instance.
 **/
-(CEXProduct*) initWithAmount : (NSString*) theAmount : (CEXAmountUnit) theAmountUnit : (NSString*)theCategory : (NSString*) theCode : (NSString*) theCurrencyCode : (NSString*) theDescription :(NSString*) theName : (NSString*) theQuantity : (CEXProductRisk) theRisk : (NSString*) theType;

@end
