/**
 * @author Creditcall Ltd
 * @brief This class contains utility methods that may be needed to integrate with the SDK
 * @class ChipDnaMobileUtils
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface ChipDnaMobileUtils : NSObject

/**
 * Check whether a email address has a valid format.
 *
 * @param email The email address to be validated.
 * @return YES if email is valid, otherwise NO.
 **/
+ (BOOL)isValidEmailAddress:(NSString *)email;

/**
 * Check whether a phone number has a valid format.
 *
 * @param phoneNumber The phone number to be validated.
 * @return YES if phone number is valid, otherwise NO.
 **/
+ (BOOL)isValidPhoneNumber:(NSString *)phoneNumber;

/**
 * Check whether an amount has a valid format. It has to be non nil, in
 * minor units and have a maximum of 7 numeric digits.
 *
 * @param amount
 *            The amount number to be validated.
 * @return YES if amount is valid, otherwise NO.
 **/
+ (BOOL)isValidAmount:(NSString *)amount;

/**
 * Check whether a reference string has a valid format. It has to 
 * contain only UTF-8 characters and have a maximum of 50 characters.
 *
 * @param reference
 *            The reference number to be validated.
 * @return YES if reference is valid, otherwise NO.
 **/
+ (BOOL)isValidReference:(NSString *)reference;

/**
 * Check whether the terminal ID has a valid format.
 *
 * This must be 8 characters long and only contain numeric characters.
 *
 * @param terminalID
 *              The terminal ID to be validated.
 * @return YES if terminal ID is valid, otherwise NO.
 */
+ (BOOL)isValidTerminalID:(NSString *)terminalID;

/**
 * Check whether the transaction key has a valid format.
 * 
 * This should be between 1 and 20 characters long and 
 * contain only alpha numeric characters.
 *
 * @param transactionKey
 *              The transaction key to be validated.
 * @return YES if transaction key is valid, otherwise NO.
 */
+ (BOOL)isValidTransactionKey:(NSString *)transactionKey;

/**
 * Validate and resize a signature image.
 *
 * Method can be called prior to calling {@link ChipDnaMobile#continueSignatureVerification:} to resize and validate a captured singature.
 *
 * @param signatureImage Image of captured signature
 * @param error If signature fails validation the error parameter will be filled with the error which occured.
 *
 * @return The validated resized image. If validation fails nil will be returned.
 */
+ (UIImage *)processSignature:(UIImage *)signatureImage error:(NSString **)error;

/**
 * Pad a string on the left to a desired length with a character of choice.
 *
 * @param originalString The original string to be padded.
 * @param paddingCharacter The character the string is to be padded with.
 * @param desiredLength The desired legth of the new, padded string.
 *
 * @return The padded string.
 */
+ (NSString *)padLeft:(NSString *)originalString paddingCharacter:(char)paddingCharacter desiredLength:(int)desiredLength;

@end
