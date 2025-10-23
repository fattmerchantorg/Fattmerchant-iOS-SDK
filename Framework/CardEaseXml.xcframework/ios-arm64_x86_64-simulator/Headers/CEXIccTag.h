/**
 * @author Creditcall Ltd
 * @brief A class used to hold ICC tag information required for an EMV transaction.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXIccTag
 * @details A class used to hold ICC tag information required for an EMV transaction.
 *
 * Each ICC tag has an associated tag, type and value.
 *
 * The tag indicates what value is held, the type indicates the type of the value held
 * and the value is the actual data.
 */
#import <Foundation/Foundation.h>
#import "CEXIccTagValueType.h"

/// @file

@interface CEXIccTag : NSObject

/**
 * @brief The tag/tag of the ICC tag. The type of this cannot be numeric due to IssuerRequest.
 * @property tag
 *
 * These are specified in hex and defined in the EMV specification. For example 0x9f02.
 * This is an alphanumeric string.
 **/
@property (strong) NSString *tag;

/**
 * @brief The type of the ICC tag value.
 * @property type
 *
 * The data held in the ICC tag value should be of the same type.
 **/
@property CEXIccTagValueType type;

/**
 * @brief The value of the ICC tag.
 * @property value
 *
 * This should be of the same type as that specified in the ICC tag type.
 * This is an alphanumeric string.
 **/
@property (strong) NSString *value;

/**
 * @brief Creates a new ICC tag with no values
 *
 * The name defaults to nil.
 * The type defaults to ICCTagValueType.AsciiHex.
 * The value defaults to nil.
 *
 * @return The initialised CEXIccTag instance.
 **/
-(CEXIccTag *)init;

/**
 * @brief Creates a new ICC tag with the specified tag.
 *
 * The value defaults to nil.
 * The type defaults to ICCTagValueType.AsciiHex.
 *
 * @param theTag The tag of the ICC tag.
 *               These are specified in hex and defined in the EMV specification.
 *               This is an alphanumeric string. If this is null no tag is specified.
 *
 * @return The initialised CEXIccTag instance.
 **/
-(CEXIccTag *)initWithTag:(NSString *)tag;

/**
 * @brief Creates a new ICC tag with the specified tag, type and value.
 *
 * @param theTag The tag of the ICC tag.
 *               These are specified in hex and defined in the EMV specification.
 *               This is an alphanumeric string. If this is null no tag is specified.
 *
 * @param theType The type of the ICC tag
 * @param theValue The value of the ICC tag. This is an alphanumeric string. If this is null no value is specified
 *
 * @return The initialised CEXIccTag instance.
 **/
-(CEXIccTag *)initWithTag:(NSString *)tag type:(CEXIccTagValueType)type value:(NSString *)value;

/**
 * @brief Creates a new ICC tag with the specified tag and value.
 *
 * The type defaults to ICCTagValueType.AsciiHex.
 *
 * @param theTag The tag of the ICC tag.
 *               These are specified in hex and defined in the EMV specification.
 *               This is an alphanumeric string. If this is null no tag is specified.</param>
 *
 * @param theValue The value of the ICC tag.
 *                 This is an alphanumeric string.
 *                 If this is null no value is specified.</param>
 *
 * @return The initialised CEXIccTag instance.
 */
-(CEXIccTag *)initWithTag:(NSString *)tag value:(NSString *)value;

/**
 * @brief Creates a new ICC tag with the specified tag.
 *
 * The type defaults to ICCTagValueType.AsciiHex.
 * The value defaults to nil.
 *
 * @param theTag The tag of the ICC tag.
 *               These are specified in hex and defined in the EMV specification.
 *               This is an alphanumeric string. If this is null no tag is specified.
 *
 * @return The allocated and initialised CEXIccTag object.
 **/
+(id)iccTagWithTag:(NSString *)tag;


/**
 * @brief Creates a new ICC tag with the specified tag and type.
 *
 * The value defaults to nil.
 *
 * @param theTag The tag of the ICC tag.
 *               These are specified in hex and defined in the EMV specification.
 *               This is an alphanumeric string. If this is null no tag is specified.
 * @param theType The type of the ICC tag
 *
 * @return The allocated and initialised CEXIccTag object.
 **/
+(id)iccTagWithTag:(NSString *)tag type:(CEXIccTagValueType)type;

/**
 * @brief Creates a new ICC tag with the specified tag, type and value.
 *
 * @param theTag The tag of the ICC tag.
 *               These are specified in hex and defined in the EMV specification.
 *               This is an alphanumeric string. If this is null no tag is specified.
 * @param theType The type of the ICC tag
 * @param theValue The value of the ICC tag. This is an alphanumeric string. If this is null no value is specified
 *
 * @return The allocated and initialised CEXIccTag object.
 **/
+(id)iccTagWithTag:(NSString *)tag type:(CEXIccTagValueType)type value:(NSString *)value;

/**
 * @brief Creates a new ICC tag with the specified tag and value.
 *
 * The type defaults to ICCTagValueType.AsciiHex.
 *
 * @param theTag The tag of the ICC tag.
 *               These are specified in hex and defined in the EMV specification.
 *               This is an alphanumeric string. If this is null no tag is specified.
 * @param theValue The value of the ICC tag. This is an alphanumeric string. If this is null no value is specified
 *
 * @return The initialised CEXIccTag object.
 **/
+(id)iccTagWithTag:(NSString *)tag value:(NSString *)value;

@end
