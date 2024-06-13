//
//  IDT_VP8800.h
//  IDTech
//
//  Created by Randy Palermo on 10/29/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "IDT_Device.h"

/** Protocol methods established for IDT_VP8800 class  **/
@protocol IDT_VP8800_Delegate <NSObject>
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

- (void) deviceMessage:(NSString*)message;//!<Receives messages from the framework
//!< @param message String message transmitted by framework


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
 Pinpad data delegate protocol
 
 Receives data from pinpad methods
 
 @param value encrypted data returned from IDT_VP3300::pin_getEncryptedData:minLength:maxLength:messageID:language:(), or encrypted account number returned from IDT_VP3300::pin_getCardAccount:max:line1:line2:(). String value returned from IDT_VP3300::pin_getAmount:maxLength:messageID:language:() or IDT_VP3300::pin_getNumeric:minLength:maxLength:messageID:language:(). PINblock returned from IDT_VP3300::pin_getEncryptedPIN:keyType:line1:line2:line3:()
 @param KSN Key Serial Number returned from IDT_VP3300::pin_getEncryptedPIN:keyType:line1:line2:line3:(), IDT_VP3300::pin_getCardAccount:max:line1:line2:() or IDT_VP3300::pin_getEncryptedData:minLength:maxLength:messageID:language:()
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
 EMV Transaction Data
 
 This protocol will receive results from IDT_Device::startEMVTransaction:otherAmount:timeout:cashback:additionalTags:()
 
 
 @param emvData EMV Results Data.  Result code, card type, encryption type, masked tags, encrypted tags, unencrypted tags and KSN
 
 @param error The error code as defined in the errors.h file
 
 
 */
- (void) emvTransactionData:(IDTEMVData*)emvData errorCode:(int)error;

- (void) gen2Data:(NSData*)tlv;//!<Receives Gen2 TLV data.
//!< @param tlv TLV data from gen2 event



@end

/**
 Class to drive the IDT_VP8800 device
 */
@interface IDT_VP8800 : NSObject<IDT_Device_Delegate>{
    id<IDT_VP8800_Delegate> delegate;
}

@property(strong) id<IDT_VP8800_Delegate> delegate;  //!<- Reference to IDT_VP8800_Delegate.



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
 Establishes an singleton instance of IDT_VP8800 class.
 
 @return  Instance of IDT_VP8800
 */
+(IDT_VP8800*) sharedController;

/**
 *Close Device
 */

-(void) close;



/**
 Cancels Transaction request (all interfaces, CTLS/MSR/INSERT).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */

-(RETURN_CODE) ctls_cancelTransaction;



/**
 Cancels Transaction request (all interfaces, CTLS/MSR/INSERT).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */

-(RETURN_CODE) emv_cancelTransaction;



/**
 Cancels Transaction request (all interfaces, CTLS/MSR/INSERT).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_cancelTransaction;


/**
 * Get Configuration Group
 *
 Retrieves the Configuration for the specified Group. Group 0 = terminal settings.
 
 @param group Configuration Group
 @param response Group TLV returned as a dictionary
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) ctls_getConfigurationGroup:(int)group response:(NSDictionary**)response;

/**
 * Set Merchant Record
 Sets the merchant record for ApplePay VAS
 *
 * @param index Merchant Record index, valid values 1-6
 * @param enabled Merchant Enabled/Valid flag
 * @param merchantID  Merchant unique identifer registered with Apple.  Example com.idtechproducts.applePay
 * @param merchantURL Merchant URL, when applicable
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_setMerchantRecord:(int)index enabled:(bool)enabled merchantID:(NSString*)merchantID merchantURL:(NSString*)merchantURL;

/**
 * Get Merchant Record
 Gets the merchant record for ApplePay VAS
 *
 * @param index Merchant Record index, valid values 1-6
 * @param record Data returned containing 99 bytes:
 * Byte 0 = Merchand Index
 * Byte 1 = Merchant Enabled (1 = enabled)
 * Byte 2 - 33 = Merchant Protocol Hash-256 value
 * Byte 34 = Length of Merchant URL
 * Bytes 35 - 99 = URL
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getMerchantRecord:(int)index record:(NSData*)record;


/**
 * Remove All Certificate Authority Public Key
 *
 Removes all the CAPK for CTLS
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()

 */
-(RETURN_CODE) ctls_removeAllCAPK;

/**
 * Remove Application Data by AID
 *
 Removes the Application Data for CTLS as specified by the AID name passed as a parameter
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010") Must be between 5 and 16 bytes
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_removeApplicationData:(NSString*)AID;

/**
 * Remove Certificate Authority Public Key
 *
 Removes the CAPK for CTLS as specified by the RID/Index
 
 @param capk 6 byte CAPK =  5 bytes RID + 1 byte INDEX
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeCAPK:(NSData*)capk;

/**
 * Remove Configuration Group
 *
 Removes the Configuration as specified by the Group.  Must not by group 0
 
 @param group Configuration Group
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeConfigurationGroup:(int)group;


/**
 * Retrieve AID list
 *
 Returns all the AID names installed on the terminal for CTLS.
 
 @param response  array of AID name as NSData*
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_retrieveAIDList:(NSArray**)response;

/**
 * Retrieve Application Data by AID
 * 
 Retrieves the CTLS Application Data for system default group 0 as specified by the AID name passed as a parameter
 
 @param AID Name of the Application ID in bytes. Must be between 5 and 16 bytes
 @param tlv The TLV elements of the requested AID in bytes
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) ctls_retrieveApplicationData:(NSData*)AID tlv:(NSData**)tlv;

/**
 * Retrieve Certificate Authority Public Key
 *
 Retrieves the CAPKfor CTLS as specified by the RID/Index  passed as a parameter.
 
 @param capk 6 bytes CAPK = 5 bytes RID + 1 byte Index
 @param key Response returned as a CAKey format:
 [5 bytes RID][1 byte Index][1 byte Hash Algorithm][1 byte Encryption Algorithm][20 bytes HashValue][4 bytes Public Key Exponent][2 bytes Modulus Length][Variable bytes Modulus]
 Where:
 - Hash Algorithm: The only algorithm supported is SHA-1.The value is set to 0x01
 - Encryption Algorithm: The encryption algorithm in which this key is used. Currently support only one type: RSA. The value is set to 0x01.
 - HashValue: Which is calculated using SHA-1 over the following fields: RID & Index & Modulus & Exponent
 - Public Key Exponent: Actually, the real length of the exponent is either one byte or 3 bytes. It can have two values: 3 (Format is 0x00 00 00 03), or  65537 (Format is 0x00 01 00 01)
 - Modulus Length: LenL LenH Indicated the length of the next field.
 - Modulus: This is the modulus field of the public key. Its length is specified in the field above.
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPK:(NSData*)capk key:(NSData**)key;


/**
 * Retrieve the Certificate Authority Public Key list
 *
 Returns all the CAPK RID and Index installed on the terminal for CTLS.
 
 @param keys NSArray of NSData CAPK name  (RID + Index)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPKList:(NSArray**)keys;

/**
 * Retrieve Terminal Data
 *
 Retrieves the Terminal Data for CTLS. This is configuration group 0 (Tag DFEE2D - > DFEE2D0100).  The terminal data
 can also be retrieved by ctls_getConfigurationGroup(0).
 
 @param tlv Response returned as a TLV
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE)  ctls_retrieveTerminalData:(NSData**)tlv;

/**
 * Set Application Data by AID
 *
 Sets the Application Data for CTLS as specified by TLV data
 
 @param tlv  Application data in TLV format
 The first tag of the TLV data must be the group number (DFEE2D).
 The second tag of the TLV data must be the AID (9F06)
 
 Example valid TLV, for Group #2, AID a0000000035010:
 "DFEE2D01029f0607a0000000051010DFEE4B0101DFEE2E0110DFEE4D0114DFEE4C0106"
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_setApplicationData:(NSData*)tlv;

/**
 * Set Certificate Authority Public Key
 *
 Sets the CAPK for CTLS as specified by the CAKey structure
 
 @param key CAKey format:
 [5 bytes RID][1 byte Index][1 byte Hash Algorithm][1 byte Encryption Algorithm][20 bytes HashValue][4 bytes Public Key Exponent][2 bytes Modulus Length][Variable bytes Modulus]
 Where:
 - Hash Algorithm: The only algorithm supported is SHA-1.The value is set to 0x01
 - Encryption Algorithm: The encryption algorithm in which this key is used. Currently support only one type: RSA. The value is set to 0x01.
 - HashValue: Which is calculated using SHA-1 over the following fields: RID & Index & Modulus & Exponent
 - Public Key Exponent: Actually, the real length of the exponent is either one byte or 3 bytes. It can have two values: 3 (Format is 0x00 00 00 03), or  65537 (Format is 0x00 01 00 01)
 - Modulus Length: LenL LenH Indicated the length of the next field.
 - Modulus: This is the modulus field of the public key. Its length is specified in the field above.
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_setCAPK:(NSData*)key;


/**
 * Set Configuration Group
 *
 Sets the Configuration Group for CTLS as specified by the TLV data
 
 @param tlv  Configuration Group Data in TLV format
 The first tag of the TLV data must be the group number (DFEE2D).
 A second tag must exist
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_setConfigurationGroup:(NSData*)tlv;


/**
 * Set Terminal Data
 *
 * Sets the Terminal Data for CTLS transaction and general terminal settings as specified by the TLV.  If the value already exists in terminal data, it will be updated.
 * If the value does not exist, it will be added.
 
 @param tlv TerminalData configuration data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_setTerminalData:(NSData*)tlv;




/**
 * Start a CTLS Transaction Request
 *
 Authorizes the CTLS transaction for an CTLS card
 
 The tags will be returned in the callback routine.
 
 @param amount Transaction amount value  (tag value 9F02)
 @param exponent Number of characters after decimile point
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param tags Any other tags to be included in the request.
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 *
 * NOTE ON INTERFACE SELECTION: For the VP8800, tag DFEF37 is used to determine which interfaces to use for the transaction:
  - 01 = MSR Only
  - 02 = CTLS Only
  - 03 = MSR + CTLS
  - 04 = EMV Only
  - 05 = EMV + MSR
  - 06 = EMV + CTLS
  - 07 = EMV + MSR + CLTS.
 This API method will automatically send DFEF37 with a value of 02 if this tag is not provided.
 *
 */
-(RETURN_CODE) ctls_startTransaction:(double)amount type:(int)type timeout:(int)timeout tags:(NSMutableDictionary *)tags;

/**
 * Start a CTLS Transaction Request
 *
 Authorizes the CTLS transaction for an CTLS card, including AppleVAS and SmartTAP
 
 The tags will be returned in the callback routine.
 
 @param amount Transaction amount value  (tag value 9F02)
 @param exponent Number of characters after decimile point
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param transTags Any other transaction tags to be included in the request (excluding Apple VAS and SmartTap)
 @param VAS Tags to enable the capture of Apple VAS
 * To enable ApplePay VAS, first a merchant record must be defined in one of the six available index positions (1-6) using device_setMerchantRecord
 * the VAS data must contain tags 9F26 and 9F22, and can optionanally contain tags 9F2B and DFO1.
 * Example 9F220201009F2604000000009F2B050100000000DF010101
 * 9F22 = two bytes = ApplePay Terminal Applicaiton Version Number.  Hard defined as 0100 for now. (required)
 * 9F26 = four bytes = ApplePay Terminal Capabilities Information (required)
 *  - Byte 1 = RFU
 *  - Byte 2 = Terminal Type
 *  - - Bit 8 = VAS Support  (1=on, 0 = off)
 *  - - Bit 7 = Touch ID Required  (1=on, 0 = off)
 *  - - Bit 6 = RFU
 *  - - Bit 5 = RFU
 *  - - Bit 1,2,3,4
 *  - - - 0 = Payment Terminal
 *  - - - 1 = Transit Terminal
 *  - - - 2 = Access Terminal
 *  - - - 3 = Wireless Handoff Terminal
 *  - - - 4 = App Handoff Terminal
 *  - - - 15 = Other Terminal
 *  - Byte 3 = RFU
 *  - Byte 4 = Terminal Mode
 *  - - 0 = ApplePay VAS OR ApplePay
 *  - - 1 = ApplePay VAS AND ApplePay
 *  - - 2 = ApplePay VAS ONLY
 *  - - 3 = ApplePay ONLY
 *  9F2B = 5 bytes = ApplePay VAS Filter.  Each byte filters for that specific merchant index  (optional)
 *  DF01 = 1 byte = ApplePay VAS Protocol.  (optional)
 *  - - Bit 1 : 1 = URL VAS, 0 = Full VAS
 *  - - Bit 2 : 1 = VAS Beeps, 0 = No VAS Beeps
 *  - - Bit 3 : 1 = Silent Comm Error, 2 = EMEA Comm Error
 *  - - Bit 4-8 : RFU
 *
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 *
 * NOTE ON INTERFACE SELECTION: For the VP8800, tag DFEF37 is used to determine which interfaces to use for the transaction:
 - 01 = MSR Only
 - 02 = CTLS Only
 - 03 = MSR + CTLS
 - 04 = EMV Only
 - 05 = EMV + MSR
 - 06 = EMV + CTLS
 - 07 = EMV + MSR + CLTS.
 This API method will automatically send DFEF37 with a value of 02 if this tag is not provided.
 *
 *
 * NOTE ON APPLEPAY VAS:

 */
-(RETURN_CODE) ctls_startTransaction:(double)amount type:(int)type timeout:(int)timeout transTags:(NSData *)transTags VAS:(NSData *)VAS;

/**
 * Display Online Authorization Result Extended
 * 
 Use this command to display the status of an online authorization request on the reader's display (OK, NOT OK, or ARC). Use this command after the reader sends an online request to the issuer
 
 @param statusCode 0 = NOT OK, 1 = OK, 2 = ARC
 @param TLV Optional TLV data for AOSA, can be empty
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) ctls_displayOnlineAuthResult_ext:(Byte)statusCode TLV:(NSData*)TLV;

/**
 * Display Online Authorization Result
 * 
 Use this command to display the status of an online authorization request on the reader's display (OK or NOT OK). Use this command after the reader sends an online request t the issuer
 @param isOK True = OK, False = NOT OK
 @param TLV Optional TLV for the AOSA, can be empty
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) ctls_displayOnlineAuthResult:(BOOL)isOK TLV:(NSData*)TLV;

/**
 * Reset Configuration Group
 * 
 This command allows resetting a dataset to its default configuration
 If the file exists, it will be overwritten. If not, it will be created
 
 @param group Configuration group
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) ctls_resetConfigurationGroup:(int)group;


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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;


/**
 * Begins searching for Bluetooth Low Energy devices in range
 - VP8800
 *
 * This method will UNPAIR and DISCONNECT from any current PAIRED and CONNECTED devices before starting a new BLE Device Search.  This function is for PAIRING a new Device and
 * connecting to it.  Once a device is PAIRED/CONNECTED, the search will terminate.  If a paired devices becomes disconnected (out of range, power cycle), the BLE search will automatically
 * start again to reconnect to the devices once available
 *
 * @param identifier This will only connect to a device with this calculated UUID identifier.  If nil is passed, it will connect to the first devices with the default friendly name (IDT_VP8800::device_getBLEFriendlyName / IDT_VP8800::device_setBLEFriendlyName)
 *
 * @return bool  If successful, polling has started
 *
 * Any of the following BLE status messages may be returned to the deviceMessage delegate:
 - This device does not support Bluetooth Low Energy.
 - This app is not authorized to use Bluetooth Low Energy.
 - Bluetooth on this device is currently powered off.
 - The BLE Manager is resetting; a state update is pending.
 - Bluetooth LE is turned on and ready for communication.
 - The state of the BLE Manager is unknown.
 *
 * Note: a Devices UUID is calculated by the iOS device using a combiniation of the iOS device UUID and the BLE device MAC address.  This value is not known until after it connects for the first time, and then every time after that, it will be the same value.  This value can be retrieved by IDT_Device::connectedBLEDevice() after the device connects.
 */
-(bool) device_enableBLEDeviceSearch:(NSUUID*)identifier;



/**
 * Get BLE Friendly Name
 *
 *
 * @return NSString  Returns the default friendly name to be used when discovering any BLE devices
 *
 */
-(NSString*) device_getBLEFriendlyName;


/**
 * Set BLE Friendly Name
 *
 *
 * @param friendlyName  Sets the default friendly name to be used when discovering any BLE devices
 *
 */
-(void) device_setBLEFriendlyName:(NSString*)friendlyName;

/**
 * Set BLE Friendly Name
 *
 *
 * @param friendlyName  Sets the default friendly name to be used when discovering any BLE devices. SDK will mount
 *
 */
-(void) device_setBLEFriendlyNamePrefix:(NSString*)friendlyName;

/**
 * Stops searching for Bluetooth Low Energy devices in range
 - VP8800
 *
 *
 * @return bool  If successful, polling was in progress and has stopped. If unsuccessful, BLE Device Search was not in progress.
 *
 * NOTE:  BLE only scans when there are no devices currently connected. After the SDK connects to any IDTech device, the scanning will pause automatically.
 */
-(bool) device_disableBLEDeviceSearch;

/**
 * Returns the UUID of the connected BLE device
 - VP8800
 *
 * @return NSUUID  UUID of the connected BLE device.  Returns nil if no BLE device connected.
 *
 */
-(NSUUID*) device_connectedBLEDevice;


/**
 * Disconnect from BLE -
 *
 * Will disconnect from existing BLE connection. You can now set another BLE Friendly Name to attach to another device.
 *
 */
-(void) device_disconnectBLE;

/**
 * Get Transaction Results
 Gets the transaction results when the reader is functioning in "Auto Poll" mode
 *
 * @param result The transaction results
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
 *
 */
-(RETURN_CODE)  device_getAutoPollTransactionResults:(IDTEMVData**)result;

/**
 * Get Response Code String
 *
 Interpret a response code and return string description.
 
 @param errorCode Error code, range 0x0000 - 0xFFFF, example 0x0300
 
 
 * @return Verbose error description
 
 
 */
-(NSString *) device_getResponseCodeString:(int) errorCode;

/**
 Is Device Connected
 
 Returns the connection status of the requested device
 
 @param device Check connectivity of device type
 
 @code
 typedef enum{
 IDT_DEVICE_VP8800_IOS = 16
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) device_sendIDGCommandV3:(unsigned char)command subCommand:(unsigned char)subCommand data:(NSData*)data response:(NSData**)response;


/**
 * Send 2nd Gen Command
 *
 Informs SDK to format command for EMV Gen2 Device Communication
 
 @param tlv  TLV Command
 @param response  TLV Response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:())
 
 */
-(RETURN_CODE) device_sendGen2Cmd:(NSData*)tlv response:(NSData**)response;

/**
 * Set Pass Through
 
 Sets Pass-Through mode on VP8800
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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setPassThrough:(BOOL)enablePassThrough;



/**
 * Send Burst Mode
 *
 * Sets the burst mode forthe device.
 *
 * @param mode 0 = OFF, 1 = Always On, 2 = Auto Exit
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE)  device_setBurstMode:(int) mode;


/**
 * Send Poll Mode
 *
 * Sets the poll mode forthe device. Auto Poll keeps reader active, Poll On Demand only polls when requested by terminal
 *
 * @param mode 0 = Auto Poll, 1 = Poll On Demand
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setPollMode:(int) mode;

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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 */

-(RETURN_CODE) device_rebootDevice;


/**
 * Authenticate Transaction
 
 Authenticates a transaction after startTransaction successfully executes.
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function must be called after a result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS returned to emvTransactionData delegate protocol is received after a startTransaction call.  If auto authorize is ENABLED (default), this method will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS after startTransaction.  The auto authorize can be enabled/disabled with IDT_DEVICE::disableAutoAuthenticateTransaction:()
 *
 The purpose of this step is to allow the merchant the chance to evaluate the data captured from the matching Application (if found) before the kernel authenticates the transaction data.  This would allow, for instance, the merchant to be told what card is being used, and if it is a specific type (like a store card), perform an action like reducing the amount before proceeding (as a promotion in using that card).
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_authenticateTransaction:(NSData*)tags;
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_callbackResponsePIN:(EMV_PIN_MODE_Types)mode KSN:(NSData*)KSN PIN:(NSData*)PIN;

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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) emv_getEMVL2Version:(NSString**)response;

/**
 * Remove Application Data by AID
 *
 This will REMOVE the an AID configuration file and all the tlv data associated with that AID.
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 If null or empty string is passed, it will remove ALL application data
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to BTPay::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCAPK:(NSString*)rid index:(NSString*)index ;

/**
 Sets the terminal major configuration in ICS .
 @param configuration A configuration value, range 1-5
 - 1 = 1C
 - 2 = 2C
 - 3 = 3C
 - 4 = 4C
 - 5 = 5C
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getTerminalMajorConfiguration:(NSUInteger**)configuration;


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
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCRLList;


/**
 * Remove Terminal Data
 *
 
 This will remove ALL configurable TLV data associated with the terminals Kernel configuration.
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeTerminalData;

/**
 * Retrieve AID list
 *
 Returns all the AID names on the terminal.  Populates response parameter with an Array of NSString* with AID names.  Each AID name represent a unique configuration file to be loaded/used when a matching application is found on a card during a transaction.
 
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
 - 0x0100 through 0xFFFF refer to BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveAIDList:(NSArray**)response;


/**
 * Retrieve Application Data by AID
 *
 Retrieves the configuration information for a provided AID name, if that AID file exists on the terminal.
 
 The TLV data in that AID is returned as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "9F06"), and the Object being the Value as NSData (example 0xa0000000031010).
 
 The data returned will be from the range of allowable kernel EMV tags.  Please see "EMV Tag Reference" at the end of this document for the listing.
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 @param responseAID  The response returned from the method as a dictionary with Key/Object to match TagValues as follows:
 
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 
 
 
 */
-(RETURN_CODE) emv_retrieveApplicationData:(NSString*)AID response:(NSDictionary**)responseAID;




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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 
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
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveCAPKList:(NSArray**)response;

/**
 * Retrieve Certificate Authority Public Key
 *
 Retrieves the CAPK for EMV Kernel as specified by the RID/Index  passed as a parameter.
 
 @param capk 6 bytes CAPK = 5 bytes RID + 1 byte Index
 @param key Response returned as a CAKey format:
 [5 bytes RID][1 byte Index][1 byte Hash Algorithm][1 byte Encryption Algorithm][20 bytes HashValue][4 bytes Public Key Exponent][2 bytes Modulus Length][Variable bytes Modulus]
 Where:
 - Hash Algorithm: The only algorithm supported is SHA-1.The value is set to 0x01
 - Encryption Algorithm: The encryption algorithm in which this key is used. Currently support only one type: RSA. The value is set to 0x01.
 - HashValue: Which is calculated using SHA-1 over the following fields: RID & Index & Modulus & Exponent
 - Public Key Exponent: Actually, the real length of the exponent is either one byte or 3 bytes. It can have two values: 3 (Format is 0x00 00 00 03), or  65537 (Format is 0x00 01 00 01)
 - Modulus Length: LenL LenH Indicated the length of the next field.
 - Modulus: This is the modulus field of the public key. Its length is specified in the field above.
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveCAPK:(NSData*)capk key:(NSData**)key;



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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveCRLList:(NSMutableArray**)response;





/**
 * Retrieve Terminal Data
 
 *
 Retrieves the tag values associated with the terminal configuration file.  This will be a combination of uneditable major configuration tags for the kernel configuration (example 9F33, Terminal Capabilities), and editable tags set with IDT_Device::emv_setTerminalData:()  (example DF13, Terminal Action Code - Default)
 
 The TLV data returned as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "DF13"), and the Object being the Value as NSData (example 0x00058003FF).
 
 The data returned will be from the range of allowable kernel EMV tags.  Please see "EMV Tag Reference" at the end of this document for the listing.
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveTerminalData:(NSDictionary**)responseData;

/**
 * Retrieve Transaction Results
 
 *
 Retrieves the requested tag values (if they exist) from the last transaction.
 
 The TLV data returned as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "5A"), and the Object being the Value as NSData (example 0x41359276429372938).
 
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveTransactionResult:(NSData*)tags retrievedTags:(NSDictionary**)retrievedTags;

/**
 * Set Application Data by AID
 *
 Sets the configuration information for a provided AID name, with TLV data that populates a NSDictionary.
 
 The TLV data for the AID is sent as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "9F06"), and the Object being the Value as NSData (example 0xa0000000031010).
 
 The data for the AID configuration will will be from the range of allowable kernel EMV tags.  Please see "EMV Tag Reference" at the end of this document for the listing.
 
 NOTES:
 There is no minimum defined set of AID TLV data that must be provided, other than 9F06 for the AID name.
 
 If this AID is selected and matched during an EMV transaction, any data in this AID will either OVERRIDE the same data in the terminal configuration file, or PROVIDE the data if it is non-existant in the terminal configuration file.
 
 AID configuration information is provided during L3 certification.  Dummy/stub AID data can be used pre-certification to test EMV transaction as long as at least tag 9F06 is defined that makes up the AID configuration locator.
 
 There are convenience utilities to turn a TLV NSData object into a NSDictionary, and a NSDictionary into a NSData object in IDTUtility:
 @code
 +(NSDictionary*) TLVtoDICT:(NSData*)param;
 +(NSData*) DICTotTLV:(NSDictionary*)tags;
 @endcode
 
 Also utilities to turn a HEX/ASCII string to NSDATA and back again
 
 @code
 + (NSData *)hexToData:(NSString*)str ;
 +(NSString*) dataToHexString:(NSData*)data;
 @endcode
 
 
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setApplicationData:(NSString*)aidName configData:(NSDictionary*)data;



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
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCRLEntries:(NSData*)data;




/**
 * Set Terminal Data
 
 *
 Sets the terminal configuration information, with TLV data that populates a NSDictionary.
 
 The TLV data for the terminal configuration is sent as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "DF13"), and the Object being the Value as NSData (example 0x00080039FF).
 
 The data for the terminal configuration will will be from the range of allowable kernel EMV tags.  Please see "EMV Tag Reference" at the end of this document for the listing.
 
 NOTES:
 There is an uneditable set of tags that make up the current kernel configuration major parameters.  Any attempt to set those will return an error.
 
 If an AID is selected and matched during an EMV transaction, any data in that AID will either OVERRIDE the same data in the terminal configuration file, or PROVIDE the data if it is non-existant in the terminal configuration file.
 
 
 There are convenience utilities to turn a TLV NSData object into a NSDictionary, and a NSDictionary into a NSData object in IDTUtility:
 @code
 +(NSDictionary*) TLVtoDICT:(NSData*)param;
 +(NSData*) DICTotTLV:(NSDictionary*)tags;
 @endcode
 
 Also utilities to turn a HEX/ASCII string to NSDATA and back again
 
 @code
 + (NSData *)hexToData:(NSString*)str ;
 +(NSString*) dataToHexString:(NSData*)data;
 @endcode
 
 
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setTerminalData:(NSDictionary*)data;

/**
 * Set Terminal Data VP8800
 * 
 Sets the terminal data for the VP8800
 
 @param tlv Terminal tlv data
 @param config Config verification, valid values 1-4
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_setTerminalDataVP8800:(NSData*)tlv config:(int)config;

/**
 * Get EMV Configuration Check Value
 * 
 Polls device for the EMV Configuration Check Value
 
 @param response Response returned of the Check Value of the Configuration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVConfigurationCheckValue:(NSString**)response;

/**
 * Callback Response MSR Entry
 * 
 Provides MSR information to kernel after a callback was received with type EMV_CALLBACK_MSR
 
 @param MSR Swiped track data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_callbackResponseMSR:(NSData*)MSR;

/**
 * Set EMV Exception
 * 
 Adds an entry to the EMV Exception List
 
 @param exception EMV exception entry containing the PAN and sequence number where:
 *      -Exception is 12 bytes
 *      -[1 byte len][10 bytes PAN][1 byte sequence number]
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_setException:(NSData*)exception;

/**
 * Remove EMV Exception
 * 
 Removes an exception EMV entry containing the PAN and Sequence number where:
 @param exception  [Exception] is 12 bytes:
 *      - [1 byte len][10 bytes PAN][1 byte sequence number]
 *      -PAN, in compressed numeric format, is padded with 0xff if required. For example 0x5413339000001596FFFF)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:() 
 */
-(RETURN_CODE) emv_removeException:(NSData*)exception;

/**
 * Remove All EMV Exceptions
 * 
 Removes all the entries from the EMV Exception List
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_removeAllExceptions;

/**
 * Retrieve the EMV Exception List
 * 
 Returns the EMV Exception entries on the terminal
 
 @param list The list of exception entries ([Exeption1], [Exception2],...[ExceptionN] where each is 12 bytes:
 *      - [1 byte Len][10 bytes PAN][1 byte sequence number]
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveExceptionList:(NSData**)list;

/**
 * Start EMV Transaction Request
 *
 Authorizes the EMV transaction  for an ICC card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function will complete with a return of EMV_RESULT_CODE_START_TRANSACTION_SUCCESS to emvTransactionData delegate protocol, and then IDT_VP8800::emv_authenticateTransaction() must be executed.  If auto authorize is ENABLED (default), IDT_VP8800::emv_authenticateTransaction() will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS.  The auto authorize can be enabled/disabled with IDT_VP8800::emv_disableAutoAuthenticateTransaction:()
 
 @param amount Transaction amount value  (tag value 9F02)
 @param amtOther Other amount value, if any  (tag value 9F03)
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param tags Any other tags to be included in the request.  Passed as NSData.  Example, tag 9F0C with amount 0x000000000100 would be 0x9F0C06000000000100
 If tags 9F02 (amount),9F03 (other amount), or 9C (transaction type) are included, they will take priority over these values supplied as individual parameters to this method.
 Tag DFEE1A can be used to specify tags to be returned in response, in addition to the default tags. Example DFEE1A049F029F03 will return tags 9F02 and 9F03 with the response
 
 @param forceOnline TRUE = do not allow offline approval,  FALSE = allow ICC to approve offline if terminal capable
 @param autoAuthenticate Will automatically execute Authenticate Transacation after start transaction returns successful
 @param fallback Indicate if it supports fallback to MSR
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 *
 * NOTE ON INTERFACE SELECTION: For the VP8800, tag DFEF37 is used to determine which interfaces to use for the transaction:
 - 01 = MSR Only
 - 02 = CTLS Only
 - 03 = MSR + CTLS
 - 04 = EMV Only
 - 05 = EMV + MSR
 - 06 = EMV + CTLS
 - 07 = EMV + MSR + CLTS.
 This API method will automatically send DFEF37 with a value of 04 if this tag is not provided.
 *
 */
-(RETURN_CODE) emv_startTransaction:(double)amount amtOther:(double)amtOther type:(int)type timeout:(int)timeout tags:(NSData*)tags forceOnline:(BOOL)forceOnline fallback:(BOOL)fallback;

/**
 * Get Transaction Log Record
 * 
 Retrieves oldest transaction record on the transaction log. Upon successful completion, the oldest transaction record is deleted from the transaction log
 
 @param record Transaction Record
 @param remaining The number of records remaining on the transaction log
 
 Transaction Record
 --------------------------
 Length | Description | Type
 ----- | ----- | -----
 4 | Transaction Log State (TLS) | Enum (4-byte number, LSB first), SENT ONLINE = 0, NOT SENT = 1
 4 | Transaction Log Content (TLC) | Enum (4-byte number, LSB first), BATCH = 0, OFFLINE ADVICE = 1, ONLINEADVICE = 2, REVERSAL = 3
 4 | AppExpDate | unsigned char [4]
 3 | AuthRespCode | unsigned char [3]
 3 | MerchantCategoryCode | unsigned char [3]
 16 | MerchantID | unsigned char [16]
 2 | PosEntryMode | unsigned char [2]
 9 | TermID | unsigned char [9]
 3 | AIP | unsigned char [3]
 3 | ATC | unsigned char [3]
 33 | IssuerAppData | unsigned char [33]
 6 | TVR | unsigned char [6]
 3 | TSI | unsigned char [3]
 11 | Pan | unsigned char [11]
 2 | PanSQNCNum | unsigned char [2]
 3 | TermCountryCode | unsigned char [3]
 7 | TranAmount | unsigned char [7]
 3 | TranCurCode | unsigned char [3]
 4 | TranDate | unsigned char [4]
 2 | TranType | unsigned char [2]
 9 | IFDSerialNum | unsigned char [9]
 12 | AcquirerID | unsigned char [12]
 2 | CID | unsigned char [2]
 9 | AppCryptogram | unsigned char [9]
 5 | UnpNum | unsigned char [5]
 7 | AmountAuth | unsigned char [7]
 4 | AppEffDate | unsigned char [4]
 4 | CVMResults | unsigned char [4]
 129 | IssScriptResults | unsigned char [129]
 4 | TermCap | unsigned char [4]
 2 | TermType | unsigned char [2]
 20 | Track2 | unsigned char [20]
 4 | TranTime | unsigned char [4]
 7 | AmountOther | unsigned char [7]
 1 | Unused | Unsigned char [1]
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_getTransactionLogRecord:(NSData**)record remaining:(int*)remaining;

/**
 * Get EMV Exemption Log Status
 * 
 This command returns information about the EMV Exemption log. The version number, record size, and number of records contained in the file are returned
 
 @param status 12 bytes are returned
 *  - bytes 0-3: Version Number
 *  - bytes 4-7: Number of records
 *  - bytes 8-11: Size of record
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getExemptionLogStatus:(NSData**)status;

/**
 * Get Kernel Check Value
 *
 Polls the device for the Kernel Check Value
 
 @param response Response returned of the Check Value of the Kernel
 * @return RETURN_CODE: Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelCheckValue:(NSString**)response;

/**
 * Set Certificate Authority Public Key
 *
 Sets the CAPK for EMV Kernel as specified by the CAKey structure
 
 @param key CAKey format:
 [5 bytes RID][1 byte Index][1 byte Hash Algorithm][1 byte Encryption Algorithm][20 bytes HashValue][4 bytes Public Key Exponent][2 bytes Modulus Length][Variable bytes Modulus]
 Where:
 - Hash Algorithm: The only algorithm supported is SHA-1. The value is set to 0x01.
 - Encryption Algorithm: The encryption algorithm in which this key is used. Currently support only one type: RSA. The value is set to 0x01.
 - HashValue: Which is calculated using SHA-1 over the following fields: RID & Index & Modulus & Exponent.
 - Pubic Key Exponent: Actually, the real length of the exponent is either one byte or 3 bytes. It can have two values: 3 (Format is 0x00 00 00 03), or 65537 (Format is 0x00 01 00 01)
 - Modulus Length: LenL LenH Indicated the length of the next field.
 - Modulus: This is the modulus field of the public key. Its length is specified in the field above.
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCAPK:(NSData*)key;


/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) icc_exchangeAPDU:(NSData*)dataAPDU response:(APDUResponse**)response;
/**
 * Get Reader Status
 *
 Returns the reader status
 
 @param readerStatus Pointer that will return with the ICCReaderStatus results.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) icc_setKeyFormatForICCDUKPT:(NSData*)encryption;

/**
 * Disable MSR Swipe
 
 
 
 Cancels Swipe.
 
 Cancels Transaction request (all interfaces, CTLS/MSR/INSERT).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;

/**
 * Enable MSR Swipe
 
 *
 Enables CLTS and MSR, waiting for swipe or tap to occur. Returns IDTEMVData to deviceDelegate::emvTransactionData:() 
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe;

/**
 * Flush Track Data
 *
 Clears any track data being retained in memory by future PIN Block request
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_flushTrackData;

/**
 * Control LED
 *
 Controls the LED for the reader. This command will only operate in pass-through mode
 
 @param indexLED The LED to control starting from the left
 - 00: LED 0
 - 01: LED 1
 - 02: LED 2
 - 03: LED 3
 - FF: All LEDs
 @param control Turns chosen LED(s) OFF (00) or ON (01)
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_controlLED:(Byte)indexLED control:(Byte)control;

/**
 * Control Indicator
 *
 Control the reader. If connected, returns success, otherwise, returns timeout
 
 @param indicator Selects what is being controlled
 - 00h: ICC LED
 - 01h: Blue MSR
 - 02h: Red MSR
 - 03h: Green MSR
 @param enable True = ON, False = OFF
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_controlIndicator:(int)indicator enable:(BOOL)enable;

/**
 * Device Certificate Type
 *
 Returns the device certificate type
 
 @param type 0 = Unknown, 1 = Demo, 2 = Production
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_certificateType:(int*)type;

/**
 * Delete File
 *
 This command deletes a file or group of files
 
 @param filename Complete path and file name of the file you want to delete. You do not need to specify the root directory. Indicate subdirectories with a foward slash (/)
 @param isSD True = Delete from SD card, False = Delete from Flash
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_deleteFile:(NSString*)filename isSD:(BOOL)isSD;

/**
 * Delete Directory
 *
 This command deletes an empty directory
 
 @param filename Complete path of the directory you want to delete.
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_deleteDirectory:(NSString*)filename;

/**
 * List Directory
 *
 This command retrieves a directory listing of user accessible files from the reader
 
 @param directoryName The directory name. If null, root directory is selected
 @param recursive Include sub-directories
 @param onSD True = use SD card, False = use Flash
 @param directory The returned directory information
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_listDirectory:(NSString*)directoryName recursive:(BOOL)recursive onSD:(BOOL)onSD directory:(NSString**)directory;

/**
 * Create Directory
 *
 This command adds a subdirectory to the indicated path
 
 @param directoryName The directory name. The data for this command is an ASCII string with the complete path and directory name you want to create. You do not need to specify the root directory. Indicate subdirectories with a forward slash (/)
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_createDirectory:(NSString*)directoryName;

/**
 * Drive Free Space
 *
 This command returns the free and used disk space on the flash drive
 
 @param free Free bytes available on device
 @param used Used bytes on device
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getDriveFreeSpace:(int*)free used:(int*)used;

/**
 * Set Buzzer/LED
 *
 Sets the reader's buzzer and LEDs
 
 @param buzzer Sets the configuration of the buzzer
 - 00h: No Beep
 - 01h: Single Beep
 - 02h: Two beeps
 - 03h: Three beeps
 - 04h: Four beeps
 - 05h: One long beep of 200ms
 - 06h: One long beep of 400ms
 - 07h: One long beep of 600ms
 - 08h: One long beep of 800ms
 @param led
 - 00h: LED0 (leftmost LED)
 - 01h: LED1
 - 02h: LED2
 - 03h: LED3
 - FFh: All LEDs
 @param ledON True = ON, False = OFF
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setBuzzerLED:(Byte)buzzer led:(Byte)LED ledON:(BOOL)ledON;

/**
 * Control User Interface
 *
 Controls the Display, Beep, and LED
 
 @param values Four bytes to control the user interface elements
 Byte[0] = LCD Message
 - 00h: Idle Message (Welcome)
 - 01h: Present Card (Please Present Card)
 - 02h: Timeout or Transaction Canceled (No Card)
 - 03h: Transaction between reader and card is in progress (Processing...)
 - 04h: Transaction success (Thank You)
 - 05h: Transaction failed (Failed)
 - 06h: Amount (Amount $ 0.00 Tap Card)
 - 07h: Balance or Offline available funds (Balance $ 0.00)
 - 08h: Insert card (Use Chip & PIN)
 - 09h: Try again (Tap Again)
 - 0Ah: Tells the customer to present only one card (Present 1 Card Only)
 - 0Bh: Tells the customer to wait for authentication/authorization (Wait)
 - FFh: Indicates the command is setting the LED/Buzzer only
 Byte[1] = Beep Indicator
 - 00h: No beep
 - 01h: Single beep
 - 02h: Double beep
 - 03h: Triple beep
 - 04h: Quadruple beep
 - 05h: Single long beep (200 ms)
 - 06h: Single long beep (400 ms)
 - 07h: Single long beep (600 ms)
 - 08h: Single long beep (800 ms)
 Byte[2] = LED
 - 00h: LED 0 (Power LED)
 - 01h: LED 1
 - 02h: LED 2
 - 03h: LED 3
 - FFh: All LEDs
 Byte[3] = LED Power
 - 00h: LED OFF
 - 01h: LED ON
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_controlUserInterface:(NSData*)values;

/**
 * Get MSR Secure Parameters
 *
 Gets the parameters from the flash setting
 
 @param b0 True = T1 force encryption
 @param b1 True = T2 force encryption
 @param b2 True = T3 force encryption
 @param b3 True = T3 force encryption when card type is 80
 @param tlv MSR secure parameters TLV objects
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getMsrSecurePara:(BOOL)b0 b1:(BOOL)b1 b2:(BOOL)b2 b3:(BOOL)b3 tlv:(NSData**)tlv;

/**
 * Get Module Version Information
 *
 Gets the 16 byte UID of the MCU
 
 @param uid The string representation of the UID
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getModuleVer:(NSString**)moduleVer;

/**
 * Calibrate reference parameters
 * 
 Calibrates parameters with a given delta value
 
 @param delta Delta value (0x02 is the standard value)
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_calibrateParameters:(Byte)delta;

/**
 *Check if device is connected
 */
-(bool) isConnected;



/**
 * Start a  Transaction Request
 *
 Authorizes the CTLS transaction for an CTLS card
 
 The tags will be returned in the callback routine.
 
 @param amount Transaction amount value  (tag value 9F02)
 @param exponent Number of characters after decimile point
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param tags Any other tags to be included in the request.
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 *
 * NOTE ON INTERFACE SELECTION: For the VP8800, tag DFEF37 is used to determine which interfaces to use for the transaction:
 - 01 = MSR Only
 - 02 = CTLS Only
 - 03 = MSR + CTLS
 - 04 = EMV Only
 - 05 = EMV + MSR
 - 06 = EMV + CTLS
 - 07 = EMV + MSR + CLTS.
 This API method will automatically send DFEF37 with a value of 07 if this tag is not provided.
 *
 * USE tag DFEF3C for fallback support and timeout waiting for insertion
 * byte 1: = fallback support  01 = YES, 00 = NO
 * byte 2-3 = timeout in BCD.  Example 60 seconds is 0060
 *
 */
-(RETURN_CODE) device_startTransaction:(double)amount type:(int)type timeout:(int)timeout tags:(NSData*)tags;

/**
 * Get Poll Mode
 *
 * Gets the current poll mode of the device
 *
 * @param mode Response from the device of the current poll mode
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getPollMode:(NSData**)mode;
/**
 * Get Transaction Results
 * 
 Gets the transaction results when the reader is functioning in "Auto Poll" mode
 
 @param results The transaction results
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getTransactionResults:(NSData**)results;

/**
 * Get EMV Kernel Version
 * 
 Polls the device for the EMV Kernel Version
 
 @param response The kernel version response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersion:(NSString**)response;

/**
 * Remove Transaction Amount Log
 * 
 This command can delete transaction amount log in the reader. (When EMV transaction is offline approved, or online, transaction amout log saves to the reader)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_removeTransactionAmountLog;

/**
 * Continue Transaction for Cardholder Verification
 * 
 Use this command to send the results of Online PIN Request or Signature Request and to continue a Contact EMV transaction
 If the prior response Status Code is 0x31h (Request Online PIN) or 0x32h (Request Signature), this is the next command to send
 
 The tags will be returned in the callback routine
 
 @param result Cardholder Verification result:
 - 0: Success (PIN or Signature)
 - 1: Cancelled PIN request
 - 2: PIN pad not working
 - 3: Timeout
 - 4: Error
 @param pinblock Encrypted PIN block if Success PIN result
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_continueTransactionForCV:(int)result pinblock:(NSData*)pinblock;

/**
 * Get Extended EMV Kernel Version
 * 
 Polls the device for the extended EMV kernel version
 
 @param response The extended kernel verion response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersionExt:(NSString**)response;

/**
 * Clear Transaction Log
 * 
 Clears the transaction log on the VP8800
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_clearTransactionLog;

/**
 * Get CRL Status
 * 
 This command returns information about the EMV Certificate Revocation List. The version number, record size, and number of records contained in the file are returned.
 
 @param status 12 bytes returned
 - bytes 0-3: Version number
 - bytes 4-7: Number of records
 - bytes 8-11: Size of record
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_getCRLStatus:(NSData**)status;

/**
 * Get Transaction Log Status
 * 
 This command returns information about the EMV transaction log. The version number, record size, and number of records contained in the file are returned.
 
 @param status 12 bytes returned
 - bytes 0-3: Version number
 - bytes 4-7: Number of records
 - bytes 8-11: Size of record
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_getTransactionLogStatus:(NSData**)status;

/**
 * Ping Device
 * 
 Pings the reader. If it is connected, returns success, otherwise returns timeout
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_pingDevice;

/**
 * Capture Function Key
 *
 
 Captures a function key entry on the pinpad
 
 Results returned to pinpadData delegate

 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) pin_captureFunctionKey;

/**
 * Cancel PIN Command
 *
 
 This command can cancel IDT_Device:getEncryptedPIN:keyType:line1:line2:line3:() and IDT_Device::getNumeric:minLength:maxLength:messageID:language:() and IDT_Device::getAmount:maxLength:messageID:language:() and IDT_Device::getCardAccount:max:line1:line2:() and
 IDT_Device::pin_getFunctionKey() and IDT_Device::getEncryptedData:minLength:maxLength:messageID:language:()
 
  * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 */
-(RETURN_CODE) pin_cancelPin;

/**
 * Capture PIN
 *
 
 @param type PAN and Key Type
   - 00h = MKSK to encrypt PIN, Internal PAN (from MSR)
   - 01h = DUKPT to encrypt PIN, Internal PAN (from MSR)
   - 10h = MKSK to encrypt PIN, External Plaintext PAN
   - 11h = DUKPT to encrypt PIN, External Plaintext PAN
   - 20h = MKSK to encrypt PIN, External Ciphertext PAN
   - 21h = DUKPT to encrypt PIN, External Ciphertext PAN

 @param PAN Personal Account Number (if internal, value is 0)
 @param minPIN Minimum PIN Length
 @param maxPIN Maximum PIN Length
 @param message LCD Message
 
 Results returned to pinpadData delegate

 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) pin_capturePin:(int)type PAN:(NSString*)PAN minPIN:(int)minPIN maxPIN:(int)maxPIN message:(NSString*)message;

/**
 * Set BluetoothParameters
 
 Sets the name and password for the BLE module.
 
 Sending nil to all three parameters resets the default password to 123456
 *
 * @param name  Device name, 1-25 characters
 * @param oldPW  Old password, as a six character string, example "123456"
 * @param newPW  New password, as a six character string, example "654321"
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_setBluetoothParameters:(NSString*)name oldPW:(NSString*)oldPW newPW:(NSString*)newPW;

/**
 * FeliCa Authentication
 *
 Provides a key to be used in a follow up FeliCa Read with MAC (3 blocks max) or Write with MAC (1 block max).
 This command must be executed before each Read w/MAC or Write w/MAC command
 
 NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param key 16 byte key used for MAC generation of Read or Write with MAC
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_authentication:(NSData*)key;



/**
 * FeliCa Read with MAC Generation
 *
 Reads up to 3 blocks with MAC Generation.  FeliCa Authentication must be performed first
 
  NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.

 @param numBlocks Number of blocks
 @param blockList Block to read. Each block in blockList   Maximum 3 block requests
 @param blocks  Blocks read.  Each block 16 bytes.
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_readWithMac:(int)numBlocks blockList:(NSData*)blockList blocks:(NSData**)blocks;


/**
 * FeliCa Write with MAC Generation
 *
 Writes a block with MAC Generation.  FeliCa Authentication must be performed first
 
  NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param blockNumber Number of block
 @param data  Block to write.  Must be 16 bytes.
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_writeWithMac:(int)blockNumber data:(NSData*)data;


/**
 * FeliCa Read
 *
 Reads up to 4 blocks.
 
  NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param serviceCode Service Code List. Each service code in Service Code List = 2 bytes of data
 @param numBlocks Number of blocks
 @param blockList Blocks to read. Maximum 4 block requests
 @param blocks  Blocks read.  Each block 16 bytes.
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_read:(NSData*)serviceCode numBlocks:(int)numBlocks blockList:(NSData*)blockList blocks:(NSData**)blocks;


/**
 * FeliCa Write
 *
 Writes a block
 
  NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param serviceCode Service Code list.  Each service code must be be 2 bytes
 @param blockCount Block Count
 @param blockList Block list.
 @param data  Block to write.  Must be 16 bytes.
 @param statusFlag  Status flag response as explained in FeliCA Lite-S User's Manual, Section 4.5
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_write:(NSData*)serviceCode blockCount:(int)blockCount  blockList:(NSData*)blockList data:(NSData*)data statusFlag:(NSData**)statusFlag;


/**
 * FeliCa Poll Card
 *
 Perform functions a Felica Card Poll
 
  NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param systemCode System Code
 @param response  Response as explained in FeliCA Lite-S User's Manual
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_nfcCommand:(NSData*)systemCode response:(NSData**)response;


        /**
         * NFC Command
         *
            This command uses nfcCmdPkt[0] in command data field to implement different functions.
            This command should be used in Pass-Through mode and command with “Poll for a NFC Tag”
            data should be used first. Command with other data can only be used once the “Poll for
            a NFC Tag” command has indicated that a NFC tag is present.
 
         @param nfcCmdPkt System Code
            - Poll for NFC Tag:  nfcCmdPkt[0] = 0xff, nfcCmdPkt[1] = timeout value (in seconds)
            - Tag1 Static Get All Data:  nfcCmdPkt[0] = 0x11
            - Tag1 Static Read a Byte:  nfcCmdPkt[0] = 0x12, nfcCmdPkt[1] = Address of Data
            - Tag1 Static Write a Byte:  nfcCmdPkt[0] = 0x13 nfcCmdPkt[1] = Address of Data,  nfcCmdPkt[2] = Data to be written
            - Tag1 Static Write a Byte NE:  nfcCmdPkt[0] = 0x14, nfcCmdPkt[1] = Address of Data,  nfcCmdPkt[2] = Data to be written
            - Tag1 Dynamic Read a Segment:  nfcCmdPkt[0] = 0x15, nfcCmdPkt[1] = Address of Segment
            - Tag1 Dynamic Read 8 Bytes:  nfcCmdPkt[0] = 0x16, nfcCmdPkt[1] = Address of Data
            - Tag1 Dynamic Write 8 Bytes:  nfcCmdPkt[0] = 0x17, nfcCmdPkt[1] = Address of Data,  nfcCmdPkt[2]~nfcCmdPkt[9] = Data to be written
            - Tag1 Dynamic Write 8 Bytes NE:  nfcCmdPkt[0] = 0x18, nfcCmdPkt[1] = Address of Data,  nfcCmdPkt[2]~nfcCmdPkt[9] = Data to be written
            - Tag2 Read Data (16 bytes):  nfcCmdPkt[0] = 0x21, nfcCmdPkt[1] = Address of Data
            - Tag2 Write Data (4 bytes):  nfcCmdPkt[0] = 0x22, nfcCmdPkt[1] = Address of Data,  nfcCmdPkt[2]~nfcCmdPkt[5] = Data to be written
            - Tag2 Select Sect:  nfcCmdPkt[0] = 0x23, nfcCmdPkt[1] = Sect number
            - Tag3 Read Data:
                -- nfcCmdPkt[0] = 0x41,
                -- nfcCmdPkt[1] = Number of services, value n
                -- nfcCmdPkt[2]~nfcCmdPkt[2n+1]: Service code list
                -- nfcCmdPkt[2n+2]: Number of blocks, value m.
                -- nfcCmdPkt[2n+3....]: Block list, length is 2m~3m
            - Tag3 Write Data:
                -- nfcCmdPkt[0] = 0x41,
                -- nfcCmdPkt[1] = Number of services, value n
                -- nfcCmdPkt[2]~nfcCmdPkt[2n+1]: Service code list
                -- nfcCmdPkt[2n+2]: Number of blocks, value m.
                -- nfcCmdPkt[2n+3....]: Block list, length is 2m~3m
                -- nfcCmdPkt[...]: Block data, length is 16m
            - Tag4 Command:  nfcCmdPkt[0] = 0x81, nfcCmdPkt[1]~nfcCmdPkt[n]:data


         @param response  Response as explained in FeliCA Lite-S User's Manual
         @param ip IP Address of target device (optional)
        * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
        */
-(RETURN_CODE) felica_requestService:(NSData*)nodeCode response:(NSData**)response;


/**
 * Capture Amount Input
 *
 
 
 @param minPIN Minimum PIN Length
 @param maxPIN Maximum PIN Length
 @param message LCD Message
 @param signature Display message signed by Numeric Private Key using RSAPSS algorithm:
 1. Calculate 32 bytes Hash for “<Display Flag><Key Max Length>< Key Min Length><Plaintext Display Message>”
 2. Using RSAPSS algorithm calculate the Hash to be 256 bytes Raw Data
 3. Using Numeric Private Key to sign the Raw Data to be 256 bytes signature
 
 Results returned to pinpadData delegate
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) pin_captureAmountInput:(int)minPIN maxPIN:(int)maxPIN message:(NSString*)message signature:(NSData*)signature;

/**
 * Capture Numeric Input
 *
 
 
 @param mask True = mask input with "*", False = no masking of input
 @param minPIN Minimum PIN Length
 @param maxPIN Maximum PIN Length
 @param message LCD Message
 @param signature Display message signed by Numeric Private Key using RSAPSS algorithm:
 1. Calculate 32 bytes Hash for “<Display Flag><Key Max Length>< Key Min Length><Plaintext Display Message>”
 2. Using RSAPSS algorithm calculate the Hash to be 256 bytes Raw Data
 3. Using Numeric Private Key to sign the Raw Data to be 256 bytes signature
 
 Results returned to pinpadData delegate
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP8800::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) pin_captureNumericInput:(bool)mask minPIN:(int)minPIN maxPIN:(int)maxPIN message:(NSString*)message signature:(NSData*)signature;

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
