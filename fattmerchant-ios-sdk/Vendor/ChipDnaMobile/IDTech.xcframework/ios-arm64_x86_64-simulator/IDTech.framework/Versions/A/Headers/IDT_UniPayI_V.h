//
//  IDT_UniPayI_V.h
//  IDTech
//
//  Created by Randy Palermo on 10/29/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "IDT_Device.h"

/** Protocol methods established for IDT_UniPayI_V class  **/
@protocol IDT_UniPayI_V_Delegate <NSObject>
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


/**
 LCD Display Request
 During an EMV transaction, this delegate will receive data to clear virtual LCD display, display messages, display menu, or display language.  Applies to UniPay III
 
 @param mode LCD Display Mode:
 - 0x01: Menu Display.  A selection must be made to resume the transaction
 - 0x02: Normal Display get function key.  A function must be selected to resume the transaction
 - 0x03: Display without input.  Message is displayed without pausing the transaction
 - 0x04: List of languages are presented for selection. A selection must be made to resume the transaction
 - 0x10: Clear Screen. Command to clear the LCD screen
 
 */

- (void) lcdDisplay:(int)mode  lines:(NSArray*)lines;

/**
 PIN Request
 During an EMV transaction, this delegate will receive data that is a request to collect a PIN
 
 @param mode PIN Mode:
  - EMV_PIN_MODE_CANCEL = 0X00,
  - EMV_PIN_MODE_ONLINE_PIN_DUKPT = 0X01,
  - EMV_PIN_MODE_ONLINE_PIN_MKSK = 0X02,
  - EMV_PIN_MODE_OFFLINE_PIN = 0X03
 @param key Either DUKPT or SESSION, depending on mode. If offline plaintext, value is nil
 @param PAN PAN for calculating PINBlock
 @param startTO Timeout value to start PIN entry
 @param intervalTO Timeout value between key presses
 @param language "EN"=English, "ES"=Spanish, "ZH"=Chinese, "FR"=French
 
 */

- (void) pinRequest:(EMV_PIN_MODE_Types)mode  key:(NSData*)key  PAN:(NSData*)PAN startTO:(int)startTO intervalTO:(int)intervalTO language:(NSString*)language;

/**
 EMV Transaction Data
 
 This protocol will receive results from IDT_Device::startEMVTransaction:otherAmount:timeout:cashback:additionalTags:()
 
 
 @param emvData EMV Results Data.  Result code, card type, encryption type, masked tags, encrypted tags, unencrypted tags and KSN
 
 @param error The error code as defined in the errors.h file
 
 
 */
- (void) emvTransactionData:(IDTEMVData*)emvData errorCode:(int)error;

@end

/**
 Class to drive the IDT_UniPayI_V device
 */
@interface IDT_UniPayI_V : NSObject<IDT_Device_Delegate>{
    id<IDT_UniPayI_V_Delegate> delegate;
}

@property(strong) id<IDT_UniPayI_V_Delegate> delegate;  //!<- Reference to IDT_UniPayI_V_Delegate.



/**
 * SDK Version
 *
 Returns the current version of IDTech.framework
 
 @return  Framework version
 */
+(NSString*) SDK_version;

/**
 * Singleton Instance
 *
 Establishes an singleton instance of IDT_UniPayI_V class.
 
 @return  Instance of IDT_UniPayI_V
 */
+(IDT_UniPayI_V*) sharedController;

/**
 *Close Device
 */

-(void) close;






/**
 * Cancel Connect To Audio Reader
 * @return RETURN_CODE
 *
 Cancels a connection attempt to an IDTech MSR device connected via the audio port.
 
 */


-(RETURN_CODE) device_cancelConnectToAudioReader;
/**
 * Connect To Audio Reader
 * @return RETURN_CODE
 *
 Attemps to recognize and connect to an IDTech MSR device connected via the audio port.
 
 */

-(RETURN_CODE) device_connectToAudioReader;


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
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;

/**
 * Get Poll Mode
 *
 * Gets the current poll mode of the device
 *
 * @param mode Response from the device of the current poll mode
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getPollMode:(NSData**)mode;

/**
 * Get EMV Kernel Version
 * 
 Polls the device for the EMV Kernel Version
 
 @param response The kernel version response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersion:(NSString**)response;

/**
 * Get Extended EMV Kernel Version
 * 
 Polls the device for the extended EMV kernel version
 
 @param response The extended kernel verion response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersionExt:(NSString**)response;

/**
 * Ping Device
 * 
 Pings the reader. If it is connected, returns success, otherwise returns timeout
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_pingDevice;

/**
 * Device Get DateTime
 * 
 Gets the current system date and time of the device for VP3300 and UNIPAYI_V
 
 @param dateTime The date time returned as follows:
 -byte 0: Year 00-99
 -byte 1: Month 01-12
 -byte 2: Day 01-31
 -byte 3: Hour 00-23
 -byte 4: Minute 00-59
 -byte 5: Second 00-59
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getDateTime:(NSData**)dateTime;

/**
 * Set Date Time
 * 
 Sets the date and time of the device to the current date and time of the iOS device
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setDateTime;

/**
 * Get DRS Status
 *
 Gets the status of DRS(Destructive Reset)
 
 @param codeDRS The data format is [DRS SourceBlk Number] [SourceBlk1] ... [SourceBlkN]
 [DRS SourceBlk Number] is 2 bytes, formate is NumL NumH. It is the number of [SourceBlkX]
 [SourceBlkX] is n bytes, format is [SourceID] [SourceLen] [SourceData]
 [SourceID] is 1 byte
 [SourceLen] is 1 byte, it is the length of [SourceData]
 Examples:
 [SourceID]     [SourceLen]     [SourceData]
 00                1                01 – Application Error
 01                1                01 – Application Error
 02                1                0x01 – EMV L2 Configuration Check Value Error
 0x02 – Future Key Check Value Error
 10                1                01 – Battery Error
 11                1                Bit 0 – Tamper Switch 1 (0-No, 1-Error, string ident = "")
 Bit 1– Tamper Switch 2 (0-No, 1-Error, string ident = "")
 Bit 2– Tamper Switch 3 (0-No, 1-Error, string ident = "")
 Bit 3– Tamper Switch 4 (0-No, 1-Error, string ident = "")
 Bit 4– Tamper Switch 5 (0-No, 1-Error, string ident = "")
 Bit 5– Tamper Switch 6 (0-No, 1-Error, string ident = "")
 12                1                01 –TemperatureHigh or Low
 13                1                01 –Voltage High or Low
 1F                4                Reg31~24bits, Reg23~16bits, Reg15~8bits, Reg7~0bits
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getDRS:(NSData**)codeDRS;

/**
 * Self Check
 *
 If Self-Test function failed, then work into De-activation state
 If device work into De-activation state, all sensitive data will be erased and it needs to be fixed by manufacturer.
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_selfCheck;

/**
 * Is Audio Reader Connected
 
 *
 Returns value on device connection status when device is an audio-type connected to headphone plug.
 
 @return BOOL True = Connected, False = Disconnected
 
 */

-(BOOL) device_isAudioReaderConnected;

/**
 * Get Response Code String
 *
 Interpret a response code and return string description.
 
 @param errorCode Error code, range 0x0000 - 0xFFFF, example 0x0300
 
 
 * @return Verbose error description
 
 
 */
-(NSString *) device_getResponseCodeString: (int) errorCode;

/**
 Is Device Connected
 
 Returns the connection status of the requested device
 
 @param device Check connectivity of device type
 
 @code
 typedef enum{
 IDT_DEVICE_UniPayI_V_IOS
 IDT_DEVICE_UniPayI_V_OSX_USB
 }IDT_DEVICE_Types;
 
 @endcode
 */
-(bool) device_isConnected:(IDT_DEVICE_Types)device;

/**
 * Send NEO IDG Command
 Send a NEO IDG ViVOtech 2.0 command
 *
 * @param command  One byte command as per NEO IDG Reference Guide
 * @param subCommand  One byte sub-command as per NEO IDG Reference Guide
 * @param data  Command data (if applicable)
 * @param response  Returns next Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) device_sendIDGCommand:(unsigned char)command subCommand:(unsigned char)subCommand data:(NSData*)data response:(NSData**)response;

/**
 * Send NEO IDG Command
 Send a NEO IDG ViVOtech 3.0 command
 *
 * @param command  One byte command as per NEO IDG Reference Guide
 * @param subCommand  One byte sub-command as per NEO IDG Reference Guide
 * @param data  Command data (if applicable)
 * @param response  Returns next Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) device_sendIDGCommandV3:(unsigned char)command subCommand:(unsigned char)subCommand data:(NSData*)data response:(NSData**)response;

/**
 * Set Volume To Audio Reader
 
 *
 Set the iPhone’s volume for command communication with audio-based readers. The the range of iPhone’s volume is from 0.1 to 1.0.
 
 @param val Volume level from 0.1 to 1.0
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setAudioVolume:(float)val;

/**
 * Set Pass Through
 
 Sets Pass-Through mode on UniPayI_V
 *
 @param enablePassThrough  TRUE = Pass through enabled
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setPassThrough:(BOOL)enablePassThrough;

/**
 * Reboot Device
 - NEO2
 - VP3300
 -VP8800
 -UniPayI_V
 
 *
 Executes a command to restart the device.
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
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */

-(RETURN_CODE) device_rebootDevice;

/**
 * Authenticate Transaction
 
 Authenticated a transaction after startTransaction successfully executes.
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function must be called after a result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS returned to emvTransactionData delegate protocol is received after a startTransaction call.  If auto authorize is ENABLED (default), this method will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS after startTransaction.  The auto authorize can be enabled/disabled with IDT_DEVICE::disableAutoAuthenticateTransaction:()
 *
 
 @param tags Any tags to modify original tags submitted with startTransaction.  Passed as NSData.  Example, tag 9F0C with amount 0x000000000100 would be 0x9F0C06000000000100
 Tag DFEE1A can be used to specify tags to be returned in response, in addition to the default tags. Example DFEE1A049F029F03 will return tags 9F02 and 9F03 with the response
 
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_authenticateTransaction:(NSData*)tags;

/**
 * Start Remote Key Injection
 
 *
 Attempts to perform a Remote Key Injection with IDTech's RKI servers.
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_startRKI;

/**
 * Callback Response LCD Display
 *
 Provides menu selection responses to the kernel after a callback was received lcdDisplay delegate.
 
 @param mode The choices are as follows
 - 0x00 Cancel
 - 0x01 Menu Display
 - 0x02 Normal Display get Function Key  supply either 0x43 ('C') for Cancel, or 0x45 ('E') for Enter/accept
 - 0x08 Language Menu Display
 
 @param selection Line number in hex (0x01, 0x02), or 'C'/'E' of function key
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_callbackResponseLCD:(int)mode selection:(unsigned char) selection;


/**
 * Callback Response PIN Request
 *
 Provides PIN data  to the kernel after a callback was received pinRequest delegate.
 
 @param mode PIN Mode:
 - EMV_PIN_MODE_CANCEL = 0X00,
 - EMV_PIN_MODE_ONLINE_PIN_DUKPT = 0X01,
 - EMV_PIN_MODE_ONLINE_PIN_MKSK = 0X02,
 - EMV_PIN_MODE_OFFLINE_PIN = 0X03
 @param KSN  Key Serial Number. If no pairing and PIN is plaintext, value is nil
 @param PIN PIN data, encrypted.  If no pairing, PIN will be sent plaintext
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_callbackResponsePIN:(EMV_PIN_MODE_Types)mode KSN:(NSData*)KSN PIN:(NSData*)PIN;


/**
 Sets the terminal major configuration in ICS .
 @param configuration A configuration value, range 1-5
 - 1 = 1C
 - 2 = 2C
 - 3 = 3C
 - 4 = 4C
 - 5 = 5C
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_setTerminalMajorConfiguration:(int)configuration;

/**
 Gets the terminal major configuration in ICS .
 @param configuration A configuration value, range 1-5
 - 1 = 1C
 - 2 = 2C
 - 3 = 3C
 - 4 = 4C
 - 5 = 5C
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getTerminalMajorConfiguration:(NSUInteger**)configuration;


/**
 * Complete EMV Transaction Online Request
 *
 Completes an online EMV transaction request by the card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 @param isSuccess Determines if connection to host was successful:
 - TRUE: Online processing with the host (issuer) was completed
 - FALSE: Online processing could not be completed due to connection error with the host (issuer). No further data (tags) required.
 @param tags Host response tag (see below)
 
 \par Host response tag:
 
 Tag | Length | Description
 ----- | ----- | -----
 8A | 2 | Data element Authorization Response Code. Mandatory
 91 | 8-16 | Issuer Authentication Data. Optional
 71 | 0-256 | Issuer Scripts. Optional
 72 | 0-256 | Issuer Scripts. Optional
 DFEE1A | 0-256 | Tag list of additional tags to return
 
 Tag DFEE1A will force additional tags to be returned.  Example DFEE1A049F029F03 will return tags
 9F02 and 9F03 with the response
 
 *  @return RETURN_CODE:
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0xFE00: Authorization Accepted - RETURN_CODE_EMV_AUTHORIZATION_ACCEPTED
 - 0xFE01: Online Failure - RETURN_CODE_EMV_AUTHORIZATION_UNABLE_TO_GO_ONLINE
 - 0xFE02: Technical Issue - RETURN_CODE_EMV_AUTHORIZATION_TECHNICAL_ISSUE
 - 0xFE03: Declined - RETURN_CODE_EMV_AUTHORIZATION_DECLINED
 - 0xFE04: Issuer Referral - RETURN_CODE_EMV_AUTHORIZATION_ISSUER_REFERRAL
 
 
 
 */
-(RETURN_CODE) emv_completeOnlineEMVTransaction:(BOOL)isSuccess hostResponseTags:(NSData*)tags;



/**
 * Disable Auto Authenticate Transaction
 *
 If auto authenticate is DISABLED, authenticateTransaction must be called after a successful startEMV execution.
 
 @param disable  FALSE = auto authenticate ENABLED, TRUE = auto authenticate DISABLED
 
 */
-(void) emv_disableAutoAuthenticateTransaction:(BOOL)disable;



/**
 * Polls device for EMV L2 Version
 *
 * @param response Response returned of Level 2 Version
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
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) emv_getEMVL2Version:(NSString**)response;

/**
 * Remove Application Data by AID
 *
 Removes the Application Data as specified by the AID name passed as a parameter
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeApplicationData:(NSString*)AID;

/**
 * Remove Certificate Authority Public Key
 *
 Removes the CAPK as specified by the RID/Index passed as a parameter in the CAKey structure
 
 @param rid RID of the key to remove
 @param index Index of the key to remove
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCAPK:(NSString*)rid index:(NSString*)index ;




/**
 * Remove Certificate Revocation List
 *
 Removes all CRLEntry entries
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCRLList;


/**
 * Remove Terminal Data
 *
 Removes the Terminal Data
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeTerminalData;

/**
 * Retrieve AID list
 *
 Returns all the AID names on the terminal.  Populates response parameter with an Array of NSString* with AID names
 
 @param response Returns a NSArray of NSString of AID Names
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveAIDList:(NSArray**)response;


/**
 * Retrieve Application Data by AID
 - UniPay II
 *
 Retrieves the Application Data as specified by the AID name passed as a parameter.  The AID will be in the response parameter responseAID
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 @param responseAID  The response returned from the method as a dictionary with Key/Object to match TagValues as follows:
 
 Tag | Description
 ===== | =====
 5F57 | Account Type
 9F01 | Acquirer Identifier
 9F09 | Terminal application version number
 5F36 | Transaction Currency Exponent
 9F1B | Terminal Floor Limit
 9F49 | Dynamic Data Authentication Data Object List(DDOL)
 97 | Transaction Certificate Data Object List(TDOL)
 9F39 | POS Entry Mode
 9F3C | Transaction Reference Currency Code
 9F3D | Transaction Reference Currency Exponent
 99 | PIN Block
 DF10 | LANGUAGE
 DF11 | Use Trans Log
 DF13 | TAC-Default
 DF14 | TAC-Denial
 DF15 | TAC-Online
 DF17 | Threshold Value for Biased Random Selection
 DF18 | Target Percentage For Random Transaction Selection
 DF19 | Maximum Target Percentage For Random Transaction Selection
 DF20 | Trace
 DF22 | Merchant Forced Transaction Online
 DF25 | Default DDOL
 DF26 | Use Revocation list
 DF27 | Use Exception  list
 DF28 | TDOL
 DF30 | Online DOL
 DF62 | Application Selection Flag
 DF63 | Transaction Reference Currency Conversion
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 
 
 
 */
-(RETURN_CODE) emv_retrieveApplicationData:(NSString*)AID response:(NSDictionary**)responseAID;

/**
 * Retrieve Certificate Authority Public Key
 *
 Retrieves the CAPK as specified by the RID/Index  passed as a parameter in the CAKey structure.  The CAPK will be in the response parameter
 
 @param rid The RID of the key to retrieve
 @param index The Index of the key to retrieve
 @param response Response returned as a CAKey
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) emv_retrieveCAPK:(NSString*)rid index:(NSString*)index response:(CAKey**)response;


/**
 * Retrieve Certificate Authority Public Key
 - BTPay 200
 *
 Retrieves the CAPK as specified by the RID/Index  passed as a parameter in the CAKey structure.  The CAPK will be in the response parameter
 
 @param rid The RID of the key to retrieve
 @param index The Index of the key to retrieve
 @param response Response returned as a NSData object with the following data:
 - 5 bytes RID
 - 1 byte Index
 - 1 byte Hash Algorithm
 - 1 byte Encryption Algorithm
 - 20 bytes HashValue
 - 4 bytes Public Key Exponent
 - 2 bytes Modulus Length
 - Variable bytes Modulus>
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) emv_retrieveCAPKFile:(NSString*)rid index:(NSString*)index response:(NSData**)response;

/**
 * Retrieve the Certificate Authority Public Key list
 *
 Returns all the CAPK RID and Index.  Populates response parameter with an array of NSString items, 12 characters each, characters 1-10 RID, characters 11-12 index.
 
 @param response Response returned contains an NSArray of NSString items, 12 characters each, characters 1-10 RID, characters 11-12 index.  Example "a00000000357" = RID a00000003, Index 57
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveCAPKList:(NSArray**)response;



/**
 * Retrieve the Certificate Revocation List
 
 *
 Returns all the RID in the CRL.
 @param response Response returned as an NSArray of NSData objects 9-bytes each:
 - 5-bytes RID, 1-byte Index, 3-byte Serial Number
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveCRLList:(NSMutableArray**)response;





/**
 * Retrieve Terminal Data
 - UniPay II
 *
 Retrieves the Terminal Data .  The Terminal Data will be in the response parameter responseData
 
 @param responseData  The response returned from the method as a dictionary with Key/Object to match TagValues as follows:
 
 9F1A | Terminal Country Code
 9F35 | Terminal Type
 9F33 | Terminal Capability
 9F40 | Additional Terminal Capability
 9F1E | IFD Serial Number
 9F15 | Merchant Category Code
 9F16 | Merchant Identifier
 9F1C | Terminal Identification
 9F4E | Merchant Name and Location
 DF10 | LANGUAGE
 DF11 | Use Trans Log
 DF13 | TAC-Default
 DF14 | TAC-Denial
 DF15 | TAC-Online
 DF17 | Threshold Value for Biased Random Selection
 DF18 | Target Percentage For Random Transaction Selection
 DF19 | Maximum Target Percentage For Random Transaction Selection
 DF20 | Trace
 DF22 | Merchant Forced Transaction Online
 DF25 | Default DDOL
 DF26 | Use Revocation list
 DF27 | Use Exception  list
 DF28 | TDOL
 DF30 | Online DOL
 DF62 | Application Selection Flag
 DF63 | Transaction Reference Currency
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
  */
-(RETURN_CODE) emv_retrieveTerminalData:(NSDictionary**)responseData;



/**
 * Retrieve Transaction Results
 
 *
 Retrieves specified EMV tags from the currently executing transaction.
 
 @param tags Tags to be retrieved.  Example 0x9F028A will retrieve tags 9F02 and 8A
 @param tlv All requested tags returned as unencrypted, encrypted and masked tags. The tlv NSDictionary will
        contain a NSDictionary with key "tags" that has the unencrypted tag data, a NSDictionary with the key
        "masked" that has the masked tag data, and a NSDictionary with the key "encrypted" that has the
        encrypted tag data
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveTransactionResult:(NSData*)tags retrievedTags:(NSDictionary**)retrievedTags;


/**
 * Set Application Data by AID
 *
 Sets the Application Data as specified by the dictionary containing Tags (dictionary keys) and Values (dictionary objects) according to the following table
 
 Tag | Description
 ===== | =====
 5F57 | Account Type
 9F01 | Acquirer Identifier
 9F09 | Terminal application version number
 5F36 | Transaction Currency Exponent
 9F1B | Terminal Floor Limit
 9F49 | Dynamic Data Authentication Data Object List(DDOL)
 97 | Transaction Certificate Data Object List(TDOL)
 9F39 | POS Entry Mode
 9F3C | Transaction Reference Currency Code
 9F3D | Transaction Reference Currency Exponent
 99 | PIN Block
 DF10 | LANGUAGE
 DF11 | Use Trans Log
 DF13 | TAC-Default
 DF14 | TAC-Denial
 DF15 | TAC-Online
 DF17 | Threshold Value for Biased Random Selection
 DF18 | Target Percentage For Random Transaction Selection
 DF19 | Maximum Target Percentage For Random Transaction Selection
 DF20 | Trace
 DF22 | Merchant Forced Transaction Online
 DF25 | Default DDOL
 DF26 | Use Revocation list
 DF27 | Use Exception  list
 DF28 | TDOL
 DF30 | Online DOL
 DF62 | Application Selection Flag
 DF34 | Authorization Response Code: 1-2 bytes=approved code, 3-4 bytes = referral code, 5-6 bytes = declined code
 
 @param aidName aidName AID name.  Example "a0000000031010"
 @param data NSDictionary with Tags/Values for the AID configuration file
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setApplicationData:(NSString*)aidName configData:(NSDictionary*)data;

/**
 * Set Certificate Authority Public Key
 *
 Sets the CAPK as specified by the CAKey structure
 
 @param key CAKey containing the RID, Index, and key data to set
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCAPK:(CAKey)key;

/**
 * Set Certificate Authority Public Key
 *
 Sets the CAPK as specified by the CAKey raw format
 
 @param key CAKey format:
 [5 bytes RID][1 byte Index][1 byte Hash Algorithm][1 byte Encryption Algorithm][20 bytes HashValue][4 bytes Public Key Exponent][2 bytes Modulus Length][Variable bytes Modulus]
 Where:
 - Hash Algorithm: The only algorithm supported is SHA-1.The value is set to 0x01
 - Encryption Algorithm: The encryption algorithm in which this key is used. Currently support only one type: RSA. The value is set to 0x01.
 - HashValue: Which is calculated using SHA-1 over the following fields: RID & Index & Modulus & Exponent
 - Public Key Exponent: Actually, the real length of the exponent is either one byte or 3 bytes. It can have two values: 3 (Format is 0x00 00 00 03), or  65537 (Format is 0x00 01 00 01)
 - Modulus Length: LenL LenH Indicated the length of the next field.
 - Modulus: This is the modulus field of the public key. Its length is specified in the field above.
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCAPKFile:(NSData*)file;

/**
 * Set Certificate Revocation List
 *
 Sets the CRL list
 
 @param data CRLEntries as a repeating occurance of CRL: CRL1 CRL2 … CRLn.
  CRL format is
  - 5Bytes RID
  - 1Byte CA public key Index
  - 3Bytes Certificate Serial Number
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCRLEntries:(NSData*)data;




/**
 * Set Terminal Data
 - UniPay II
 *
 Sets the Terminal Data as specified by the dictionary containing Tags (dictionary keys) and Values (dictionary objects) according to the following table
 
 Tag | Description
 ===== | =====
 5F36 | Transaction Currency Exponent
 9F1A | Terminal Country Code
 9F35 | Terminal Type
 9F33 | Terminal Capability
 9F40 | Additional Terminal Capability
 9F1E | IFD Serial Number
 9F15 | Merchant Category Code
 9F16 | Merchant Identifier
 9F1C | Terminal Identification
 9F4E | Merchant Name and Location
 DF10 | LANGUAGE
 DF11 | Use Trans Log
 DFEE13 | TAC-Default
 DFEE14 | TAC-Denial
 DFEE2A | Threshold Value for Biased Random Selection
 DF18 | Target Percentage For Random Transaction Selection
 DF19 | Maximum Target Percentage For Random Transaction Selection
 DF20 | Trace
 DF22 | Merchant Forced Transaction Online
 DF25 | Default DDOL
 DF26 | Use Revocation list
 DF27 | Use Exception  list
 DF28 | TDOL
 DF30 | Online DOL
 DFEE15 | Application Selection Flag
 DF63 | Transaction Reference Currency
 Conversion
 
 
 @param data NSDictionary with Tags/Values for the Terminal configuration file
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setTerminalData:(NSDictionary*)data;





/**
 * Start EMV Transaction Request
 *
 Authorizes the EMV transaction  for an ICC card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function will complete with a return of EMV_RESULT_CODE_START_TRANSACTION_SUCCESS to emvTransactionData delegate protocol, and then IDT_UniPayII::emv_authenticateTransaction() must be executed.  If auto authorize is ENABLED (default), IDT_UniPayII::emv_authenticateTransaction() will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS.  The auto authorize can be enabled/disabled with IDT_UniPayI_V::emv_disableAutoAuthenticateTransaction:()
 
 @param amount Transaction amount value  (tag value 9F02)
 @param amtOther Other amount value, if any  (tag value 9F03)
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param tags Any other tags to be included in the request.  Passed as a NSData.  Example, tag 9F0C with amount 0x000000000100 would be 0x9F0C06000000000100
 If tags 9F02 (amount),9F03 (other amount), or 9C (transaction type) are included, they will take priority over these values supplied as individual parameters to this method.
 Tag DFEE1A can be used to specify tags to be returned in response, in addition to the default tags. Example DFEE1A049F029F03 will return tags 9F02 and 9F03 with the response

 @param forceOnline TRUE = do not allow offline approval,  FALSE = allow ICC to approve offline if terminal capable
 @param autoAuthenticate Will automatically execute Authenticate Transacation after start transaction returns successful
 @param fallback Indicate if it supports fallback to MSR
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_startTransaction:(double)amount amtOther:(double)amtOther type:(int)type timeout:(int)timeout tags:(NSData*)tags forceOnline:(BOOL)forceOnline fallback:(BOOL)fallback;

/**
 * Remove All Application Data
 * 
 Removes all the application data for the EMV kernel
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE)emv_removeAllApplicationData;

/**
 * Get Kernel Check Value
 *
 Polls the device for the Kernel Check Value
 
 @param response Response returned of the Check Value of the Kernel
 * @return RETURN_CODE: Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelCheckValue:(NSString**)response;

/**
 * Get EMV Configuration Check Value
 * 
 Polls device for the EMV Configuration Check Value
 
 @param response Response returned of the Check Value of the Configuration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVConfigurationCheckValue:(NSString**)response;

/**
 * Callback Response MSR Entry
 * 
 Provides MSR information to kernel after a callback was received with type EMV_CALLBACK_MSR
 
 @param MSR Swiped track data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_callbackResponseMSR:(NSData*)MSR;


/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getSerialNumber:(NSString**)response;

/**
 * Exchange APDU (unencrypted)
 
 *
 * Sends an APDU packet to the ICC.  If successful, response is returned in APDUResult class instance in response parameter.
 
 @param dataAPDU  APDU data packet
 @param response Unencrypted/encrypted parsed APDU response
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) icc_exchangeAPDU:(NSData*)dataAPDU response:(APDUResponse**)response;
/**
 * Get Reader Status
 *
 Returns the reader status
 
 @param readerStatus Pointer that will return with the ICCReaderStatus results.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 @code
 ICCReaderStatus* readerStatus;
 RETURN_CODE rt = [[IDT_Device sharedController] getICCReaderStatus:&readerStatus];
 if(RETURN_CODE_DO_SUCCESS != rt){
    NSLog(@"Fail");
 }
 else{
    NSString *sta;
if(readerStatus->iccPower)
    sta =@"[ICC Powered]";
 else
    sta = @"[ICC Power not Ready]";
if(readerStatus->cardSeated)
    sta =[NSString stringWithFormat:@"%@,[Card Seated]", sta];
 else
    sta =[NSString stringWithFormat:@"%@,[Card not Seated]", sta];
 
 
 @endcode
 */

-(RETURN_CODE) icc_getICCReaderStatus:(ICCReaderStatus**)readerStatus;

/**
 * Power On ICC
 
 
 *
 * Power up the currently selected microprocessor card in the ICC reader
 *
 * @param response Response returned. If Success, ATR String. If Failure, ASCII encoded data of error string
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE. 
 
 
 
 
 */
-(RETURN_CODE) icc_powerOnICC:(NSData**)response;



/**
 * Power Off ICC
 
 
 *
 * Powers down the ICC
 
 * @param error Returns the error, if any
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.
 
 If Success, empty
 If Failure, ASCII encoded data of error string
 */
-(RETURN_CODE) icc_powerOffICC:(NSString**)error;

/**
 * Get Key format for ICC DUKPT
 
 Specifies how data is being encrypted with Data Key or PIN key
 
 *
 * @param format  Response return from method:
 -0x00 : Encrypted card data with TDES if DUKPT Key had been loaded
 -0x01 : Encrypted card data with AES if DUKPT Key had been loaded
 -Other Data : No Encryption
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) icc_getKeyFormatForICCDUKPT:(NSData**)format;

/**
 * Set Key format for ICC DUKPT
 
 Specifies how data will be encrypted with Data Key or PIN key
 
 *
 * @param encryption  The type of encryption to be used
 -0x00 : Encrypted card data with TDES if DUKPT Key had been loaded
 -0x01 : Encrypted card data with AES if DUKPT Key had been loaded
 -Other Data : No Encryption
 
 * @return RETURN_CODE:  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) icc_setKeyFormatForICCDUKPT:(NSData*)encryption;

/**
 * Disable MSR Swipe
 
 
 
 Cancels MSR swipe request.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;



/**
 Cancels Transaction request (EMV).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */

-(RETURN_CODE) emv_cancelTransaction;



/**
 Cancels Transaction request (EMV, swipe or CTLS).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_cancelTransaction;


/**
 * Enable MSR Swipe
 
 *
 Enables MSR, waiting for swipe or tap to occur. If swipe captured, returns IDTMSRData instance to deviceDelegate::swipeMSRData:(). 
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe;

/**
 *Check if device is connected
 */
-(bool) isConnected;

/**
* Set Reader Attached
*
Forces the device to reader attached state.
 @param attached TRUE = attached, FALSE = not attached
*/
-(void) setReaderAttached:(BOOL)attached;

/**
* Create Fast EMV Data
*
*  At the completion of a Fast EMV Transaction, after the final card decision is returned
*  and the IDTEMVData object is provided, sending that emvData object to this
*  method will populate return string data that represents the Fast EMV
*  data that would be returned from and IDTech FastEMV over KB protocol
*
* @param emvData The IDTEMVData object populated with card data.
*
* @return Fast EMV String data
*
*/
+ (NSString*) createFastEMVData:(IDTEMVData*)emvData;




/**
 * Set Terminal Data
 *
 * Sets the Terminal Data for CTLS transaction and general terminal settings as specified by the TLV.  If the value already exists in terminal data, it will be updated.
 * If the value does not exist, it will be added.
 
 @param tlv TerminalData configuration data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPayI_V::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_setTerminalData:(NSData*)tlv;

@end
