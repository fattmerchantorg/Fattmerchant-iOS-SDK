/**
 * @author Creditcall Ltd
 * @brief A class used to hold extended property information.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXExtendedProperty
 * @details Each extended property has a name and a value.
 */
#import <Foundation/Foundation.h>

@interface CEXExtendedProperty : NSObject 

/**
 * @brief The name of the extended property
 * @property name
 **/
@property (strong) NSString *name;

/**
 * @brief The value of the extended property
 * @property value
 **/
@property (strong) NSString *value;

/**
 * @brief Creates a new extended property with the specified name and value.
 *
 * @param theName The name of the extended property
 * @param theValue The value of the extended property
 **/
- (CEXExtendedProperty*)initWithProperty:(NSString*)name value:(NSString*)value;

@end
