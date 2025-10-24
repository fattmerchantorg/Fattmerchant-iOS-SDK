/**
 * @author Creditcall Ltd
 * @brief A class used to hold address information.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXAddress
 */
#import <Foundation/Foundation.h>
/// @file

@interface CEXAddress : NSObject 

/**
 * @property recipients
 * @brief The recipient lines of the address.
 **/
@property (strong) NSMutableArray *recipients;

/**
 * @property lines
 * @brief The address lines of the address.
 **/
@property (strong) NSMutableArray *lines;

/**
 * @property city
 * @brief The town/city of the address.
 **/
@property (strong) NSString *city;

/**
 * @property country
 * @brief The country of the address.
 **/
@property (strong) NSString *country;

/**
 * @property state
 * @brief The county/state of the address.
 **/
@property (strong) NSString *state;

/**
 * @property zipCode
 * @brief The post/zip code of the address.
 **/
@property (strong) NSString *zipCode;

/**
 * @brief Creates a new address with the fields.
 *
 * @param theRecipient The recipient line of the address
 * @param theLine The address line of the address
 * @param theCity The town/city of the address
 * @param theState The county/state of the address
 * @param theZipCode The post/zip code of the address
 * @param theCountry The country of the address
 * @return The initialised CEXAddress object.
 **/
- (CEXAddress*) initWithRecipient: (NSString*) recipient : (NSString*) line : (NSString*) city : (NSString*) state : (NSString*) zipCode : (NSString*) country;

/**
 * @brief Creates a new address with the fields.
 *
 * @param theRecipients The recipient lines of the address
 * @param theLines The address lines of the address
 * @param theCity The town/city of the address
 * @param theState The county/state of the address
 * @param theZipCode The post/zip code of the address
 * @param theCountry The country of the address
 * @return The initialised CEXAddress object.
 **/
- (CEXAddress*) initWithRecipients: (NSArray*) theRecipients : (NSArray*) theLines : (NSString*) theCity : (NSString*) theState : (NSString*) theZipCode : (NSString*) theCountry;

-(BOOL) isEmpty;
@end
