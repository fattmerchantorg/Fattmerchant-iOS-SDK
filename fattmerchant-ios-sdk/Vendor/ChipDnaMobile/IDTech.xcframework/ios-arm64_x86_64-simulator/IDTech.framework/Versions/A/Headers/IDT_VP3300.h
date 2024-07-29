//
//  IDT_VP3300.h
//  IDTech
//
//  Created by Randy Palermo on 10/29/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "IDT_Device.h"

/** Protocol methods established for IDT_VP3300 class  **/
@protocol IDT_VP3300_Delegate <NSObject>
@optional

-(void) bleStatus:(BLE_MSG)status; //!<Fires status messages during the BLE connection/verification process;
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

- (void) bypassData:(NSData*)data;//!<When bypass output is enabled, all data intended for the current device will be sent here .
//!< @param data The data intended for the device


/**
 Contactless Event
 During a Contactless transaction, if events are enabled, they will be sent to this protocol,
 
 @param event Event Type:
 - 01 = LED Event
 - 02 = Buzzer Event
 - 03 = LCD Message
 @param scheme LCD Message Scheme
 @param data Data
	- When Event Type 01:
	-- 0x00 = LED0 off
	-- 0x10 = LED1 off
 	-- 0x20 = LED2 off
 	-- 0x30 = LED3 off
 	-- 0xF0 = ALL off
	-- 0x01 = LED0 on
	-- 0x11 = LED1 on
 	-- 0x21 = LED2 on
 	-- 0x31 = LED3 on
 	-- 0xF1 = ALL on
 	- When Event Type 02:
 	-- 0x10 = Short Beep No Change
 	-- 0x11 = Short Beep No Change
 	-- 0x12 = Double Short Beep
 	-- 0x13 = Triple Short Beep
 	-- 0x20 = 200ms Beep
 	-- 0x21 = 400ms Beep
 	-- 0x22 = 600ms Beep
 	- When Event Type 03:
 	-- Message ID (please refer to table in NEO Reference Guide)
 */

- (void) ctlsEvent:(Byte)event scheme:(Byte)scheme  data:(Byte)data;


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
 Bluetooth Picker Alert
 When a bluetooth scan is requested, this delegate will return an UIAlertView for displaying to allow the selection of a found device
 
 @param view UIAlertView:
 
 */

- (void) bluetoothPickerAlert:(UIAlertView*)view;


/**
 Bluetooth Device Names
 When a bluetooth name scan is requested, this delegate will return a NSArray with the names of the found devices
 
 @param names NSArray of device names:
 
 */

- (void) bluetoothDeviceNames:(NSArray*)names;


- (void) gen2Data:(NSData*)tlv;//!<Receives Gen2 TLV data.
//!< @param tlv TLV data from gen2 event



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
 Class to drive the IDT_VP3300 device
 */
@interface IDT_VP3300 : NSObject<IDT_Device_Delegate>{
    id<IDT_VP3300_Delegate> delegate;
    id<IDT_VP3300_Delegate> bypassDelegate;

}

@property(strong) id<IDT_VP3300_Delegate> delegate;  //!<- Reference to IDT_VP3300_Delegate.
@property(strong) id<IDT_VP3300_Delegate> bypassDelegate;  //!<- Reference to IDT_VP3300_Delegate.

/**
 Disable Audio Detection.
 For BLE implementations.  Removes monitoring headphone jack for audio devices.
 */
+(void) disableAudioDetection;




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
 Establishes an singleton instance of IDT_VP3300 class.
 
 @return  Instance of IDT_VP3300
 */
+(IDT_VP3300*) sharedController;

/**
 *Close Device
 */

-(void) close;



/**
 * Disconnect from BLE -
 *
 * Will disconnect from existing BLE connection. You can now set another BLE Friendly Name to attach to another device.
 *
 */
-(void) device_disconnectBLE;


/**
 Cancels Transaction request (swipe or CTLS).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) ctls_cancelTransaction;



/**
 Cancels Transaction request (EMV).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) emv_cancelTransaction;


/**
 * Set Terminal Data

 *
 Sets the Terminal Data .
 
 * @param responseData  The data to set
 *
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setTerminalData:(NSData*)tags;

/**
 * Retrieve Terminal Data

 *
 Retrieves the Terminal Data .  The Terminal Data will be in the response parameter responseData
 
 * @param responseData  The response returned from the method as NSData
 *
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_retrieveTerminalData:(NSData**)responseData;

/**
 * Add Terminal Data

 *
 Adds the specified TLV to the current terminal data .
 
 * @param tlv  The data to set, overwriting any existing value
 *
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_addTLVToTerminalData:(NSData*)tlv;

/**
 Cancels Transaction request (EMV, swipe or CTLS).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_cancelTransaction;
/**
 * Get Transaction Results
 * 
 Gets the transaction results when the reader is functioning in "Auto Poll" mode
 
 @param results The transaction results
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getTransactionResults:(NSData**)results;

/**
 * Get Configuration Group
 *
 Retrieves the Configuration for the specified Group. Group 0 = terminal settings.
 
 @param group Configuration Group
 @param response Group TLV returned as a dictionary
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) ctls_getConfigurationGroup:(int)group response:(NSDictionary**)response;


/**
 Process Bypass Response.
 
 When output is being bypassed, and a command is received in the delegate redirectedOutput and sent to the device, the device rexponse must be sent to this
 method so the SDK can finishing processing the results;
 
 @param data The data received from the device that the SDK must process to complete the executing command
 */
-(void) processBypassResponse:(NSData*)data;

/**
 * Set Bypass Delegate
 *
 Sets the bypass delegate. .
 
 @param del  IDT_Device_Delegate delegate.
 
 
 */

-(void) assignBypassDelegate:(id<IDT_VP3300_Delegate>)del;




/**
 * Remove All Certificate Authority Public Key
 *
 Removes all the CAPK for CTLS
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()

 */
-(RETURN_CODE) ctls_removeAllCAPK;

/**
 * Remove Application Data by AID
 *
 Removes the Application Data for CTLS as specified by the AID name passed as a parameter
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010") Must be between 5 and 16 bytes
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_removeApplicationData:(NSString*)AID;

/**
 * Remove Certificate Authority Public Key
 *
 Removes the CAPK for CTLS as specified by the RID/Index
 
 @param capk 6 byte CAPK =  5 bytes RID + 1 byte INDEX
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeCAPK:(NSData*)capk;

/**
 * Remove Configuration Group
 *
 Removes the Configuration as specified by the Group.  Must not by group 0
 
 @param group Configuration Group
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeConfigurationGroup:(int)group;


/**
 * Retrieve AID list
 *
 Returns all the AID names installed on the terminal for CTLS.
 
 @param response  array of AID name as NSData*
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_retrieveAIDList:(NSArray**)response;

/**
 * Retrieve Application Data by AID
 *
 Retrieves the CTLS Application Data as specified by the AID name passed as a parameter.
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010"). Must be between 5 and 16 bytes
 @param response  The TLV elements of the requested AID
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveApplicationData:(NSString*)AID response:(NSDictionary**)response;

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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPK:(NSData*)capk key:(NSData**)key;


/**
 * Retrieve the Certificate Authority Public Key list
 *
 Returns all the CAPK RID and Index installed on the terminal for CTLS.
 
 @param keys NSArray of NSData CAPK name  (RID + Index)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPKList:(NSArray**)keys;

/**
 * Retrieve Terminal Data
 *
 Retrieves the Terminal Data for CTLS. This is configuration group 0 (Tag FFEE - > FFEE0100).  The terminal data
 can also be retrieved by ctls_getConfiguraitonGroup(0).
 
 @param tlv Response returned as a TLV
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE)  ctls_retrieveTerminalData:(NSData**)tlv;


/**
 * Send 2nd Gen Command
 *
 Informs SDK to format command for EMV Gen2 Device Communication
 
 @param tlv  TLV Command
 @param response  TLV Response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:())
 
 */
-(RETURN_CODE) device_sendGen2Cmd:(NSData*)tlv response:(NSData**)response;


/**
 * Set Application Data by AID
 *
 Sets the Application Data for CTLS as specified by TLV data
 
 @param tlv  Application data in TLV format
 The first tag of the TLV data must be the group number (FFE4).
 The second tag of the TLV data must be the AID (9F06)
 
 Example valid TLV, for Group #2, AID a0000000035010:
 "ffe401029f0607a0000000051010ffe10101ffe50110ffe30114ffe20106"
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_setCAPK:(NSData*)key;


/**
 * Set Configuration Group
 *
 Sets the Configuration Group for CTLS as specified by the TLV data
 
 @param tlv  Configuration Group Data in TLV format
 The first tag of the TLV data must be the group number (FFE4).
 A second tag must exist
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 *
 * NOTE ON APPLEPAY VAS:
 * To enable ApplePay VAS, first a merchant record must be defined in one of the six available index positions (1-6) using device_setMerchantRecord, then container tag FFEE06
 * must be sent as part of the additional tags parameter of ctls_startTransaction.  Tag FFEE06 must contain tag 9F26 and 9F22, and can optionanally contain tags 9F2B and DFO1.
 * Example FFEE06189F220201009F2604000000009F2B050100000000DF010101
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
 */
-(RETURN_CODE) ctls_startTransaction:(double)amount type:(int)type timeout:(int)timeout tags:(NSMutableDictionary *)tags;



/**
 * Enable Transaction Request
 
 *
 Enables CLTS and MSR, waiting for swipe or tap to occur. Returns IDTEMVData to deviceDelegate::emvTransactionData:()
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) ctls_startTransaction;



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
 - 0x0100 through 0xFFFF refer to IDT_UniPay::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;


/**
 * Begins searching for Bluetooth Low Energy devices in range
 - VP3300
 *
 * @param identifier This will only connect to a device with this calculated UUID identifier.  If nil is passed, it will connect to the first devices with the default friendly name (IDT_VP3300::device_getBLEFriendlyName / IDT_VP3300::device_setBLEFriendlyName)
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
 * Sets BLE search filter for VP3300 family
 *
 */
- (void)setBLEDeviceTypeVP3300;

/**
 * Set BLE to filter for specified service UUID
 *
 *
 * @param setServiceUUID  Sets the UUID filter to be used when discovering  BLE devices
 *
 */
- (void)setServiceUUID:(nullable NSArray<CBUUID *> *)serviceUUIDs;


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
 * Stops searching for Bluetooth Low Energy devices in range
 - VP3300
 *
 *
 * @return bool  If successful, polling was in progress and has stopped. If unsuccessful, BLE Device Search was not in progress.
 *
 * NOTE:  BLE only scans when there are no devices currently connected. After the SDK connects to any IDTech device, the scanning will pause automatically.
 */
-(bool) device_disableBLEDeviceSearch;

/**
 * Returns the UUID of the connected BLE device
 - VP3300
 *
 * @return NSUUID  UUID of the connected BLE device.  Returns nil if no BLE device connected.
 *
 */
-(NSUUID*) device_connectedBLEDevice;



/**
 * Is Audio Reader Connected
 
 *
 Returns value on device connection status when device is an audio-type connected to headphone plug.
 
 @return BOOL True = Connected, False = Disconnected
 
 */

-(BOOL) device_isAudioReaderConnected;


/**
 * Get Transaction Results
 Gets the transaction results when the reader is functioning in "Auto Poll" mode
 *
 * @param result The transaction results
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
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
-(NSString *) device_getResponseCodeString: (int) errorCode;

/**
 Is Device Connected
 
 Returns the connection status of the requested device
 
 @param device Check connectivity of device type
 
 @code
 typedef enum{
 IDT_DEVICE_VP3300_IOS = 8
 IDT_DEVICE_VP3300_OSX_USB = 9
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMagIII::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMagIII::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setAudioVolume:(float)val;

/**
 * Set Pass Through
 
 Sets Pass-Through mode on VP3300
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setPassThrough:(BOOL)enablePassThrough;


/**
 * Send Burst Mode
 *
 * Sets the burst mode forthe device.
 *
 * @param mode 0 = OFF, 1 = Always On, 2 = Auto Exit
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE)  device_setBurstMode:(int) mode;


/**
 * Send Poll Mode
 *
 * Sets the poll mode forthe device. Auto Poll keeps reader active, Poll On Demand only polls when requested by terminal
 *
 * @param mode 0 = Auto Poll, 1 = Poll On Demand
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setPollMode:(int) mode;


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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_startRKI;

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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
 */

-(RETURN_CODE) device_rebootDevice;

/**
 * Get Merchant Record
 * 
 Gets the merchant record for ApplePay VAS
 
 @param index Merchant record index, valid values 1-6
 @param record Data returned containing 99 bytes
 - Byte 0: Merchant index
 - Byte 1: Merchant enabled (1 = enabled, 0 = disabled)
 - Bytes 2 - 33: Merchant protocol hash-256 value
 - Byte 34: Length of Merchant URL
 - Bytes 35-99: URL
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getMerchantRecord:(int)index record:(NSData**)record;

/**
 * Set Merchant Record
 *
 Sets the merchant record for ApplePay VAS
 
 @param index Merchant Record Index, valid values: 1-6
 @param enabled Merchant Enabled / valid flag
 @param merchantID Merchant unique identifier registered with Apple. Example: ID TECH
 @param merchantURL Merchant URL, when applicable Example: com.idtechproducts.applePay
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setMerchantRecord:(int)index enabled:(BOOL)enabled merchantID:(NSString*)merchantID merchantURL:(NSString*)merchantURL;

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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) emv_getEMVL2Version:(NSString**)response;

/**
 * Remove Application Data by AID
 *
 This will REMOVE the an AID configuration file and all the tlv data associated with that AID.
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
 
 
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setTerminalData:(NSDictionary*)data;





/**
 * Start EMV Transaction Request
 *
 Authorizes the EMV transaction  for an ICC card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function will complete with a return of EMV_RESULT_CODE_START_TRANSACTION_SUCCESS to emvTransactionData delegate protocol, and then IDT_VP3300::emv_authenticateTransaction() must be executed.  If auto authorize is ENABLED (default), IDT_VP3300::emv_authenticateTransaction() will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS.  The auto authorize can be enabled/disabled with IDT_VP3300::emv_disableAutoAuthenticateTransaction:()
 
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
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_startTransaction:(double)amount amtOther:(double)amtOther type:(int)type timeout:(int)timeout tags:(NSData*)tags forceOnline:(BOOL)forceOnline fallback:(BOOL)fallback;

/**
 * Remove All Application Data
 * 
 Removes all the application data for the EMV kernel
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE)emv_removeAllApplicationData;

/**
 * Get Kernel Check Value
 *
 Polls the device for the Kernel Check Value
 
 @param response Response returned of the Check Value of the Kernel
 * @return RETURN_CODE: Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelCheckValue:(NSString**)response;

/**
 * Get EMV Configuration Check Value
 * 
 Polls device for the EMV Configuration Check Value
 
 @param response Response returned of the Check Value of the Configuration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVConfigurationCheckValue:(NSString**)response;

/**
 * Callback Response MSR Entry
 * 
 Provides MSR information to kernel after a callback was received with type EMV_CALLBACK_MSR
 
 @param MSR Swiped track data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_callbackResponseMSR:(NSData*)MSR;

/**
 * Get All Configuration Groups
 *
 Retrieves all Configuration Groups.
 
 @param response Groups returned in a dictionary
 * 			Key = <String>Group
 * 			Value = <Dictionary<tag,value>>
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 
 */
-(RETURN_CODE) ctls_getAllConfigGroups:(NSData**)response;


/**
 * Get All Configuration Groups
 *
 Retrieves all Configuration Groups.
 
 @param response @param response Groups returned as Data stream
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 
 */
-(RETURN_CODE) ctls_getAllConfigurationGroups:(NSDictionary<NSString*, NSDictionary*>**)response;

/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) icc_exchangeAPDU:(NSData*)dataAPDU response:(APDUResponse**)response;
/**
 * Get Reader Status
 *
 Returns the reader status
 
 @param readerStatus Pointer that will return with the ICCReaderStatus results.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) icc_setKeyFormatForICCDUKPT:(NSData*)encryption;

/**
 * Disable MSR Swipe
 
 
 
 Cancels MSR swipe request.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;

/**
 * Enable MSR Swipe
 
 *
 Enables CLTS and MSR, waiting for swipe or tap to occur. Returns IDTEMVData to deviceDelegate::emvTransactionData:() 
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe;

/**
 *Check if device is connected
 */
-(bool) isConnected;



/**
 * Start A Transaction Request
 *
 Authorizes the EMV, CTLS, or MSR transaction
 
 
 For a contact ICC EMV transaction, by default auto authorize is ENABLED.  If auto authorize is DISABLED, this function will complete with a return of EMV_RESULT_CODE_START_TRANSACTION_SUCCESS to emvTransactionData delegate protocol, and then IDT_UniPayII::emv_authenticateTransaction() must be executed.  If auto authorize is ENABLED (default), IDT_UniPayII::emv_authenticateTransaction() will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS.  The auto authorize can be enabled/disabled with IDT_VP3300::emv_disableAutoAuthenticateTransaction:()
 
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
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_startTransaction:(double)amount amtOther:(double)amtOther type:(int)type timeout:(int)timeout tags:(NSData*)tags forceOnline:(BOOL)forceOnline  fallback:(BOOL)fallback;

/**
 * Get Poll Mode
 *
 * Gets the current poll mode of the device
 *
 * @param mode Response from the device of the current poll mode
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getPollMode:(NSData**)mode;

/**
 * Set QuickChip HID Mode
 * 
 Puts the device into QuickChip KB Output Mode while in HID
 
 @param mode
 - 0 = Disable
 - 2 = CT + MSR + CL
 - 3 = CT + MSR
 - 4 = CL + MSR
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setQuickChipHIDMode:(Byte)mode;

/**
 * Get Product Type
 * 
 Returns a "product type" value in a proprietary TLV
 
 @param type Product type
 * ----------------------------------------
 * Product Type      |    Description
 *-----------------------------------------
 *    42 37 00      |    ViVOpay 5000
 *    43 33 00      |    ViVOpay 4500
 *    43 35 00      |    ViVOpay Vend
 *    43 36 00      |    Vendi (NEO, string ident = "")
 *    43 37 00      |    ViVOpay Kiosk1 (ATM1, string ident = "")
 *    43 38 00      |    Kiosk2
 *    43 39 00      |    Kiosk3 (NEO, string ident = "")
 *    55 31 00      |    UniPay 1.5 (NEO, string ident = "")
 *    55 33 00      |    UniPay III (NEO) 
 *    55 33 31      |    VP3300, VP3300 OEM (NEO) (iBase/Cake same code, string ident = "")
 *    55 33 32      |    VP3300E(NEO, string ident = "")
 *    55 33 33      |    VP3300C(NEO, string ident = "")
 *    55 33 34      |    BTPay Mini (NEO) (UniPayIII + BLE, string ident = "")
 *    56 31 00      |    VP3600
 *    56 32 00      |    VP5200
 *    56 33 00      |    VP5300
 *    56 34 00      |    VP6300
 *    56 35 00      |    VP6800
 *    56 36 00      |    VP8300
 *    56 37 00      |    VP8310
 *    56 38 00      |    VP8800
 *    56 39 00      |    VP8810
 *    56 40 00      |    VP9000
 *    44 30 00      |    QX120
 *    44 31 00      |    Mx8Series
 *    44 32 00      |    NETs
 *    44 33 00      |    Magtek
 *    44 35 00      |    ICP
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getProductType:(NSData**)type;

/**
 * Get Processor Type
 * 
 Returns a processor type TLV
 
 @param type Processor type
 *------------------------------------------
 * Processor Type     |     Description
 *------------------------------------------
 *     45 00             |     ARM7/ LPC21xx
 *     4D 00             |     ARM Cortex-M4/ K21 Family
 *     4E 00             |     ARM Cortex-M4/ K81 Family
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getProcessorType:(NSData**)type;

/**
 * Get Hardware Info
 * 
 Returns an ASCII string for the hardware information
 
 @param response The ASCII character string
 *-----------------------------------------------------------
 *            ASCII                          |    Description
 *-----------------------------------------------------------
 * HW,VPVendi<CR><LF>K21F Rev9                          |    Vendi
 * HW,VP3300 Audio Jack<CR><LF>K21F Rev9         |    Unipay III
 * HW,VPUnipay1.5<CR><LF>K21F Rev9                   |    Unipay 1.5
 * HW,VPUniPay1.5TTK<CR><LF>K21F Rev9            |    UniPay 1.5 TTK 
 * HW,VP3300 USB<CR><LF>K21F Rev9                   |    VP3300 USB, VP3300 USB OEM (iBase/Cake same code, string ident = "")
 * HW,VP3300 USB-E<CR><LF>K21F Rev9               |    VP3300 USB-E
 * HW,VP3300 USB-C<CR><LF>K21F Rev9               |    VP3300 USB-C 
 * HW,VPVP3300 Bluetooth<CR><LF>K21F Rev9      |    VP3300 Bluetooth 
 * HW,.VP6300<CR><LF>K81F.Rev4                          |    VP6300 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getHardwareInfo:(NSString**)response;

/**
 * Get UID of MCU
 * 
 Returns the UID of the device
 
 @param response The module UID information
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getUIDofMCU:(NSString**)response;

/**
 * Ping Device
 * 
 Pings the reader. If it is connected, returns success, otherwise returns timeout
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getDateTime:(NSData**)dateTime;

/**
 * Set Date Time
 * 
 Sets the date and time of the device to the current date and time of the iOS device
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setDateTime;

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
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_controlLED:(Byte)indexLED control:(Byte)control;

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
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_controlUserInterface:(NSData*)values;

/**
 * Get Cash Transaction Reader Risk Parameters
 *
 Returns the TTQ and reader risk parameters that will be used for cash transactions, if enabled
 
 @param tlv TLV data objects
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getCashTranRiskPara:(NSData**)tlv;

/**
 * Get DRL Reader Risk Parameters
 *
 Gets the index, application program ID, and reader risk parameters for the DRL settings
 
 @param index DRL index (01-04)
 @param tlv TLV data objects
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getDrlReaderRiskPara:(Byte)index tlv:(NSData**)tlv;

/**
 * Get Module Version Information
 *
 Gets the 16 byte UID of the MCU
 
 @param uid The string representation of the UID
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getModuleVer:(NSString**)moduleVer;

/**
 * Get EMV Kernel Version
 * 
 Polls the device for the EMV Kernel Version
 
 @param response The kernel version response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersion:(NSString**)response;

/**
 * Get Extended EMV Kernel Version
 * 
 Polls the device for the extended EMV kernel version
 
 @param response The extended kernel verion response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersionExt:(NSString**)response;

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
* Begins searching for Bluetooth Low Energy devices in range and return UIAlertView with choices
- VP3300, NEO2
*
* This will initiate a bluetooth searech, and return an UIAlertView to the delegate bluetoothPickerAlert
 
* @param scanTime Amount of time to search for devices before returning alert view.
* @param serviceUUIDs  A null terminated array of CBUUID to filter for specific device type.  Sending nil will report all ble devices in range
 @param options scan options
*
*
*
* Note: Please refer to CBCentralManager scanForPeripheralsWithServices for information about serviceUUIDs and options parameters
*/

- (void)scanForBLEDevices:(NSTimeInterval)scanTime serviceUUIDs:(nullable NSArray<CBUUID *> *)serviceUUIDs options:(nullable NSDictionary<NSString *, id> *)options;

/**
* Begins searching for Bluetooth Low Energy devices in range and return NSArray with found device names
- VP3300, NEO2
*
* This will initiate a bluetooth searech, and return an UIAlertView to the delegate bluetoothPickerAlert
 
* @param scanTime Amount of time to search for devices before returning alert view.
* @param serviceUUIDs  A null terminated array of CBUUID to filter for specific device type.  Sending nil will report all ble devices in range
 @param options scan options
*
*
*
* Note: Please refer to CBCentralManager scanForPeripheralsWithServices for information about serviceUUIDs and options parameters
*/

- (void)scanForBLEDeviceNames:(NSTimeInterval)scanTime serviceUUIDs:(nullable NSArray<CBUUID *> *)serviceUUIDs options:(nullable NSDictionary<NSString *, id> *)options;


/**
 * Activate Transaction
 *
 - VP3300
 Initiates a CTLS transaction
 
 Use this command when the ctls reader is in “Poll on Demand” mode to begin an EMV or contactless MagStripe Card transaction. When the reader is in “Poll on Demand” mode, the RF is turned on only after receiving an Activate Transaction command. When a valid Activate Transaction command is sent to the ctls reader, it starts polling for cards.
 
 If the ctls reader does not find a supported card (an AID that matches one of the configured AIDs in the reader) for the specified time duration, it times out and ends the transaction. If the ctls reader finds a card within the specified time interval, it attempts to carry out the transaction. The transaction flow between the reader and the card depends on the type of card detected.
 
 If the transaction is successful, the reader returns the data in CTLSResponse. If the transaction is not successful, yet it proceeded into the transaction state machine, the reader returns a Failed Transaction Record in the response data. The presence and format of the Clearing Record, Track Data and Failed Transaction record depends on the type of card that was detected.
 
 Note: While an Activate command is in progress, only a Cancel may be sent. Do not send other commands until Activate Transaction has completed, because the reader will interpret these as a Cancel Transaction command.
 
 

 @param tags Activate TLV tags
 @param timeout Timeout value in seconds.

 
 Activate TVL Tag | Description | Format | Length
  ------ | ------ | ------ | ------
 9A | Transaction Date | n6 (YYMMDD) | 3
 9C | Transction Type | n2 | 2
 5F2A | Transaction Currency Code | n2 | 2
 5F36 | Transaction Currency Exponent | n1 | 1
 9F02 | Amount, Authorized | n12 | 6
 9F03 | Amount Other | n12 | 6
 9F1A | Terminal Country Code |  n3| 2
 9F21 | Transaction Time | n6 (HHMMSS} | 3
 9F5A | Terminal Transaction Type | b | 1
 
 Transaction Types: 0x00 = Purchase Goods/Services, 0x20 = Refund
 Terminal Transaction Type (Interac)  0x00 = Purchase, 0x01 = Refund
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0xFF00: Accept the online transaction RETURN_CODE_EMV_APPROVED
 - 0xFF01: Decline the online transaction RETURN_CODE_EMV_DECLINED
 - 0xFF02: Request to go online RETURN_CODE_EMV_GO_ONLINE
 - 0xFF03: Transaction is terminated RETURN_CODE_EMV_FAILED
 - 0xFF05: ICC format error or ICC missing data error RETURN_CODE_EMV_SYSTEM_ERROR
 - 0xFF07: ICC didn't accept transaction RETURN_CODE_EMV_NOT_ACCEPTED
 - 0xFF0A: Application may fallback to magstripe technology RETURN_CODE_EMV_FALLBACK
 - 0xFF0C: Transaction was cancelled RETURN_CODE_EMV_CANCEL
 - 0xFF0D: Timeout RETURN_CODE_EMV_TIMEOUT
 - 0xFF0F: Other EMV Error RETURN_CODE_EMV_OTHER_ERROR
 - 0xFF10: Accept the offline transaction RETURN_CODE_EMV_OFFLINE_APPROVED
 - 0xFF11: Decline the offline transaction RETURN_CODE_EMV_OFFLINE_DECLINED
 
 
 
 \par Converting TLV to NSMutableDictionary
 
 EMV data is  received in TLV (Tag, Length, value) format:
 `950500000080009B02E8009F2701018A025A339F26080C552B9364D55CE5`
 
 This data contains the following EMV tags/values:
 
 Tag | Length | Value
 ----- | ----- | -----
 9502 | 06 | 000000001995
 9A | 03 | 140530
 9C | 01 | 00
 
 An example how to create an NSMutableDictionary with these values follows.
 
 @code
 -(NSMutableDictionary*) createTLVDict{
 
 NSMutableDictionary *emvTags = [[NSMutableDictionary alloc] initWithCapacity:0];
 
 [emvTags setObject:@"000000001995" forKey:@"9502"];
 [emvTags setObject:@"140530" forKey:@"9A"];
 [emvTags setObject:@"00" forKey:@"9C"];
 
 return emvTags;
 
 }
 @endcode
 
 */

-(RETURN_CODE) activateTransaction:(NSMutableDictionary<NSString*,NSString*> *_Nullable)tags timeout:(int)timeout;



/**
 * Sync Clock
 Syncs the VP3300 clock with the iOS device clock
 *
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
 *
 */

-(RETURN_CODE)  device_syncClock;


/**
 * Sync Time
 Syncs the VP3300 time with the iOS device clock
 *
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_VP3300::device_getResponseCodeString:().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
 *
 */

-(RETURN_CODE)  device_syncTime;

/**
* Request Remote Key Injection Gen4
    
*
Attempts to perform a Remote Key Injection with IDTech's RKI servers.
*
*@param isTest True = Test/Demo Key, False = Production/Live Key
*@param keyName Request specific key to load.  Passing nil will request default key associated with device serial number
 *@param isForeGround TRUE = run sync on main UI thread, FALSE = run async on background thread (no UI blocking)

    
* @return RETURN_CODE:
- 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
- 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
- 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
- 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
- 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
- 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
- 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
- 0x0100 through 0xFFFF refer to IDT_VP3300::device_getResponseCodeString:()
    
*/
-(RETURN_CODE) device_remoteKeyInjection:(bool)isTest keyName:(NSString*)keyName isForeground:(bool)isForeGround;


/**
* BLE Connect Attempt
    
*
Attempts to connect to a BLE device uisng a BLE_MSG as parameters.
*
*@param BLE_MSG Data parameters for the connection

*/

-(bool) bleConnect:(IDTech_BLE*)data;

@end
