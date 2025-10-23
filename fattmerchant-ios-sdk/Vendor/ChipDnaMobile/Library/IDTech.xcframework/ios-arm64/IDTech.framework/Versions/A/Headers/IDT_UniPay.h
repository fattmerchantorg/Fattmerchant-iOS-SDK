//
//  IDT_UniPay.h
//  IDTech
//
//  Created by Randy Palermo on 7/2/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//



#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "APDUResponse.h"
#import "IDT_Device.h"




/** Protocol methods established for IDT_UniPay class  **/
@protocol IDT_UniPay_Delegate <NSObject>

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
 UniPay ICC Event
 This function will be called when an ICC is attached or detached from reader. Applies to UniPay only
 
 @param nICC_Attached Can be one of the following values:
 - 0x01: ICC attached while reader is idle
 - 0x00: ICC detached while reader is idle
 - 0x11: ICC attached while reader is in MSR mode
 - 0x10: After ICC Powered On, ICC Card Removal,Power off ICC
 
 @code
 -(void) eventFunctionICC: (Byte) nICC_Attached
 {
 switch (nICC_Attached) {
 case 0x01:
 case 0x11:
 {
 LOGI(@"ICC event: ICC attached.");
 }
 break;
 
 case 0x00:
 case 0x10:
 {
 LOGI(@"ICC event: ICC detached.");
 }
 break;
 }
 }
 @endcode
 */
-(void) eventFunctionICC: (Byte) nICC_Attached;



@end


/**
 Class to drive the IDT_UniPay device
 */
@interface IDT_UniPay : NSObject<IDT_Device_Delegate>{
    id<IDT_UniPay_Delegate> delegate;
}

@property(strong) id<IDT_UniPay_Delegate> delegate;  //!<- Reference to IDT_UniPay_Delegate.




/**
 * SDK Version
 - All Devices
 *
 Returns the current version of IDTech.framework
 
 @return  Framework version
 */
+(NSString*) SDK_version;

/**
 * Singleton Instance
 - All Devices
 *
 Establishes an singleton instance of IDT_UniPay class.
 
 @return  Instance of IDT_UniPay
 */
+(IDT_UniPay*) sharedController;





/**
 * Polls device for Model Number
 
 *
 * @param response  Returns Model Number
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) config_getModelNumber:(NSString**)response;

/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getSerialNumber:(NSString**)response;

/**
 * Command Acknowledgement Timout
 *
 * Sets the amount of seconds to wait for an {ACK} to a command before a timeout.  Responses should normally be received under one second.  Default is 3 seconds
 *
 * @param nSecond  Timout value.  Valid range .1 - 60 seconds
 
 * @return Success flag.  Determines if value was set and in range.
 */
-(BOOL) config_setCmdTimeOutDuration: (float) nSecond;

/**
 * Set Serial Number
 
 *
 Set device's serial number and Bluetooth name, then reboots device. Bluetooth name will be set as IDT_UniPay + Space + Serial number
 *
 @param strSN Device serial number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) config_setSerialNumber:(NSString*)strSN;





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
 
 * Polls device for Battery Voltage
 *
 * @param response  Returns Battery Voltage as 4-chararacter string * 100.  Example: "0186" = 1.86v. "1172" = 11.72v.
 
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
-(RETURN_CODE) device_getBatteryVoltage:(NSString**)response;

/**
 * Polls device for Firmware Version
 
 *
 * @param response Response returned of Firmware Version
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;

/**
 * Get Account KSN
 *
 * Retrieves the KSN for a key slot
 @param keySlot 0x02=MSR DUKPT Key, 0x0C = Admin DUKPT Key, 0x22 = ICC DUKPT Key
 * @param ksn Returns the Account DUKPT KSN
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE
 
 - If  Key was not loaded, unit should respond error code 0x0400.
 - If  Key is end of useful life, unit should respond error code 0x7300
 
 */
-(RETURN_CODE) device_getKSN:(int)keySlot ksn:(NSData**)ksn;

/**
 * Get Level and Baude
 *
 @param response  The Baud Rate and Audio Level.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getLevelAndBaud:(NSString**)response;

/**
 * Get Response Code String
 *
 Interpret a IDT_UniPay response code and return string description.
 
 @param errorCode Error code, range 0x0000 - 0xFFFF, example 0x0300
 
 
 * @return Verbose error description
 
 HEX VALUE | Description
 ------- | -------
 0x0000 | No error, beginning task
 0x0001 | No response from reader
 0x0002 | Invalid response data
 0x0003 | Time out for task or CMD
 0x0004 | Wrong parameter
 0x0005 | SDK is doing MSR or ICC task
 0x0006 | SDK is doing PINPad task
 0x0007 | SDK is doing Other task
 0x0300 | Key Type(TDES) of Session Key is not same as the related Master Key.
 0x0400 | Related Key was not loaded.
 0x0500 | Key Same.
 0x0702 | PAN is Error Key.
 0x0D00 | This Key had been loaded.
 0x0E00 | Base Time was loaded.
 0x1800 | Send “Cancel Command” after send “Get Encrypted PIN” &”Get Numeric “& “Get Amount”
 0x1900 | Press “Cancel” key after send “Get Encrypted PIN” &”Get Numeric “& “Get Amount”
 0x30FF | Security Chip is not connect
 0x3000 | Security Chip is deactivation & Device is In Removal Legally State.
 0x3101 | Security Chip is activation &  Device is In Removal Legally State.
 0x5500 | No Admin DUKPT Key.
 0x5501 | Admin  DUKPT Key STOP.
 0x5502 | Admin DUKPT Key KSN is Error.
 0x5503 | Get Authentication Code1 Failed.
 0x5504 | Validate Authentication Code Error.
 0x5505 | Encrypt or Decrypt data failed.
 0x5506 | Not Support the New Key Type.
 0x5507 | New Key Index is Error.
 0x5508 | Step Error.
 0x550F | Other Error.
 0x6000 | Save or Config Failed / Or Read Config Error.
 0x6200 | No Serial Number.
 0x6900 | Invalid Command - Protocol is right, but task ID is invalid.
 0x6A00 | Unsupported Command - Protocol and task ID are right, but command is invalid.
 0x6B00 | Unknown parameter in command - Protocol task ID and command are right, but parameter is invalid.
 0x7200 | Device is suspend (MKSK suspend or press password suspend).
 0x7300 | PIN DUKPT is STOP (21 bit 1).
 0x7400 | Device is Busy.
 0xE100 | Can not enter sleep mode.
 0xE200 | File has existed.
 0xE300 | File has not existed.
 0xE400 | Open File Error.
 0xE500 | SmartCard Error.
 0xE600 | Get MSR Card data is error.
 0xE700 | Command time out.
 0xE800 | File read or write is error.
 0xE900 | Active 1850 error!
 0xEA00 | Load bootloader error.
 0xEF00 | Protocol Error- STX or ETX or check error.
 0xEB00 | Picture is not exist.
 0x2C06 | no card seated to request ATR
 0x2D01 | Card Not Supported,
 0x2D03 | Card Not Supported, wants CRC
 0x690D | Command not supported on reader without ICC support
 0x8100 | ICC error time out on power-up
 0x8200 | invalid TS character received
 0x8500 | pps confirmation error
 0x8600 | Unsupported F, D, or combination of F and D
 0x8700 | protocol not supported EMV TD1 out of range
 0x8800 | power not at proper level
 0x8900 | ATR length too long
 0x8B01 | EMV invalid TA1 byte value
 0x8B02 | EMV TB1 required
 0x8B03 | EMV Unsupported TB1 only 00 allowed
 0x8B04 | EMV Card Error, invalid BWI or CWI
 0x8B06 | EMV TB2 not allowed in ATR
 0x8B07 | EMV TC2 out of range
 0x8B08 | EMV TC2 out of range
 0x8B09 | per EMV96 TA3 must be > 0xF
 0x8B10 | ICC error on power-up
 0x8B11 | EMV T=1 then TB3 required
 0x8B12 | Card Error, invalid BWI or CWI
 0x8B13 | Card Error, invalid BWI or CWI
 0x8B17 | EMV TC1/TB3 conflict*
 0x8B20 | EMV TD2 out of range must be T=1
 0x8C00 | TCK error
 0xA304 | connector has no voltage setting
 0xA305 | ICC error on power-up invalid (SBLK(IFSD) exchange
 0xE301 | ICC error after session star
 0xFF00 | EMV: Request to go online
 0xFF01 | EMV: Accept the offline transaction
 0xFF02 | EMV: Decline the offline transaction
 0xFF03 | EMV: Accept the online transaction
 0xFF04 | EMV: Decline the online transaction
 0xFF05 | EMV: Application may fallback to magstripe technology
 0xFF06 | EMV: ICC detected that the conditions of use are not satisfied
 0xFF07 | EMV: ICC didn't accept transaction
 0xFF08 | EMV: Transaction was cancelled
 0xFF09 | EMV: Application was not selected by kernel or ICC format error or ICC missing data error
 0xFF0A | EMV: Transaction is terminated
 0xFF0B | EMV: Other EMV Error
 
 */
-(NSString *) device_getResponseCodeString: (int) errorCode;


/**
 * Is Audio Reader Connected
 
 *
 Returns value on device connection status when device is an audio-type connected to headphone plug.
 
 @return BOOL True = Connected, False = Disconnected
 
 */

-(BOOL) device_isAudioReaderConnected;



/**
 Is Device Connected
 
 Returns the connection status of the requested device
 
 @param device Check connectivity of device type
 
 @code
 typedef enum{
 IDT_DEVICE_UniPay_IOS = 3,
 IDT_DEVICE_UniPay_OSX_USB = 4
 }IDT_DEVICE_Types;
 
 @endcode
 */
-(bool) device_isConnected:(IDT_DEVICE_Types)device;

/**
 * Reboot Device
 
 
 *
 Executes a command to restart the device.
 *
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */

-(RETURN_CODE) device_rebootDevice;


/**
 * Send a NSData object to device
 *
 * Sends a command represented by the provide NSData object to the device through the accessory protocol.
 *
 * @param cmd NSData representation of command to execute
 * @param lrc If <c>TRUE</c>, this will wrap command with start/length/lrc/sum/end:  '{STX}{Len_Low}{Len_High} data {CheckLRC} {CheckSUM} {ETX}'
 @param response Response data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:())
 */
-(RETURN_CODE) device_sendDataCommand:(NSData*)cmd calcLRC:(BOOL)lrc response:(NSData**)response;
/**
 * Set Volume To Audio Reader
 
 *
 Set the iPhone’s volume for command communication with audio-based readers. The the range of iPhone’s volume is from 0.1 to 1.0.
 
 @param val Volume level from 0.1 to 1.0
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setAudioVolume:(float)val;



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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_startRKI;



/**
 * Exchange APDU
 
 *
 * Sends an APDU packet to the ICC.  If successful, response is returned in APDUResult class instance in response parameter.
 
 @param dataAPDU  APDU data packet
 @param encrypted  Send data encrypted for special case
 @param response Unencrypted/encrypted parsed APDU response
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:())
 
 
 */

-(RETURN_CODE) icc_exchangeAPDU:(NSData*)dataAPDU encrypted:(BOOL)encrypted response:(APDUResponse**)response;


/**
 * Exchange Encrypted APDU
 - UniPay
 *
 * Sends an encrypted APDU packet to the ICC.  If successful, response is returned in APDUResult class instance in response parameter.
 
 @param dataAPDU  APDU data packet
 @param response encrypted parsed APDU response
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 
 */

-(RETURN_CODE) icc_exchangeEncryptedAPDU:(NSData*)dataAPDU response:(APDUResponse**)response;

/**
 * Exchange Multi APDU
 *
 Sends multiple APDU commands within on command
 
 @param dataAPDU  An array of NSData APDU commands
 @param response  The combined response of the multiple APDU commands
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) icc_exchangeMultiAPDU:(NSArray*)dataAPDU response:(NSData**)response;

/**
 * Get APDU KSN
 
 *
 * Retrieves the KSN used in ICC Encypted APDU usage
 
 * @param ksn Returns the encrypted APDU packet KSN
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE
 
 */
-(RETURN_CODE) icc_getAPDU_KSN:(NSData**)ksn;


/**
 * Get Expiry Date Option
 UniPay
 
 *
 Executes a command to get the Expiry Date Option.
 *
 @param response Expiry Option..
 - "0" Output masked for Tag 57 and only output encrypted for Tag 5F24
 - "1" Output plaintext
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) icc_getExpiryDateOption:(NSString**)response;


/**
 * Get Reader Status
 
 *
 Returns the reader status
 
 @param readerStatus Pointer that will return with the ICCReaderStatus results.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 @code
 ICCReaderStatus readerStatus;
 RETURN_CODE rt = [[IDT_UniPay sharedController] icc_getICCReaderStatus:&readerStatus];
 if(RETURN_CODE_DO_SUCCESS != rt){
 LOGI(@"Fail");
 }
 else{
 NSString *sta;
 if(readerStatus.iccPower)
 sta =@"[ICC Powered]";
 else
 sta = @"[ICC Power not Ready]";
 if(readerStatus.cardSeated)
 sta =[NSString stringWithFormat:@"%@,[Card Seated]", sta];
 else
 sta =[NSString stringWithFormat:@"%@,[Card not Seated]", sta];
 
 LOGI(@"Card Status = %@",sta);
 }
 @endcode
 */

-(RETURN_CODE) icc_getICCReaderStatus:(ICCReaderStatus**)readerStatus;

/**
 * Get Key Format For ICC DUKPT
 *
 * Specifies how data will be encrypted with Data Key or PIN key (if DUKPT key loaded)
 *
 * @param response Response returned from method:
 - 'TDES': Encrypted card data with TDES if  DUKPT Key had been loaded.(default)
 - 'AES': Encrypted card data with AES if DUKPT Key had been loaded.
 - 'NONE': No Encryption.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) icc_getKeyFormatForICCDUKPT:(NSString**)response;

/**
 * Get Key Type for ICC DUKPT
 *
 * Specifies the key type used for ICC DUKPT encryption
 *
 * @param response Response returned from method:
 - 'DATA': Encrypted card data with Data Key DUKPT Key had been loaded.(default)
 - 'PIN': Encrypted card data with PIN Key if DUKPT Key had been loaded.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) icc_getKeyTypeForICCDUKPT:(NSString**)response;


/**
 * Power Off ICC
 
 
 *
 * Powers down the ICC
 
 * @param error Returns the error, if any
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 If Success, empty
 If Failure, ASCII encoded data of error string
 */
-(RETURN_CODE) icc_powerOffICC:(NSString**)error;


/**
 * Power On ICC
 
 
 *
 * Power up the currently selected microprocessor card in the ICC reader
 *
 * @param response Response returned. If Success, ATR String. If Failure, ASCII encoded data of error string
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 
 
 */
-(RETURN_CODE) icc_powerOnICC:(NSData**)response;





/**
 * Set ICC Notifications
 *
 Determins if card insert/remove events are captured and sent to delegate UniPay_EventFunctionICC
 
 
 @param turnON  TRUE = monitor ICC card events, FALSE = ignore ICC card events.  Default value is FALSE/OFF.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) icc_setICCNotification:(BOOL)turnON;



/**
 * Set Key Format for ICC DUKPT
 *
 * Sets how data will be encrypted, with either TDES or AES (if DUKPT key loaded)
 *
 @param encryption Encryption Type
 - 00: Encrypt with TDES
 - 01: Encrypt with AES
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 
 */
-(RETURN_CODE) icc_setKeyFormatForICCDUKPT:(int)encryption;






/**
 * Set Key Type for ICC DUKPT Key
 *
 * Sets which key the data will be encrypted with, with either Data Key or PIN key (if DUKPT key loaded)
 *
 @param encryption Encryption Type
 - 00: Encrypt with Data Key
 - 01: Encrypt with PIN Key
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:())
 
 
 */
-(RETURN_CODE) icc_setKeyTypeForICCDUKPT:(int)encryption;







/**
 * Disable MSR Swipe
 
 
 
 Cancels MSR swipe request.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;



/**
 * Get Clear PAN Digits
 *
 * Returns the number of digits that begin the PAN that will be in the clear
 *
 * @param response Number of digits in clear.  Values are ASCII '0' - '6':
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getClearPANID:(NSString**)response;

/**
 * Set Multi MSR setting value
 *
 Sets the MSR setting to the value given
 
 @param setting MSR setting to set
 @param value MSR setting value
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) msr_setSetting:(Byte)setting value:(Byte)val;


/**
 * Get Expiration Masking
 *
 * Get the flag that determines if to mask the expiration date
 *
 * @param response '0' = masked, '1' = not-masked
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getExpirationMask:(NSString**)response;

/**
 * Get Swipe Data Encryption
 *
 * Returns the encryption used for sweip data
 *
 * @param response 'TDES', 'AES', 'NONE'
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */

-(RETURN_CODE) msr_getSwipeEncryption:(NSString**)response;

/**
 * Get Swipe Data Encryption
 *
 * Gets the swipe force encryption options
 *
 * @param response A string with for flags separated by PIPE character  f1|f2|f3|f4, example "1|0|0|1" where:
 - f1 = Track 1 Force Encrypt
 - f2 = Track 2 Force Encrypt
 - f3 = Track 3 Force Encrypt
 - f4 = Track 3 Force Encrypt when card type is 0
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getSwipeForcedEncryptionOption:(NSString**)response;

/**
 * Get Swipe Mask Option
 *
 * Gets the swipe mask/clear data sending option
 *
 * @param response A string with for flags separated by PIPE character  f1|f2|f3, example "1|0|0" where:
 - f1 = Track 1 Mask Allowed
 - f2 = Track 2 Mask Allowed
 - f3 = Track 3 Mask Allowed
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getSwipeMaskOption:(NSString**)response;




/**
 * Set Clear PAN Digits
 *
 * Sets the amount of digits shown in the clear (not masked) at the beginning of the returned PAN value
 *
 @param digits Number of digits to show in clear.  Range 0-6.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setClearPANID:(int)digits;



/**
 * Set Expiration Masking
 *
 * Sets the flag to mask the expiration date
 *
 @param masked TRUE = mask expiration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setExpirationMask:(BOOL)masked;

/**
 * Set Swipe Data Encryption
 *
 * Sets the swipe encryption method
 *
 @param encryption 1 = TDES, 2 = AES
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setSwipeEncryption:(int)encryption;



/**
 * Set Swipe Force Encryption
 *
 * Sets the swipe force encryption options
 *
 @param track1 Force encrypt track 1
 @param track2 Force encrypt track 2
 @param track3 Force encrypt track 3
 @param track3card0 Force encrypt track 3 when card type is 0
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setSwipeForcedEncryptionOption:(BOOL)track1 track2:(BOOL)track2 track3:(BOOL)track3 track3card0:(BOOL)track3card0;



/**
 * Set Swipe Mask Option
 *
 * Sets the swipe mask/clear data sending option
 *
 @param track1 Mask track 1 allowed
 @param track2 Mask track 2 allowed
 @param track3 Mask track 3 allowed
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setSwipeMaskOption:(BOOL)track1 track2:(BOOL)track2 track3:(BOOL)track3;


/**
 * Enable MSR Swipe
 
 *
 Enables MSR, waiting for swipe to occur. Allows track selection. Returns IDTMSRData instance to deviceDelegate::swipeMSRData:()
 
 
 
 
 @param track Track Selection Option
 
 Track Selection Option	| Val
 ------------ | ------------
 Any Track | 0
 Track 1 Only | 1
 Track 2 Only | 2
 Track 1 & Track 2 | 3
 Track 3 Only | 4
 Track 1 & Track 3 | 5
 Track 2 & Track 3 | 6
 All three Tracks | 7
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe:(int)track;



/**
 Device Connected
 
 @return isConnected  Boolean indicated if UniPay is connected
 
 */

-(bool) isConnected;

/**
 * Attempt connection
 *
 Requests a connection attempt.  Internal use only.
 */
-(void) attemptConnect;

/**
* Set Reader Attached
*
Forces the device to reader attached state.
 @param attached TRUE = attached, FALSE = not attached
*/
-(void) setReaderAttached:(BOOL)attached;

@end
