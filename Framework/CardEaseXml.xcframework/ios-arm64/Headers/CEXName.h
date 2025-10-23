/**
 * @author Creditcall Ltd
 * @brief A class used to hold name information
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXName
 */
#import <Foundation/Foundation.h>

/// @file

@interface CEXName : NSObject

/**
 * @brief The first name of the person.
 * @property firstName
 **/
@property (strong) NSString *firstName;

/**
 * @brief The initials of the person.
 * @property initials
 **/
@property (strong) NSString *initials;

/**
 * @brief The last name of the person.
 * @property lastName
 **/
@property (strong) NSString *lastName;

/**
 * @brief The title of the person.
 * @property title
 **/
@property (strong) NSString *title;

/**
 * @brief Creates a new name with the fields.
 *
 * @param theTitle The title of the person
 * @param theFirstName The first name of the person
 * @param theInitials The initials of the person
 * @param theLastName The last name of the person
 *
 * @return An initialised CEXName instance
 **/
-(CEXName*) initWithTitle: (NSString*) theTitle : (NSString*) theFirstName : (NSString*) theInitials : (NSString*) theLastName;
-(BOOL) isEmpty;
@end
