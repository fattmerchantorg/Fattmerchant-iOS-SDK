//
//  IDT_BTPay.h
//  IDTech
//
//  Created by Randy Palermo on 7/2/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "APDUResponse.h"
#import "IDT_Device.h"
#import "IDTEMVData.h"



/** Protocol methods established for IDT_BTPay class  **/
@protocol IDT_BTPay_Delegate <NSObject>

@optional
-(void) deviceConnected; //!<Fires when device connects.  If a connection is established before the delegate is established (no delegate to send initial connection notification to), this method will fire upon establishing the delegate.
-(void) deviceDisconnected; //!<Fires when device disconnects.
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

/**
 EMV Transaction Data
 
 This protocol will receive results from IDT_Device::startEMVTransaction:otherAmount:timeout:cashback:additionalTags:()
 
 
 @param emvData EMV Results Data.  Result code, card type, encryption type, masked tags, encrypted tags, unencrypted tags and KSN
 
 @param error The error code as defined in the errors.h file
 
 
 */
- (void) emvTransactionData:(IDTEMVData*)emvData errorCode:(int)error;




/**
 Pinpad data delegate protocol
 
 Receives data from pinpad methods
 
 @param value encrypted data returned from IDT_BTPay::pin_getEncryptedData:minLength:maxLength:messageID:language:(), or encrypted account number returned from IDT_BTPay::pin_getCardAccount:max:line1:line2:(). String value returned from IDT_BTPay::pin_getAmount:maxLength:messageID:language:() or IDT_BTPay::pin_getNumeric:minLength:maxLength:messageID:language:(). PINblock returned from IDT_BTPay::pin_getEncryptedPIN:keyType:line1:line2:line3:()
 @param KSN Key Serial Number returned from IDT_BTPay::pin_getEncryptedPIN:keyType:line1:line2:line3:(), IDT_BTPay::pin_getCardAccount:max:line1:line2:() or IDT_BTPay::pin_getEncryptedData:minLength:maxLength:messageID:language:()
 @param event EVENT_PINPAD_Types PINpad event that solicited the data capture
 
 @code
 typedef enum{
 EVENT_PINPAD_UNKNOWN = 11,
 EVENT_PINPAD_ENCRYPTED_PIN,
 EVENT_PINPAD_NUMERIC,
 EVENT_PINPAD_AMOUNT,
 EVENT_PINPAD_ACCOUNT,
 EVENT_PINPAD_ENCRYPTED_DATA,
 EVENT_PINPAD_CANCEL,
 EVENT_PINPAD_TIMEOUT,
 EVENT_PINPAD_FUNCTION_KEY,
 EVENT_PINPAD_DATA_ERROR
 }EVENT_PINPAD_Types;
 @endcode
 */
- (void) pinpadData:(NSData*)value keySN:(NSData*)KSN event:(EVENT_PINPAD_Types)event;


@end


/**
 Class to drive the IDT_BTPay device
 */
@interface IDT_BTPay : NSObject <IDT_Device_Delegate>{
    id<IDT_BTPay_Delegate> delegate;
}

@property(strong) id<IDT_BTPay_Delegate> delegate;  //!<- Reference to IDT_BTPay_Delegate.



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
 Establishes an singleton instance of IDT_BTPay class.
 
 @return  Instance of IDT_BTPay
 */
+(IDT_BTPay*) sharedController;

/**
 * Sets the OSX Connection Method
 *
When using BTPay on OSX, the device can connect either via Bluetooth or USB-HID.  Default is Bluetooth.  Use this function to change the connection method to USB-HID
 
 @param isUSB  TRUE = connect via USB-HID, FALSE = connect via BlueTooth.
 
 
 */
+(void) connectUSB:(BOOL)isUSB;

/**
 * Polls device for current Date/Time
 *
 * @param response Response returned as ASCII Data of Date  YYMMDDhhmmss. Example 140215171628 = Feb. 15, 2014, 28 seconds into 5:16pm.
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 
 @code
 NSString* response;
 RETURN_CODE rt = [[IDT_BTPay sharedController] config_getDateTime:&response];
 if (RETURN_CODE_DO_SUCCESS == rt)
 {
 LOGI* (@"Date Time (YYMMDDhhmmss) = %@",response);
 }
 @endcode
 *
 */
-(RETURN_CODE) config_getDateTime:(NSString**)response;



/**
 * Get interface device's serial number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getInterfaceDeviceSN:(NSString**)response;

/**
 * Get Language Type
 
 *
 Gets the language type
 
 @param response LANGUAGE_TYPE of the BTPay
 ENGLISH: 01
 PORTUGUESE: 02
 SPANISH: 03
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) config_getLanguageType:(NSUInteger**)response;
/**
 * Get Mask and Encryption
 
 *
 Retrieves the MSR Mask and Encryption settings
 
 @param data Pointer that will return location of MaskAndEncryption structure.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getMaskAndEncryption:(MaskAndEncryption**)data;

/**
 * Polls device for Model Number
 *
 * @param response  Returns Model Number
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) config_getModelNumber:(NSString**)response;

/**
 * Polls device for Serial Number
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getSerialNumber:(NSString**)response;

/**
 * Get terminal identification
 *
 * @param response  Returns device terminal identification
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getTerminalIdentification:(NSString**)response;

/**
 * Set Bluetooth Address
 
 *
 Sets the Bluetooth address of the device. 6 bytes, example F0DE07CCA03F.
 
 @param address 6 Byte address represented by a 12-character HEX string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */

-(RETURN_CODE) config_setBluetoothAddress:(NSString*)address;



/**
 * Command Acknowledgement Timout
 *
 * Sets the amount of seconds to wait for an {ACK} to a command before a timeout.  Responses should normally be received under one second.  Default is 3 seconds.
 *
 * @param nSecond  Timout value.  Valid range .1 - 60 seconds
 
 * @return Success flag.  Determines if value was set and in range.
 */
-(BOOL) config_setCmdTimeOutDuration: (float) nSecond;

/**
 * Set device device Date/Time
 *
 Set device's date/time
 *
 @param date Device date represented by a YYMMDDhhmmss.  Example March 12, 2014, 6:30pm (and 12 seconds) = 140312183012
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) config_setDateTime:(NSString*)date;


/**
 * Set Interface Device serial number.
 *
 EMV serial number can be set only once
 *
 @param sn Device serial number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) config_setInterfaceDeviceSN:(NSString*)sn;

/**
 * Set Language Type
 
 *
 Sets the language type of BTPay prompts
 
 @param lang LANGUAGE_TYPE
 ENGLISH: 01
 PORTUGUESE: 02
 SPANISH: 03
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:())
 
 */
-(RETURN_CODE) config_setLanguageType:(LANGUAGE_TYPE)lang;
/**
 * Set Serial Number
 *
 Set device's serial number and Bluetooth name, then reboots device. Bluetooth name will be set as BTPay + Space + Serial number
 *
 @param strSN Device serial number must be 10 characters
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) config_setSerialNumber:(NSString*)strSN;




/**
 * Set terminal identifcation
 *
 Set device's serial number and Bluetooth name, then reboots device. Bluetooth name will be set as IDT_Device + Space + Serial number
 *
 @param sn Device terminal identification
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) config_setTerminalIdentification:(NSString*)sn;




/**
 * Keep Accessory Connected
 *
 * Framework attempts to disconnect from accessory whever application goes to backround.  Setting this value to TRUE will disable framework disconnect attempts.  NOTE:  ExternalAccessory may still disconnect device when going to background by default.  If you want to stay connected to device, you must also set the .pist "Required Background Modes" to "App communicates using CoreBluetooth", "App communicates with an accessory", and "App shares data using CoreBluetooth"
 *
 * @param stayConnected  TRUE = stay connected while in background (assuming .plist is properly configured)
 */
-(void) config_stayConnected:(BOOL)stayConnected;









/**
 * Polls device for Firmware Version
 *
 * @param response Response returned of Firmware Version
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;




/**
 * Get Response Code String
 - All Devices
 *
 Interpret a IDT_BTPay response code and return string description.
 
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
 Is Device Connected
 
 Returns the connection status of the BTPay
 
 @param device Check connectivity of device type
 
 @code
 typedef enum{
 IDT_DEVICE_BTPay_IOS = 0,
 IDT_DEVICE_BTPay_OSX_BT,
 IDT_DEVICE_BTPay_OSX_USB
 }IDT_DEVICE_Types;
 @endcode
 */
-(bool) device_isConnected:(IDT_DEVICE_Types)device;


/**
 * Sends a Beep Value
 
 *
 Executes a beep on the BT200.  The complete beep may be defined as a multiple of single beep tones.
 *
 @param beep Unsigned short array containing freq1,dur1,freq2,dur2,. . . freq#,dur#.  Frequency is in Hz and must be in the range 2000-4000. Duration is in milliseconds.
 @param num Number of tones in the beep array.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 Example Code-
 @code
 unsigned short beep[] = {0xb00,0x400,0x800,0x300};
 RETURN_CODE rt = [[IDT_BTPay sharedController] device_sendBeep:beep numberOfTones:2];
 LOGI(@"\nControl Beep Return Status Code %i ",  rts);
 @endcode
 
 */
-(RETURN_CODE) device_sendBeep:(unsigned short*)beep numberOfTones:(int)num;




/**
 * Send a NSData object to device
 *
 * Sends a command represented by the provide NSData object to the device through the accessory protocol.
 *
 * @param cmd NSData representation of command to execute
 * @param lrc If <c>TRUE</c>, this will wrap command with start/length/lrc/sum/end:  '{STX}{Len_Low}{Len_High} data {CheckLRC} {CheckSUM} {ETX}'
 @param response Response data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_sendDataCommand:(NSData*)cmd calcLRC:(BOOL)lrc response:(NSData**)response;

/**
 * Restore Mask and Encryption default settings
 - BTPay
 
 *
 Restores the default values for MSR Mask and Encryption settings
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_restoreMaskAndEncryptionDefaults;





/**
 * Set Expiration Date masking
 - BTPay
 
 *
 Sets the flag to enable Expiratin Date masking
 
 @param mask TRUE = mask expiration date.  FALSE = display expiration date in cleartext
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setExpMasking:(BOOL)mask;





/**
 * Set PAN masking character
 - BTPay
 
 *
 Sets the character for PAN masking
 
 @param maskChar Masking character.  Default value '*';
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setPANMaskingCharacter:(char)maskChar;
/**
 * Set PostPAN Clear Digits
 - BTPay
 
 *
 Sets the number of digits to show in clear text at the ending of PAN
 
 @param clearDigits Amount of characters to display cleartext at end of PAN. Valid range 0-4.  Default value 4.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setPostPANClearDigits:(int)clearDigits;

/**
 * Set PrePAN Clear Digits
 - BTPay
 
 *
 Sets the number of digits to show in clear text at the beginning of PAN
 
 @param clearDigits Amount of characters to display cleartext at beginning of PAN. Valid range 0-6.  Default value 4.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:())
 
 */
-(RETURN_CODE) device_setPrePANClearDigits:(int)clearDigits;


/**
 * Reboot Device
 
 *
 Executes a command to restart the device.
 *
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */

-(RETURN_CODE) device_rebootDevice;

/**
 * Upload JPEG to device
 
 *
 Stores a picture on the device. The picture's dimensions must not exceed the display resolution of 128 x 64. The picture must be RGB JPEG.
 @param picture RGB JPEG image data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_uploadJPEG:(NSData*)picture;
/**
 * Complete EMV Transaction Online Request
 *
 Completes an online EMV transaction request by the card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 @param result Determines if connection to host was successful.
 @param tags Host response tag
 @param response returns the response tags
 
 Tag | Length | Description
 ----- | ----- | -----
 8A | 2 | Data element Authorization Response Code. Mandatory
 91 | 8-16 | Issuer Authentication Data. Optional
 71 | 0-256 | Issuer Scripts. Optional
 72 | 0-256 | Issuer Scripts. Optional
 
 
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 
 
 \par Converting TLV to NSMutableDictionary
 
 EMV data is  received in TLV (Tag, Length, value) format:
 `950500000080009B02E8009F2701018A025A339F26080C552B9364D55CE5`
 
 This data contains the following EMV tags/values:
 
 Tag | Length | Value
 ----- | ----- | -----
 95 | 05 | 0000008000
 9B | 02 | E800
 9F27 | 01 | 01
 8A | 02 | 5A33
 9F26 | 08 | 0C552B9364D55CE5
 
 An example how to create an NSMutableDictionary with these values follows.
 
 @code
 -(NSMutableDictionary*) createTLVDict{
 
 NSMutableDictionary *emvTags = [[NSMutableDictionary alloc] initWithCapacity:0];
 
 [emvTags setObject:@"0000008000" forKey:@"95"];
 [emvTags setObject:@"E800" forKey:@"9B"];
 [emvTags setObject:@"01" forKey:@"9F27"];
 [emvTags setObject:@"5A33" forKey:@"8A"];
 [emvTags setObject:@"0C552B9364D55CE5" forKey:@"9F26"];
 
 return emvTags;
 
 }
 @endcode
 
 */
-(RETURN_CODE) emv_completeOnlineEMVTransaction:(EMV_AUTHORIZATION_RESULT)result hostResponseTags:(NSMutableDictionary *)tags responseTags:(IDTEMVData**)response;


/**
 * Polls device for EMV Kernel Version
 *
 * @param response Response returned of Kernel Version
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) emv_getEMVKernelVersion:(NSString**)response;

/**
 * Get All Tags
 
 *
 Retrieves all EMV tags from the inserted card.  Only available after the card has been processed after executing IDT_BTPay::emv_startEMVTransaction:otherAmount:timeout:transactionType:additionalTags:()
 
 @param data Pointer that will return location of dictionary with all tag values.  Key is NSString, Object is NSData.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_getAllTags:(NSDictionary**)data;


/**
 * Get Tag
 
 *
 Retrieves an EMV tag from the inserted card.  Only available after the card has been processed after executing IDT_BTPay::emv_startEMVTransaction:otherAmount:timeout:transactionType:additionalTags:()
 
 @param tagName Name fo tag to retrieve
 @param data Pointer that will return location of tag data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_getTag:(NSString*)tagName tagData:(NSData**)data;
/**
 * Remove Application Data by AID
 *
 Removes the Application Data as specified by the AID name passed as a parameter
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeApplicationData:(NSString*)AID;

/**
 * Remove Certificate Authority Public Key
 *
 Removes the CAPK as specified by the RID/Index passed as a parameter in the CAKey structure
 
 @param rid RID of the key to remove
 @param index Index of the key to remove
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCAPK:(NSString*)rid index:(NSString*)index ;




/**
 * Remove Certificate Revocation List RID
 *
 Removes all CRLEntry as specified by the RID and Index passed as a parameter in the CRLEntry structure
 
 @param key CRLEntry containing the RID and Index  of the of the entries to remove
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCRL:(CRLEntry)key;

/**
 * Remove Certificate Revocation List unit
 *
 Removes a single CRLEntry as specified by the RID/Index/Serial Number passed as a parameter in the CRLEntry structure
 
 @param key CRLEntry containing the RID, Index and serial number of the of the entry to remove
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCRLUnit:(CRLEntry)key;

/**
 * Remove Terminal Data
 *
 Removes the Terminal Data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeTerminalData;

/**
 * Retrieve AID list
 *
 Returns all the AID name/length on the inserted ICC.  Populates response parameter with an dictionary with Keys of AID Names (NSData*), and values of AID Lengths (NSData*)
 
 @param response Returns a NSArray of NSString of AID Names
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveAIDList:(NSArray**)response;


/**
 * Retrieve Application Data by AID
 *
 Retrieves the Application Data as specified by the AID name passed as a parameter.  The AID will be in the response parameter responseAID
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 @param responseAID  The response returned from the method
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 
 */
-(RETURN_CODE) emv_retrieveApplicationData:(NSString*)AID response:(ApplicationID**)responseAID;

/**
 * Retrieve Certificate Authority Public Key
 *
 Retrieves the CAPK as specified by the RID/Index  passed as a parameter in the CAKey structure.  The CAPK will be in the response parameter
 
 @param rid The RID of the key to retrieve
 @param index The Index of the key to retrieve
 @param response Response returned as a CAKey
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) emv_retrieveCAPK:(NSString*)rid index:(NSString*)index response:(CAKey**)response;




/**
 * Retrieve the Certificate Authority Public Key list
 *
 Returns all the CAPK RID and Index.  Populates response parameter with an array of NSString items, 12 characters each, characters 1-10 RID, characters 11-12 index.
 
 @param response Response returned contains an NSArray of NSString items, 12 characters each, characters 1-10 RID, characters 11-12 index.  Example "a00000000357" = RID a00000003, Index 57
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveCAPKList:(NSArray**)response;

/**
 * Retrieve the Certificate Revocation List specific to RID and index
 *
 Returns all the serial numbers for a specific RID/Index on the CRL.
 
 @param rid RID of the certificate to search for
 @param response Response returned as an NSArray of NSData* objects for each CRLEntry:
 5 bytes: AID
 1 byte: Index
 3 bytes: Serial Number
 
 The following code can map the NSData entries into crlEntries
 @code
 NSArray* returnArray;
 [[IDT_BTPay sharedController] emv_retrieveCRLForRID:@"a000000003" response:&returnArray];
 for (NSData* obj in returnArray) {
 Byte *keyByte = (Byte*) obj.bytes;
 CRLEntry* crl = (CRLEntry*)keyByte;
 }
 @endcode
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) emv_retrieveCRLForRID:(NSString*)rid response:(NSArray**)response;

/**
 * Retrieve the Certificate Revocation List
 *
 Returns all the RID in the CRL.
 @param response Response returned as an NSArray of NSData 5-byte objects for each RID
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveCRLList:(NSMutableArray**)response;



/**
 * Retrieve Terminal Data
 *
 Retrieves the Terminal Data.  The data will be in the response parameter
 
 @param response Response returned as a TerminalData
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) emv_retrieveTerminalData:(TerminalData**)response;


/**
 * Set Application Data by AID
 *
 Sets the Application Data as specified by the ApplicationID structure passed as a parameter
 
 @param data ApplicationID configuration file
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setApplicationData:(ApplicationID)data;

/**
 * Set Certificate Authority Public Key
 *
 Sets the CAPK as specified by the CAKey structure
 
 @param key CAKey containing the RID, Index, and key data to set
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCAPK:(CAKey)key;

/**
 * Set Certificate Revocation List Entry
 *
 Sets the CRL entry as specified by the CRLEntry structure
 
 @param key CRLEntry containing the RID, Index, and serial number to set
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCRL:(CRLEntry)key;




/**
 * Set Terminal Data
 *
 Sets the Terminal Data as specified by the TerminalData structure passed as a parameter
 
 @param data TerminalData configuration file
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setTerminalData:(TerminalData)data;



/**
 * Start EMV Transaction Request
 *
 Authorizes the EMV transaction for an ICC card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 @param amount Transaction amount value  (tag value 9F02)
 @param amtOther Other amount value, if any  (tag value 9F03)
 @param timeout Timeout value in seconds.
 @param type Transaction type (tag value 9C).
 @param tags Any other optional tags to be included in the request.  Passed as a mutable dictionary.
 
 
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 
 \par Converting TLV to NSMutableDictionary
 
 EMV data is  received in TLV (Tag, Length, value) format:
 `950500000080009B02E8009F2701018A025A339F26080C552B9364D55CE5`
 
 This data contains the following EMV tags/values:
 
 Tag | Length | Value
 ----- | ----- | -----
 95 | 05 | 0000008000
 9B | 02 | E800
 9F27 | 01 | 01
 8A | 02 | 5A33
 9F26 | 08 | 0C552B9364D55CE5
 
 An example how to create an NSMutableDictionary with these values follows.
 
 @code
 -(NSMutableDictionary*) createTLVDict{
 
 NSMutableDictionary *emvTags = [[NSMutableDictionary alloc] initWithCapacity:0];
 
 [emvTags setObject:@"0000008000" forKey:@"95"];
 [emvTags setObject:@"E800" forKey:@"9B"];
 [emvTags setObject:@"01" forKey:@"9F27"];
 [emvTags setObject:@"5A33" forKey:@"8A"];
 [emvTags setObject:@"0C552B9364D55CE5" forKey:@"9F26"];
 
 return emvTags;
 
 }
 @endcode
 
 */




-(RETURN_CODE) emv_startEMVTransaction:(float)amount otherAmount:(float)amtOther timeout:(int)timeout transactionType:(unsigned char)type additionalTags:(NSMutableDictionary *)tags;








/**
 * Exchange APDU (unencrypted)
 
 *
 * Sends an APDU packet to the ICC.  If successful, response is returned in APDUResult class instance in response parameter.
 
 @param dataAPDU  APDU data packet
 @param response Unencrypted/encrypted parsed APDU response
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) icc_exchangeAPDU:(NSData*)dataAPDU response:(APDUResponse**)response;

/**
 * Exchange Encrypted APDU
 
 *
 * Sends an Encrypted APDU packet to the ICC.  If successful, response is returned in APDUResult class instance in response parameter.
 
 @param dataAPDU  APDU data packet
 @param ksn  KSN Value
 @param response Unencrypted/encrypted parsed APDU response
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) icc_exchangeEncryptedAPDU:(NSData*)dataAPDU ksn:(NSData*)ksn response:(APDUResponse**)response;


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
 * Get Reader Status
 *
 Returns the reader status
 
 @param readerStatus Pointer that will return with the ICCReaderStatus results.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 @code
 ICCReaderStatus readerStatus;
 RETURN_CODE rt = [[IDT_BTPay sharedController] icc_getICCReaderStatus:&readerStatus];
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
 * Power Off ICC
 
 *
 * Powers down the ICC
 
 * @param error Returns the error, if any
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 
 
 */
-(RETURN_CODE) icc_powerOnICC:(NSData**)response;

/**
 * Power On ICC with Options
 
 *
 * Power up the currently selected microprocessor card in the ICC reader, specifying IFS/pps options.
 
 @param options ATR Options
 * @param response Response returned. If Success, ATR String. If Failure, ASCII encoded data of error string
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 
 */
-(RETURN_CODE) icc_powerOnICC:(PowerOnStructure)options response:(NSData**)response;







/**
 * Show stored picture on the LCD
 
 *
 Show stored picture on the LCD defined by top left point [X0][Y0] and bottom right point [X1][Y1].  The values of X must be in the range 0-127, and the values of Y must be in the range of 0-63
 @param X0 Upper left X coordinate
 @param Y0 Upper left Y coordinate
 @param X1 Lower left X coordinate
 @param Y1 Lower left Y coordinate
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 */

-(RETURN_CODE) lcd_showJPEG:(int)X0 Y0:(int)Y0 X1:(int)X1 Y1:(int)Y1;


/**
 * DisplayMessage
 
 *
 Display up to 4 lines of text in the device LCD.
 
 @param line1 Display line 1, up to 12 characters
 @param line2 Display line 2, up to 16 characters
 @param line3 Display line 3, up to 16 characters
 @param line4 Display line 4, up to 16 characters
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) lcd_displayMessage:(NSString*)line1 line2:(NSString*)line2 line3:(NSString*)line3 line4:(NSString*)line4;


/**
 * Put device to sleep
 
 *
 Set device to enter  sleep mode. In sleep mode, LCD display and backlight is off. It can be waked up by key press or sending commands
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) lcd_enterSleepMode;
/**
 * Set Enter Sleep Mode Time
 
 *
 Sets seconds of idle that must pass before entering sleep mode
 
 @param seconds  Amount of time (in seconds) that must pass during idle before unit goes to sleep
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) lcd_setIdleTimeForSleep: (int) seconds;




/**
 * Disable MSR Swipe
 
 
 Cancels MSR swipe request.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 */
-(RETURN_CODE) msr_cancelMSRSwipe;




/**
 * Get Card Data Encrypted Algorithm
 
 *
 * Sets the encrypted algorightm of MSR card data and SmartCard data (if MSR DUKPT key loaded
 *
 * @param response Response returned from method:
 - '0': Encrypted card data with Data Key if MSR DUKPT Key had been loaded.(default)
 - '1': Encrypted card data with PIN Key if MSR DUKPT Key had been loaded.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getCardDataEncryptedAlgorithm:(NSString**)response;


/**
 * Get encrypted MSR Data Output Format
 
 *
 * Specifies if data will be encrypted with Data Key or PIN key (if MSR DUKPT key loaded)
 *
 * @param response Response returned from method:
 - '0': Encrypted card data with Data Key if MSR DUKPT Key had been loaded.(default)
 - '1': Encrypted card data with PIN Key if MSR DUKPT Key had been loaded.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getEncryptMSRFormat:(NSString**)response;


/**
 * Set Card Data Encrypted Algorithm
 
 *
 * Sets the encrypted algorightm of MSR card data and SmartCard data (if MSR DUKPT key loaded)
 *
 @param encryption Encryption Type
 - 01: 3DES (Default)
 - 02: AES
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 
 */
-(RETURN_CODE) msr_setCardDataEncryptedAlgorithm:(int)encryption;

/**
 * Set encrypted MSR Data Output Format
 
 *
 * Sets how data will be encrypted, with either Data Key or PIN key (if MSR DUKPT key loaded)
 *
 @param encryption Encryption Type
 - 00: Encrypt with Data Key
 - 01: Encrypt with PIN Key
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 
 */
-(RETURN_CODE) msr_setEncryptMSRFormat:(int)encryption;


/**
 * Display Message and Enable MSR Swipe
 
 *
 Prompts the user with up to 3 lines of text. Enables MSR, waiting for swipe to occur. Returns IDTMSRData instance to deviceDelegate::swipeMSRData:()
 
 During waiting for swiping card, it will receive all commands except IDT_BTPay::pin_getEncryptedPIN:keyType:line1:line2:line3:() and IDT_BTPay::pin_getNumeric:minLength:maxLength:messageID:language:() and IDT_BTPay::pin_getAmount:maxLength:messageID:language:() and IDT_BTPay::pin_getCardAccount:max:line1:line2:() and IDT_BTPay::pin_getEncryptedData:minLength:maxLength:messageID:language:()
 
 
 @param line1 Display line 1, up to 12 characters
 @param line2 Display line 2, up to 16 characters
 @param line3 Display line 3, up to 16 characters
 -
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_startMSRSwipeWithDisplay:(NSString*)line1 line2:(NSString*)line2 line3:(NSString*)line3;










/**
 * Cancel PIN Command
 *
 
 This command can cancel IDT_BTPay:pin_getEncryptedPIN:keyType:line1:line2:line3:() and IDT_BTPay::pin_getNumeric:minLength:maxLength:messageID:language:() and IDT_BTPay::pin_getAmount:maxLength:messageID:language:() and IDT_BTPay::pin_getCardAccount:max:line1:line2:() and IDT_BTPay::pin_getFunctionKey() and IDT_BTPay::pin_getEncryptedData:minLength:maxLength:messageID:language:() */
-(RETURN_CODE) pin_cancelPin;


/**
 * Display Message and Get Amount
 
 *
 Decrypt and display message on LCD. Requires secure message data. Returns value in inputValue of deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_AMOUNT
 
 @param minLength Minimum account number length - not less than 1
 @param maxLength Maximum account number length - not more than 16
 @param mID Message ID from approved message list.
 @param lang Language file to use for message
 @code
 typedef enum{
 LANGUAGE_TYPE_ENGLISH,
 LANGUAGE_TYPE_PORTUGUESE,
 LANGUAGE_TYPE_SPANISH
 }LANGUAGE_TYPE;
 @endcode
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - If there is no any enter in 20 seconds, the entered namount key will be cleared.
 - When press Enter key , it will end this Command and response package with NGA  format .
 - When press Cancel key, the entered amount key will be cleared and if press Cancel key again, this command terminated.
 - Cancel Command can terminate this command.
 
 \par Secure Messages
 Secure messages to be used with General Prompts commands
 
 Msg Id |English Prompt | Portuguese Prompt | Spanish Prompt
 ---------- | ---------- | ----------  | ----------
 1 | ENTER | ENTER | INGRESE
 2 | REENTER | RE-INTRODUZIR | REINGRESE
 3 | ENTER YOUR | INTRODUZIR O SEU | INGRESE SU
 4 | REENTER YOUR | RE-INTRODUZIR O SEU | REINGRESE SU
 5 | PLEASE ENTER | POR FAVOR DIGITE | POR FAVOR INGRESE
 6 | PLEASE REENTER | POR FAVO REENTRAR | POR FAVO REINGRESE
 7 | PO NUMBER | NÚMERO PO | NUMERO PO
 8 | DRIVER ID | LICENÇA | LICENCIA
 9 | ODOMETER | ODOMETER | ODOMETRO
 10 | ID NUMBER | NÚMERO ID | NUMERO ID
 11 | EQUIP CODE | EQUIP CODE | CODIGO EQUIP
 12 | DRIVERS ID | DRIVER ID | ID CONDUCTOR
 13 | JOB NUMBER | EMP NÚMERO | NUMERO EMP
 14 | WORK ORDER | TRABALHO ORDEM | ORDEN TRABAJO
 15 | VEHICLE ID | ID VEÍCULO | ID VEHICULO
 16 | ENTER DRIVER | ENTER DRIVER | INGRESE CONDUCTOR
 17 | ENTER DEPT | ENTER DEPT | INGRESE DEPT
 18 | ENTER PHONE | ADICIONAR PHONE | INGRESE TELEFONO
 19 | ENTER ROUTE | ROUTE ADD | INGRESE RUTA
 20 | ENTER FLEET | ENTER FROTA | INGRESE FLOTA
 21 | ENTER JOB ID | ENTER JOB ID | INGRESE ID TRABAJO
 22 | ROUTE NUMBER | NÚMERO PATH | RUTA NUMERO
 23 | ENTER USER ID | ENTER USER ID | INGRESE ID USUARIO
 24 | FLEET NUMBER | NÚMERO DE FROTA | FLOTA NUMERO
 25 | ENTER PRODUCT | ADICIONAR PRODUTO | INGRESE PRODUCTO
 26 | DRIVER NUMBER | NÚMERO DRIVER | CONDUCTOR NUMERO
 27 | ENTER LICENSE | ENTER LICENÇA | INGRESE LICENCIA
 28 | ENTER FLEET NO | ENTER NRO FROTA | INGRESE NRO FLOTA
 29 | ENTER CAR WASH | WASH ENTER | INGRESE LAVADO
 30 | ENTER VEHICLE | ENTER VEÍCULO | INGRESE VEHICULO
 31 | ENTER TRAILER | TRAILER ENTER | INGRESE TRAILER
 32 | ENTER ODOMETER | ENTER ODOMETER | INGRESE ODOMETRO
 33 | DRIVER LICENSE | CARTEIRA DE MOTORISTA | LICENCIA CONDUCTOR
 34 | ENTER CUSTOMER | ENTER CLIENTE | INGRESE CLIENTE
 35 | VEHICLE NUMBER | NÚMERO DO VEÍCULO | VEHICULO NUMERO
 36 | ENTER CUST DATA | ENTER CLIENTE INFO | INGRESE INFO CLIENTE
 37 | REENTER DRIVID | REENTRAR DRIVER ID | REINGRESE ID CHOFER
 38 | ENTER USER DATA | ENTER INFO USUÁRIO | INGRESE INFO USUARIO
 39 | ENTER CUST CODE | ENTER CODE. CLIENTE | INGRESE COD. CLIENTE
 40 | ENTER EMPLOYEE | ENTER FUNCIONÁRIO | INGRESE EMPLEADO
 41 | ENTER ID NUMBER | ENTER NÚMERO ID | INGRESE NUMERO ID
 42 | ENTER DRIVER ID | ENTER ID DRIVER | INGRESE ID CONDUCTOR
 43 | ENTER FLEET PIN | ENTER PIN FROTA | INGRESE PIN DE FLOTA
 44 | ODOMETER NUMBER | NÚMERO ODOMETER | ODOMETRO NUMERO
 45 | ENTER DRIVER LIC | ENTER DRIVER LIC | INGRESE LIC CONDUCTOR
 46 | ENTER TRAILER NO | NRO TRAILER ENTER | INGRESE NRO TRAILER
 47 | REENTER VEHICLE | REENTRAR VEÍCULO | REINGRESE VEHICULO
 48 | ENTER VEHICLE ID | ENTER VEÍCULO ID | INGRESE ID VEHICULO
 49 | ENTER BIRTH DATE | INSERIR DATA NAC | INGRESE FECHA NAC
 50 | ENTER DOB MMDDYY | ENTER FDN MMDDYY | INGRESE FDN MMDDAA
 51 | ENTER FLEET DATA | ENTER FROTA INFO | INGRESE INFO DE FLOTA
 52 | ENTER REFERENCE | ENTER REFERÊNCIA | INGRESE REFERENCIA
 53 | ENTER AUTH NUMBR | ENTER NÚMERO AUT | INGRESE NUMERO AUT
 54 | ENTER HUB NUMBER | ENTER HUB NRO | INGRESE NRO HUB
 55 | ENTER HUBOMETER | MEDIDA PARA ENTRAR HUB | INGRESE MEDIDO DE HUB
 56 | ENTER TRAILER ID | TRAILER ENTER ID | INGRESE ID TRAILER
 57 | ODOMETER READING | QUILOMETRAGEM | LECTURA ODOMETRO
 58 | REENTER ODOMETER | REENTRAR ODOMETER | REINGRESE ODOMETRO
 59 | REENTER DRIV. ID | REENTRAR DRIVER ID | REINGRESE ID CHOFER
 60 | ENTER CUSTOMER ID | ENTER CLIENTE ID | INGRESE ID CLIENTE
 61 | ENTER CUST. ID | ENTER CLIENTE ID | INGRESE ID CLIENTE
 62 | ENTER ROUTE NUM | ENTER NUM ROUTE | INGRESE NUM RUTA
 63 | ENTER FLEET NUM | FROTA ENTER NUM | INGRESE NUM FLOTA
 64 | FLEET PIN | FROTA PIN | PIN DE FLOTA
 65 | DRIVER # | DRIVER # | CONDUCTOR #
 66 | ENTER DRIVER # | ENTER DRIVER # | INGRESE CONDUCTOR #
 67 | VEHICLE # | VEÍCULO # | VEHICULO #
 68 | ENTER VEHICLE # | ENTER VEÍCULO # | INGRESE VEHICULO #
 69 | JOB # | TRABALHO # | TRABAJO #
 70 | ENTER JOB # | ENTER JOB # | INGRESE TRABAJO #
 71 | DEPT NUMBER | NÚMERO DEPT | NUMERO DEPTO
 72 | DEPARTMENT # | DEPARTAMENTO # | DEPARTAMENTO #
 73 | ENTER DEPT # | ENTER DEPT # | INGRESE DEPTO #
 74 | LICENSE NUMBER | NÚMERO DE LICENÇA | NUMERO LICENCIA
 75 | LICENSE # | LICENÇA # | LICENCIA #
 76 | ENTER LICENSE # | ENTER LICENÇA # | INGRESE LICENCIA #
 77 | DATA | INFO | INFO
 78 | ENTER DATA | ENTER INFO | INGRESE INFO
 79 | CUSTOMER DATA | CLIENTE INFO | INFO CLIENTE
 80 | ID # | ID # | ID #
 81 | ENTER ID # | ENTER ID # | INGRESE ID #
 82 | USER ID | USER ID | ID USUARIO
 83 | ROUTE # | ROUTE # | RUTA #
 84 | ENTER ROUTE # | ADD ROUTE # | INGRESE RUTA #
 85 | ENTER CARD NUM | ENTER NÚMERO DE CARTÃO | INGRESE NUM TARJETA
 86 | EXP DATE(YYMM) | VALIDADE VAL (AAMM) | FECHA EXP (AAMM)
 87 | PHONE NUMBER | TELEFONE | NUMERO TELEFONO
 88 | CVV START DATE | CVV DATA DE INÍCIO | CVV FECHA INICIO
 89 | ISSUE NUMBER | NÚMERO DE EMISSÃO | NUMERO DE EMISION
 90 | START DATE (MMYY) | DATA DE INÍCIO (AAMM) | FECHA INICIO (AAMM)
 */
-(RETURN_CODE) pin_getAmount:(int)minLength maxLength:(int)maxLength messageID:(int)mID language:(LANGUAGE_TYPE)lang;


/**
 * Display Message and Get Card Account
 
 *
 Show message on LCD and get card account number from keypad, then return encrypted card account number. Returns encryptedData of entered account in deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_ACCOUNT
 
 @param minLength Minimum account number length - not less than 1
 @param maxLength Maximum account number length - not more than 16
 @param line1 Display line 1, up to 12 characters
 @param line2 Display line 2, up to 16 characters
 -
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 \par Notes
 - If  there is no any enter in 3 minutes, this command time out.
 - If  there is no any enter in 20 seconds, the entered account numbers will be cleared.
 - When press Enter key, it will end this command and respond package with NGA format.
 - When press Cancel key, the entered account numbers will be cleared and if press Cancel key again, this command terminated.
 - Cancel command can terminate this command.
 */
-(RETURN_CODE) pin_getCardAccount:(int)minLength max:(int)maxLength line1:(NSString*)line1 line2:(NSString*)line2;


/**
 * Get Function Key
 *
 Returns function key value of pressed key in deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_FUNCTION_KEY.  Value passed as NSData in pinpadData with one of the following values
 - 0x43: Cancel Key
 - 0x42: Backspace Key
 - 0x45: Enter Key
 - 0x23: # Key
 - 0x2A: * Key
 
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - Cancel Command can terminate this command.
 
 */
-(RETURN_CODE) pin_getFunctionKey;

/**
 Get PIN Length
 
 * Returns encrypted PIN Length
 *
 @param response Returned Value of Minimum/Maximum PIN length:
 - response[0] = minimum length
 - response[1] = maximum length
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 @code
 NSData* res;
 RETURN_CODE rt = [[IDT_BTPay sharedController] pin_getPinLength:&res];
 uint8_t b[res.length];
 [data getBytes:b];
 if(RETURN_CODE_DO_SUCCESS == rt && res.length>1){
 LOGI(@"GetPinLength: min=%d max=%d", b[0], b[1]);
 }
 @endcode
 */
-(RETURN_CODE) pin_getPinLength:(NSData**)response;

/**
 Get PINPad Status
 
 * Returns status of PINpad
 *
 @param response Pinpad status. response[0]:
 - 0x01: PINpad is inactivate.
 - 0x02: PINpad  has been activated, but Public Key is not loaded.
 - 0x03: Public key has been loaded, but Firmware Key, Numeric Key and Check Value is not loaded.
 - 0x10: PINpad  normal work status.
 - 0x30: PINpad  suspend status if password input error.
 - 0x31: PINpad  suspend status if get PIN(MKSK)120 times in one hours.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 @code
 Byte *b = (Byte*)malloc(1);
 RETURN_CODE rt = [[IDT_BTPay sharedController] pin_getPINpadStatus];
 if(RETURN_CODE_DO_SUCCESS == rt){
 [self appendMessageToResults:[NSString stringWithFormat:@"PINPad Status: %d", b[0]]];
 
 }
 @endcode
 }
 
 */
-(RETURN_CODE) pin_getPINpadStatus:(NSData**)response;




/**
 * Display Message and Get Encrypted Data
 
 *
 Decrypt and display message on LCD. Prompts the user with up to 2 lines of text. Returns value of encrypted data (using MSR DUKPT key) and KSN to deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_ENCRYPTED_DATA
 
 @param lastPackage Last package flag
 @param minLength Minimum account number length - not less than 1
 @param maxLength Maximum account number length - not more than 16
 @param mID Message ID from approved message list.
 @param lang Language file to use for message
 @code
 typedef enum{
 LANGUAGE_TYPE_ENGLISH,
 LANGUAGE_TYPE_PORTUGUESE,
 LANGUAGE_TYPE_SPANISH
 }LANGUAGE_TYPE;
 @endcode
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 \par Notes
 
 -  If  there is no any enter in 3 minutes, this command time out.
 -  If  there is no any enter in 20 seconds, the entered account numbers will be cleared.
 -  When press Enter key, it will end this command and respond package with NGA format.
 -  When press Cancel key, the entered account numbers will be cleared and if press Cancel key again, this command terminated.
 -  Cancel command can terminate this command.
 -  Maximum pattern number allowed is 10. If any error or invalid command is sent, input data patterns will be cleared and command fail.
 
 \par Secure Messages
 Secure messages to be used with General Prompts commands
 
 Msg Id |English Prompt | Portuguese Prompt | Spanish Prompt
 ---------- | ---------- | ----------  | ----------
 1 | ENTER | ENTER | INGRESE
 2 | REENTER | RE-INTRODUZIR | REINGRESE
 3 | ENTER YOUR | INTRODUZIR O SEU | INGRESE SU
 4 | REENTER YOUR | RE-INTRODUZIR O SEU | REINGRESE SU
 5 | PLEASE ENTER | POR FAVOR DIGITE | POR FAVOR INGRESE
 6 | PLEASE REENTER | POR FAVO REENTRAR | POR FAVO REINGRESE
 7 | PO NUMBER | NÚMERO PO | NUMERO PO
 8 | DRIVER ID | LICENÇA | LICENCIA
 9 | ODOMETER | ODOMETER | ODOMETRO
 10 | ID NUMBER | NÚMERO ID | NUMERO ID
 11 | EQUIP CODE | EQUIP CODE | CODIGO EQUIP
 12 | DRIVERS ID | DRIVER ID | ID CONDUCTOR
 13 | JOB NUMBER | EMP NÚMERO | NUMERO EMP
 14 | WORK ORDER | TRABALHO ORDEM | ORDEN TRABAJO
 15 | VEHICLE ID | ID VEÍCULO | ID VEHICULO
 16 | ENTER DRIVER | ENTER DRIVER | INGRESE CONDUCTOR
 17 | ENTER DEPT | ENTER DEPT | INGRESE DEPT
 18 | ENTER PHONE | ADICIONAR PHONE | INGRESE TELEFONO
 19 | ENTER ROUTE | ROUTE ADD | INGRESE RUTA
 20 | ENTER FLEET | ENTER FROTA | INGRESE FLOTA
 21 | ENTER JOB ID | ENTER JOB ID | INGRESE ID TRABAJO
 22 | ROUTE NUMBER | NÚMERO PATH | RUTA NUMERO
 23 | ENTER USER ID | ENTER USER ID | INGRESE ID USUARIO
 24 | FLEET NUMBER | NÚMERO DE FROTA | FLOTA NUMERO
 25 | ENTER PRODUCT | ADICIONAR PRODUTO | INGRESE PRODUCTO
 26 | DRIVER NUMBER | NÚMERO DRIVER | CONDUCTOR NUMERO
 27 | ENTER LICENSE | ENTER LICENÇA | INGRESE LICENCIA
 28 | ENTER FLEET NO | ENTER NRO FROTA | INGRESE NRO FLOTA
 29 | ENTER CAR WASH | WASH ENTER | INGRESE LAVADO
 30 | ENTER VEHICLE | ENTER VEÍCULO | INGRESE VEHICULO
 31 | ENTER TRAILER | TRAILER ENTER | INGRESE TRAILER
 32 | ENTER ODOMETER | ENTER ODOMETER | INGRESE ODOMETRO
 33 | DRIVER LICENSE | CARTEIRA DE MOTORISTA | LICENCIA CONDUCTOR
 34 | ENTER CUSTOMER | ENTER CLIENTE | INGRESE CLIENTE
 35 | VEHICLE NUMBER | NÚMERO DO VEÍCULO | VEHICULO NUMERO
 36 | ENTER CUST DATA | ENTER CLIENTE INFO | INGRESE INFO CLIENTE
 37 | REENTER DRIVID | REENTRAR DRIVER ID | REINGRESE ID CHOFER
 38 | ENTER USER DATA | ENTER INFO USUÁRIO | INGRESE INFO USUARIO
 39 | ENTER CUST CODE | ENTER CODE. CLIENTE | INGRESE COD. CLIENTE
 40 | ENTER EMPLOYEE | ENTER FUNCIONÁRIO | INGRESE EMPLEADO
 41 | ENTER ID NUMBER | ENTER NÚMERO ID | INGRESE NUMERO ID
 42 | ENTER DRIVER ID | ENTER ID DRIVER | INGRESE ID CONDUCTOR
 43 | ENTER FLEET PIN | ENTER PIN FROTA | INGRESE PIN DE FLOTA
 44 | ODOMETER NUMBER | NÚMERO ODOMETER | ODOMETRO NUMERO
 45 | ENTER DRIVER LIC | ENTER DRIVER LIC | INGRESE LIC CONDUCTOR
 46 | ENTER TRAILER NO | NRO TRAILER ENTER | INGRESE NRO TRAILER
 47 | REENTER VEHICLE | REENTRAR VEÍCULO | REINGRESE VEHICULO
 48 | ENTER VEHICLE ID | ENTER VEÍCULO ID | INGRESE ID VEHICULO
 49 | ENTER BIRTH DATE | INSERIR DATA NAC | INGRESE FECHA NAC
 50 | ENTER DOB MMDDYY | ENTER FDN MMDDYY | INGRESE FDN MMDDAA
 51 | ENTER FLEET DATA | ENTER FROTA INFO | INGRESE INFO DE FLOTA
 52 | ENTER REFERENCE | ENTER REFERÊNCIA | INGRESE REFERENCIA
 53 | ENTER AUTH NUMBR | ENTER NÚMERO AUT | INGRESE NUMERO AUT
 54 | ENTER HUB NUMBER | ENTER HUB NRO | INGRESE NRO HUB
 55 | ENTER HUBOMETER | MEDIDA PARA ENTRAR HUB | INGRESE MEDIDO DE HUB
 56 | ENTER TRAILER ID | TRAILER ENTER ID | INGRESE ID TRAILER
 57 | ODOMETER READING | QUILOMETRAGEM | LECTURA ODOMETRO
 58 | REENTER ODOMETER | REENTRAR ODOMETER | REINGRESE ODOMETRO
 59 | REENTER DRIV. ID | REENTRAR DRIVER ID | REINGRESE ID CHOFER
 60 | ENTER CUSTOMER ID | ENTER CLIENTE ID | INGRESE ID CLIENTE
 61 | ENTER CUST. ID | ENTER CLIENTE ID | INGRESE ID CLIENTE
 62 | ENTER ROUTE NUM | ENTER NUM ROUTE | INGRESE NUM RUTA
 63 | ENTER FLEET NUM | FROTA ENTER NUM | INGRESE NUM FLOTA
 64 | FLEET PIN | FROTA PIN | PIN DE FLOTA
 65 | DRIVER # | DRIVER # | CONDUCTOR #
 66 | ENTER DRIVER # | ENTER DRIVER # | INGRESE CONDUCTOR #
 67 | VEHICLE # | VEÍCULO # | VEHICULO #
 68 | ENTER VEHICLE # | ENTER VEÍCULO # | INGRESE VEHICULO #
 69 | JOB # | TRABALHO # | TRABAJO #
 70 | ENTER JOB # | ENTER JOB # | INGRESE TRABAJO #
 71 | DEPT NUMBER | NÚMERO DEPT | NUMERO DEPTO
 72 | DEPARTMENT # | DEPARTAMENTO # | DEPARTAMENTO #
 73 | ENTER DEPT # | ENTER DEPT # | INGRESE DEPTO #
 74 | LICENSE NUMBER | NÚMERO DE LICENÇA | NUMERO LICENCIA
 75 | LICENSE # | LICENÇA # | LICENCIA #
 76 | ENTER LICENSE # | ENTER LICENÇA # | INGRESE LICENCIA #
 77 | DATA | INFO | INFO
 78 | ENTER DATA | ENTER INFO | INGRESE INFO
 79 | CUSTOMER DATA | CLIENTE INFO | INFO CLIENTE
 80 | ID # | ID # | ID #
 81 | ENTER ID # | ENTER ID # | INGRESE ID #
 82 | USER ID | USER ID | ID USUARIO
 83 | ROUTE # | ROUTE # | RUTA #
 84 | ENTER ROUTE # | ADD ROUTE # | INGRESE RUTA #
 85 | ENTER CARD NUM | ENTER NÚMERO DE CARTÃO | INGRESE NUM TARJETA
 86 | EXP DATE(YYMM) | VALIDADE VAL (AAMM) | FECHA EXP (AAMM)
 87 | PHONE NUMBER | TELEFONE | NUMERO TELEFONO
 88 | CVV START DATE | CVV DATA DE INÍCIO | CVV FECHA INICIO
 89 | ISSUE NUMBER | NÚMERO DE EMISSÃO | NUMERO DE EMISION
 90 | START DATE (MMYY) | DATA DE INÍCIO (AAMM) | FECHA INICIO (AAMM)
 */
-(RETURN_CODE) pin_getEncryptedData:(BOOL)lastPackage minLength:(int)minLength maxLength:(int)maxLength messageID:(int)mID language:(LANGUAGE_TYPE)lang;

/**
 * Display Message and Get Encrypted PIN online
 *
 Prompts the user with up to 3 lines of text. Returns pinblock/ksn of entered PIN value in deviceDelegate::pinpadData:keySN:event:() with event MessageID_PINEntry
 
 @param account Card account number
 @param type Encryption Key Type:
 - 0x00: External Account Key PIN_KEY_TDES_MKSK_extp
 - 0x01: External Account Key PIN_KEY_TDES_DUKPT_extp
 - 0x10: Internal Account Key PIN_KEY_TDES_MKSK_intl
 - 0x11: Internal Account Key PIN_KEY_TDES_DUKPT_intl
 @param line1 Display line 1, up to 12 characters
 @param line2 Display line 2, up to 16 characters
 @param line3 Display line 3, up to 16 characters
 -
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - If there is no any enter in 20 seconds, the entered PIN key will be cleared.
 - When press Enter key , it will end this Command and response package with NGA format .
 - When press Cancel key, the entered PIN  key will be cleared and if press Cancel key again, this command terminated.
 - Cancel Command can terminate this command.
 
 */
-(RETURN_CODE) pin_getEncryptedPIN:(NSString*)account keyType:(PIN_KEY_Types)type line1:(NSString*)line1 line2:(NSString*)line2 line3:(NSString*)line3;

/**
 * Display Message and Get Numeric Key(s)
 
 *
 Decrypt and display message on LCD. Requires secure message data. Returns value in inputValue of deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_NUMERIC
 
 @param maskInput If true, all entered data will be masked with asterik (*)
 @param minLength Minimum account number length - not less than 1
 @param maxLength Maximum account number length - not more than 16
 @param mID Message ID from approved message list.
 @param lang Language file to use for message
 @code
 typedef enum{
 LANGUAGE_TYPE_ENGLISH,
 LANGUAGE_TYPE_PORTUGUESE,
 LANGUAGE_TYPE_SPANISH
 }LANGUAGE_TYPE;
 @endcode
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - If there is no any enter in 20 seconds, the entered numeric key will be cleared.
 - When press Enter key , it will end this Command and response package with NGA  format .
 - When press Cancel key, the entered numeric key will be cleared and if press Cancel key again, this command terminated.
 - Cancel Command can terminate this command.
 
 \par Secure Messages
 Secure messages to be used with General Prompts commands
 
 Msg Id |English Prompt | Portuguese Prompt | Spanish Prompt
 ---------- | ---------- | ----------  | ----------
 1 | ENTER | ENTER | INGRESE
 2 | REENTER | RE-INTRODUZIR | REINGRESE
 3 | ENTER YOUR | INTRODUZIR O SEU | INGRESE SU
 4 | REENTER YOUR | RE-INTRODUZIR O SEU | REINGRESE SU
 5 | PLEASE ENTER | POR FAVOR DIGITE | POR FAVOR INGRESE
 6 | PLEASE REENTER | POR FAVO REENTRAR | POR FAVO REINGRESE
 7 | PO NUMBER | NÚMERO PO | NUMERO PO
 8 | DRIVER ID | LICENÇA | LICENCIA
 9 | ODOMETER | ODOMETER | ODOMETRO
 10 | ID NUMBER | NÚMERO ID | NUMERO ID
 11 | EQUIP CODE | EQUIP CODE | CODIGO EQUIP
 12 | DRIVERS ID | DRIVER ID | ID CONDUCTOR
 13 | JOB NUMBER | EMP NÚMERO | NUMERO EMP
 14 | WORK ORDER | TRABALHO ORDEM | ORDEN TRABAJO
 15 | VEHICLE ID | ID VEÍCULO | ID VEHICULO
 16 | ENTER DRIVER | ENTER DRIVER | INGRESE CONDUCTOR
 17 | ENTER DEPT | ENTER DEPT | INGRESE DEPT
 18 | ENTER PHONE | ADICIONAR PHONE | INGRESE TELEFONO
 19 | ENTER ROUTE | ROUTE ADD | INGRESE RUTA
 20 | ENTER FLEET | ENTER FROTA | INGRESE FLOTA
 21 | ENTER JOB ID | ENTER JOB ID | INGRESE ID TRABAJO
 22 | ROUTE NUMBER | NÚMERO PATH | RUTA NUMERO
 23 | ENTER USER ID | ENTER USER ID | INGRESE ID USUARIO
 24 | FLEET NUMBER | NÚMERO DE FROTA | FLOTA NUMERO
 25 | ENTER PRODUCT | ADICIONAR PRODUTO | INGRESE PRODUCTO
 26 | DRIVER NUMBER | NÚMERO DRIVER | CONDUCTOR NUMERO
 27 | ENTER LICENSE | ENTER LICENÇA | INGRESE LICENCIA
 28 | ENTER FLEET NO | ENTER NRO FROTA | INGRESE NRO FLOTA
 29 | ENTER CAR WASH | WASH ENTER | INGRESE LAVADO
 30 | ENTER VEHICLE | ENTER VEÍCULO | INGRESE VEHICULO
 31 | ENTER TRAILER | TRAILER ENTER | INGRESE TRAILER
 32 | ENTER ODOMETER | ENTER ODOMETER | INGRESE ODOMETRO
 33 | DRIVER LICENSE | CARTEIRA DE MOTORISTA | LICENCIA CONDUCTOR
 34 | ENTER CUSTOMER | ENTER CLIENTE | INGRESE CLIENTE
 35 | VEHICLE NUMBER | NÚMERO DO VEÍCULO | VEHICULO NUMERO
 36 | ENTER CUST DATA | ENTER CLIENTE INFO | INGRESE INFO CLIENTE
 37 | REENTER DRIVID | REENTRAR DRIVER ID | REINGRESE ID CHOFER
 38 | ENTER USER DATA | ENTER INFO USUÁRIO | INGRESE INFO USUARIO
 39 | ENTER CUST CODE | ENTER CODE. CLIENTE | INGRESE COD. CLIENTE
 40 | ENTER EMPLOYEE | ENTER FUNCIONÁRIO | INGRESE EMPLEADO
 41 | ENTER ID NUMBER | ENTER NÚMERO ID | INGRESE NUMERO ID
 42 | ENTER DRIVER ID | ENTER ID DRIVER | INGRESE ID CONDUCTOR
 43 | ENTER FLEET PIN | ENTER PIN FROTA | INGRESE PIN DE FLOTA
 44 | ODOMETER NUMBER | NÚMERO ODOMETER | ODOMETRO NUMERO
 45 | ENTER DRIVER LIC | ENTER DRIVER LIC | INGRESE LIC CONDUCTOR
 46 | ENTER TRAILER NO | NRO TRAILER ENTER | INGRESE NRO TRAILER
 47 | REENTER VEHICLE | REENTRAR VEÍCULO | REINGRESE VEHICULO
 48 | ENTER VEHICLE ID | ENTER VEÍCULO ID | INGRESE ID VEHICULO
 49 | ENTER BIRTH DATE | INSERIR DATA NAC | INGRESE FECHA NAC
 50 | ENTER DOB MMDDYY | ENTER FDN MMDDYY | INGRESE FDN MMDDAA
 51 | ENTER FLEET DATA | ENTER FROTA INFO | INGRESE INFO DE FLOTA
 52 | ENTER REFERENCE | ENTER REFERÊNCIA | INGRESE REFERENCIA
 53 | ENTER AUTH NUMBR | ENTER NÚMERO AUT | INGRESE NUMERO AUT
 54 | ENTER HUB NUMBER | ENTER HUB NRO | INGRESE NRO HUB
 55 | ENTER HUBOMETER | MEDIDA PARA ENTRAR HUB | INGRESE MEDIDO DE HUB
 56 | ENTER TRAILER ID | TRAILER ENTER ID | INGRESE ID TRAILER
 57 | ODOMETER READING | QUILOMETRAGEM | LECTURA ODOMETRO
 58 | REENTER ODOMETER | REENTRAR ODOMETER | REINGRESE ODOMETRO
 59 | REENTER DRIV. ID | REENTRAR DRIVER ID | REINGRESE ID CHOFER
 60 | ENTER CUSTOMER ID | ENTER CLIENTE ID | INGRESE ID CLIENTE
 61 | ENTER CUST. ID | ENTER CLIENTE ID | INGRESE ID CLIENTE
 62 | ENTER ROUTE NUM | ENTER NUM ROUTE | INGRESE NUM RUTA
 63 | ENTER FLEET NUM | FROTA ENTER NUM | INGRESE NUM FLOTA
 64 | FLEET PIN | FROTA PIN | PIN DE FLOTA
 65 | DRIVER # | DRIVER # | CONDUCTOR #
 66 | ENTER DRIVER # | ENTER DRIVER # | INGRESE CONDUCTOR #
 67 | VEHICLE # | VEÍCULO # | VEHICULO #
 68 | ENTER VEHICLE # | ENTER VEÍCULO # | INGRESE VEHICULO #
 69 | JOB # | TRABALHO # | TRABAJO #
 70 | ENTER JOB # | ENTER JOB # | INGRESE TRABAJO #
 71 | DEPT NUMBER | NÚMERO DEPT | NUMERO DEPTO
 72 | DEPARTMENT # | DEPARTAMENTO # | DEPARTAMENTO #
 73 | ENTER DEPT # | ENTER DEPT # | INGRESE DEPTO #
 74 | LICENSE NUMBER | NÚMERO DE LICENÇA | NUMERO LICENCIA
 75 | LICENSE # | LICENÇA # | LICENCIA #
 76 | ENTER LICENSE # | ENTER LICENÇA # | INGRESE LICENCIA #
 77 | DATA | INFO | INFO
 78 | ENTER DATA | ENTER INFO | INGRESE INFO
 79 | CUSTOMER DATA | CLIENTE INFO | INFO CLIENTE
 80 | ID # | ID # | ID #
 81 | ENTER ID # | ENTER ID # | INGRESE ID #
 82 | USER ID | USER ID | ID USUARIO
 83 | ROUTE # | ROUTE # | RUTA #
 84 | ENTER ROUTE # | ADD ROUTE # | INGRESE RUTA #
 85 | ENTER CARD NUM | ENTER NÚMERO DE CARTÃO | INGRESE NUM TARJETA
 86 | EXP DATE(YYMM) | VALIDADE VAL (AAMM) | FECHA EXP (AAMM)
 87 | PHONE NUMBER | TELEFONE | NUMERO TELEFONO
 88 | CVV START DATE | CVV DATA DE INÍCIO | CVV FECHA INICIO
 89 | ISSUE NUMBER | NÚMERO DE EMISSÃO | NUMERO DE EMISION
 90 | START DATE (MMYY) | DATA DE INÍCIO (AAMM) | FECHA INICIO (AAMM)
 */
-(RETURN_CODE) pin_getNumeric:(bool)maskInput minLength:(int)minLength maxLength:(int)maxLength messageID:(int)mID language:(LANGUAGE_TYPE)lang;





/**
 * Set PIN Length
 *
 Sets the encrypted PIN length.
 
 @param minLength Minimum PIN length at least 4
 @param maxLength Maximum PIN length not to exceed 12
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) pin_setPinLength:(int)minLength maxLength:(int)maxLength;



/**
 *Close Device
 */

-(void) close;
/**
 *Check if device is connected
 */
-(bool) isConnected;

/**
 * Attempt connection
 *
 Requests a connection attempt
 */
-(void) attemptConnect;

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



@end
