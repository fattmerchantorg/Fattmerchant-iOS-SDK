//
//  IDT_NEO2.h
//  IDTech
//
//  Created by Randy Palermo on 10/29/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "IDT_Device.h"

/** Protocol methods established for IDT_NEO2 class  **/
@protocol IDT_NEO2_Delegate <NSObject>
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

-(void) updateStatus:(PK_STATUS_Type)type currentBlock:(int)currentBlock totalBlocks:(int)totalBlocks error:(RETURN_CODE)error;//!<Reports PK Update status.
//!< @param type The stage of the PK update
//!< @param currentBlock The number of the block that has transferred
//!< @param totalBlocks The total number of blocks to transfer
//!< @param error The error condition when failure is encoutered

/**
 * Sends a PK Update
 *
 Starts a device firmware upatate using the provided path to the .pk file.
 * Update proceeds on a background thread.
 * Use protocol UpdateStatus to monitor progress.
 * Keep device attached and do interrupt process until PK_STATUS_FAILED or PK_STATUS_COMPLETED has been received
 *
 @param pkFile The .pk file
 
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
-(RETURN_CODE) sendPKUpdate:(NSData*)pkFile;

/**
 * Sends a PK Update (Faster)
 *
 Starts a device firmware upatate using the provided path to the .pk file.
 * Update proceeds on a background thread.
 * Use protocol UpdateStatus to monitor progress.
 * Keep device attached and do interrupt process until PK_STATUS_FAILED or PK_STATUS_COMPLETED has been received
 * 
 * Data will be sent in packet size of 244 bytes. Sleep timers are modified or removed where possible to achieve faster times
 * During transfer, no response will be sent by the device. 
 *
 @param pkData The .pk file
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) sendPKUpdateBLE:(NSData*)pkData;

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

@end

/**
 Class to drive the IDT_NEO2 device
 */
@interface IDT_NEO2 : NSObject<IDT_Device_Delegate>{
    id<IDT_NEO2_Delegate> delegate;
}

@property(strong) id<IDT_NEO2_Delegate> delegate;  //!<- Reference to IDT_NEO2_Delegate.



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
 Establishes an singleton instance of IDT_NEO2 class.
 
 @return  Instance of IDT_NEO2
 */
+(IDT_NEO2*) sharedController;

/**
 *Close Device
 */

-(void) close;



/**
 Cancels Transaction request (all interfaces, CTLS/MSR/INSERT).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */

-(RETURN_CODE) ctls_cancelTransaction;



/**
 Cancels Transaction request (all interfaces, CTLS/MSR/INSERT).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */

-(RETURN_CODE) emv_cancelTransaction;



/**
 Cancels Transaction request (all interfaces, CTLS/MSR/INSERT).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_cancelTransaction;

/**
 * Log Clear
 *
 
 Instructs the device to delete all log data
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_logClear;

/**
 * Log Read
 *
 Instructs the device to output all log data
 
 @param response Full response received from NEO2 device, including log
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_logRead:(NSData**)response;

/**
 * Enable Log
 * 
 Instructs the device to enable/disable the log
 
 @param enable True = enable log, False = disable log
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_logEnable:(BOOL)enable;

/**
 * Get RT1050 Firmware Version
 * 
 Gets the version for the RT1050 Firmware
 
 @param response Response returned of the RT1050 firmware
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getRT1050FirmwareVersion:(NSString**)response;

/**
 * Get Bootloader Version
 *
 Polls the device for the bootloader version
 *
 * @param response Response return of the bootloader version from the device
 * @return RETURN_CODE: Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getBootloaderVersion:(NSString**)response;

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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
 */

-(RETURN_CODE) device_rebootDevice;

/**
 * Buzzer OnOff
 *
 
 Causes the buzzer to beep once
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_buzzerOnOff;

/**
 * Get Configuration Group
 *
 Retrieves the Configuration for the specified Group. Group 0 = terminal settings.
 
 @param group Configuration Group
 @param response Group TLV returned as a dictionary
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getMerchantRecord:(int)index record:(NSData**)record;
/**
 * Get Transaction Results
 * 
 Gets the transaction results when the reader is functioning in "Auto Poll" mode
 
 @param results The transaction results
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getTransactionResults:(NSData**)results;

/**
 * Get Device Tree
 *
 Polls the NEO2/3 for the Device Tree
 
 @param deviceTree The device's tree returned as a NSString
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_getDeviceTreeVersion:(NSString**)deviceTree is1050:(BOOL)is1050;

/**
 * RT1050 SRK Fuse Status
 * 
 This command retrieves the status fo the reader's RT1050 SRK fuse
 
 @param status First 8 bytes of the SRK Fuse, if programmed. Otherwise, an empty array is returned
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */ 
-(RETURN_CODE) device_get1050FuseStatus:(NSData**)status;

/**
 * Get 1050 Bootloader Version
 * 
 Polls the device for the 1050 bootloader version
 
 @param version Response returned of the 1050 bootloader version
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_get1050BootloaderVersion:(NSString**)version;

/**
 * Get 1050 Device Tree
 *
 Polls the NEO2/3 for the 1050 Device Tree
 
 @param deviceTree The device tree returned as a NSString
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:() 
 */
-(RETURN_CODE) device_get1050DeviceTreeVersion:(NSString**)deviceTree;

/**
 * Reset non-volatile memory
 
 The Set Configuration Defaults and Keep Encryption Key command provides an external method for resetting parameters
 in non-volatile memory (NVM) to their default values. When the reader receives this command, it
 erases EEPROM (but retains encryption keys). After completing initialization, the reader reboots.
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_resetNVM;

/**
 * Check DUKPT Key
 * 
 This command checks whether a valid DUKPT key is stored at the specified slot and if a valid key
 is found then some basic information related to the type of key is returned. The actual key data
 is never returned.
 This command can be used to check whether a key is already present before injecting a key in
 a KeyIndex to prevent overwriting an existing DUKPT key.
 
 @param keyIndex Data Encryption key (usually 5)
 @param val
 * -Byte 0 = Key State: 00h = Unused, 01h = Valid, 02h = End of Life, FFh = Not Available
 * -Byte 1-2 = Key Usage (ASCII). "D0" = Used to encrypt data
 * -Byte 3 = Algorithm (ASCII). "T" = Triple DES
 * -Byte 4 = Mode of Use (ASCII). "E" = Encryption Only
 * -Byte 5-6 = Key Version (ASCII). "00" = Key version not used
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_checkDUKPTKey:(Byte)keyIndex value:(NSData**)val;

/**
 * Get Data Encryption Key Variant Type
 *
 Returns the data encryption key type
 
 @param type
 * -0 = Data Variant
 * -1 = Pin Variant
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_getDEKVariantType:(Byte*)type;

/**
 * Set Data Encryption Key Variant Type
 * 
 This command exists to specify the key variant type of Data encryption key (Slot = 0), and MUST be used
 before the initial loading of the Data encryption key into the device. The key variant type CANNOT
 be changed once the Data encryption key is present. It must remain either Data variant or PIN variant.
 
 @param type
 * -0 = Data Variant
 * -1 = PIN Variant
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_setDEKVariantType:(Byte)type;

/**
 * Get DUKPT Key Serial Number (KSN)
 * 
 Host can use this command to retrieve the KSN of the DUKPT key.
 
 @param KSN Key Serial Number
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_getDUKPT_KSN:(NSData**)KSN;

/**
 * Get Salt KCV
 * 
 Host can use this command to retrieve the Salt KCV
 
 @param KCV KCV
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_getSalt_KCV:(NSData**)KCV;

/**
 * Set RKL Keys
 * 
 Sets the RKL Keys
 
 @param keyNumber Number of keys remaining to load
 @param tr31 ASN.1 structure of encrypted key(s)
 @param nonce Nonce generated by RKMS to generate an HMAC used for auth
 @param hmac HMAC-SHA256 generated from RKMS
 @param kv ASN.1 Key Verification Structure returned from device
 @param nonce_device Nonce generated by the device to generate a MAC used for auth
 @param hmac_device HMAC-SHA256 generated from device terminal
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_setRKLKeys:(short) keyNumber tr31:(NSData*)tr31 nonce:(NSData*)nonce hmac:(NSData*)hmac kv:(NSData**)kv nonceDevice:(NSData**)nonce_device hmacDevice:(NSData**)hmac_device;

/**
 * Set KeySlot for DUKPT PEK and DUKPT DEK
 * 
 Selects one of the available PEKs for use in subsequent PIN encryption operations.
 Selects one of the available DEKs for use in subsequent Data encryption operations.
 
 @param isPEK TRUE = set PEK, FALSE = set DEK
 @param keySlot The KeySlot number specifies the key this command will select
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_setKeyslot_PEK_DEK:(BOOL)isPEK keyslot:(Byte)keySlot;
/**
 * Get KeySlot for DUKPT PEK and DUKPT DEK
 * 
 Returns the KeySlot for PEK and DEK (if they exist)
 
 @param keyslotPEK PEK KeySlot (0-9). Value of 255 (0xff) = does not exist
 @param keyslotDEK DEK KeySlot (0-9). Value of 255 (0xff) = does not exist
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_getKeyslot_PEK_DEK:(NSData**)keyslotPEK keyslotDEK:(NSData**)keyslotDEK;

/**
 * Get DUKPT DEK Attribution based on KeySlot
 * 
 Command settings can only be changed for each key one time
 
 @param keyslot Key Slot
 @param mode 0 = TDES, 1 = AES
 @param outputMode_workingKey 0 = TDES / TDES, 1 = AES-128 / TDES3 2 = TransArmor / AES-128, Output Mode is when TDES, Working Key is when AES
 @param variant_keyUsage 0 = Data Key / Encrypt/Decrypt, 1 = PIN Key / Encrypt, 2 = N/A / Decrypt, Variant is when TDES, Key Usage is when AES
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_getDUKPT_DEK_Attribution:(Byte)keyslot mode:(Byte*)mode outputModeWorkingKey:(Byte*)outputMode_workingKey variantKeyUsage:(Byte*)variant_keyUsage;

/**
 * Set DUKPT DEK Atribution based on KeySlot AES
 * 
 Command settings can only be changed for each key one time
 
 @param keyslot Key Slot
 @param workingKey 0 = TDES, 1 = TDES3, 2 = AES-128
 @param keyUsage 0 = Encrypt/Decrypt, 1 = Encrypt, 2 = Decrypt
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_setDUKPT_DEK_Attribution_AES:(Byte)keyslot working_Key:(Byte)workingKey key_Usage:(Byte)keyUsage;

/**
 * Set DUKPT DEK Attribution based on KeySlot TDES
 * 
 Command settings can only be changed for each key one time. However, switching the Output mode (TDES and TransArmor TDES) is always valid.
 
 @param keyslot Key Slot
 @param outputMode 0 = TDES, 1 = AES-128, 2 = TransArmor TDES
 @param variant 0 = Data Variant, 1 = PIN Variant
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) config_setDUKPT_DEK_Attribution_TDES:(Byte)keyslot output_Mode:(Byte)outputMode variant:(Byte)variant;

/**
 * Remove All Certificate Authority Public Key
 *
 Removes all the CAPK for CTLS
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()

 */
-(RETURN_CODE) ctls_removeAllCAPK;

/**
 * Remove Application Data by AID
 *
 Removes the Application Data for CTLS as specified by the AID name passed as a parameter
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010") Must be between 5 and 16 bytes
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_removeApplicationData:(NSString*)AID;

/**
 * Remove Certificate Authority Public Key
 *
 Removes the CAPK for CTLS as specified by the RID/Index
 
 @param capk 6 byte CAPK =  5 bytes RID + 1 byte INDEX
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeCAPK:(NSData*)capk;

/**
 * Update Balance
 *
 This command is the authorization respose sent by the issuer to the terminal including the Authorization Status (OK or NOT OK).
 
 This command is also being ussed in some implementations (i.e. EMEA) to communicate the results of Issuer Authorization
        to the reader in order to display the correct LCD messages. With this command, the POS passes the authorization
        result (OK/NOT OK), and possibly the Authorization Code (authCode)/Date/Time to the terminal.
 
 @param statusCode 00:ok, 01:not ok, 02: (ARC response 89 for Interac)
 @param authCode Authorization code from host. Six bytes. Optional
 @param date Transaction date. If null, uses current terminal date. 3 bytes compressed numeric YYMMDD (tag value 9A)
 @param time Transaction time. If null, uses current terminal time. 3 bytes compressed numeric HHMMSS (tag value 9F21)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) ctls_updateBalance:(NSData*)statusCode authCode:(NSData*)authCode date:(NSData*)date time:(NSData*)time;

/**
 * Get All Configuration Groups
 *
 Retrieves all Configuration Groups.
 
 @param response@param response Groups returned as Data stream
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 
 */
-(RETURN_CODE) ctls_getAllConfigGroups:(NSData**)response;



/**
 * Get All Configuration Groups
 *
 Retrieves all Configuration Groups.
 
 @param response Groups returned in a dictionary
 * 			Key = <String>Group
 * 			Value = <Dictionary<tag,value>>
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 
 */
-(RETURN_CODE) ctls_getAllConfigurationGroups:(NSDictionary<NSString*, NSDictionary*>**)response;



/**
 * Remove Configuration Group
 *
 Removes the Configuration as specified by the Group.  Must not by group 0
 
 @param group Configuration Group
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeConfigurationGroup:(int)group;


/**
 * Retrieve AID list
 *
 Returns all the AID names installed on the terminal for CTLS.
 
 @param response  array of AID name as NSData*
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_retrieveAIDList:(NSArray**)response;

/**
 * Retrieve Application Data by AID
 *
 Retrieves the CTLS Application Data as specified by the AID name passed as a parameter.
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010"). Must be between 5 and 16 bytes
 @param response  The TLV elements of the requested AID
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPK:(NSData*)capk key:(NSData**)key;


/**
 * Retrieve the Certificate Authority Public Key list
 *
 Returns all the CAPK RID and Index installed on the terminal for CTLS.
 
 @param keys NSArray of NSData CAPK name  (RID + Index)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPKList:(NSArray**)keys;

/**
 * Retrieve Terminal Data
 *
 Retrieves the Terminal Data for CTLS. This is configuration group 0 (Tag DFEE2D - > DFEE2D0100).  The terminal data
 can also be retrieved by ctls_getConfigurationGroup(0).
 
 @param tlv Response returned as a TLV
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_setApplicationData:(NSData*)tlv;

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
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCAPK:(CAKey)key;

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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_setCAPK:(NSData*)key;


/**
 * Set Configuration Group
 *
 Sets the Configuration Group for CTLS as specified by the TLV data
 
 @param tlv  Configuration Group Data in TLV format
 The first tag of the TLV data must be the group number (DFEE2D).
 A second tag must exist
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 *
 * NOTE ON INTERFACE SELECTION: For the NEO2, tag DFEF37 is used to determine which interfaces to use for the transaction:
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
 * Reset Configuration Group
 * 
 This command allows resetting a dataset to its default configuration
 If the file exists, it will be overwritten. If not, it will be created
 
 @param group Configuration group
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE) ctls_resetConfigurationGroup:(int)group;

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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 *
 * NOTE ON INTERFACE SELECTION: For the NEO2, tag DFEF37 is used to determine which interfaces to use for the transaction:
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
//-(RETURN_CODE) ctls_startTransaction:(double)amount type:(int)type timeout:(int)timeout transTags:(NSData *)transTags VAS:(NSData *)VAS;

/**
 * Get Key format for ICC DUKPT
 
 Specifies how data is being encrypted with Data Key or PIN key
 
 *
 * @param format  Response return from method:
 -'TDES' : Encrypted card data with TDES if DUKPT Key had been loaded
 -'AES' : Encrypted card data with AES if DUKPT Key had been loaded
 -NONE' : No Encryption
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) icc_setKeyFormatForICCDUKPT:(NSData*)encryption;

/**
 * Update NEO 2 Firmware
 *
 Reserved for system use
 */
-(RETURN_CODE) updateFirmwareNeo2:(FIRMWARE_TYPE)type data:(NSData*)firmwareData;

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
 * Battery Voltage
 *
 Polls the device for the current battery voltage
 
 @param response Returns battery voltage string representing millivolts
 * @return RETURN_CODE: Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getBatteryVoltage:(NSString**)response;

/**
 * Battery Percentage
 * 
 Polls the device for the current battery percentage
 
 @param response Returns the battery percentage represented as a whole number 5 - 100%
 * @return RETURN_CODE: Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_getBatteryPercentage:(NSString**)response;

/**
 * Begins searching for Bluetooth Low Energy devices in range
 - NEO2
 *
 * This method will UNPAIR and DISCONNECT from any current PAIRED and CONNECTED devices before starting a new BLE Device Search.  This function is for PAIRING a new Device and
 * connecting to it.  Once a device is PAIRED/CONNECTED, the search will terminate.  If a paired devices becomes disconnected (out of range, power cycle), the BLE search will automatically
 * start again to reconnect to the devices once available
 *
 * @param identifier This will only connect to a device with this calculated UUID identifier.  If nil is passed, it will connect to the first devices with the default friendly name (IDT_NEO2::device_getBLEFriendlyName / IDT_NEO2::device_setBLEFriendlyName)
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
 * Stops searching for Bluetooth Low Energy devices in range
 - NEO2
 *
 *
 * @return bool  If successful, polling was in progress and has stopped. If unsuccessful, BLE Device Search was not in progress.
 *
 * NOTE:  BLE only scans when there are no devices currently connected. After the SDK connects to any IDTech device, the scanning will pause automatically.
 */
-(bool) device_disableBLEDeviceSearch;

/**
 * Returns the UUID of the connected BLE device
 - NEO2
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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
 *
 */
-(RETURN_CODE)  device_getAutoPollTransactionResults:(IDTEMVData**)result;

/**
 * Extended Error Condition
 *
 Enables/disables extended error condition for commands 02-40, 61-xx, 62-xx, 83-41 when error is 0xd0a or 0xd0b
 *
 * @param enable TRUE = enable log, FALSE = disable log
 * @return RETURN_CODE: Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_extendedErrorCondition:(BOOL)enable;

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
 IDT_DEVICE_NEO2_IOS = 16
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
 Set BLE Service Scan Filter.
 
 When searching for BLE devices, this will limit the service search to the provided service ID's
 
 Example data format:
 NSArray<CBUUID *> *filter = [[NSArray alloc] initWithObjects:[CBUUID UUIDWithString:@"1820"], nil];
 
 @param filter The array of services to filter for
 */
-(void) setServiceScanFilter:(NSArray<CBUUID *> *) filter;


/**
 * Set Pass Through
 
 Sets Pass-Through mode on NEO2
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setPassThrough:(BOOL)enablePassThrough;


/**
* Poll for Token
*
Once Pass-Through Mode is started, ViVOpay will not poll for any cards until the “Poll for Token”
	command is received. This command tells ViVOpay to start polling for a Type A or Type B PICC
	until a PICC is detected or a timeout occurs.

	This command automatically turns the RF Antenna on.

If a PICC is detected within the specified time limit, ViVOpay activates it and responds back to the
	terminal with card related data such as the Serial Number.
If no PICC is detected within the specified time limit, ViVOpay stops polling and responds back
	indicating that no card was found. No card related data is returned in this case

 @param timeout  Timeout, in seconds to wait for card to be detected
 @param card Card Type:
   -  00h None (Card Not Detected or Could not Activate)
   -  01h ISO 14443 Type A (Supports ISO 14443-4 Protocol)
   -  02h ISO 14443 Type B (Supports ISO 14443-4 Protocol)
   -  03h Mifare Type A (Standard)
   -  04h Mifare Type A (Ultralight)
   -  05h ISO 14443 Type A (Does not support ISO 14443-4 Protocol)
   -  06h ISO 14443 Type B (Does not support ISO 14443-4 Protocol)
   -  07h ISO 14443 Type A and Mifare (NFC phone)

 @param serialNumber  Serial Number or the UID of the PICC
 @param ident Device ID to send command to.  If not specified, current SDK default device will be used.

 @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
*/

-(RETURN_CODE) device_pollForToken:(Byte)seconds card:(Byte**)card serialNumber:(NSData**)serialNumber;


/**
* Antenna Control
*
 The Antenna Control command turns the RF Antenna ON or OFF.

 @param turnON  TRUE = ON, FALSE = OFF

 @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
*/
-(RETURN_CODE) device_antennaControl:(bool)turnON;

/**
* Exchange Contactless Data
*
 The Echange Contactless Data command allows the host device to send, via the ViVOpay reader,
 application-level APDUs to a PICC that supports ISO 14443-4 Protocol. The reader sends the PICC
 response back to the host

 @param sendData  APDU Out
 @param receiveData  APDU response

 @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
*/
-(RETURN_CODE) device_exchangeContactlessData:(NSData*)sendData  receiveData:(NSData**)receiveData;

/**
* Set Special Function or Feature Configuration Command
*
 The Set Special Function or Feature Configuration command sets a specific special configuration.

 @param feature  Function/Feature ID
 @param addRequirement  Additional Requirement

 @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
*/
-(RETURN_CODE) device_setSpecialFunctionOrFeature:(NSData*)feature addRequirement:(NSData*)addRequirement;


/**
* Get Special Function or Feature Configuration Command
*
 The Get Special Function or Feature Configuration command returns the specific special configuration.

 @param feature  Function/Feature ID
 @param addRequirement  Additional Requirement

 @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
*/
-(RETURN_CODE) device_getSpecialFunctionOrFeature:(NSData**)feature addRequirement:(NSData**)addRequirement;


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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_retrieveTerminalData:(NSData**)responseData;
/**
 * Query File
 * 
 Report if tthe file exists, and if so will report the file timestamp and the file size
 
 @param directory The file directory to search in. If blank, will use the root directory
 @param filename Name of the file to retrieve
 @param isSD True = query SD card, False = query internal storage
 @param exists True = file exists, False = file does not exist
 @param timestamp If the file exists, reports the timestamp of the file
 @param fileSize If the file exists, reports the size of the file
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_queryFile:(NSString*)directory filename:(NSString*)filename isSD:(BOOL)isSD exists:(BOOL*)exists timestamp:(NSDate**)timestamp fileSize:(int*)fileSize;
/**
 * Read File from SD card
 * 
 Reads a file from the SD card
 
 @param directory The file directory to read from. If empty, the root directory is used
 @param filename The name of the file to retrieve
 @param fileData The contents of the file if it exists
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_readFileFromSD:(NSString*)directory filename:(NSString*)filename fileData:(NSData**)fileData;

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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_addTLVToTerminalData:(NSData*)tlv;

/**
 * Send Burst Mode
 *
 * Sets the burst mode forthe device.
 *
 * @param mode 0 = OFF, 1 = Always On, 2 = Auto Exit
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE)  device_setBurstMode:(int) mode;


/**
 * Send Poll Mode
 *
 * Sets the poll mode forthe device. Auto Poll keeps reader active, Poll On Demand only polls when requested by terminal
 *
 * @param mode 0 = Auto Poll, 1 = Poll On Demand
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setPollMode:(int) mode;


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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
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
 * Get Poll Mode
 *
 * Gets the current poll mode of the device
 *
 * @param mode Response from the device of the current poll mode
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getPollMode:(NSData**)mode;

/**
 * Get TransArmor ID
 *
 Gets the TransArmor ID from the device
 *
 * @param TID TransArmor ID
 * @return RETURN_CODE: Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getTransArmorID:(NSString**)TID;

/**
 * Get Kernel Check Value
 *
 Polls the device for the Kernel Check Value
 
 @param response Response returned of the Check Value of the Kernel
 * @return RETURN_CODE: Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelCheckValue:(NSString**)response;

/**
 * Get EMV Kernel Version
 * 
 Polls the device for the EMV Kernel Version
 
 @param response The kernel version response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersion:(NSString**)response;

/**
 * Callback Response PIN Entry for ETC
 * 
 Provides (or cancels) PIN entry information to kernel after a callback was received with callback type PINPAD ETC
 
 @param type If cancel button is pressed during PIN entry, then this value is EMV_PIN_MODE_CANCEL
 - If PIN bypass is pressed during PIN entry, then this value is EMV_PIN_MODE_BYPASS. Otherwise the value can be EMV_PIN_MODE_ONLINE_DUKPT, EMV_PIN_MODE_ONLINE_MKSK, or EMV_PIN_MODE_OFFLINE
 @param KSN If enciphered PIN, this is either PINK DUKPT Key or PIN Session Key or PIN Pairing DUKPT
 @param PIN If enciphered PIN, this is an ecrypted PIN block. If device does not implement pairing fuctions, this plaintext PIN
 */
-(RETURN_CODE) emv_callbackResponsePIN_ETC:(EMV_PIN_MODE_Types)type ksn:(NSData*)KSN pin:(NSData*)PIN;

/**
 * Callback Response Get ETC DUKPT key KSN
 * 
 Provides a status code to device request of DUKPT IK loaded status, from callback type GET KSN
 
 @param KSN The KSN data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_callbackResponseKSN:(NSData*)KSN;

/**
 * Verify DUKPT IK Loaded on ETC
 * 
 Use this command to verify the DUKPT IK isloaded into the ETC. NEO2 is activated and it can request PIN from ETC after this command
 
 @param KCV ASN.1 structure of KCV
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_verifyDUKPTLoaded:(NSData*)KCV;

/**
 * Get Extended EMV Kernel Version
 * 
 Polls the device for the extended EMV kernel version
 
 @param response The extended kernel verion response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersionExt:(NSString**)response;

/**
 * Remove All Application Data
 * 
 Removes all the application data for the EMV kernel
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE)emv_removeAllApplicationData;

/**
 * Enable L80 Passthrough
 * 
 Enables Passthrough mode for direct communicaiton to L80 hooked up to NEO2 device
 
 @param enablePassThrough True = passthrough ON, False = passthrough OFF
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_enableL80PassThrough:(BOOL)enablePassThrough;

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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getHardwareInfo:(NSString**)response;

/**
 * Get UID of MCU
 * 
 Returns the UID of the device
 
 @param response The module UID information
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_getUIDofMCU:(NSString**)response;

/**
 * Ping Device
 * 
 Pings the reader. If it is connected, returns success, otherwise returns timeout
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_pingDevice;

/**
 * Enable L100 Passthrough
 * 
 Enable passthrough mode for direct communication to L100 hooked up to NEO2 device
 
 @param enablePassThrough True = passthrough ON, False = passthrough OFF
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_enableL100PassThrough:(BOOL)enablePassThrough;

/**
 * Set TransArmor ID
 *
 Sets the TransArmor ID
 
 @param TID TransArmor ID
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setTransArmorID:(NSString*)TID;

/**
 * Listen for Notifcations
 *
 Instructs SDK to listen for unsolicited data
 
 @param enable True = Listen, False = Don't listen
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_listenForNotifications:(BOOL)enable; 

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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_controlLED:(Byte)indexLED control:(Byte)control;

/**
 * Device Certificate Type
 *
 Returns the device certificate type
 
 @param type 0 = Unknown, 1 = Demo, 2 = Production
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_certificateType:(int*)type;

/**
 * Delete File
 *
 This command deletes a file or group of files
 
 @param filename Complete path and file name of the file you want to delete. You do not need to specify the root directory. Indicate subdirectories with a foward slash (/)
 @param isSD True = Delete from SD card, False = Delete from Flash
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_deleteFile:(NSString*)filename isSD:(BOOL)isSD;

/**
 * Delete Directory
 *
 This command deletes an empty directory
 
 @param filename Complete path of the directory you want to delete.
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_listDirectory:(NSString*)directoryName recursive:(BOOL)recursive onSD:(BOOL)onSD directory:(NSString**)directory;

/**
 * Create Directory
 *
 This command adds a subdirectory to the indicated path
 
 @param directoryName The directory name. The data for this command is an ASCII string with the complete path and directory name you want to create. You do not need to specify the root directory. Indicate subdirectories with a forward slash (/)
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_createDirectory:(NSString*)directoryName;

/**
 * Enter Low Power Mode
 *
 Puts the terminal in sleep or stop mode, with the option to wake on swipe/tap
 
 @param stopMode True = Stop Mode (POR required), False = Sleep Mode (resume from last instruction)
 @param wakeOnTrans True = Swipe/Tap will wake from low power, False = Will not wake from swipe/tap
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_lowPowerMode:(BOOL)stopMode wakeOnTrans:(BOOL)wakeOnTrans;

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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_controlUserInterface:(NSData*)values;

/**
 * Load CA Certificate
 *
 Loads the CA certificate data onto the device
 
 @param type The type of certificate
 -00h: Application CA
 -01h: TLS CA
 
 @param cert The certificate data bytes
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_loadCertCA:(Byte)type CertData:(NSData*)cert;


/**
* RRC Connect
*
The RRC Connect command allows a host to establish an RRC connection to a reader. A host
must first establish an RRC connection to the reader before issuing other RRC IDG commands.


* @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()

*/
-(RETURN_CODE) device_rrcConnect;


/**
* RRC Disonnect
*
The RRC Disonnect command allows a host to terminate the RRC connection to a reader.


* @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()

*/
-(RETURN_CODE) device_rrcDisconnect;

/**
 * RRC Run application
 *  
 This command allows the reader to run an installed application
 
 @param appName The name of the installed application to run
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_rrcRunApp:(NSString*)appName;

/**
 * RRC Install Application
 * 
 The RRC Install Application command installs a downloaded application to a reader. Only installed applications can run on the reader.
 
 @param appName The name of the application that will appear on the Application Manager list
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) device_rrcInstallApp:(NSString*)appName;
/**
 * RRC Uninstall Application
 * 
 The RRC Uninstall Application command uninstalls an application on a reader device. The application will remain in the file system of the reader but cannot be executed.
 
 @param appName The name of the application that appears on the Application Manager list
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_rrcUninstallApp:(NSString*)appName;
/**
 * RRC Download Application
 * 
 The RRC Download Application command allows the transfer of a compressed application file from a host to a reader, extracts it, and performs signature verification on its contents
 
 The reader receives all the chunked data from the host and, once completed, the reader combines it into one file
 
 @param appName The name of the application that appears on the Application Manager list
 @param appData The compressed application file to be sent in chunks
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_rrcDownloadApp:(NSString*)appName appData:(NSData*)appData;

/**
 * Get MSR Secure Parameters
 *
 Gets the parameters from the flash setting
 
 @param b0 True = T1 force encryption
 @param b1 True = T2 force encryption
 @param b2 True = T3 force encryption
 @param b3 True = T3 force encryption when card type is 80
 @param tlv MSR secure parameters TLV objects
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getMsrSecurePara:(BOOL)b0 b1:(BOOL)b1 b2:(BOOL)b2 b3:(BOOL)b3 tlv:(NSData**)tlv;

/**
 * Get Module Version Information
 *
 Gets the 16 byte UID of the MCU
 
 @param uid The string representation of the UID
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getModuleVer:(NSString**)moduleVer;

/**
 * Disable Blue LED Sequence
 * 
 Stop the blue LEDs on the ViVOpay Vendi reader from flashing in the left to right sequence and the LEDs off. Contactless function is also disabled at the same time
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_disBlueLED;

/**
 * Enable the blue LED and modify the behaviour
 * 
 Control the blue LED behaviour on the Vendi reader
 
 @param dataCmd LED control. Minimum 4 bytes, maximum 25 bytes. First byte is cycle, next three bytes are the sequence. Then sequence can repeat up to 8 times.
 - Byte 0 = Cycle (0 = Cycle once, 1 = Repeat)
 - Byte 1 = LED state map
 *      - bit 7 = Left blue LED
 *      - bit 6 = Center blue LED
 *      - bit 5 = Right blue LED
 *      - bit 4 = Yellow LED
 *      - bit 3 = Reserved
 *      - bit 2 = Reserved
 *      - bit 1 = Reserved
 *      - bit 0 = Reserved
 - Bytes 2 & 3 = Duration (Given in multiples of 10 ms, i.e. 10/20/30 etc.)
 If Cycle = 1, more pairs be after 3
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_enaBlueLED:(NSData*)dataCmd;

/**
 * Turn On Yellow LED
 * 
 Turn on the ViVOpay Vendi reader yellow LED. This LED is located below the three blue LEDs
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_onYellowLED;

/**
 * Turn Off Yellow LED
 * 
 Turn off the ViVOpay Vendi reader yellow LED. This LED is located below the three blue LEDs
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_offYellowLED;

/**
 * Enter standby mode
 * 
 Puts unit into low power standby mode
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_enterStandbyMode;

/**
 * Get Light Sensor Value
 * 
 Gets the value from the sensor
 
 @param lightVal Value of the light sensor
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getLightSensorVal:(UInt16*)lightVal;

/**
 * Set TransArmor Encryption
 * 
 Sets the TransArmor encryption from the given certificate
 
 @param cert Certificate in PEM format or DER format. PEM format must be string data (converted to binary) starting with "---". DER format is binary data.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_setTransArmorEncryption:(NSData*)cert;

/**
 * Get Module Bytes
 * 
 Retrieves the first 64 bytes of the module information running in an ADF environment
 
 @param type The ADF type
 - ADF_TYPE_SDK = SDK
 - ADF_TYPE_APP = Application
 @param adfInfo List of modules information
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_getModuleBytes:(ADF_TYPE)type adfInfo:(NSArray<NSData*>**)adfInfo;

/**
 * Get Module Info
 * 
 Retrieves the module information when running in an ADF environment
 
 @param type The ADF type
 - ADF_TYPE_SDK = SDK
 - ADF_TYPE_APP = Application
 @param adfInfo List of modules information
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_getModuleInfo:(ADF_TYPE)type adfInfo:(NSArray**)adfInfo;

/**
 * Erase ADF Flash
 * 
 Erases the ADF flash memory
 
 @param type The ADF type
 - ADF_TYPE_SDK = SDK
 - ADF_TYPE_APP = Application
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_eraseFlash:(ADF_TYPE)type;

/**
 * Set JTAG
 * 
 Enables/Disables the JTAG pin in an ADF environment
 
 @param enable True = JTAG enabled, False = JTAG disabled
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_setJTAG:(BOOL)enable;

/**
 * Set ADF Mode
 * 
 Enables/Disables the ADF environment
 
 @param enable True = ADF enabled, False = ADF disabled
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_setADFMode:(BOOL)enable;

/**
 * Get ADF Mode
 * 
 Retrieves the state of the ADF environment
 
 @param enable True = ADF is enabled, False = ADF is disabled
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_getADFMode:(BOOL*)enable;

/**
 * Get MSR Track
 *
 Returns the track value of the MSR
 
 @param val The value of the current MSR tracks setting
 - 0 : Any track
 - 1 : Track 1
 - 2 : Track 2
 - 3 : Track 1, Track 2
 - 4 : Track 3
 - 5 : Track 1, Track 3
 - 6 : Track 2, Track 3
 - 7 : Track 1, Track 2 , Track 3
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getMSRTrack:(int*)val;

/**
 * Get MSR Configuration
 *
 Gets the MSR configuration data
 
 @param config Configuration data retrieved
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getConfiguration:(NSData**)config;

/**
 * Set MSR Configuration
 *
 Sets MSR configuration data
 
 @param config Configuration data to send
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setConfiguration:(NSData*)config;

/**
 * Retrieve MSR White List
 *
 Retrieves the whitelist
 
 @param value The whitelist data which is in ASN.1 block format
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_retrieveWhiteList:(NSData**)value;

/**
 * Set MSR Track
 *
 Sets the track value of the MSR
 
 @param val The value of the current MSR tracks setting
 - 0 : Any track
 - 1 : Track 1
 - 2 : Track 2
 - 3 : Track 1, Track 2
 - 4 : Track 3
 - 5 : Track 1, Track 3
 - 6 : Track 2, Track 3
 - 7 : Track 1, Track 2 , Track 3
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setMSRTrack:(int)val;

/**
 Sets the terminal major configuration in ICS .
 @param configuration A configuration value, range 1-5
 - 1 = 1C
 - 2 = 2C
 - 3 = 3C
 - 4 = 4C
 - 5 = 5C
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
 
 
 
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
 
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
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) emv_retrieveCAPK:(NSString*)rid index:(NSString*)index response:(CAKey**)response;

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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 - 0x0100 through 0xFFFF refer to IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setTerminalData:(NSDictionary*)data;





/**
 * Start EMV Transaction Request
 *
 Authorizes the EMV transaction  for an ICC card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function will complete with a return of EMV_RESULT_CODE_START_TRANSACTION_SUCCESS to emvTransactionData delegate protocol, and then IDT_NEO2::emv_authenticateTransaction() must be executed.  If auto authorize is ENABLED (default), IDT_NEO2::emv_authenticateTransaction() will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS.  The auto authorize can be enabled/disabled with IDT_NEO2::emv_disableAutoAuthenticateTransaction:()
 
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
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 *
 * NOTE ON INTERFACE SELECTION: For the NEO2, tag DFEF37 is used to determine which interfaces to use for the transaction:
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
 * Exchange Certificates, Nonces, and Keys
 * 
 Use this command to send the ETC certificate, nonce, and signature
 The returned data is the NEO2 certificate, nonce, and signature
 
 @param cert Send ETC certificate for signature verification, Receives NEO2 certificate for signature verification
 @param nonce Send ETC random nonce, Receives NEO2 random nonce
 @param signature Send ETC signature, Receives NEO2 signature. Signature of (CertETC_SV || NONCE_ETC) with PKCS1-v1_5 padding
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_exchangeCerts:(NSData**)cert nonce:(NSData**)nonce signature:(NSData**)signature;

/**
 * Get EMV Configuration Check Value
 * 
 Polls device for the EMV Configuration Check Value
 
 @param response Response returned of the Check Value of the Configuration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVConfigurationCheckValue:(NSString**)response;

/**
 * Callback Response MSR Entry
 * 
 Provides MSR information to kernel after a callback was received with type EMV_CALLBACK_MSR
 
 @param MSR Swiped track data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_callbackResponseMSR:(NSData*)MSR;

/**
 * Generate DUKPT IK using KEK
 *
 Use this command to send the encrypted KEK and signature generated by the ETC.
 NEO2 returns the DUKPT IK in TR-31 format encrypted with the KEK and signature
 
 @param cert ETC certificate for signature verification
 @param signature Signature of (KEK || NONCE_ETC) with PKCS1-v1_5 padding
 @param key ASN.1 structure of DUKPT IK used between NEO2 and ETC
 * @return RETURN_CODE: Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) emv_generateDUKPT:(NSData*)cert signature:(NSData*)signature key:(NSData**)key;

/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) icc_exchangeAPDU:(NSData*)dataAPDU response:(APDUResponse**)response;
/**
 * Get Reader Status
 *
 Returns the reader status
 
 @param readerStatus Pointer that will return with the ICCReaderStatus results.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 * Disable MSR Swipe
 
 
 
 Cancels Swipe.
 
 Cancels Transaction request (all interfaces, CTLS/MSR/INSERT).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;

/**
 * Enable MSR Swipe
 
 *
 Enables CLTS and MSR, waiting for swipe or tap to occur. Returns IDTEMVData to deviceDelegate::emvTransactionData:() 
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe;

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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 *
 * NOTE ON INTERFACE SELECTION: For the NEO2, tag DFEF37 is used to determine which interfaces to use for the transaction:
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
 * Capture Function Key
 *
 
 Captures a function key entry on the pinpad
 
 Results returned to pinpadData delegate

 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 */
-(RETURN_CODE) pin_captureFunctionKey;

/**
 * Cancel PIN Command
 *
 
 This command can cancel IDT_Device:getEncryptedPIN:keyType:line1:line2:line3:() and IDT_Device::getNumeric:minLength:maxLength:messageID:language:() and IDT_Device::getAmount:maxLength:messageID:language:() and IDT_Device::getCardAccount:max:line1:line2:() and
 IDT_Device::pin_getFunctionKey() and IDT_Device::getEncryptedData:minLength:maxLength:messageID:language:()
 
  * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
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

 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_readWithMac:(int)numBlocks blockList:(NSData*)blockList blocks:(NSData**)blocks;


/**
 * FeliCa Send Command
 *
 Send a Felica Command
 
 NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param command Command data from settlement center to be sent to felica card
 @param response Response data from felica card
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_SendCommand:(NSData*)command response:(NSData**)response;

/**
 * FeliCa Write with MAC Generation
 *
 Writes a block with MAC Generation.  FeliCa Authentication must be performed first
 
  NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param blockNumber Number of block
 @param data  Block to write.  Must be 16 bytes.
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
  * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
  
  */
 -(RETURN_CODE) felica_write:(NSData*)serviceCode blockCount:(int)blockCount  blockList:(NSData*)blockList data:(NSData*)data statusFlag:(NSData**)statusFlag;


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
        * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
        */
-(RETURN_CODE) ctls_nfcCommand:(NSData*)systemCode response:(NSData**)response;

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
 * FeliCa Request Service
 *
 Perform functions a Felica Request Service
 
  NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param nodeCode Node Code
 @param response  Response as explained in FeliCA Lite-S User's Manual
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_requestService:(NSData*)nodeCode response:(NSData**)response;


/**
 * Set BLE to filter for specified service UUID
 *
 *
 * @param setServiceUUID  Sets the UUID filter to be used when discovering  BLE devices
 *
 */
- (void)setServiceUUID:(nullable NSArray<CBUUID *> *)serviceUUIDs;



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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_NEO2::device_getResponseCodeString:()
 
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
