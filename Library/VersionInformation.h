/**
 * @author Creditcall Ltd
 * @brief This class represents the version information for ChipDNA Mobile.
 * @class VersionInformation
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

#import <Foundation/Foundation.h>

@interface VersionInformation : NSObject

/**
 * Version number of ChipDNA Mobile.
 */
@property (nonatomic, strong, readonly)NSString *chipDnaVersion;

/**
 * Release Name of ChipDNA Mobile.
 */
@property (nonatomic, strong, readonly)NSString *chipDnaReleaseName;

/**
 * Application name of ChipDNA Mobile.
 */
@property (nonatomic, strong, readonly)NSString *chipDnaApplicationName;

@end
