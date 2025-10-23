//
//  IDT_UniMag.h
//  IDTech
//
//  Created by Randy Palermo on 2/5/15.
//  Copyright (c) 2015 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "APDUResponse.h"
#import "IDT_Device.h"





/** Protocol methods established for IDT_UniMag class  **/
@protocol IDT_UniMag_Delegate <NSObject>

@optional
-(void) deviceConnected; //!<Fires when device connects.  If a connection is established before the delegate is established (no delegate to send initial connection notification to), this method will fire upon establishing the delegate.
-(void) deviceDisconnected; //!<Fires when device disconnects.
- (void) plugStatusChange:(BOOL)deviceInserted; //!<Monitors the headphone jack for device insertion/removal.
//!< @param deviceInserted TRUE = device inserted, FALSE = device removed
- (void) dataInOutMonitor:(NSData*)data  incoming:(BOOL)isIncoming; //!<All incoming/outgoing data going to the device can be monitored through this delegate.
//!< @param data The serial data represented as a NSData object
//!< @param isIncoming The direction of the data
//!<- <c>TRUE</c> specifies data being received from the device,
//!<- <c>FALSE</c> indicates data being sent to the device.

- (void) dataInOutMonitorDate:(NSData*)data  incoming:(BOOL)isIncoming timestamp:(NSDate*)timestamp;
//!<All incoming/outgoing data going to the device can be monitored through this delegate.
//!< @param data The serial data represented as a NSData object
//!< @param isIncoming The direction of the data
//!<- <c>TRUE</c> specifies data being received from the device,
//!<- <c>FALSE</c> indicates data being sent to the device.
//!< @param timestamp The timestamp of the data

- (void) swipeMSRData:(IDTMSRData*)cardData;//!<Receives card data from MSR swipe.
//!< @param cardData Captured card data from MSR swipe
- (void) deviceMessage:(NSString*)message;//!<Receives messages from the framework
//!< @param message String message transmitted by framework

@end

@interface IDT_UniMag : NSObject<IDT_Device_Delegate>{
    id<IDT_UniMag_Delegate> delegate;
}

@property(strong) id<IDT_UniMag_Delegate> delegate;  //!<- Reference to IDT_UniMag_Delegate.

/**
 * SDK Version
 - All Devices
 *
 Returns the current version of IDTech.framework
 
 @return  Framework version
 */
+(NSString*) SDK_version;


/**
* Send UniMag Command

*
* @param command  A command to execute

* @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_sendUniMagCmd:(NSString*)command;


/**
 * Singleton Instance
 - All Devices
 *
 Establishes an singleton instance of IDT_UniMag class.
 
 @return  Instance of IDT_UniMag
 */
+(IDT_UniMag*) sharedController;

/**
 * Send UniMag Command
 
 *
 * @param command  A command to execute from UNIMAG_COMMAND_Types enumeration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 @code
 typedef enum{
    UNIMAG_COMMAND_DEFAULT_GENERAL_SETTINGS,
    UNIMAG_COMMAND_ENABLE_ERR_NOTIFICATION,
    UNIMAG_COMMAND_DISABLE_ERR_NOTIFICATION,
    UNIMAG_COMMAND_ENABLE_EXP_DATE,
    UNIMAG_COMMAND_DISABLE_EXP_DATE,
    UNIMAG_COMMAND_CLEAR_BUFFER,
    UNIMAG_COMMAND_RESET_BAUD_RATE
 }UNIMAG_COMMAND_Types;
 @endcode
 *
 */
-(RETURN_CODE) device_sendUniMagCommand:(UNIMAG_COMMAND_Types)command;

/**
 * Send Payment Application Engine Command
 * 
 Executes a PAE command
 
 @param command ASCII command string, should be started with "*PAE"
 @param response Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_sendPAE:(NSString*)command response:(NSString**)response;

/**
 * Set Swipe Data Encryption
 *
 * Sets the swipe encryption method
 *
 @param encryption 1 = TDES, 2 = AES
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setSwipeEncryption:(int)encryption;


/**
 * Set Swipe Force Encryption
 *
 * Sets the swipe force encryption options
 *
 @param forceON TRUE = Force Encryption ON,  FALSE = Force Encryption OFF

 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setSwipeForcedEncryptionOption:(BOOL)forceON;

/**
 * Polls device for Firmware Version
 
 *
 * @param response Response returned of Firmware Version
 *
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPay::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;

/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getSerialNumber:(NSString**)response;

/**
 * Get Response Code String
 *
 Interpret a IDT_UniMag response code and return string description.
 
 @param errorCode Error code, range 0x0000 - 0xFFFF, example 0x0300
 
 
 * @return Verbose error description
 
 
 */
-(NSString *) device_getResponseCodeString: (int) errorCode;


/**
 * Connect To Audio Reader
 * @return RETURN_CODE
 *
 Attemps to recognize and connect to an IDTech MSR device connected via the audio port.
 
 */

-(RETURN_CODE) device_connectToAudioReader;

/**
 * Disable MSR Swipe
 
 
 
 Cancels MSR swipe request.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;


/**
 * Enable MSR Swipe
 
 *
 Enables MSR, waiting for swipe to occur. Allows track selection. Returns IDTMSRData instance to deviceDelegate::swipeMSRData:()
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe;


/**
 * Set PrePAN Clear Digits
 
 *
 Sets the number of digits to show in clear text at the beginning of PAN
 
 @param clearDigits Amount of characters to display cleartext at beginning of PAN. Valid range 0-6.  Default value 4.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:())
 
 */
-(RETURN_CODE) device_setPrePANClearDigits:(int)clearDigits;


/**
 * Polls device for next KSN
 
 *
 * @param response  Returns next KSN
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) msr_getNextKSN:(NSData**)response;

/**
 * Get Multi MSR setting values
 *
 Returns the settings used for swipe data
 
 @param setting MSR setting to retrieve
 @param value MSR setting values
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) msr_getSettings:(Byte)setting value:(NSData**)value;


/**
 * Set Swipe Force Encryption
 *
 * Sets the swipe force encryption options
 *
 @param track1 Force encrypt track 1
 @param track2 Force encrypt track 2
 @param track3 Force encrypt track 3
 @param track3card0 Force encrypt track 3 when card type is 0
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setSwipeForcedEncryptionOption:(BOOL)track1 track2:(BOOL)track2 track3:(BOOL)track3 track3card0:(BOOL)track3card0;

/**
 Device Connected
 
 @return isConnected  Boolean indicated if UniMag is connected
 
 */

-(bool) isConnected;

@end
