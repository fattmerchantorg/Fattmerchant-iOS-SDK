/**
 * @author Creditcall Ltd
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 * @file CCCardSchemeIdEnum.h
 */

#ifndef CCCardSchemeIdEnum_h
#define CCCardSchemeIdEnum_h


/**
 * @typedef CCCardSchemeIdEnum
 *
 * Possible calues returned for CCCardSchemIdEnum.
 */
typedef NS_ENUM(NSUInteger, CCCardSchemeIdEnumValue) {
    CCCardSchemeIdEnumOther = 0, /**< (0) Other */
    CCCardSchemeIdEnumVisa = 1, /**< (1) Visa */
    CCCardSchemeIdEnumMasterCard = 2, /**< (2) MasterCard */
    CCCardSchemeIdEnumMaestro = 3, /**< (3) Maestro */
    CCCardSchemeIdEnumAmex = 4, /**< (4) Amex */
    CCCardSchemeIdEnumJcb = 5, /**< (5) JCB */
    CCCardSchemeIdEnumDiners = 6, /**< (6) Diners */
    CCCardSchemeIdEnumDiscover = 7, /**< (7) Discover */
    CCCardSchemeIdEnumCarteBleue = 8, /**< (8) Carte Bleue */
    CCCardSchemeIdEnumCarteBlanc = 9, /**< (9) Carte Blanc */
    CCCardSchemeIdEnumVoyager = 10, /**< (10) Voyager */
    CCCardSchemeIdEnumWex = 11, /**< (11) WEX */
    CCCardSchemeIdEnumChinaUnionPay = 12, /**< (12) China Union Pay */
    CCCardSchemeIdEnumStyle = 100, /**< (100) Style */
    CCCardSchemeIdEnumValueLink = 101, /**< (101) Value Link */
    CCCardSchemeIdEnumInterac = 1241, /**< (1241) Interac */
    CCCardSchemeIdEnumLaser = 3721 /**< (3721) Laser */
};

@interface CCCardSchemeIdEnum : NSObject

/**
 * Get the CCCardSchemeIdEnumValue from a string.
 *
 * @return the {@Link CCCardSchemeIdEnum} corresponding to the string.
 */
+(CCCardSchemeIdEnumValue)stringToEnum:(NSString *)strValue;

@end

#endif /* CCCardSchemeIdEnum_h */
