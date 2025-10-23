/**
 * @author Creditcall Ltd
 * @brief A class used to hold point of interaction information.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXPoinOfInteraction
 */

#import <Foundation/Foundation.h>

@interface CEXPointOfInteraction : NSObject <NSCoding>

/**
 * @brief Make of the point of interaction.
 * @property make
 */
@property (strong) NSString * make;

/**
 * @brief Model of the point of interaction.
 * @property model
 */
@property (strong) NSString * model;

/**
 * @brief Firmware name of the point of interaction.
 * @property firmwareName
 */
@property (strong) NSString * firmwareName;

/**
 * @brief Firmware version of the point of interaction.
 * @property firmwareVersion
 */
@property (strong) NSString * firmwareVersion;

/**
 * @brief Serial number of the point of interaction.
 * @property serialNumber
 */
@property (strong) NSString * serialNumber;


/**
 * @brief Creates a new PointOfInteraction object.
 *
 * @param make The make of the point of interaction.
 * @param model The model of the point of interaction.
 * @param firmwareName The firmware name of the point of interaction.
 * @param firmwareVersion The firmware version of the point of interaction.
 * @param serialNumber The serial number of the point of interfaction
 *
 *@return An initialised point of interaction.
 */
-(id)initWithMake:(NSString *)make model:(NSString *)model firmwareName:(NSString *)firmwareName firmwareVersion:(NSString *)firmwareVersion serialNumber:(NSString *)serialNumber;

@end
