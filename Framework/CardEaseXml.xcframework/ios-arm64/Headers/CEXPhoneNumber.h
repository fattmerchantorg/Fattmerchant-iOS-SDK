/**
 * @author Creditcall Ltd
 * @brief A class used to hold phone number information.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @details Each phone number has a number and a type.
 * @class CEXPhoneNumber
 */
#import <Foundation/Foundation.h>
#import "CEXPhoneNumberType.h"

/// @file

@interface CEXPhoneNumber : NSObject

/**
 * @brief The phone number.
 * @property number
 **/
@property (strong) NSString* number;

/**
 * @brief The type of the phone number.
 * @property type
 **/
@property CEXPhoneNumberType type;

/**
 * @brief Creates a new phone number with the fields.
 *
 * @param theNumber The phone number.
 * @param theType The type of the phone number
 * @return An initialised CEXPhoneNumber instance.
 **/
- (CEXPhoneNumber*) initWithNumber: (NSString*) theNumber : (CEXPhoneNumberType) theType;

@end
