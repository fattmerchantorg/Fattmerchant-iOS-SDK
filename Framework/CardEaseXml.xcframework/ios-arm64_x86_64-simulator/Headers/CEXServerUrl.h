/**
 * @author Creditcall Ltd
 * @brief A URL and associated attributes that can be used for processing CardEaseXML requests.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXServerUrl
 * @details At least one of these needs to be supplied to the CEXClient for processing to take place.
 */
#import <Foundation/Foundation.h>

@interface CEXServerUrl : NSObject
{
 @private NSInteger timeout;
}

/// @file

/**
 * @brief The server URL read timeout.
 *
 * The read timeout for the specified server URL in milliseconds.  If zero is
 * specified an infinite timeout is used. For most requests a timeout of 45 seconds
 * (45000) is recommended.
 *
 * A timeout of less than 30 seconds is not permitted as some authorisations may
 * take this long.
 *
 * @property timeout
 **/
@property NSInteger timeout;

/**
 * @brief The actual URL of the server.
 *
 * This should be a HTTP URL and in the form: "http://..." or "https://...".
 *
 * This must not be null.
 *
 * @property url
 **/
@property (strong) NSURL *url;

/**
 * @brief Constructs a new CEXServerUrl instance.
 *
 * url defaults to nil.
 * timeout defaults to 45000.
 *
 * @return An initialised CEXServerUrl instance.
 **/
-(CEXServerUrl *)init;

/**
 * @brief Constructs a new server URL with specified URL and timeout.
 *
 * @param theUrl The actual URL of the server URL.
 *               This should be a HTTP URL and in the form: "http://..." or "https://...".
 *               This must not be null.
 *
 * @param theTimeout The read timeout for the specified server URL in milliseconds.
 *                   If zero is specified an infinite timeout is used.
 *                   For most requests a timeout of 45 seconds (45000) is recommended.
 *                   A timeout of less than 30 seconds is not permitted as some authorisations may take this long.
 *
 * @return An initialised CEXServerUrl instance.
 **/
-(CEXServerUrl *)initWithUrl:(NSURL *)url timeout:(NSInteger)timeout;

/**
 * @brief Constructs a new server URL with specified URL and timeout.
 *
 * @param theUrl The actual URL of the server URL.
 *               This should be a HTTP URL and in the form: "http://..." or "https://...".
 *               This must not be null.
 *
 * @param theTimeout The read timeout for the specified server URL in milliseconds.
 *                   If zero is specified an infinite timeout is used.
 *                   For most requests a timeout of 45 seconds (45000) is recommended.
 *                   A timeout of less than 30 seconds is not permitted as some authorisations may take this long.
 *
 * @return An allocated and initialised CEXServerUrl instance.
 **/
+(id)serverUrlWithUrl:(NSURL *)url timeout:(NSInteger)timeout;

@end
