/**
 * @author Creditcall Ltd
 * @brief The current status of the configured Terminal
 * @class TerminalStatus
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

#import <Foundation/Foundation.h>
#import "CCParameters.h"
@interface TerminalStatus : NSObject

/**
 * Terminal ID of terminal currently configured.
 */
@property (nonatomic, strong, readonly)NSString *terminalId;

/**
 * Transaction key of terminal currently configured.
 */
@property (nonatomic, strong, readonly)NSString *transactionKey;

/**
 * WebMIS username configured.
 */
@property (nonatomic, strong, readonly)NSString *webMisUserName;

/**
 * WebMIS password configured.
 */
@property (nonatomic, strong, readonly)NSString *webMisPassword;

/**
 * API Key configured.
 */
@property (nonatomic, strong, readonly)NSString *apiKey;

/**
 * Indictates whether terminal is currently enabled. Will always be false if Terminal environment doesn't match current environment of ChipDNA Mobile. 
 */
@property (nonatomic, readonly)BOOL enabled;

/**
 * Environment in which the credentialis are intended to be used. Possible values are {@link CCParameters#CCValueEnvironmentLive LIVE} or {@link CCParameters#CCValueEnvironmentTest TEST}.
 */
@property (nonatomic, strong, readonly)CCParameterValue environment;

@end
