/**
 * @author Creditcall Ltd
 * @brief An error that can be returned as the result of a CardEaseXML request in a CardEase response.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @details Contains both an error code and a error message. This class is used to hold these.
 * @class CEXError
 */
#import <Foundation/Foundation.h>

#import "CEXErrorCode.h"

/// @file

@interface CEXError : NSObject

/**
 * @brief The error code associated with this error.
 * @property code
 **/
@property CEXErrorCode code;

/**
 * @brief The error message associated with this error. This is an alphanumeric string.
 * @property message
 **/
@property (strong) NSString *message;

/**
 * @brief Constructs a new error with the given error code and error message.
 *
 * @param theCode    The error code associated with this error. This should be a valid integer.
 * @param theMessage The error message associated with this error. This is an alphanumeric string.
 *                   This should not be null.
 *
 * @return An initialised CEXErrorCode instance.
 **/
-(CEXError *)initWithCode:(CEXErrorCode)code message:(NSString *)message;

/**
 * @brief Constructs a new error with the given error code.
 *
 * The message defaults to nil.
 *
 * @param theCode The error code associated with this error. This should be a valid integer.
 *
 * @return An initialised CEXErrorCode instance.
 **/
+(id)errorWithCode:(CEXErrorCode)code;

/**
 * @brief Constructs a new error with the given error code and error message.
 *
 * @param theCode    The error code associated with this error. This should be a valid integer.
 * @param theMessage The error message associated with this error. This is an alphanumeric string.
 *                   This should not be null.
 *
 * @return An initialised CEXErrorCode instance.
 **/
+(id)errorWithCode:(CEXErrorCode)code message:(NSString *)message;

@end
