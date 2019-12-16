/**
 * @author Creditcall Ltd
 * @brief A utility class which can be used to serialize and deserialize information returned from ChipDNA Mobile.
 * @class ChipDnaMobileSerializer
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */


#import <Foundation/Foundation.h>
#import "VersionInformation.h"
#import "DeviceStatus.h"
#import "RequestQueueStatus.h"
#import "TerminalStatus.h"
#import "TmsStatus.h"
#import "PaymentPlatformStatus.h"
#import "ReceiptField.h"
#import "CCCardHash.h"

@interface ChipDnaMobileSerializer : NSObject
/**
 * Serialize the NSDictionary of available PIN pads into XML format.
 *
 * @param allpinpads NSDictionary containing NSArrays of PIN pad names mapped against their connection type.
 * @return XML representation of available PIN pads.
 */
+(NSString *)serializeAvailablePinPadsDictionary:(NSDictionary *)allpinpads;

/**
 * Deserialize XML returned from {@link ChipDnaMobile#getAvailablePinPads: getAvailablePinPads} to retrieve NSDictionary mapping NSArrays of PINPad names to their connection type.
 *
 * @param pinpadString XML representation of PIN pad names as is returned from {@link ChipDnaMobile#getAvailablePinPads: getAvailablePinPads}.
 * @return A NSDictionary mapping NSArrays of PIN Pad names to their connection type.
 */
+(NSDictionary *)deserializeAvailablePinPadsString:(NSString *)pinpadString;

/**
 * Deserialize XML returned from {@link ChipDnaMobile#getStatus: getStatus} into {@link VersionInformation} object.
 *
 * @param versionInformation XML representation of {@link VersionInformation} object.
 * @return {@link VersionInformation} object.
 */
+(VersionInformation *)deserializeVersionInformation:(NSString *)versionInformation;

/**
 * Serialize {@link VersionInformation} object into XML representation.
 *
 * @param versionInfo VersionInformation object to be serialized.
 * @param XML representation of version information.
 */
+(NSString *)serializeVersionInformation:(VersionInformation *)versionInfo;

/**
 * Serialize {@link DeviceStatus} object into XML representation.
 *
 * @param deviceStatus {@link DeviceStatus} object to be serialized.
 * @return XML representation of {@link deviceStatus}.
 */
+(NSString *)serializeDeviceStatus:(DeviceStatus *)deviceStatus;

/**
 * Deserialize XML returned during device update callback.
 *
 * @param deviceStatus XML representation of {@link DeviceStatus} object.
 * @return DeviceStatus object
 */
+(DeviceStatus *)deserializeDeviceStatus:(NSString *)deviceStatus;

/**
 * Serialize {@link TmsStatus} object into XML representation.
 *
 * @param tmsStatus {@link TmsStatus} object to be serialized.
 * @return XML representation of {@link TmsStatus}.
 */
+(NSString *)serializeTmsStatus:(TmsStatus *)tmsStatus;

/**
 * Deserialize XML representation of {@link TmsStatus} into object.
 *
 * @param tmsStatus XML representation of {@link TmsStatus} object.
 * @return TmsStatus object
 */
+(TmsStatus *)deserializeTmsStatus:(NSString *)tmsStatus;

/**
 * Serialize {@link TerminalStatus} object into XML representation.
 *
 * @param terminalStatus {@link TerminalStatus} object to be serialized.
 * @return XML representation of {@link TerminalStatus} object.
 */
+(NSString *)serializeTerminalStatus:(TerminalStatus *)terminalStatus;

/**
 * Deserialize XML representation of {@link TerminalStatus} into object.
 *
 * @param terminalStatus XML representation of {@link TerminalStatus} object.
 * @return {@link TerminalStatus} object.
 */
+(TerminalStatus *)deserializeTerminalStatus:(NSString *)terminalStatus;

/**
 * Serialize {@link RequestQueueStatus} object into XML representation.
 *
 * @param requestQueueStatus {@link RequestQueueStatus} object to be serialized.
 * @return XML representation of {@link RequestQueueStatus} object.
 */
+(NSString *)serializeRequestQueueStatus:(RequestQueueStatus *)requestQueueStatus;

/**
 * Deserialize XML representation of {@link RequestsQueueStatus} into object.
 *
 * @param requestQueueString XML representation of {@link RequestQueueStatus} object.
 * @return {@link RequestQueueStatus} object.
 */
+(RequestQueueStatus *)deserializeRequestQueueStatus:(NSString *)requestQueueString;

/**
 * Serialize {@link PaymentPlatformStatus} object into XML representation.
 *
 * @param paymentPlatformStatus {@link PaymentPlatformStatus} object to be serialized.
 * @return XML representation of {@link PaymentPlatformStatus} object.
 */
+(NSString *)serializePaymentPlatformStatus:(PaymentPlatformStatus *)paymentPlatformStatus;

/**
 * Deserialize XML representation of {@link PaymentPlatformStatus} into object.
 *
 * @param paymentPlatformStatus XML presentation of {@link PaymentPlatformStatus} object.
 * @return {@link PaymentPlatforStatus} object.
 */
+(PaymentPlatformStatus *)deserializePaymentPlatformStatus:(NSString *)paymentPlatformStatus;

/**
 * Serialize an NSArray of {@link ReceiptField} objects.
 *
 * @param receiptData The array of {@ReceiptField} objects to be serialized.
 * @return XML representation of an array of {@link ReceiptField} objects.
 */
+(NSString *)serializeReceiptData:(NSDictionary *)receiptData;

/**
 * Deserialize XML representation of an array of {@link ReceiptField} objects.
 *
 * @param receiptData XML representation of an array of {@link ReceiptField} objects.
 * @return Dictionary of {@link ReceiptField} objects.
 */
+(NSDictionary<NSString *,ReceiptField *> *)deserializeReceiptData:(NSString *)receiptData;

/**
 * Serialize {@link CCCardHash} object into XML representation.
 *
 * @param cardHashes An array of {@link CCCardHash} objects to be serialized.
 * @return XML representation of an array of {@link CCCardHash} objects.
 */
+(NSString *)serializeCardHash:(NSArray *)cardHashes;

/**
 * Deserialize XML representation of an array of {@link CCCardHash} objects.
 *
 * @param cardHsah XML representation of {@link CCCardHash} object.
 * @return Array of {@link CCCardHash} objects.
 */
+(NSArray<CCCardHash *> *)deserializeCardHash:(NSString *)cardhash;

/**
 * Serialize a dictionary of {@link Currency} objects.
 *
 * @param availableCurrencies A dictionary of {@link Currency} objects to be serialized.
 * @return XML representation of a dictionary of {@link Currency} objects.
 */
+(NSString *)serializeAvailableCurrencies:(NSDictionary *)availableCurrencies;

/**
 * Deserialize XML representation of a dictionary of {@link Currency} objects.
 *
 * @param availableCurrencies XML representation of a dictionary of {@link Currency} objects.
 * @return Dictionary of {@link Currency} objects.
 */
+(NSDictionary *)deserializedAvailableCurrencies:(NSString *)availableCurrencies;

/**
 * Serialize a dictionary of {@link CardApplication} objects.
 *
 * @param availableCardApplications A dictionary of {@link CardApplication} objects to be serialized.
 * @return XML representation of a dictionary of {@link CardApplication} objects.
 */
+(NSString *)serializeAvailableCardApplications:(NSArray *)availableCardApplications;

/**
 * Deserialize XML representation of a dictionary of {@link CardApplication} objects.
 *
 * @param availableCardApplications XML representation of a dictionary of {@link CardApplication} objects.
 * @return Dictionary of {@link CardApplication} objects.
 */
+(NSArray *)deserializedAvailableCardApplications:(NSString *)availableCardApplications;
@end
