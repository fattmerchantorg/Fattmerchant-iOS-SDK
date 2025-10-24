/**
 * @author Creditcall Ltd
 * @brief Contains data representing information about a generated card hash.
 * @class CCCardHash
 * @copyright Copyright Creditcall Limited 2017.
 */
#import <Foundation/Foundation.h>

/**
 * @typedef CCCardHashSource
 *
 * Source from where the CardHash was generated.
 */
typedef NS_ENUM(NSUInteger, CCCardHashSource) {
    CCCardHashSourceUnknown, /**< Unknown. **/
    CCCardHashSourcePlatform, /**< Provided by platform.*/
    CCCardHashSourcePaymentDevice, /**< Provided by payment device.*/
};

/**
 * @typedef CCCardHashScope
 *
 * Scope define the set of devices that would generate this same hash for a particular card.
 */
typedef NS_ENUM(NSUInteger, CCCardHashScope) {
    CCCardHashScopeUnknown = 0x00, /**< Unknown. */
    CCCardHashScopeTerminal = 0x01, /**< Hash is locked to this terminal only. */
    CCCardHashScopeTerminalGroup = 0x02, /**< hash is locked to this terminal group. */
    CCCardHashScopeMerchant = 0x03, /**< Hash is locked to terminal on this merchant. */
    CCCardHashScopeClient = 0x04, /**< Hash is locked to terminal on this client. */
    CCCardHashScopeProcessor = 0x05, /**< Hash is locked to the same processor. */
    CCCardHashScopeTerritory = 0x06, /**< Hash is locked to the same territory. */
    CCCardHashScopePaymentDeviceModel = 0x07, /**< Hash is locked to devices of the same model*/
    CCCardHashScopePaymentDeviceManufacturer = 0x08, /**< Hash is locked to devices by the same manufacturer. */
    CCCardHashScopeTokenPaymentCompatible = 0x09, /**< Hash can be used with CardReference to perform tokenised payments. */
    CCCardHashScopePlatform = 0x10 /**< Hash is locked to the Creditcall Ltd platform. */
};

@interface CCCardHash : NSObject

/**
 * Scope define the set of devices that would generate this same hash for a particular card.
 *
 * @return the scope for the card hash.
 */
@property (readonly)CCCardHashScope scope;
/**
 * Source from where the CardHash was generated.
 * 
 * @return the source of the card hash.
 */
@property (readonly)CCCardHashSource source;

/**
 * Get the card hash value.
 *
 *@return The card hash value.
 */
@property (readonly)NSString *value;
@end
