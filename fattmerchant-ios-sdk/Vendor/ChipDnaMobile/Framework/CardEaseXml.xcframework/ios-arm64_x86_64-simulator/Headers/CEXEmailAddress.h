/**
 * @author Creditcall Ltd
 * @brief A class used to hold email address information.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @details Each email address has an address and a type.
 * @class CEXEmailAddress
 */
#import <Foundation/Foundation.h>
#import "CEXEmailAddressType.h"

/// @file

@interface CEXEmailAddress : NSObject

/**
 * @brief The email address
 * @property address
 **/
@property (strong) NSString *address;

/**
 * @brief The type of email address
 * @property type
 **/
@property CEXEmailAddressType type;

/**
 * @brief Creates a new email address with the fields.
 *
 * @param theAddress The email address
 * @param theType The type of the email address
 * @return An initialised CEXEmailAddress instance.
 **/
- (CEXEmailAddress*) initWithAddress: (NSString*) theAddress : (CEXEmailAddressType) theType;

@end
