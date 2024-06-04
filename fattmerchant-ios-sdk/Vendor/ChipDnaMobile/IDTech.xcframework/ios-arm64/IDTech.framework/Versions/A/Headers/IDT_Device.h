//
//  IDT_Device.h
//  
//  IDT_Device SDK
//  V1.01.006
//  ceated by Xinhu Li on 14-1-9. IDTECH. 
//


#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED)
    #import <Cocoa/Cocoa.h>
    #import <IOBluetooth/objc/IOBluetoothDevice.h>
    #import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
    #import <IOBluetooth/objc/IOBluetoothRFCOMMChannel.h>
    #import <IOBluetoothUI/objc/IOBluetoothDeviceSelectorController.h>

#else
    #import <ExternalAccessory/ExternalAccessory.h>
    #import <UIKit/UIKit.h>
    #import <CoreBluetooth/CoreBluetooth.h>
#if !TARGET_IPHONE_SIMULATOR
#import "UniPay.h"
#import "uniMag.h"
#endif

#endif


#import "IDTMSRData.h"
#import "APDUResponse.h"
#import "IDTEMVData.h"

#import "IDTCommon.h"




/** Protocol methods established for IDT_Device class  **/
@protocol IDT_Device_Delegate <NSObject>

@optional

-(void) updateStatus:(PK_STATUS_Type)type currentBlock:(int)currentBlock totalBlocks:(int)totalBlocks error:(RETURN_CODE)error;//!<Reports PK Update status.
//!< @param type The stage of the PK update
//!< @param currentBlock The number of the block that has transferred
//!< @param totalBlocks The total number of blocks to transfer
//!< @param error The error condition when failure is encoutered


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

- (void) swipeMSRData:(IDTMSRData*)cardData;//!<Receives card data from MSR swipe or error condition in .
//!< @param cardData Captured card data from MSR swipe

- (void) deviceMessage:(NSString*)message;//!<Receives messages from the framework
//!< @param message String message transmitted by framework

- (void) bypassData:(NSData*)data;//!<When bypass output is enabled, all data intended for the current device will be sent here .
//!< @param data The data intended for the device

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



- (void) gen2Data:(NSData*)tlv;//!<Receives Gen2 TLV data.
//!< @param tlv TLV data from gen2 event


/**
 UniPay ICC Event
 This function will be called when an ICC is attached or detached from reader. Applies to UniPay only
 
 @param nICC_Attached Can be one of the following values:
 - 0x01: ICC attached while reader is idle
 - 0x00: ICC detached while reader is idle
 - 0x11: ICC attached while reader is in MSR mode
 - 0x10: After ICC Powered On, ICC Card Removal,Power off ICC
 
 @code
 -(void) UniPay_EventFunctionICC: (Byte) nICC_Attached
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
-(void) UniPay_EventFunctionICC: (Byte) nICC_Attached;



/**
 EMV Transaction Data
 
 This protocol will receive results from IDT_Device::startEMVTransaction:otherAmount:timeout:cashback:additionalTags:()
 
 
 @param emvData EMV Results Data.  Result code, card type, encryption type, masked tags, encrypted tags, unencrypted tags and KSN.  For UniPay1.5 / VP3300 / VP4880, the EMV results can be found in emvData.resultCodeV2

 @param error The error code as defined in the errors.h file.  For UniPay1.5 / VP3300 / VP4880, the error is defined as RETURN_CODE
 
 
 */
- (void) emvTransactionData:(IDTEMVData*)emvData errorCode:(int)error;

/**
 Pinpad data delegate protocol
 
 Receives data from pinpad methods
 
  @param value encrypted data returned from IDT_Device::getEncryptedData:minLength:maxLength:messageID:language:(), or encrypted account number returned from IDT_Device::getCardAccount:max:line1:line2:(). String value returned from IDT_Device::getAmount:maxLength:messageID:language:() or IDT_Device::getNumeric:minLength:maxLength:messageID:language:(). PINblock returned from IDT_Device::getEncryptedPIN:keyType:line1:line2:line3:()
  @param KSN Key Serial Number returned from IDT_Device::getEncryptedPIN:keyType:line1:line2:line3:(), IDT_Device::getCardAccount:max:line1:line2:() or IDT_Device::getEncryptedData:minLength:maxLength:messageID:language:()
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
 Class to drive the IDTech device
 */

#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED)

@interface IDT_Device : NSObject <NSStreamDelegate>{
    id<IDT_Device_Delegate> delegate;
    IOBluetoothDevice *mBluetoothDevice;
}
#else
#if TARGET_IPHONE_SIMULATOR
@interface IDT_Device : NSObject<EAAccessoryDelegate, NSStreamDelegate>{
    id<IDT_Device_Delegate> delegate;
    id<IDT_Device_Delegate> delegate2;
    id<IDT_Device_Delegate> bypassDelegate;
}
#else
@interface IDT_Device : NSObject<EAAccessoryDelegate, NSStreamDelegate,UniPay_Delegate,CBCentralManagerDelegate, CBPeripheralDelegate, UIAlertViewDelegate>{
    id<IDT_Device_Delegate> delegate;
    id<IDT_Device_Delegate> delegate2;
    id<IDT_Device_Delegate> bypassDelegate;
	
}
@property (nonatomic, strong) CBCentralManager *centralManager;
@property (nonatomic, strong) CBPeripheral *bleDevice;
@property (nonatomic, strong) NSMutableArray *peripherals;
@property (nonatomic, strong) NSMutableArray *peripheralsLocal;
@property bool scanOnly;
#endif

#endif

;
@property(strong) id<IDT_Device_Delegate> delegate;  //!<- Reference to IDT_Device_Delegate.
@property(strong) id<IDT_Device_Delegate> delegate2;  //!<- Reference to IDT_Device_Delegate.
	@property(strong) id<IDT_Device_Delegate> bypassDelegate;  //!<- Reference to IDT_Device_Delegate.
@property int bleRetryCount;

/**
* Reset Singleton
-  All Devices
*
Resets the singleton instance of the SDK

*/

+(void) resetSingleton;



/**
 * Bypass Time Sync
 *
 Bypasses the time sync function on device connection
  */
+(bool) BypassSync;


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
 Establishes an singleton instance of IDT_Device class.
 
 @return  Instance of IDT_Device
 */
+(IDT_Device*) sharedController;

/**
 Sets the flag to bypass output.
 @param bypass TRUE = bypass output
 */
+(void) bypassOutput:(bool)bypass;

+(void) setPKBlockSize:(int)size;

+(int) getPKBlockSize;

/**
 Sets the flag to bypass Ble Validation.
 @param bypass TRUE = bypass output
 */
+(void) bypassBLEValidation:(bool)bypass;

/**
 Outputs MSR swipe data in tag DFEE23.
 @param keep TRUE = DFEE23 is output, FALSE = Parsed as MSR data
 */
+(void) keepDFEE23:(bool)keep;


+(bool) EAOutputStreamSpaceAvailable;

/**
 Sets the flag to bypass checking SDK Status.
 @param bypass TRUE = bypass checking
 */
+(void) bypassEventCheck:(bool)bypass;


/**
 * Gets the IDTech Device Type
 - All Devices
 *
 Returns the current device type the SDK is efaulting to
 
 @return deviceType  Select from the IDT_DEVICE_Types enumeration
 
 @code
 typedef enum{
    IDT_DEVICE_BTPAY_IOS = 0,
    IDT_DEVICE_BTPAY_OSX_BT,
    IDT_DEVICE_BTPAY_OSX_USB,
    IDT_DEVICE_UNIPAY_IOS,
    IDT_DEVICE_UNIPAY_OSX_USB,
    IDT_DEVICE_UNIPAYII_IOS,
    IDT_DEVICE_UNIPAYII_OSX_USB,
    IDT_DEVICE_IMAG_IOS,
    IDT_DEVICE_VP3300_IOS,
    IDT_DEVICE_VP3300_OSX_USB,
    IDT_DEVICE_UNIMAG,
    IDT_DEVICE_BTMAG_IOS,
    IDT_DEVICE_BTMAG_OSX_BT,
    IDT_DEVICE_BTMAG_OSX_USB,
    IDT_DEVICE_VP3300_IOS,
    IDT_DEVICE_VP4880,
 *  IDT_DEVICE_UNIMAG_PRO
 }IDT_DEVICE_Types;

 @endcode
 

 
 
 */
+(IDT_DEVICE_Types) getDeviceType;


/**
 * Sets the IDTech Device Type
 - All Devices
 *
 Tells the framework what device type to configure for, in addition to connection type (BlueTooth or USB-HID) when applicable.  Default is BTPay 200 over iOS
 
 @param deviceType  Select from the IDT_DEVICE_Types enumeration
 
 @code
 typedef enum{
    IDT_DEVICE_BTPAY_IOS = 0,
    IDT_DEVICE_BTPAY_OSX_BT,
    IDT_DEVICE_BTPAY_OSX_USB,
    IDT_DEVICE_UNIPAY_IOS,
    IDT_DEVICE_UNIPAY_OSX_USB,
    IDT_DEVICE_UNIPAYII_IOS,
    IDT_DEVICE_UNIPAYII_OSX_USB,
    IDT_DEVICE_IMAG_IOS,
    IDT_DEVICE_VP3300_IOS,
    IDT_DEVICE_VP3300_OSX_USB,
    IDT_DEVICE_UNIMAG,
    IDT_DEVICE_BTMAG_IOS,
    IDT_DEVICE_BTMAG_OSX_BT,
    IDT_DEVICE_BTMAG_OSX_USB,
    IDT_DEVICE_VP3300_IOS,
    IDT_DEVICE_VP4880,
 *  IDT_DEVICE_UNIMAG_PRO
 }IDT_DEVICE_Types;

 @endcode
 

 
 
 */
+(void) setDeviceType:(IDT_DEVICE_Types)deviceType;

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
 Initialize class.
  - All Devices
 
 DEPRECATED: Use singleton instance [IDT_Device sharedConroller];
 */
-(id)init;

/**
 Disable Audio Detection.
 For BLE implementations.  Removes monitoring headphone jack for audio devices.
 */
+(void) disableAudioDetection;

/**
Remove Command Delay.
For Audio Jack implementations.  Removes delay between playing and reading audio waves.
*/
+(void) removeCommandDelay;

/**
 Set BLE Service Scan Filter.
 
 When searching for BLE devices, this will limit the service search to the provided service ID's
 
 Example data format:
 NSArray<CBUUID *> *filter = [[NSArray alloc] initWithObjects:[CBUUID UUIDWithString:@"1820"], nil];
 
 @param filter The array of services to filter for
 */
-(void) setServiceScanFilter:(NSArray<CBUUID *> *) filter;

/**
 Process Bypass Response.
 
 When output is being bypassed, and a command is received in the delegate redirectedOutput and sent to the device, the device rexponse must be sent to this
 method so the SDK can finishing processing the results;
 
 @param data The data received from the device that the SDK must process to complete the executing command
 */
-(void) processBypassResponse:(NSData*)data;

/**
 Opens connection to device.
  - All Devices
 
 DEPRECATED: Use singleton instance [IDT_Device sharedConroller] to automatically open/close device
  */
-(BOOL)open;

 /**
  Closes connection to device. 
   - All Devices
  
  DEPRECATED: Use singleton instance [IDT_Device sharedConroller] to automatically open/close device
   */
 -(void)close;

 /**
  Destroys connection to device.
   - All Devices
  
  DEPRECATED: Use singleton instance [IDT_Device sharedConroller] to automatically open/close device
   */
 -(void)destroy;

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
 * Begins searching for Bluetooth Low Energy devices in range
 - VP3300
 *
 * @param type The device type to attempt to connect to.
 * @param identifier This will only connect to a device with this calculated UUID identifier
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
-(bool) enableBLEDeviceSearch:(IDT_DEVICE_Types)type identifier:(nullable NSUUID*)identifier;


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
 - VP3300
 *
 *
 * @return NSString  Returns the default friendly name to be used when discovering any BLE devices
 *
 */
-(NSString*) getBLEFriendlyName;


/**
 * Disconnect from BLE -
 *
 * Will disconnect from existing BLE connection. You can now set another BLE Friendly Name to attach to another device.
 *
 */
-(void) device_disconnectBLE;

/**
 * Set BLE Friendly Name
 - VP3300
 *
 *
 * @param friendlyName  Sets the default friendly name to be used when discovering any BLE devices
 *
 */
-(void) setBLEFriendlyName:(nonnull NSString*)friendlyName;


/**
 * Set BluetoothParameters
 
 Sets the name and password for the BLE module.
 
 Sending nil to all three parameters resets the default password to 123456
 *
 * @param name  Device name, 1-25 characters
 * @param oldPW  Old password, as a six character string, example "123456"
 * @param newPW  New password, as a six character string, example "654321"
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_setBluetoothParameters:(NSString*)name oldPW:(NSString*)oldPW newPW:(NSString*)newPW;

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
 * Stops searching for Bluetooth Low Energy devices in range
 - VP3300
 *
 *
 * @return bool  If successful, polling was in progress and has stopped. If unsuccessful, BLE Device Search was not in progress.
 *
 * NOTE:  BLE only scans when there are no devices currently connected. After the SDK connects to any IDTech device, the scanning will pause automatically.
 */
-(bool) disableBLEDeviceSearch;

/**
 * Returns the UUID of the connected BLE device
 - VP3300
 *
 * @return NSUUID  UUID of the connected BLE device.  Returns nil if no BLE device connected.
 *
 */
-(NSUUID*) connectedBLEDevice;

/**
 * Polls device for EMV L1 Version
 - UniPayII
 *
 * @param response Response returned of Level 1 Version
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
 
 *
 */
-(RETURN_CODE) getEMVL1Version:(NSString**)response;

/**
 * Polls device for EMV L2 Version
 - UniPayII
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) getEMVL2Version:(NSString**)response;

/**
 * Polls device for EMV Kernel Version
  - BTPay 200
 *
 * @param response Response returned of Kernel Version
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

 *
 */
-(RETURN_CODE) getEMVKernelVersion:(NSString**)response;



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

-(RETURN_CODE) activateTransaction:(NSMutableDictionary<NSString*,NSString*> *)tags timeout:(int)timeout;


/**
 * Sync Clock
 Syncs the VP3300 clock with the iOS device clock
 *
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
 *
 */

-(RETURN_CODE)  device_syncClock;


/**
 * Sync Time
 Syncs the VP3300 time with the iOS device clock
 *
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
 *
 */

-(RETURN_CODE)  device_syncTime;


/**
 * Get Auto Poll Transaction Results
 Gets the transaction results when the reader is functioning in "Auto Poll" mode
 *
 * @param result The transaction results
 *
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
 *
 */
-(RETURN_CODE) getAutoPollTransactionResults:(IDTEMVData**)result;

/**
 * Retrieve Transaction Results
 
 *
 Retrieves specified EMV tags from the currently executing transaction.
 
 @param tags Tags to be retrieved.  Example 0x9F028A will retrieve tags 9F02 and 8A
 @param tlv All requested tags returned as unencrypted, encrypted and masked tags. The tlv NSDictionary will
 contain a NSDictionary with key "tags" that has the unencrypted tag data, a NSDictionary with the key
 "masked" that has the masked tag data, and a NSDictionary with the key "encrypted" that has the
 encrypted tag data
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) retrieveTransactionResult:(NSData*)tags retrievedTags:(NSDictionary**)retrievedTags;


/**
 * Send Burst Mode
 *
 * Sets the burst mode forthe device.
 *
 * @param mode 0 = OFF, 1 = Always On, 2 = Auto Exit
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE)  setBurstMode:(int) mode;


/**
 * Send Poll Mode
 *
 * Sets the poll mode forthe device. Auto Poll keeps reader active, Poll On Demand only polls when requested by terminal
 *
 * @param mode 0 = Auto Poll, 1 = Poll On Demand
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) setPollMode:(int) mode;

/**
 * Get Poll Mode
 *
 * Gets the current poll mode of the device
 *
 * @param mode Response from the device of the current poll mode
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_getPollMode:(NSData**)mode;

/**
 * Cancel the Activate Transaction
 * - VP3300
 
 Cancels the CTLS transaction.
 
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
 */
-(RETURN_CODE) cancelCTLSTransaction;

 /**
 * Start EMV Transaction Request
 *
 - BTPay 200
 Authorizes the EMV transaction  for an ICC card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
  @param amount Transaction amount value  (tag value 9F02)
  @param amtOther Other amount value, if any  (tag value 9F03)
  @param timeout Timeout value in seconds.
  @param type Transaction type (tag value 9C).
  @param tags Any other optional tags to be included in the request.  Passed as a mutable dictionary.


 
 
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




-(RETURN_CODE) startEMVTransaction:(float)amount otherAmount:(float)amtOther timeout:(int)timeout transactionType:(unsigned char)type additionalTags:(NSMutableDictionary *)tags;




/**
 * Start EMV Transaction Request
 *
 Authorizes the EMV transaction  for an ICC card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function will complete with a return of EMV_RESULT_CODE_START_TRANSACTION_SUCCESS to emvTransactionData delegate protocol, and then IDT_UniPayII::emv_authenticateTransaction() must be executed.  If auto authorize is ENABLED (default), IDT_UniPayII::emv_authenticateTransaction() will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS.  The auto authorize can be enabled/disabled with IDT_VP3300::emv_disableAutoAuthenticateTransaction:()
 
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
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) startTransaction:(double)amount amtOther:(double)amtOther type:(int)type timeout:(int)timeout tags:(NSData*)tags forceOnline:(BOOL)forceOnline  fallback:(BOOL)fallback;


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
 
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_startTransaction:(double)amount amtOther:(double)amtOther type:(int)type timeout:(int)timeout tags:(NSData*)tags forceOnline:(BOOL)forceOnline  fallback:(BOOL)fallback;

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
 * Reset non-volatile memory
 
 The Set Configuration Defaults and Keep Encryption Key command provides an external method for resetting parameters
 in non-volatile memory (NVM) to their default values. When the reader receives this command, it
 erases EEPROM (but retains encryption keys). After completing initialization, the reader reboots.
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_resetNVM;

/**
 * Complete EMV Transaction Online Request
  - BTPay 200
 *
 Completes an online EMV transaction request by the card
 
The tags will be returned in the response parameter.
 
 @param result Determines if connection to host was successful.
 @param tags Host response tag
 @param response returns the response tags
 
 Tag | Length | Description
 ----- | ----- | -----
 8A | 2 | Data element Authorization Response Code. Mandatory
 91 | 8-16 | Issuer Authentication Data. Optional
 71 | 0-256 | Issuer Scripts. Optional
 72 | 0-256 | Issuer Scripts. Optional
 

 
 
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
-(RETURN_CODE) completeOnlineEMVTransaction:(EMV_AUTHORIZATION_RESULT)result hostResponseTags:(NSMutableDictionary *)tags responseTags:(IDTEMVData**)response;

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
-(RETURN_CODE) completeOnlineEMVTransaction:(BOOL)isSuccess hostResponseTags:(NSData*)tags;


/**
 * Set Certificate Revocation List Entry
  - BTPay 200
 *
 Sets the CRL entry as specified by the CRLEntry structure
 
 @param key CRLEntry containing the RID, Index, and serial number to set
 
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
-(RETURN_CODE) setCRL:(CRLEntry)key;

/**
 * Remove Certificate Revocation List RID
  - BTPay 200
 *
 Removes all CRLEntry as specified by the RID and Index passed as a parameter in the CRLEntry structure
 
 @param key CRLEntry containing the RID and Index  of the of the entries to remove
 
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
-(RETURN_CODE) removeCRL:(CRLEntry)key;

/**
 * Remove Certificate Revocation List unit
  - BTPay 200
 *
 Removes a single CRLEntry as specified by the RID/Index/Serial Number passed as a parameter in the CRLEntry structure
 
 @param key CRLEntry containing the RID, Index and serial number of the of the entry to remove
 
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
-(RETURN_CODE) removeCRLUnit:(CRLEntry)key;

/**
 * Retrieve the Certificate Revocation List specific to RID and index
  - BTPay 200
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
 [[IDT_Device sharedController] retrieveCRLForRID:@"a000000003" response:&returnArray];
 for (NSData* obj in returnArray) {
    Byte *keyByte = (Byte*) obj.bytes;
    CRLEntry* crl = (CRLEntry*)keyByte;
 }
 @endcode
 
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
-(RETURN_CODE) retrieveCRLForRID:(NSString*)rid response:(NSArray**)response;

/**
 * Retrieve the Certificate Revocation List
  - BTPay 200
  - UniPay II
 *
 Returns all the RID in the CRL.
 @param response Response returned as an NSArray of NSData objects (either 5 or 9 bytes each):
  - On BTPay, 5-byte objects for each RID
  - On UniMagII, 9-bytes objects:  5-bytes RID, 1-byte Index, 3-byte Serial Number
 
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
-(RETURN_CODE) retrieveCRLList:(NSMutableArray**)response;

/**
 * Remove Certificate Revocation List
 *
 - UniPay II
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) removeCRLList;

/**
 * Retrieve Certificate Authority Public Key
  - BTPay 200
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 

 */
-(RETURN_CODE) retrieveCAPK:(NSString*)rid index:(NSString*)index response:(CAKey**)response;

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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) setCRLEntries:(NSData*)data;
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 
 */
-(RETURN_CODE) retrieveCAPKFile:(NSString*)rid index:(NSString*)index response:(NSData**)response;

/**
 * Remove Certificate Authority Public Key
  - BTPay 200
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) removeCAPK:(NSString*)rid index:(NSString*)index ;

/**
 * Set Certificate Authority Public Key
  - BTPay 200
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) setCAPK:(CAKey)key;

/**
 * Retrieve the Certificate Authority Public Key list
  - BTPay 200
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
*/
-(RETURN_CODE) retrieveCAPKList:(NSArray**)response;



/**
 * Retrieve Terminal Data
  - BTPay 200
 *
 Retrieves the Terminal Data.  The data will be in the response parameter
 
 @param response Response returned as a TerminalData
 
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
-(RETURN_CODE) retrieveTerminalData:(TerminalData**)response;

/**
 * Remove Terminal Data
  - BTPay 200
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) removeTerminalData;

/**
 * Set Terminal Data
  - BTPay 200
 *
 Sets the Terminal Data as specified by the TerminalData structure passed as a parameter
 
 @param data TerminalData configuration file
 
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
-(RETURN_CODE) setTerminalData:(TerminalData)data;

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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 
 
 
 */
-(RETURN_CODE) retrieveTerminalDataUniPay:(NSDictionary**)responseData;



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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_setTerminalData:(NSData*)tags;

/**
 * Get Key format for ICC DUKPT
 
 Specifies how data is being encrypted with Data Key or PIN key
 
 *
 * @param format  Response return from method:
 -0x00 : Encrypted card data with TDES if DUKPT Key had been loaded
 -0x01 : Encrypted card data with AES if DUKPT Key had been loaded
 -Other Data : No Encryption
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) icc_setKeyFormatForICCDUKPT:(NSData*)encryption;

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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 */

-(RETURN_CODE) device_addTLVToTerminalData:(NSData*)tlv;

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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()

 
 
 
 */
-(RETURN_CODE) retrieveApplicationDataUniPay:(NSString*)AID response:(NSDictionary**)responseAID;

/**
 * Retrieve Application Data by AID
  - BTPay 200
 *
 Retrieves the Application Data as specified by the AID name passed as a parameter.  The AID will be in the response parameter responseAID
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 @param responseAID  The response returned from the method
 
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
-(RETURN_CODE) retrieveApplicationData:(NSString*)AID response:(ApplicationID**)responseAID;

/**
 * Remove Application Data by AID
  - BTPay 200
  - UniPay II
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) removeApplicationData:(NSString*)AID;

/**
 * Set Application Data by AID
 - UniPay II
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
 DF63 | Transaction Reference Currency Conversion
 
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) setApplicationDataUniPay:(NSString*)aidName configData:(NSDictionary*)data;

/**
 * Set Terminal Data
 - UniPay II
 *
 Sets the Terminal Data as specified by the dictionary containing Tags (dictionary keys) and Values (dictionary objects) according to the following table
 
 Tag | Description
 ===== | =====
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) setTerminalDataUniPay:(NSDictionary*)data;

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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) setCAPKFile:(NSData*)file;

/**
 * Set Application Data by AID
  - BTPay 200
 *
 Sets the Application Data as specified by the ApplicationID structure passed as a parameter
 
 @param data ApplicationID configuration file
 
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
-(RETURN_CODE) setApplicationData:(ApplicationID)data;

/**
 * Retrieve AID list
  - BTPay 200
  - UniPay II
 *
 Returns all the AID names supported by the terminal.
 
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) retrieveAIDList:(NSArray**)response;



/**
 * Keep Accessory Connected
  - BTPay 200
 *
 * Framework attempts to disconnect from accessory whever application goes to backround.  Setting this value to TRUE will disable framework disconnect attempts.  NOTE:  ExternalAccessory may still disconnect device when going to background by default.  If you want to stay connected to device, you must also set the .pist "Required Background Modes" to "App communicates using CoreBluetooth", "App communicates with an accessory", and "App shares data using CoreBluetooth"
 *
 * @param stayConnected  TRUE = stay connected while in background (assuming .plist is properly configured)
 */
-(void)stayConnected:(BOOL)stayConnected;


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

-(RETURN_CODE) exchangeEncryptedAPDU:(NSData*)dataAPDU response:(APDUResponse**)response;

/**
 * Exchange APDU
  - BTPay 200
  - UniPay
 *
 * Sends an APDU packet to the ICC.  If successful, response is returned in APDUResult class instance in response parameter. If encrypted, a KSN must be initially provided.  If encrypted and no KSN provided, the last provided KSN will be utilized
 
 @param dataAPDU  APDU data packet
 @param encrypted  Send data encrypted
 @param ksn For encrypted APDU, 10-byte KSN value, or nil if unencrypted or encrypted and use previous KSN
 @param response Unencrypted/encrypted parsed APDU response
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

-(RETURN_CODE) exchangeAPDU:(NSData*)dataAPDU encrypted:(BOOL)encrypted ksn:(NSData*)ksn response:(APDUResponse**)response;

/**
 * Get APDU KSN
 - BTPay 200
 
 *
 * Retrieves the KSN used in ICC Encypted APDU usage
 
 * @param ksn Returns the encrypted APDU packet KSN
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
-(RETURN_CODE) getAPDU_KSN:(NSData**)ksn;

/**
 * Power Off ICC
 - BTPay 200
 - UniPay

 *
 * Powers down the ICC
 
 * @param error Returns the error, if any
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
 
 If Success, empty
 If Failure, ASCII encoded data of error string
 */
-(RETURN_CODE) powerOffICC:(NSString**)error;


/**
 * Power On ICC
 - BTPay 200
 - UniPay

 *
 * Power up the currently selected microprocessor card in the ICC reader
 *
 * @param response Response returned. If Success, ATR String. If Failure, ASCII encoded data of error string
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
-(RETURN_CODE) powerOnICC:(NSData**)response;

/**
 * Power On ICC with Options
 - BTPay 200

 *
 * Power up the currently selected microprocessor card in the ICC reader, specifying IFS/pps options.
 
 @param options ATR Options
* @param response Response returned. If Success, ATR String. If Failure, ASCII encoded data of error string
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
-(RETURN_CODE) powerOnICC:(PowerOnStructure)options response:(NSData**)response;


/**
 * Command Acknowledgement Timout
 *
 * Sets the amount of seconds to wait for an {ACK} to a command before a timeout.  Responses should normally be received under one second.  Default is 3 seconds.
 *
 * @param nSecond  Timout value.  Valid range .1 - 60.0 seconds
 
 * @return Success flag.  Determines if value was set and in range.
 */
-(BOOL) setCmdTimeOutDuration: (float) nSecond;


/**
 * Send a NSData object to device
 - All Devices
 *
 * Sends a command represented by the provide NSData object to the device through the accessory protocol.
 *
 * @param cmd NSData representation of command to execute
 * @param lrc If <c>TRUE</c>, this will wrap command with start/length/lrc/sum/end:  '{STX}{Len_Low}{Len_High} data {CheckLRC} {CheckSUM} {ETX}'
   @param response Response data
 
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
-(RETURN_CODE) sendDataCommand:(NSData*)cmd calcLRC:(BOOL)lrc response:(NSData**)response;


/**
 * Get Reader Status
 - BTPay 200
 - UniPay
 *
 Returns the reader status
 
 @param readerStatus Pointer that will return with the ICCReaderStatus results.
 
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
 
@code
    ICCReaderStatus readerStatus;
    RETURN_CODE rt = [[IDT_Device sharedController] getICCReaderStatus:&readerStatus];
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

-(RETURN_CODE) getICCReaderStatus:(ICCReaderStatus**)readerStatus;



/**
 * Set Bluetooth Address
 - BTPay 200

 *
 Sets the Bluetooth address of the device. 6 bytes, example F0DE07CCA03F.
 
 @param address 6 Byte address represented by a 12-character HEX string
 
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

-(RETURN_CODE) setBluetoothAddress:(NSString*)address;


/**
 * Reboot Device
 - BTPay 200
 - UniPay

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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) rebootDevice;

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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) device_rebootDevice;

/**
 * Clear Display
 - UniPay II
 
 *
 Clears the display of the UniPay II.
 *
 
 @param option 0=Clear First Line, 1 = Clear Second Line, 2= Clear Both Lines
 
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

-(RETURN_CODE) clearDisplay:(int)option;

/**
 * Backlight Control
 - UniPay II
 
 *
 Turns on/off the backlight of the UniPay II.
 *
 
 @param turnON TRUE = Turn On, FALSE = Turn Off
 
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

-(RETURN_CODE) backlightControl:(BOOL)turnON;

/**
 * Restore Default PINpad Settings
 - UniPay II
 
 *
 Executes a command to restore default PINpad settings.  PIN Length 4-12, Numeric Len 1-16;
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
-(RETURN_CODE) defaultSettingsPinPadUniPay;


/**
 * Sets the Beep Value
 - UniPayII
 
 *
 Sets a beep value on the UniPayII.
 *
 @param frequency Frequence of the beep.  Valid range 1000-20000.
 @param duration Duration in milliseconds.  Valid range 16-65535.
 
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
-(RETURN_CODE) sendBeepUniPayII:(int)frequency duration:(int)duration;

/**
 * Sends a Beep Value
 - BTPay 200

 *
 Executes a beep on the BT200.  The complete beep may be defined as a multiple of single beep tones.
 *
 @param beep Unsigned short array containing freq1,dur1,freq2,dur2,. . . freq#,dur#.  Frequency is in Hz and must be in the range 2000-4000. Duration is in milliseconds.
 @param num Number of tones in the beep array.
 
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
 
 Example Code-
 @code
 unsigned short beep[] = {0xb00,0x400,0x800,0x300};
 RETURN_CODE rt = [[IDT_Device sharedController] sendBeep:beep numberOfTones:2];
 LOGI(@"\nControl Beep Return Status Code %i ",  rts);
 @endcode
 
 */
-(RETURN_CODE) sendBeep:(unsigned short*)beep numberOfTones:(int)num;

/**
 * UniPay 200
 
 *
 Display a message on either line 1 or line 2  in the UniPayII LCD.
 
 @param message Display message, up to 16 characters
 @param line 0 = First Line, 1 = Second Line
 
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
-(RETURN_CODE) displayMessageUniPayII:(NSString*)message lineNumber:(int)line;

/**
 * UniPay 200
 
 *
 Saves a prompt into UniPay memory.
 
 @param message Prompt message, up to 16 characters
 @param location Memory location 0-9
 
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
-(RETURN_CODE) savePromptUniPayII:(NSString*)message location:(int)location;


/**
 - UniPay II
 
 Display a Prompt stored in UniPay II.
 
 @param prompt Prompt number, 0-9
 @param line 0 = First Line, 1 = Second Line
 
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
-(RETURN_CODE) displayPromptUniPayII:(int)prompt lineNumber:(int)line;
/**
 * DisplayMessage
 - BTPay 200

 *
 Display up to 4 lines of text in the device LCD.
 
 @param line1 Display line 1, up to 12 characters
 @param line2 Display line 2, up to 16 characters
 @param line3 Display line 3, up to 16 characters
 @param line4 Display line 4, up to 16 characters
 
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
-(RETURN_CODE) displayMessage:(NSString*)line1 line2:(NSString*)line2 line3:(NSString*)line3 line4:(NSString*)line4;

/**
 * Set Enter Sleep Mode Time
 - BTPay 200

 *
Sets seconds of idle that must pass before entering sleep mode
 
 @param seconds  Amount of time (in seconds) that must pass during idle before unit goes to sleep
 
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
-(RETURN_CODE) setIdleTimeForSleep: (int) seconds;

/**
 * Put device to sleep
 - BTPay 200

 *
 Set device to enter  sleep mode. In sleep mode, LCD display and backlight is off. It can be waked up by key press or sending commands
 
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
-(RETURN_CODE) enterSleepMode;

/**
 * Polls device for Firmware Version
 - BTPay 200
 - UniPay
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
  *
 */
-(RETURN_CODE) getFirmwareVersion:(NSString**)response;

/**
 * Polls device for Model Number
 - BTPay 200
 - UniPay
 *
 * @param response  Returns Model Number
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
 *
 */
-(RETURN_CODE) getModelNumber:(NSString**)response;

/**
 * Polls device for status of the keys
 - UniPay II
 *
 * @param response  Returns Key Status.  NSData byte stream following following format: 
    PIN DUKPT Status + PIN Master Key Status + PIN Session Key Status + Account DUKPT Key Status + AccountDUKPT Key Status + Admin DUKPT Key
 PIN DUKPT Key:
 - 0: None.
 - 1: Exist
 - 0xFF: STOP
 
 PIN Master Key:
 - 0: None
 - 1: At least Exist a Master Key
 
 PIN Session Key:
 - 0: None.
 - 1: Exist
 
 Account DUKPT Key:
 - 0: None.
 - 1: Exist
 - 0xFF: STOP
 
 Account DUKPT Key:
 - 0: None.
 - 1: Exist
 - 0xFF: STOP
 
 Admin DUKPT Key:
 - 0: None.
 - 1: Exist
 - 0xFF: STOP
 
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
 *
 */
-(RETURN_CODE) getKeyStatus:(NSData**)response;

/**

 * Polls device for Battery Voltage
 - UniPay
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) getBatteryVoltage:(NSString**)response;

/**
 * Polls device for Serial Number
 - BTPay 200
 - UniPay
 *
 * @param response  Returns Serial Number
 
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
 
 *
 */
-(RETURN_CODE) getSerialNumber:(NSString**)response;

/**
 * Set Serial Number
 - BTPay 200
 - UniPay
 *
 Set device's serial number and Bluetooth name, then reboots device. Bluetooth name will be set as IDT_Device + Space + Serial number
 *
 @param strSN Device serial number
 
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
-(RETURN_CODE) setSerialNumber:(NSString*)strSN;

/**
 * Get interface device's serial number

 *
 * @param response  Returns Serial Number
 
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
 
 *
 */
-(RETURN_CODE) getInterfaceDeviceSN:(NSString**)response;

/**
 * Set Interface Device serial number.
 *
 EMV serial number can be set only once
 *
 @param sn Device serial number
 
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
-(RETURN_CODE) setInterfaceDeviceSN:(NSString*)sn;

/**
 * Get terminal identification
 *
 * @param response  Returns device terminal identification
 
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
 
 *
 */
-(RETURN_CODE) getTerminalIdentification:(NSString**)response;

/**
 * Set terminal identifcation
 *
 Set device's serial number and Bluetooth name, then reboots device. Bluetooth name will be set as IDT_Device + Space + Serial number
 *
 @param sn Device terminal identification
 
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
-(RETURN_CODE) setTerminalIdentification:(NSString*)sn;

/**
 * Polls device for current Date/Time
 - BTPay 200
 *
 * @param response Response returned as ASCII Data of Date  YYMMDDhhmmss. Example 140215171628 = Feb. 15, 2014, 28 seconds into 5:16pm.
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
 

 
 @code
 NSString* response;
 RETURN_CODE rt = [[IDT_Device sharedController] getDateTime:&response];
    if (RETURN_CODE_DO_SUCCESS == rt)
    {
        LOGI* (@"Date Time (YYMMDDhhmmss) = %@",response);
    }
 @endcode
 *
 */
-(RETURN_CODE) getDateTime:(NSString**)response;

/**
 * Get BCD Mask Character
 UniPay
 
 *
 Executes a command to get the BCD Masking Charatger.
 *
 @param response Masking character. Range is 0x0A - 0x0F.  Response range is "A" to "F"
 - Default character is 0x0C ("C")
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) getBCDMaskChar:(NSString**)response;

/**
 * Set BCD Mask Character
 UniPay
 
 *
 Executes a command to set the ASCII Masking Character.
 *
 @param mask Masking character. Range is 0x0A - 0x0F.
 - Default character is 0x0C
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) setBCDMaskChar:(char)mask;

/**
*
* Restores the default ICC group settings
*
- Card Type: EMV
- ICC Notifications
- Prefix PAN = 4
- PostFix PAN = 4
- ASCII Mask Char = 0x2A ("*")
- BCD Mask Char = 0x0C
- L1 Transaction Timeout = 8 Seconds
*
* @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
*/
-(RETURN_CODE) restoreDefaultSettings;
/**
 * Get Expiry Date Option
 UniPay
 
 *
 Executes a command to get the Expiry Date Option.
 *
 @param response Expiry Option..
 - "0" Output masked for Tag 57 and only output encrypted for Tag 5F24
 - "1" Output plaintext
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) getExpiryDateOption:(NSString**)response;



/**
 * Get ASCII Mask Character
 UniPay
 
 *
 Executes a command to get the ASCII Masking Character.
 *
 @param response Masking character. Range is 0x20 - 0x7E.
 - Default character is 0x2A ("*")
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) getASCIIMaskChar:(NSString**)response;


/**
 * Set ASCII Mask Character
 UniPay
 
 *
 Executes a command to set the ASCII Masking Character.
 *
 @param mask Masking character. Range is 0x20 - 0x7E.
 - Default character is 0x2A ("*")
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) setASCIIMaskChar:(char)mask;

/**
 * Get Prefix/PostFix PAN
 UniPay
 
 *
 Executes a command to retrieve Prefix and Postfix PAN Control Data.
 *
 @param response Prefix.Postfix values separated by dot ("4.3" = 4 Prefix and 3 Postfix)
 - Prefix range is 0-6, default 4
 - Postfix range is 0-4, default 4
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) getPrePostFixPAN:(NSString**)response;

/**
 * Set Prefix/PostFix PAN
 UniPay
 
 *
 Executes a command to retrieve Prefix and Postfix PAN Control Data.
 *
 @param prefix Prefix value range is 0-6
 @param postfix Postfix range is 0-4
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) setPrePostFixPAN:(int)prefix postfix:(int)postfix;


/**
 * Get Card Type Option
 UniPay
 
 *
 Executes a command to retrieve card type.
 *
 @param response Card type
  - "EMV" = EMV Card
  - "ISO" = ISO Card
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) getCardOption:(NSString**)response;

/**
 * Set Card Type Option
 UniPay
 
 *
 Executes a command to set card type.
 *
 @param option Card type
 - 0xFF = EMV Card
 - 0x00 = ISO Card
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) setCardOption:(int)option;



/**
 * Get Remote Key Injection Timeout
 UniPay
 
 *
 Executes a command to retrieve RKI Timout value.  Range 120-3600 seconds
 *
 @param response Response data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) getRKITimeout:(NSNumber**)response;

/**
 * Set Remote Key Injection Timeout
 UniPay
 
 *
 Executes a command to Set RKI Timout value.  Range 120-3600 seconds
 *
 @param seconds Timeout value in seconds (120-3600)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */

-(RETURN_CODE) setRKITimeout:(int)seconds;

/**
 * Get KSN
 *
 * Retrieves the KSN for a key slot
 @param keySlot 0x02=MSR DUKPT Key, 0x0C = Admin DUKPT Key, 0x22 = ICC DUKPT Key
 * @param ksn Returns the Account DUKPT KSN
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE
 
 - If  Key was not loaded, unit should respond error code 0x0400.
 - If  Key is end of useful life, unit should respond error code 0x7300
 
 */
-(RETURN_CODE) getKSN:(int)keySlot ksn:(NSData**)ksn;

/**
 * Set device device Date/Time
 - BTPay 200
 *
 Set device's date/time
 *
 @param date Device date represented by a YYMMDDhhmmss.  Example March 12, 2014, 6:30pm (and 12 seconds) = 140312183012
 
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
-(RETURN_CODE) setDateTime:(NSString*)date;


/**
 * Cancel PIN Command
 *
 
 This command can cancel IDT_Device:getEncryptedPIN:keyType:line1:line2:line3:() and IDT_Device::getNumeric:minLength:maxLength:messageID:language:() and IDT_Device::getAmount:maxLength:messageID:language:() and IDT_Device::getCardAccount:max:line1:line2:() and 
     IDT_Device::pin_getFunctionKey() and IDT_Device::getEncryptedData:minLength:maxLength:messageID:language:() */
-(RETURN_CODE) cancelPin;


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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) pin_capturePin:(int)type PAN:(NSString*)PAN minPIN:(int)minPIN maxPIN:(int)maxPIN message:(NSString*)message;

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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) pin_captureNumericInput:(bool)mask minPIN:(int)minPIN maxPIN:(int)maxPIN message:(NSString*)message signature:(NSData*)signature;

/**
 * Capture Function Key
 *
 
 Captures a function key entry on the pinpad
 
  Results returned to pinpadData delegate
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) pin_captureFunctionKey;

/**
 * Set Numeric Length
 - UniPay II
 *
 Sets the Numeric length.
 
 @param minLength Minimum Numeric length at least 1
 @param maxLength Maximum Numeric length not to exceed 16
 
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
-(RETURN_CODE) setNumericLength:(int)minLength maxLength:(int)maxLength;

/**
 * Set PIN Length
 - BTPay 200
 - UniPayII
 *
 Sets the encrypted PIN length.
 
 @param minLength Minimum PIN length at least 4
 @param maxLength Maximum PIN length not to exceed 12
 
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
-(RETURN_CODE) setPinLength:(int)minLength maxLength:(int)maxLength;

/**
 Get Numeric Length
 - UniPay II
 
 * Returns Numeric Length
 *
 @param response Returned Value of Minimum/Maximum Numeric length:
 - response[0] = minimum length
 - response[1] = maximum length
 
 
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
 
 @code
 NSData* res;
 RETURN_CODE rt = [[IDT_Device sharedController] getNumericLength:&res];
 uint8_t b[res.length];
 [data getBytes:b];
 if(RETURN_CODE_DO_SUCCESS == rt && res.length>1){
 LOGI(@"getNumericLength: min=%d max=%d", b[0], b[1]);
 }
 @endcode
 */
-(RETURN_CODE) getNumericLength:(NSData**)response;

/**
 Get PIN Length
 - BTPay 200
 
 * Returns encrypted PIN Length
 *
 @param response Returned Value of Minimum/Maximum PIN length:
  - response[0] = minimum length
  - response[1] = maximum length
 
 
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
 
 @code
    NSData* res;
    RETURN_CODE rt = [[IDT_Device sharedController] getPinLength:&res];
    uint8_t b[res.length];
    [data getBytes:b];
    if(RETURN_CODE_DO_SUCCESS == rt && res.length>1){
    LOGI(@"GetPinLength: min=%d max=%d", b[0], b[1]);
    }
 @endcode
 */
-(RETURN_CODE) getPinLength:(NSData**)response;

/**
 Get PINPad Status
 - BTPay 200
 
 * Returns status of PINpad
 *
 @param response Pinpad status. response[0]:
 - 0x01: PINpad is inactivate.
 - 0x02: PINpad  has been activated, but Public Key is not loaded.
 - 0x03: Public key has been loaded, but Firmware Key, Numeric Key and Check Value is not loaded.
 - 0x10: PINpad  normal work status.
 - 0x30: PINpad  suspend status if password input error.
 - 0x31: PINpad  suspend status if get PIN(MKSK)120 times in one hours.
 
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
 
 @code
    Byte *b = (Byte*)malloc(1);
    RETURN_CODE rt = [[IDT_Device sharedController] getPINpadStatus];
    if(RETURN_CODE_DO_SUCCESS == rt){
    [self appendMessageToResults:[NSString stringWithFormat:@"PINPad Status: %d", b[0]]];
 
 }
 @endcode
 }

 */
-(RETURN_CODE) getPINpadStatus:(NSData**)response;

/**
 * Set encrypted MSR Data Output Format
 - BTPay 200

 *
 * Sets how data will be encrypted, with either Data Key or PIN key (if MSR DUKPT key loaded)
 *
 @param encryption Encryption Type
  - 00: Encrypt with Data Key
  - 01: Encrypt with PIN Key
 
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
-(RETURN_CODE) setEncryptMSRFormat:(int)encryption;

/**
 * Get encrypted MSR Data Output Format
 - BTPay 200

 *
 * Gets the encrypted algorightm of MSR card data and SmartCard data (if MSR DUKPT key loaded)
 *
 * @param response Response returned from method:
 - '1': 3DES (default)
 - '2': AES
 
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
-(RETURN_CODE) getEncryptMSRFormat:(NSString**)response;


/**
 * Set Card Data Encrypted Algorithm
 - BTPay 200
 
 *
 * Sets the encrypted algorightm of MSR card data and SmartCard data (if MSR DUKPT key loaded)
 *
 @param encryption Encryption Type
 - 01: 3DES (Default)
 - 02: AES
 
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
-(RETURN_CODE) setCardDataEncryptedAlgorithm:(int)encryption;

/**
 * Get Card Data Encrypted Algorithm
 - BTPay 200
 
 *
 * Sets the encrypted algorightm of MSR card data and SmartCard data (if MSR DUKPT key loaded
 *
 * @param response Response returned from method:
 - '0': Encrypted card data with Data Key if MSR DUKPT Key had been loaded.(default)
 - '1': Encrypted card data with PIN Key if MSR DUKPT Key had been loaded.
 
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
-(RETURN_CODE) getCardDataEncryptedAlgorithm:(NSString**)response;


/**
 * Set Clear PAN Digits
 - UniPay
 *
 * Sets the amount of digits shown in the clear (not masked) at the beginning of the returned PAN value
 *
 @param digits Number of digits to show in clear.  Range 0-6.
 
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
-(RETURN_CODE) setClearPANID:(int)digits;

/**
 * Set Clear PAN Digits
 - UniPay II
 *
 * Sets the amount of digits shown in the clear (not masked) at the beginning of the returned PAN value
 *
 @param digits Number of digits to show in clear.  Range 0-6.
 
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
-(RETURN_CODE) setClearPANIDII:(int)digits;

/**
 * Get Clear PAN Digits
 - UniPay
 *
 * Returns the number of digits that begin the PAN that will be in the clear
 *
 * @param response Number of digits in clear.  Values are ASCII '0' - '6':
 
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
-(RETURN_CODE) getClearPANID:(NSString**)response;

/**
 * Get Backlight Status
 - UniPay
 *
 * Returns the backlight status of the UniPayII Display
 *
 * @param response Staus = "ON" or "OFF"
 
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
-(RETURN_CODE) getBacklightStatus:(NSString**)response;


/**
 * Set Expiration Masking
 - UniPay
 *
 * Sets the flag to mask the expiration date
 *
 @param masked TRUE = mask expiration
 
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
-(RETURN_CODE) setExpirationMask:(BOOL)masked;

/**
 * Set Expiration Masking (UniPayII)
 - UniPay
 *
 * Sets the flag to mask the expiration date
 *
 @param masked TRUE = mask expiration
 
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
-(RETURN_CODE) setExpirationMaskII:(BOOL)masked;

/**
 * Get Expiration Masking
 - UniPay
 *
 * Get the flag that determines if to mask the expiration date
 *
 * @param response '0' = masked, '1' = not-masked
 
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
-(RETURN_CODE) getExpirationMask:(NSString**)response;

/**
 * Set Swipe Data Encryption
 - UniPay
 *
 * Sets the swipe encryption method
 *
 @param encryption 1 = TDES, 2 = AES
 
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
-(RETURN_CODE) setSwipeEncryption:(int)encryption;

/**
 * Get Swipe Data Encryption
 - UniPay
 *
 * Returns the encryption used for sweip data
 *
 * @param response 'TDES', 'AES', 'NONE'
 
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
-(RETURN_CODE) getSwipeEncryption:(NSString**)response;

/**
 * Set Swipe Force Encryption
 - UniPay
 *
 * Sets the swipe force encryption options
 *
 @param track1 Force encrypt track 1
 @param track2 Force encrypt track 2
 @param track3 Force encrypt track 3
 @param track3card0 Force encrypt track 3 when card type is 0
 
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
-(RETURN_CODE) setSwipeForcedEncryptionOption:(BOOL)track1 track2:(BOOL)track2 track3:(BOOL)track3 track3card0:(BOOL)track3card0;

/**
 * Get Swipe Data Encryption
 - UniPay
 *
 * Sets the swipe force encryption options
 *
 * @param response A string with for flags separated by PIPE character  f1|f2|f3|f4, example "1|0|0|1" where:
 - f1 = Track 1 Force Encrypt
 - f2 = Track 2 Force Encrypt
 - f3 = Track 3 Force Encrypt
 - f4 = Track 3 Force Encrypt when card type is 0
 
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
-(RETURN_CODE) getSwipeForcedEncryptionOption:(NSString**)response;

/**
 * Set Swipe Mask Option
 - UniPay
 *
 * Sets the swipe mask/clear data sending option
 *
 @param track1 Mask track 1 allowed
 @param track2 Mask track 2 allowed
 @param track3 Mask track 3 allowed
 
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
-(RETURN_CODE) setSwipeMaskOption:(BOOL)track1 track2:(BOOL)track2 track3:(BOOL)track3;

/**
 * Get Swipe Mask Option
 - UniPay
 *
 * Gets the swipe mask/clear data sending option
 *
 * @param response A string with for flags separated by PIPE character  f1|f2|f3, example "1|0|0" where:
 - f1 = Track 1 Mask Allowed
 - f2 = Track 2 Mask Allowed
 - f3 = Track 3 Mask Allowed
 
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
-(RETURN_CODE) getSwipeMaskOption:(NSString**)response;


/**
 * Set Key Type for ICC DUKPT Key
  - UniPay
 *
 * Sets which key the data will be encrypted with, with either Data Key or PIN key (if DUKPT key loaded)
 *
 @param encryption Encryption Type
 - 00: Encrypt with Data Key
 - 01: Encrypt with PIN Key
 
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
-(RETURN_CODE) setKeyTypeForICCDUKPT:(int)encryption;

/**
 * Get Key Type for ICC DUKPT
 - UniPay
 *
 * Specifies the key type used for ICC DUKPT encryption
 *
 * @param response Response returned from method:
 - 'DATA': Encrypted card data with Data Key DUKPT Key had been loaded.(default)
 - 'PIN': Encrypted card data with PIN Key if DUKPT Key had been loaded.
 
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
-(RETURN_CODE) getKeyTypeForICCDUKPT:(NSString**)response;

/**
 * Set Key Format for ICC DUKPT
 - UniPay
 *
 * Sets how data will be encrypted, with either TDES or AES (if DUKPT key loaded)
 *
 @param encryption Encryption Type
 - 00: Encrypt with TDES
 - 01: Encrypt with AES
 
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
-(RETURN_CODE) setKeyFormatForICCDUKPT:(int)encryption;

/**
 * Get Key Format For ICC DUKPT
  - UniPay
 *
 * Specifies how data will be encrypted with Data Key or PIN key (if DUKPT key loaded)
 *
 * @param response Response returned from method:
 - 'TDES': Encrypted card data with TDES if  DUKPT Key had been loaded.(default)
 - 'AES': Encrypted card data with AES if DUKPT Key had been loaded.
 - 'NONE': No Encryption.
 
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
-(RETURN_CODE) getKeyFormatForICCDUKPT:(NSString**)response;

/**
 * Display Message and Get Encrypted PIN online
 - BTPay 200
 *
 Prompts the user with up to 3 lines of text. Returns pinblock/ksn of entered PIN value in deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_ENCRYPTED_PIN
 
 @param account Card account number
 @param type Encryption Key Type:
 - 0x00: External Account Key PIN_KEY_TDES_MKSK_extp
 - 0x01: External Account Key PIN_KEY_TDES_DUKPT_extp
 - 0x20: Internal Account Key PIN_KEY_TDES_MKSK_intl
 - 0x21: Internal Account Key PIN_KEY_TDES_DUKPT_intl
 - 0x20: Internal Account Key PIN_KEY_TDES_MKSK2_intl
 - 0x21: Internal Account Key PIN_KEY_TDES_DUKPT2_intl
 @param line1 Display line 1, up to 12 characters
 @param line2 Display line 2, up to 16 characters
 @param line3 Display line 3, up to 16 characters
 -
 
 
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
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - If there is no any enter in 20 seconds, the entered PIN key will be cleared.
 - When press Enter key , it will end this Command and response package with NGA format .
 - When press Cancel key, the entered PIN  key will be cleared and if press Cancel key again, this command terminated.
 - Cancel Command can terminate this command.
 
 */
-(RETURN_CODE) getEncryptedPIN:(NSString*)account keyType:(PIN_KEY_Types)type line1:(NSString*)line1 line2:(NSString*)line2 line3:(NSString*)line3;


/**
 * Display Message and Enable MSR Swipe
 - BTPay 200

 *
 Prompts the user with up to 3 lines of text. Enables MSR, waiting for swipe to occur. Returns IDTMSRData instance to deviceDelegate::swipeMSRData:()
 
 During waiting for swiping card, it will receive all commands except IDT_Device::getEncryptedPIN:keyType:line1:line2:line3:() and IDT_Device::getNumeric:minLength:maxLength:messageID:language:() and IDT_Device::getAmount:maxLength:messageID:language:() and IDT_Device::getCardAccount:max:line1:line2:() and IDT_Device::getEncryptedData:minLength:maxLength:messageID:language:()
 
 
 @param line1 Display line 1, up to 12 characters
 @param line2 Display line 2, up to 16 characters
 @param line3 Display line 3, up to 16 characters
 -
 
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
-(RETURN_CODE) startMSRSwipeWithDisplay:(NSString*)line1 line2:(NSString*)line2 line3:(NSString*)line3;

/**
 * Enable MSR Swipe
 - UniPay
 
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
-(RETURN_CODE) startMSRSwipe:(int)track;

/**
 * Disable MSR Swipe
 - BTPay 200
 - UniPay

 
 Cancels MSR swipe request.
 
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
-(RETURN_CODE) cancelMSRSwipe;

/**
 * Get Response Code String
 - All Devices
 *
 Interpret a IDT_Device response code and return string description.
 
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
-(NSString *) getResponseCodeString: (int) errorCode;



/**
 * Display Message and Get Card Account
 - BTPay 200

 *
 Show message on LCD and get card account number from keypad, then return encrypted card account number. Returns encryptedData of entered account in deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_ACCOUNT
 
 @param minLength Minimum account number length - not less than 1
 @param maxLength Maximum account number length - not more than 16
 @param line1 Display line 1, up to 12 characters
 @param line2 Display line 2, up to 16 characters
 -
 
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
 
 \par Notes
 - If  there is no any enter in 3 minutes, this command time out.
 - If  there is no any enter in 20 seconds, the entered account numbers will be cleared.
 - When press Enter key, it will end this command and respond package with NGA format.
 - When press Cancel key, the entered account numbers will be cleared and if press Cancel key again, this command terminated.
 - Cancel command can terminate this command.
 */
-(RETURN_CODE) getCardAccount:(int)minLength max:(int)maxLength line1:(NSString*)line1 line2:(NSString*)line2;


/**
 * Display Message and Get Encrypted Data
 - BTPay 200

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
 LANGUAGE_TYPE_SPANISH,
 LANGUAGE_TYPE_FRENCH
 }LANGUAGE_TYPE;
 @endcode
 
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
 
 \par Notes
 
 -  If  there is no any enter in 3 minutes, this command time out.
 -  If  there is no any enter in 20 seconds, the entered account numbers will be cleared.
 -  When press Enter key, it will end this command and respond package with NGA format.
 -  When press Cancel key, the entered account numbers will be cleared and if press Cancel key again, this command terminated.
 -  Cancel command can terminate this command.
 -  Maximum pattern number allowed is 10. If any error or invalid command is sent, input data patterns will be cleared and command fail.
 
 \par Secure Messages
 Secure messages to be used with General Prompts commands
 
 Msg Id |English Prompt | Portuguese Prompt | Spanish Prompt | French Prompt
 ---------- | ---------- | ----------  | ---------- | ----------
 1 | ENTER | ENTER | INGRESE | ENTREZ
 2 | REENTER | RE-INTRODUZIR | REINGRESE | RE-ENTREZ
 3 | ENTER YOUR | INTRODUZIR O SEU | INGRESE SU | ENTREZ VOTRE
 4 | REENTER YOUR | RE-INTRODUZIR O SEU | REINGRESE SU | RE-ENTREZ VOTRE
 5 | PLEASE ENTER | POR FAVOR DIGITE | POR FAVOR INGRESE | SVP ENTREZ
 6 | PLEASE REENTER | POR FAVO REENTRAR | POR FAVO REINGRESE | SVP RE-ENTREZ
 7 | PO NUMBER | NÚMERO PO | NUMERO PO | No COMMANDE
 8 | DRIVER ID | LICENÇA | LICENCIA | ID CONDUCTEUR
 9 | ODOMETER | ODOMETER | ODOMETRO | ODOMETRE
 10 | ID NUMBER | NÚMERO ID | NUMERO ID | No IDENT
 11 | EQUIP CODE | EQUIP CODE | CODIGO EQUIP | CODE EQUIPEMENT
 12 | DRIVERS ID | DRIVER ID | ID CONDUCTOR | ID CONDUCTEUR
 13 | JOB NUMBER | EMP NÚMERO | NUMERO EMP | No TRAVAIL
 14 | WORK ORDER | TRABALHO ORDEM | ORDEN TRABAJO | FICHE TRAVAIL
 15 | VEHICLE ID | ID VEÍCULO | ID VEHICULO | ID VEHICULE
 16 | ENTER DRIVER | ENTER DRIVER | INGRESE CONDUCTOR | ENTR CONDUCTEUR
 17 | ENTER DEPT | ENTER DEPT | INGRESE DEPT | ENTR DEPARTEMNT
 18 | ENTER PHONE | ADICIONAR PHONE | INGRESE TELEFONO | ENTR No TELEPH
 19 | ENTER ROUTE | ROUTE ADD | INGRESE RUTA | ENTREZ ROUTE
 20 | ENTER FLEET | ENTER FROTA | INGRESE FLOTA | ENTREZ PARC AUTO
 21 | ENTER JOB ID | ENTER JOB ID | INGRESE ID TRABAJO | ENTR ID TRAVAIL
 22 | ROUTE NUMBER | NÚMERO PATH | RUTA NUMERO | No ROUTE
 23 | ENTER USER ID | ENTER USER ID | INGRESE ID USUARIO | ID UTILISATEUR
 24 | FLEET NUMBER | NÚMERO DE FROTA | FLOTA NUMERO | No PARC AUTO
 25 | ENTER PRODUCT | ADICIONAR PRODUTO | INGRESE PRODUCTO | ENTREZ PRODUIT
 26 | DRIVER NUMBER | NÚMERO DRIVER | CONDUCTOR NUMERO | No CONDUCTEUR
 27 | ENTER LICENSE | ENTER LICENÇA | INGRESE LICENCIA | ENTREZ PERMIS
 28 | ENTER FLEET NO | ENTER NRO FROTA | INGRESE NRO FLOTA | ENT No PARC AUTO
 29 | ENTER CAR WASH | WASH ENTER | INGRESE LAVADO | ENTREZ LAVE-AUTO
 30 | ENTER VEHICLE | ENTER VEÍCULO | INGRESE VEHICULO | ENTREZ VEHICULE
 31 | ENTER TRAILER | TRAILER ENTER | INGRESE TRAILER | ENTREZ REMORQUE
 32 | ENTER ODOMETER | ENTER ODOMETER | INGRESE ODOMETRO | ENTREZ ODOMETRE
 33 | DRIVER LICENSE | CARTEIRA DE MOTORISTA | LICENCIA CONDUCTOR | PERMIS CONDUIRE
 34 | ENTER CUSTOMER | ENTER CLIENTE | INGRESE CLIENTE | ENTREZ CLIENT
 35 | VEHICLE NUMBER | NÚMERO DO VEÍCULO | VEHICULO NUMERO | No VEHICULE
 36 | ENTER CUST DATA | ENTER CLIENTE INFO | INGRESE INFO CLIENTE | INFO CLIENT
 37 | REENTER DRIVID | REENTRAR DRIVER ID | REINGRESE ID CHOFER | RE-ENTR ID COND
 38 | ENTER USER DATA | ENTER INFO USUÁRIO | INGRESE INFO USUARIO | INFO UTILISATEUR
 39 | ENTER CUST CODE | ENTER CODE. CLIENTE | INGRESE COD. CLIENTE | ENTR CODE CLIENT
 40 | ENTER EMPLOYEE | ENTER FUNCIONÁRIO | INGRESE EMPLEADO | ENTREZ EMPLOYE
 41 | ENTER ID NUMBER | ENTER NÚMERO ID | INGRESE NUMERO ID | ENTREZ No ID
 42 | ENTER DRIVER ID | ENTER ID DRIVER | INGRESE ID CONDUCTOR | No CONDUCTEUR
 43 | ENTER FLEET PIN | ENTER PIN FROTA | INGRESE PIN DE FLOTA | NIP PARC AUTO
 44 | ODOMETER NUMBER | NÚMERO ODOMETER | ODOMETRO NUMERO | No ODOMETRE
 45 | ENTER DRIVER LIC | ENTER DRIVER LIC | INGRESE LIC CONDUCTOR | PERMIS CONDUIRE
 46 | ENTER TRAILER NO | NRO TRAILER ENTER | INGRESE NRO TRAILER | ENT No REMORQUE
 47 | REENTER VEHICLE | REENTRAR VEÍCULO | REINGRESE VEHICULO | RE-ENTR VEHICULE
 48 | ENTER VEHICLE ID | ENTER VEÍCULO ID | INGRESE ID VEHICULO | ENTR ID VEHICULE
 49 | ENTER BIRTH DATE | INSERIR DATA NAC | INGRESE FECHA NAC | ENT DT NAISSANCE
 50 | ENTER DOB MMDDYY | ENTER FDN MMDDYY | INGRESE FDN MMDDAA | NAISSANCE MMJJAA
 51 | ENTER FLEET DATA | ENTER FROTA INFO | INGRESE INFO DE FLOTA | INFO PARC AUTO
 52 | ENTER REFERENCE | ENTER REFERÊNCIA | INGRESE REFERENCIA | ENTREZ REFERENCE
 53 | ENTER AUTH NUMBR | ENTER NÚMERO AUT | INGRESE NUMERO AUT | No AUTORISATION
 54 | ENTER HUB NUMBER | ENTER HUB NRO | INGRESE NRO HUB | ENTREZ No NOYAU
 55 | ENTER HUBOMETER | MEDIDA PARA ENTRAR HUB | INGRESE MEDIDO DE HUB | COMPTEUR NOYAU
 56 | ENTER TRAILER ID | TRAILER ENTER ID | INGRESE ID TRAILER | ENT ID REMORQUE
 57 | ODOMETER READING | QUILOMETRAGEM | LECTURA ODOMETRO | LECTURE ODOMETRE
 58 | REENTER ODOMETER | REENTRAR ODOMETER | REINGRESE ODOMETRO | RE-ENT ODOMETRE
 59 | REENTER DRIV. ID | REENTRAR DRIVER ID | REINGRESE ID CHOFER | RE-ENT ID CONDUC
 60 | ENTER CUSTOMER ID | ENTER CLIENTE ID | INGRESE ID CLIENTE | ENTREZ ID CLIENT
 61 | ENTER CUST. ID | ENTER CLIENTE ID | INGRESE ID CLIENTE | ENTREZ ID CLIENT
 62 | ENTER ROUTE NUM | ENTER NUM ROUTE | INGRESE NUM RUTA | ENT No ROUTE
 63 | ENTER FLEET NUM | FROTA ENTER NUM | INGRESE NUM FLOTA | ENT No PARC AUTO
 64 | FLEET PIN | FROTA PIN | PIN DE FLOTA | NIP PARC AUTO
 65 | DRIVER # | DRIVER # | CONDUCTOR # | CONDUCTEUR
 66 | ENTER DRIVER # | ENTER DRIVER # | INGRESE CONDUCTOR # | ENT # CONDUCTEUR
 67 | VEHICLE # | VEÍCULO # | VEHICULO # | # VEHICULE
 68 | ENTER VEHICLE # | ENTER VEÍCULO # | INGRESE VEHICULO # | ENT # VEHICULE
 69 | JOB # | TRABALHO # | TRABAJO # | # TRAVAIL
 70 | ENTER JOB # | ENTER JOB # | INGRESE TRABAJO # | ENTREZ # TRAVAIL
 71 | DEPT NUMBER | NÚMERO DEPT | NUMERO DEPTO | No DEPARTEMENT
 72 | DEPARTMENT # | DEPARTAMENTO # | DEPARTAMENTO # | DEPARTEMENT
 73 | ENTER DEPT # | ENTER DEPT # | INGRESE DEPTO # | ENT# DEPARTEMENT
 74 | LICENSE NUMBER | NÚMERO DE LICENÇA | NUMERO LICENCIA | No PERMIS
 75 | LICENSE # | LICENÇA # | LICENCIA # | # PERMIS
 76 | ENTER LICENSE # | ENTER LICENÇA # | INGRESE LICENCIA # | ENTREZ # PERMIS
 77 | DATA | INFO | INFO | INFO
 78 | ENTER DATA | ENTER INFO | INGRESE INFO | ENTREZ INFO
 79 | CUSTOMER DATA | CLIENTE INFO | INFO CLIENTE | INFO CLIENT
 80 | ID # | ID # | ID # | # ID
 81 | ENTER ID # | ENTER ID # | INGRESE ID # | ENTREZ # ID
 82 | USER ID | USER ID | ID USUARIO | ID UTILISATEUR
 83 | ROUTE # | ROUTE # | RUTA # | # ROUTE
 84 | ENTER ROUTE # | ADD ROUTE # | INGRESE RUTA # | ENTREZ # ROUTE
 85 | ENTER CARD NUM | ENTER NÚMERO DE CARTÃO | INGRESE NUM TARJETA | ENTREZ NO CARTE
 86 | EXP DATE(YYMM) | VALIDADE VAL (AAMM) | FECHA EXP (AAMM) | DATE EXPIR(AAMM)
 87 | PHONE NUMBER | TELEFONE | NUMERO TELEFONO | NO TEL
 88 | CVV START DATE | CVV DATA DE INÍCIO | CVV FECHA INICIO | CVV DATE DE DEBUT
 89 | ISSUE NUMBER | NÚMERO DE EMISSÃO | NUMERO DE EMISION | NO DEMISSION
 90 | START DATE (MMYY) | DATA DE INÍCIO (AAMM) | FECHA INICIO (AAMM) | DATE DE DEBUT-AAMM
 */
-(RETURN_CODE) getEncryptedData:(BOOL)lastPackage minLength:(int)minLength maxLength:(int)maxLength messageID:(int)mID language:(LANGUAGE_TYPE)lang;

/**
 * Display Message and Get Numeric Key(s)
 - BTPay 200

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
 LANGUAGE_TYPE_SPANISH,
 LANGUAGE_TYPE_FRENCH
 }LANGUAGE_TYPE;
 @endcode
 
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
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - If there is no any enter in 20 seconds, the entered numeric key will be cleared.
 - When press Enter key , it will end this Command and response package with NGA  format .
 - When press Cancel key, the entered numeric key will be cleared and if press Cancel key again, this command terminated.
 - Cancel Command can terminate this command.
 
 \par Secure Messages
 Secure messages to be used with General Prompts commands
 
 Msg Id |English Prompt | Portuguese Prompt | Spanish Prompt | French Prompt
 ---------- | ---------- | ----------  | ---------- | ----------
 1 | ENTER | ENTER | INGRESE | ENTREZ
 2 | REENTER | RE-INTRODUZIR | REINGRESE | RE-ENTREZ
 3 | ENTER YOUR | INTRODUZIR O SEU | INGRESE SU | ENTREZ VOTRE
 4 | REENTER YOUR | RE-INTRODUZIR O SEU | REINGRESE SU | RE-ENTREZ VOTRE
 5 | PLEASE ENTER | POR FAVOR DIGITE | POR FAVOR INGRESE | SVP ENTREZ
 6 | PLEASE REENTER | POR FAVO REENTRAR | POR FAVO REINGRESE | SVP RE-ENTREZ
 7 | PO NUMBER | NÚMERO PO | NUMERO PO | No COMMANDE
 8 | DRIVER ID | LICENÇA | LICENCIA | ID CONDUCTEUR
 9 | ODOMETER | ODOMETER | ODOMETRO | ODOMETRE
 10 | ID NUMBER | NÚMERO ID | NUMERO ID | No IDENT
 11 | EQUIP CODE | EQUIP CODE | CODIGO EQUIP | CODE EQUIPEMENT
 12 | DRIVERS ID | DRIVER ID | ID CONDUCTOR | ID CONDUCTEUR
 13 | JOB NUMBER | EMP NÚMERO | NUMERO EMP | No TRAVAIL
 14 | WORK ORDER | TRABALHO ORDEM | ORDEN TRABAJO | FICHE TRAVAIL
 15 | VEHICLE ID | ID VEÍCULO | ID VEHICULO | ID VEHICULE
 16 | ENTER DRIVER | ENTER DRIVER | INGRESE CONDUCTOR | ENTR CONDUCTEUR
 17 | ENTER DEPT | ENTER DEPT | INGRESE DEPT | ENTR DEPARTEMNT
 18 | ENTER PHONE | ADICIONAR PHONE | INGRESE TELEFONO | ENTR No TELEPH
 19 | ENTER ROUTE | ROUTE ADD | INGRESE RUTA | ENTREZ ROUTE
 20 | ENTER FLEET | ENTER FROTA | INGRESE FLOTA | ENTREZ PARC AUTO
 21 | ENTER JOB ID | ENTER JOB ID | INGRESE ID TRABAJO | ENTR ID TRAVAIL
 22 | ROUTE NUMBER | NÚMERO PATH | RUTA NUMERO | No ROUTE
 23 | ENTER USER ID | ENTER USER ID | INGRESE ID USUARIO | ID UTILISATEUR
 24 | FLEET NUMBER | NÚMERO DE FROTA | FLOTA NUMERO | No PARC AUTO
 25 | ENTER PRODUCT | ADICIONAR PRODUTO | INGRESE PRODUCTO | ENTREZ PRODUIT
 26 | DRIVER NUMBER | NÚMERO DRIVER | CONDUCTOR NUMERO | No CONDUCTEUR
 27 | ENTER LICENSE | ENTER LICENÇA | INGRESE LICENCIA | ENTREZ PERMIS
 28 | ENTER FLEET NO | ENTER NRO FROTA | INGRESE NRO FLOTA | ENT No PARC AUTO
 29 | ENTER CAR WASH | WASH ENTER | INGRESE LAVADO | ENTREZ LAVE-AUTO
 30 | ENTER VEHICLE | ENTER VEÍCULO | INGRESE VEHICULO | ENTREZ VEHICULE
 31 | ENTER TRAILER | TRAILER ENTER | INGRESE TRAILER | ENTREZ REMORQUE
 32 | ENTER ODOMETER | ENTER ODOMETER | INGRESE ODOMETRO | ENTREZ ODOMETRE
 33 | DRIVER LICENSE | CARTEIRA DE MOTORISTA | LICENCIA CONDUCTOR | PERMIS CONDUIRE
 34 | ENTER CUSTOMER | ENTER CLIENTE | INGRESE CLIENTE | ENTREZ CLIENT
 35 | VEHICLE NUMBER | NÚMERO DO VEÍCULO | VEHICULO NUMERO | No VEHICULE
 36 | ENTER CUST DATA | ENTER CLIENTE INFO | INGRESE INFO CLIENTE | INFO CLIENT
 37 | REENTER DRIVID | REENTRAR DRIVER ID | REINGRESE ID CHOFER | RE-ENTR ID COND
 38 | ENTER USER DATA | ENTER INFO USUÁRIO | INGRESE INFO USUARIO | INFO UTILISATEUR
 39 | ENTER CUST CODE | ENTER CODE. CLIENTE | INGRESE COD. CLIENTE | ENTR CODE CLIENT
 40 | ENTER EMPLOYEE | ENTER FUNCIONÁRIO | INGRESE EMPLEADO | ENTREZ EMPLOYE
 41 | ENTER ID NUMBER | ENTER NÚMERO ID | INGRESE NUMERO ID | ENTREZ No ID
 42 | ENTER DRIVER ID | ENTER ID DRIVER | INGRESE ID CONDUCTOR | No CONDUCTEUR
 43 | ENTER FLEET PIN | ENTER PIN FROTA | INGRESE PIN DE FLOTA | NIP PARC AUTO
 44 | ODOMETER NUMBER | NÚMERO ODOMETER | ODOMETRO NUMERO | No ODOMETRE
 45 | ENTER DRIVER LIC | ENTER DRIVER LIC | INGRESE LIC CONDUCTOR | PERMIS CONDUIRE
 46 | ENTER TRAILER NO | NRO TRAILER ENTER | INGRESE NRO TRAILER | ENT No REMORQUE
 47 | REENTER VEHICLE | REENTRAR VEÍCULO | REINGRESE VEHICULO | RE-ENTR VEHICULE
 48 | ENTER VEHICLE ID | ENTER VEÍCULO ID | INGRESE ID VEHICULO | ENTR ID VEHICULE
 49 | ENTER BIRTH DATE | INSERIR DATA NAC | INGRESE FECHA NAC | ENT DT NAISSANCE
 50 | ENTER DOB MMDDYY | ENTER FDN MMDDYY | INGRESE FDN MMDDAA | NAISSANCE MMJJAA
 51 | ENTER FLEET DATA | ENTER FROTA INFO | INGRESE INFO DE FLOTA | INFO PARC AUTO
 52 | ENTER REFERENCE | ENTER REFERÊNCIA | INGRESE REFERENCIA | ENTREZ REFERENCE
 53 | ENTER AUTH NUMBR | ENTER NÚMERO AUT | INGRESE NUMERO AUT | No AUTORISATION
 54 | ENTER HUB NUMBER | ENTER HUB NRO | INGRESE NRO HUB | ENTREZ No NOYAU
 55 | ENTER HUBOMETER | MEDIDA PARA ENTRAR HUB | INGRESE MEDIDO DE HUB | COMPTEUR NOYAU
 56 | ENTER TRAILER ID | TRAILER ENTER ID | INGRESE ID TRAILER | ENT ID REMORQUE
 57 | ODOMETER READING | QUILOMETRAGEM | LECTURA ODOMETRO | LECTURE ODOMETRE
 58 | REENTER ODOMETER | REENTRAR ODOMETER | REINGRESE ODOMETRO | RE-ENT ODOMETRE
 59 | REENTER DRIV. ID | REENTRAR DRIVER ID | REINGRESE ID CHOFER | RE-ENT ID CONDUC
 60 | ENTER CUSTOMER ID | ENTER CLIENTE ID | INGRESE ID CLIENTE | ENTREZ ID CLIENT
 61 | ENTER CUST. ID | ENTER CLIENTE ID | INGRESE ID CLIENTE | ENTREZ ID CLIENT
 62 | ENTER ROUTE NUM | ENTER NUM ROUTE | INGRESE NUM RUTA | ENT No ROUTE
 63 | ENTER FLEET NUM | FROTA ENTER NUM | INGRESE NUM FLOTA | ENT No PARC AUTO
 64 | FLEET PIN | FROTA PIN | PIN DE FLOTA | NIP PARC AUTO
 65 | DRIVER # | DRIVER # | CONDUCTOR # | CONDUCTEUR
 66 | ENTER DRIVER # | ENTER DRIVER # | INGRESE CONDUCTOR # | ENT # CONDUCTEUR
 67 | VEHICLE # | VEÍCULO # | VEHICULO # | # VEHICULE
 68 | ENTER VEHICLE # | ENTER VEÍCULO # | INGRESE VEHICULO # | ENT # VEHICULE
 69 | JOB # | TRABALHO # | TRABAJO # | # TRAVAIL
 70 | ENTER JOB # | ENTER JOB # | INGRESE TRABAJO # | ENTREZ # TRAVAIL
 71 | DEPT NUMBER | NÚMERO DEPT | NUMERO DEPTO | No DEPARTEMENT
 72 | DEPARTMENT # | DEPARTAMENTO # | DEPARTAMENTO # | DEPARTEMENT
 73 | ENTER DEPT # | ENTER DEPT # | INGRESE DEPTO # | ENT# DEPARTEMENT
 74 | LICENSE NUMBER | NÚMERO DE LICENÇA | NUMERO LICENCIA | No PERMIS
 75 | LICENSE # | LICENÇA # | LICENCIA # | # PERMIS
 76 | ENTER LICENSE # | ENTER LICENÇA # | INGRESE LICENCIA # | ENTREZ # PERMIS
 77 | DATA | INFO | INFO | INFO
 78 | ENTER DATA | ENTER INFO | INGRESE INFO | ENTREZ INFO
 79 | CUSTOMER DATA | CLIENTE INFO | INFO CLIENTE | INFO CLIENT
 80 | ID # | ID # | ID # | # ID
 81 | ENTER ID # | ENTER ID # | INGRESE ID # | ENTREZ # ID
 82 | USER ID | USER ID | ID USUARIO | ID UTILISATEUR
 83 | ROUTE # | ROUTE # | RUTA # | # ROUTE
 84 | ENTER ROUTE # | ADD ROUTE # | INGRESE RUTA # | ENTREZ # ROUTE
 85 | ENTER CARD NUM | ENTER NÚMERO DE CARTÃO | INGRESE NUM TARJETA | ENTREZ NO CARTE
 86 | EXP DATE(YYMM) | VALIDADE VAL (AAMM) | FECHA EXP (AAMM) | DATE EXPIR(AAMM)
 87 | PHONE NUMBER | TELEFONE | NUMERO TELEFONO | NO TEL
 88 | CVV START DATE | CVV DATA DE INÍCIO | CVV FECHA INICIO | CVV DATE DE DEBUT
 89 | ISSUE NUMBER | NÚMERO DE EMISSÃO | NUMERO DE EMISION | NO DEMISSION
 90 | START DATE (MMYY) | DATA DE INÍCIO (AAMM) | FECHA INICIO (AAMM) | DATE DE DEBUT-AAMM
 */
-(RETURN_CODE) getNumeric:(bool)maskInput minLength:(int)minLength maxLength:(int)maxLength messageID:(int)mID language:(LANGUAGE_TYPE)lang;


/**
 * Display Message and Get Numeric Key(s)
 - UniPay II
 
 *
 Decrypt and display message on LCD. Requires secure message data. Returns value in inputValue of deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_NUMERIC
 
 @param maskInput If true, all entered data will be masked with asterik (*)
 @param mID Message ID from approved message list.
 @param lang Language file to use for message
 @code
 typedef enum{
 LANGUAGE_TYPE_ENGLISH,
 LANGUAGE_TYPE_PORTUGUESE,
 LANGUAGE_TYPE_SPANISH,
 LANGUAGE_TYPE_FRENCH
 }LANGUAGE_TYPE;
 @endcode
 
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
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - If there is no any enter in 20 seconds, the entered numeric key will be cleared.
 - When press Enter key , it will end this Command and response package with NGA  format .
 - When press Cancel key, the entered numeric key will be cleared and if press Cancel key again, this command terminated.
 - Cancel Command can terminate this command.
 
 \par Secure Messages
 Secure messages to be used with General Prompts commands
 
 Msg Id |English Prompt | Portuguese Prompt | Spanish Prompt | French Prompt
 ---------- | ---------- | ----------  | ---------- | ----------
 1 | ENTER | ENTER | INGRESE | ENTREZ
 2 | REENTER | RE-INTRODUZIR | REINGRESE | RE-ENTREZ
 3 | ENTER YOUR | INTRODUZIR O SEU | INGRESE SU | ENTREZ VOTRE
 4 | REENTER YOUR | RE-INTRODUZIR O SEU | REINGRESE SU | RE-ENTREZ VOTRE
 5 | PLEASE ENTER | POR FAVOR DIGITE | POR FAVOR INGRESE | SVP ENTREZ
 6 | PLEASE REENTER | POR FAVO REENTRAR | POR FAVO REINGRESE | SVP RE-ENTREZ
 7 | PO NUMBER | NÚMERO PO | NUMERO PO | No COMMANDE
 8 | DRIVER ID | LICENÇA | LICENCIA | ID CONDUCTEUR
 9 | ODOMETER | ODOMETER | ODOMETRO | ODOMETRE
 10 | ID NUMBER | NÚMERO ID | NUMERO ID | No IDENT
 11 | EQUIP CODE | EQUIP CODE | CODIGO EQUIP | CODE EQUIPEMENT
 12 | DRIVERS ID | DRIVER ID | ID CONDUCTOR | ID CONDUCTEUR
 13 | JOB NUMBER | EMP NÚMERO | NUMERO EMP | No TRAVAIL
 14 | WORK ORDER | TRABALHO ORDEM | ORDEN TRABAJO | FICHE TRAVAIL
 15 | VEHICLE ID | ID VEÍCULO | ID VEHICULO | ID VEHICULE
 16 | ENTER DRIVER | ENTER DRIVER | INGRESE CONDUCTOR | ENTR CONDUCTEUR
 17 | ENTER DEPT | ENTER DEPT | INGRESE DEPT | ENTR DEPARTEMNT
 18 | ENTER PHONE | ADICIONAR PHONE | INGRESE TELEFONO | ENTR No TELEPH
 19 | ENTER ROUTE | ROUTE ADD | INGRESE RUTA | ENTREZ ROUTE
 20 | ENTER FLEET | ENTER FROTA | INGRESE FLOTA | ENTREZ PARC AUTO
 21 | ENTER JOB ID | ENTER JOB ID | INGRESE ID TRABAJO | ENTR ID TRAVAIL
 22 | ROUTE NUMBER | NÚMERO PATH | RUTA NUMERO | No ROUTE
 23 | ENTER USER ID | ENTER USER ID | INGRESE ID USUARIO | ID UTILISATEUR
 24 | FLEET NUMBER | NÚMERO DE FROTA | FLOTA NUMERO | No PARC AUTO
 25 | ENTER PRODUCT | ADICIONAR PRODUTO | INGRESE PRODUCTO | ENTREZ PRODUIT
 26 | DRIVER NUMBER | NÚMERO DRIVER | CONDUCTOR NUMERO | No CONDUCTEUR
 27 | ENTER LICENSE | ENTER LICENÇA | INGRESE LICENCIA | ENTREZ PERMIS
 28 | ENTER FLEET NO | ENTER NRO FROTA | INGRESE NRO FLOTA | ENT No PARC AUTO
 29 | ENTER CAR WASH | WASH ENTER | INGRESE LAVADO | ENTREZ LAVE-AUTO
 30 | ENTER VEHICLE | ENTER VEÍCULO | INGRESE VEHICULO | ENTREZ VEHICULE
 31 | ENTER TRAILER | TRAILER ENTER | INGRESE TRAILER | ENTREZ REMORQUE
 32 | ENTER ODOMETER | ENTER ODOMETER | INGRESE ODOMETRO | ENTREZ ODOMETRE
 33 | DRIVER LICENSE | CARTEIRA DE MOTORISTA | LICENCIA CONDUCTOR | PERMIS CONDUIRE
 34 | ENTER CUSTOMER | ENTER CLIENTE | INGRESE CLIENTE | ENTREZ CLIENT
 35 | VEHICLE NUMBER | NÚMERO DO VEÍCULO | VEHICULO NUMERO | No VEHICULE
 36 | ENTER CUST DATA | ENTER CLIENTE INFO | INGRESE INFO CLIENTE | INFO CLIENT
 37 | REENTER DRIVID | REENTRAR DRIVER ID | REINGRESE ID CHOFER | RE-ENTR ID COND
 38 | ENTER USER DATA | ENTER INFO USUÁRIO | INGRESE INFO USUARIO | INFO UTILISATEUR
 39 | ENTER CUST CODE | ENTER CODE. CLIENTE | INGRESE COD. CLIENTE | ENTR CODE CLIENT
 40 | ENTER EMPLOYEE | ENTER FUNCIONÁRIO | INGRESE EMPLEADO | ENTREZ EMPLOYE
 41 | ENTER ID NUMBER | ENTER NÚMERO ID | INGRESE NUMERO ID | ENTREZ No ID
 42 | ENTER DRIVER ID | ENTER ID DRIVER | INGRESE ID CONDUCTOR | No CONDUCTEUR
 43 | ENTER FLEET PIN | ENTER PIN FROTA | INGRESE PIN DE FLOTA | NIP PARC AUTO
 44 | ODOMETER NUMBER | NÚMERO ODOMETER | ODOMETRO NUMERO | No ODOMETRE
 45 | ENTER DRIVER LIC | ENTER DRIVER LIC | INGRESE LIC CONDUCTOR | PERMIS CONDUIRE
 46 | ENTER TRAILER NO | NRO TRAILER ENTER | INGRESE NRO TRAILER | ENT No REMORQUE
 47 | REENTER VEHICLE | REENTRAR VEÍCULO | REINGRESE VEHICULO | RE-ENTR VEHICULE
 48 | ENTER VEHICLE ID | ENTER VEÍCULO ID | INGRESE ID VEHICULO | ENTR ID VEHICULE
 49 | ENTER BIRTH DATE | INSERIR DATA NAC | INGRESE FECHA NAC | ENT DT NAISSANCE
 50 | ENTER DOB MMDDYY | ENTER FDN MMDDYY | INGRESE FDN MMDDAA | NAISSANCE MMJJAA
 51 | ENTER FLEET DATA | ENTER FROTA INFO | INGRESE INFO DE FLOTA | INFO PARC AUTO
 52 | ENTER REFERENCE | ENTER REFERÊNCIA | INGRESE REFERENCIA | ENTREZ REFERENCE
 53 | ENTER AUTH NUMBR | ENTER NÚMERO AUT | INGRESE NUMERO AUT | No AUTORISATION
 54 | ENTER HUB NUMBER | ENTER HUB NRO | INGRESE NRO HUB | ENTREZ No NOYAU
 55 | ENTER HUBOMETER | MEDIDA PARA ENTRAR HUB | INGRESE MEDIDO DE HUB | COMPTEUR NOYAU
 56 | ENTER TRAILER ID | TRAILER ENTER ID | INGRESE ID TRAILER | ENT ID REMORQUE
 57 | ODOMETER READING | QUILOMETRAGEM | LECTURA ODOMETRO | LECTURE ODOMETRE
 58 | REENTER ODOMETER | REENTRAR ODOMETER | REINGRESE ODOMETRO | RE-ENT ODOMETRE
 59 | REENTER DRIV. ID | REENTRAR DRIVER ID | REINGRESE ID CHOFER | RE-ENT ID CONDUC
 60 | ENTER CUSTOMER ID | ENTER CLIENTE ID | INGRESE ID CLIENTE | ENTREZ ID CLIENT
 61 | ENTER CUST. ID | ENTER CLIENTE ID | INGRESE ID CLIENTE | ENTREZ ID CLIENT
 62 | ENTER ROUTE NUM | ENTER NUM ROUTE | INGRESE NUM RUTA | ENT No ROUTE
 63 | ENTER FLEET NUM | FROTA ENTER NUM | INGRESE NUM FLOTA | ENT No PARC AUTO
 64 | FLEET PIN | FROTA PIN | PIN DE FLOTA | NIP PARC AUTO
 65 | DRIVER # | DRIVER # | CONDUCTOR # | CONDUCTEUR
 66 | ENTER DRIVER # | ENTER DRIVER # | INGRESE CONDUCTOR # | ENT # CONDUCTEUR
 67 | VEHICLE # | VEÍCULO # | VEHICULO # | # VEHICULE
 68 | ENTER VEHICLE # | ENTER VEÍCULO # | INGRESE VEHICULO # | ENT # VEHICULE
 69 | JOB # | TRABALHO # | TRABAJO # | # TRAVAIL
 70 | ENTER JOB # | ENTER JOB # | INGRESE TRABAJO # | ENTREZ # TRAVAIL
 71 | DEPT NUMBER | NÚMERO DEPT | NUMERO DEPTO | No DEPARTEMENT
 72 | DEPARTMENT # | DEPARTAMENTO # | DEPARTAMENTO # | DEPARTEMENT
 73 | ENTER DEPT # | ENTER DEPT # | INGRESE DEPTO # | ENT# DEPARTEMENT
 74 | LICENSE NUMBER | NÚMERO DE LICENÇA | NUMERO LICENCIA | No PERMIS
 75 | LICENSE # | LICENÇA # | LICENCIA # | # PERMIS
 76 | ENTER LICENSE # | ENTER LICENÇA # | INGRESE LICENCIA # | ENTREZ # PERMIS
 77 | DATA | INFO | INFO | INFO
 78 | ENTER DATA | ENTER INFO | INGRESE INFO | ENTREZ INFO
 79 | CUSTOMER DATA | CLIENTE INFO | INFO CLIENTE | INFO CLIENT
 80 | ID # | ID # | ID # | # ID
 81 | ENTER ID # | ENTER ID # | INGRESE ID # | ENTREZ # ID
 82 | USER ID | USER ID | ID USUARIO | ID UTILISATEUR
 83 | ROUTE # | ROUTE # | RUTA # | # ROUTE
 84 | ENTER ROUTE # | ADD ROUTE # | INGRESE RUTA # | ENTREZ # ROUTE
 85 | ENTER CARD NUM | ENTER NÚMERO DE CARTÃO | INGRESE NUM TARJETA | ENTREZ NO CARTE
 86 | EXP DATE(YYMM) | VALIDADE VAL (AAMM) | FECHA EXP (AAMM) | DATE EXPIR(AAMM)
 87 | PHONE NUMBER | TELEFONE | NUMERO TELEFONO | NO TEL
 88 | CVV START DATE | CVV DATA DE INÍCIO | CVV FECHA INICIO | CVV DATE DE DEBUT
 89 | ISSUE NUMBER | NÚMERO DE EMISSÃO | NUMERO DE EMISION | NO DEMISSION
 90 | START DATE (MMYY) | DATA DE INÍCIO (AAMM) | FECHA INICIO (AAMM) | DATE DE DEBUT-AAMM
 */
-(RETURN_CODE) getNumericUniPayII:(bool)maskInput messageID:(int)mID language:(LANGUAGE_TYPE)lang;

/**
 * Display Message and Get Amount
 - BTPay 200

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
 LANGUAGE_TYPE_SPANISH,
 LANGUAGE_TYPE_FRENCH
 }LANGUAGE_TYPE;
 @endcode
 
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
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - If there is no any enter in 20 seconds, the entered namount key will be cleared.
 - When press Enter key , it will end this Command and response package with NGA  format .
 - When press Cancel key, the entered amount key will be cleared and if press Cancel key again, this command terminated.
 - Cancel Command can terminate this command.
 
 \par Secure Messages
 Secure messages to be used with General Prompts commands
 
 Msg Id |English Prompt | Portuguese Prompt | Spanish Prompt | French Prompt
 ---------- | ---------- | ----------  | ---------- | ----------
 1 | ENTER | ENTER | INGRESE | ENTREZ
 2 | REENTER | RE-INTRODUZIR | REINGRESE | RE-ENTREZ
 3 | ENTER YOUR | INTRODUZIR O SEU | INGRESE SU | ENTREZ VOTRE
 4 | REENTER YOUR | RE-INTRODUZIR O SEU | REINGRESE SU | RE-ENTREZ VOTRE
 5 | PLEASE ENTER | POR FAVOR DIGITE | POR FAVOR INGRESE | SVP ENTREZ
 6 | PLEASE REENTER | POR FAVO REENTRAR | POR FAVO REINGRESE | SVP RE-ENTREZ
 7 | PO NUMBER | NÚMERO PO | NUMERO PO | No COMMANDE
 8 | DRIVER ID | LICENÇA | LICENCIA | ID CONDUCTEUR
 9 | ODOMETER | ODOMETER | ODOMETRO | ODOMETRE
 10 | ID NUMBER | NÚMERO ID | NUMERO ID | No IDENT
 11 | EQUIP CODE | EQUIP CODE | CODIGO EQUIP | CODE EQUIPEMENT
 12 | DRIVERS ID | DRIVER ID | ID CONDUCTOR | ID CONDUCTEUR
 13 | JOB NUMBER | EMP NÚMERO | NUMERO EMP | No TRAVAIL
 14 | WORK ORDER | TRABALHO ORDEM | ORDEN TRABAJO | FICHE TRAVAIL
 15 | VEHICLE ID | ID VEÍCULO | ID VEHICULO | ID VEHICULE
 16 | ENTER DRIVER | ENTER DRIVER | INGRESE CONDUCTOR | ENTR CONDUCTEUR
 17 | ENTER DEPT | ENTER DEPT | INGRESE DEPT | ENTR DEPARTEMNT
 18 | ENTER PHONE | ADICIONAR PHONE | INGRESE TELEFONO | ENTR No TELEPH
 19 | ENTER ROUTE | ROUTE ADD | INGRESE RUTA | ENTREZ ROUTE
 20 | ENTER FLEET | ENTER FROTA | INGRESE FLOTA | ENTREZ PARC AUTO
 21 | ENTER JOB ID | ENTER JOB ID | INGRESE ID TRABAJO | ENTR ID TRAVAIL
 22 | ROUTE NUMBER | NÚMERO PATH | RUTA NUMERO | No ROUTE
 23 | ENTER USER ID | ENTER USER ID | INGRESE ID USUARIO | ID UTILISATEUR
 24 | FLEET NUMBER | NÚMERO DE FROTA | FLOTA NUMERO | No PARC AUTO
 25 | ENTER PRODUCT | ADICIONAR PRODUTO | INGRESE PRODUCTO | ENTREZ PRODUIT
 26 | DRIVER NUMBER | NÚMERO DRIVER | CONDUCTOR NUMERO | No CONDUCTEUR
 27 | ENTER LICENSE | ENTER LICENÇA | INGRESE LICENCIA | ENTREZ PERMIS
 28 | ENTER FLEET NO | ENTER NRO FROTA | INGRESE NRO FLOTA | ENT No PARC AUTO
 29 | ENTER CAR WASH | WASH ENTER | INGRESE LAVADO | ENTREZ LAVE-AUTO
 30 | ENTER VEHICLE | ENTER VEÍCULO | INGRESE VEHICULO | ENTREZ VEHICULE
 31 | ENTER TRAILER | TRAILER ENTER | INGRESE TRAILER | ENTREZ REMORQUE
 32 | ENTER ODOMETER | ENTER ODOMETER | INGRESE ODOMETRO | ENTREZ ODOMETRE
 33 | DRIVER LICENSE | CARTEIRA DE MOTORISTA | LICENCIA CONDUCTOR | PERMIS CONDUIRE
 34 | ENTER CUSTOMER | ENTER CLIENTE | INGRESE CLIENTE | ENTREZ CLIENT
 35 | VEHICLE NUMBER | NÚMERO DO VEÍCULO | VEHICULO NUMERO | No VEHICULE
 36 | ENTER CUST DATA | ENTER CLIENTE INFO | INGRESE INFO CLIENTE | INFO CLIENT
 37 | REENTER DRIVID | REENTRAR DRIVER ID | REINGRESE ID CHOFER | RE-ENTR ID COND
 38 | ENTER USER DATA | ENTER INFO USUÁRIO | INGRESE INFO USUARIO | INFO UTILISATEUR
 39 | ENTER CUST CODE | ENTER CODE. CLIENTE | INGRESE COD. CLIENTE | ENTR CODE CLIENT
 40 | ENTER EMPLOYEE | ENTER FUNCIONÁRIO | INGRESE EMPLEADO | ENTREZ EMPLOYE
 41 | ENTER ID NUMBER | ENTER NÚMERO ID | INGRESE NUMERO ID | ENTREZ No ID
 42 | ENTER DRIVER ID | ENTER ID DRIVER | INGRESE ID CONDUCTOR | No CONDUCTEUR
 43 | ENTER FLEET PIN | ENTER PIN FROTA | INGRESE PIN DE FLOTA | NIP PARC AUTO
 44 | ODOMETER NUMBER | NÚMERO ODOMETER | ODOMETRO NUMERO | No ODOMETRE
 45 | ENTER DRIVER LIC | ENTER DRIVER LIC | INGRESE LIC CONDUCTOR | PERMIS CONDUIRE
 46 | ENTER TRAILER NO | NRO TRAILER ENTER | INGRESE NRO TRAILER | ENT No REMORQUE
 47 | REENTER VEHICLE | REENTRAR VEÍCULO | REINGRESE VEHICULO | RE-ENTR VEHICULE
 48 | ENTER VEHICLE ID | ENTER VEÍCULO ID | INGRESE ID VEHICULO | ENTR ID VEHICULE
 49 | ENTER BIRTH DATE | INSERIR DATA NAC | INGRESE FECHA NAC | ENT DT NAISSANCE
 50 | ENTER DOB MMDDYY | ENTER FDN MMDDYY | INGRESE FDN MMDDAA | NAISSANCE MMJJAA
 51 | ENTER FLEET DATA | ENTER FROTA INFO | INGRESE INFO DE FLOTA | INFO PARC AUTO
 52 | ENTER REFERENCE | ENTER REFERÊNCIA | INGRESE REFERENCIA | ENTREZ REFERENCE
 53 | ENTER AUTH NUMBR | ENTER NÚMERO AUT | INGRESE NUMERO AUT | No AUTORISATION
 54 | ENTER HUB NUMBER | ENTER HUB NRO | INGRESE NRO HUB | ENTREZ No NOYAU
 55 | ENTER HUBOMETER | MEDIDA PARA ENTRAR HUB | INGRESE MEDIDO DE HUB | COMPTEUR NOYAU
 56 | ENTER TRAILER ID | TRAILER ENTER ID | INGRESE ID TRAILER | ENT ID REMORQUE
 57 | ODOMETER READING | QUILOMETRAGEM | LECTURA ODOMETRO | LECTURE ODOMETRE
 58 | REENTER ODOMETER | REENTRAR ODOMETER | REINGRESE ODOMETRO | RE-ENT ODOMETRE
 59 | REENTER DRIV. ID | REENTRAR DRIVER ID | REINGRESE ID CHOFER | RE-ENT ID CONDUC
 60 | ENTER CUSTOMER ID | ENTER CLIENTE ID | INGRESE ID CLIENTE | ENTREZ ID CLIENT
 61 | ENTER CUST. ID | ENTER CLIENTE ID | INGRESE ID CLIENTE | ENTREZ ID CLIENT
 62 | ENTER ROUTE NUM | ENTER NUM ROUTE | INGRESE NUM RUTA | ENT No ROUTE
 63 | ENTER FLEET NUM | FROTA ENTER NUM | INGRESE NUM FLOTA | ENT No PARC AUTO
 64 | FLEET PIN | FROTA PIN | PIN DE FLOTA | NIP PARC AUTO
 65 | DRIVER # | DRIVER # | CONDUCTOR # | CONDUCTEUR
 66 | ENTER DRIVER # | ENTER DRIVER # | INGRESE CONDUCTOR # | ENT # CONDUCTEUR
 67 | VEHICLE # | VEÍCULO # | VEHICULO # | # VEHICULE
 68 | ENTER VEHICLE # | ENTER VEÍCULO # | INGRESE VEHICULO # | ENT # VEHICULE
 69 | JOB # | TRABALHO # | TRABAJO # | # TRAVAIL
 70 | ENTER JOB # | ENTER JOB # | INGRESE TRABAJO # | ENTREZ # TRAVAIL
 71 | DEPT NUMBER | NÚMERO DEPT | NUMERO DEPTO | No DEPARTEMENT
 72 | DEPARTMENT # | DEPARTAMENTO # | DEPARTAMENTO # | DEPARTEMENT
 73 | ENTER DEPT # | ENTER DEPT # | INGRESE DEPTO # | ENT# DEPARTEMENT
 74 | LICENSE NUMBER | NÚMERO DE LICENÇA | NUMERO LICENCIA | No PERMIS
 75 | LICENSE # | LICENÇA # | LICENCIA # | # PERMIS
 76 | ENTER LICENSE # | ENTER LICENÇA # | INGRESE LICENCIA # | ENTREZ # PERMIS
 77 | DATA | INFO | INFO | INFO
 78 | ENTER DATA | ENTER INFO | INGRESE INFO | ENTREZ INFO
 79 | CUSTOMER DATA | CLIENTE INFO | INFO CLIENTE | INFO CLIENT
 80 | ID # | ID # | ID # | # ID
 81 | ENTER ID # | ENTER ID # | INGRESE ID # | ENTREZ # ID
 82 | USER ID | USER ID | ID USUARIO | ID UTILISATEUR
 83 | ROUTE # | ROUTE # | RUTA # | # ROUTE
 84 | ENTER ROUTE # | ADD ROUTE # | INGRESE RUTA # | ENTREZ # ROUTE
 85 | ENTER CARD NUM | ENTER NÚMERO DE CARTÃO | INGRESE NUM TARJETA | ENTREZ NO CARTE
 86 | EXP DATE(YYMM) | VALIDADE VAL (AAMM) | FECHA EXP (AAMM) | DATE EXPIR(AAMM)
 87 | PHONE NUMBER | TELEFONE | NUMERO TELEFONO | NO TEL
 88 | CVV START DATE | CVV DATA DE INÍCIO | CVV FECHA INICIO | CVV DATE DE DEBUT
 89 | ISSUE NUMBER | NÚMERO DE EMISSÃO | NUMERO DE EMISION | NO DEMISSION
 90 | START DATE (MMYY) | DATA DE INÍCIO (AAMM) | FECHA INICIO (AAMM) | DATE DE DEBUT-AAMM
 */
-(RETURN_CODE) getAmount:(int)minLength maxLength:(int)maxLength messageID:(int)mID language:(LANGUAGE_TYPE)lang;

/**
 * Upload JPEG to device
 - BTPay 200

 *
 Stores a picture on the device. The picture's dimensions must not exceed the display resolution of 128 x 64. The picture must be RGB JPEG.
 @param picture RGB JPEG image data
 
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
-(RETURN_CODE) uploadJPEG:(NSData*)picture;

/**
 * Show stored picture on the LCD
 - BTPay 200

 *
 Show stored picture on the LCD defined by top left point [X0][Y0] and bottom right point [X1][Y1].  The values of X must be in the range 0-127, and the values of Y must be in the range of 0-63
 @param X0 Upper left X coordinate
 @param Y0 Upper left Y coordinate
 @param X1 Lower left X coordinate
 @param Y1 Lower left Y coordinate
 
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

-(RETURN_CODE) showJPEG:(int)X0 Y0:(int)Y0 X1:(int)X1 Y1:(int)Y1;

/**
 * Is Audio Reader Connected
 - UniPay
 
 *
 Returns value on device connection status when device is an audio-type connected to headphone plug.
 
 @return BOOL True = Connected, False = Disconnected
 
 */

-(BOOL) isAudioReaderConnected;

/**
 * Connect To Audio Reader
 - UniPay
 
 *
 Attemps to recognize and connect to an IDTech MSR device connected via the audio port.
  * @return RETURN_CODE
 */

-(RETURN_CODE) connectToAudioReader;

/**
 * Cancel Connect To Audio Reader
 - UniPay
 
 *
Cancels a connection attempt to an IDTech MSR device connected via the audio port.
  * @return RETURN_CODE
 */

-(RETURN_CODE) cancelConnectToAudioReader;

/**
 * Set Volume To Audio Reader
 - UniPay
 
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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) setAudioVolume:(float)val;

/**
 * Card Insertion Timeout
 - UniPay
 
 *
 During the beginning and end of an EMV transaction, if a card insertion or removal is requested, this method sets the timeout value.  Default is 30 seconds.
 
 @param sec Timeout value in seconds
 
 
 */
-(void) cardInsertTimeout:(int)sec;


/**
 * Get Tag
 - UniPay
 - BTPay
 
 *
 Retrieves an EMV tag from the inserted card.  Only available after the card has been processed after executing IDT_Device::startEMVTransaction:otherAmount:timeout:cashback:additionalTags:()
 
 @param tagName Name fo tag to retrieve
 @param data Pointer that will return location of tag data
 
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
-(RETURN_CODE) getTag:(NSString*)tagName tagData:(NSData**)data;


/**
 * Get All Tags
 - UniPay
 - BTPay
 
 *
 Retrieves all EMV tags from the inserted card.  Only available after the card has been processed after executing IDT_Device::startEMVTransaction:otherAmount:timeout:cashback:additionalTags:()
 
 @param data Pointer that will return location of dictionary with all tag values.  Key is NSString, Object is NSData.
 
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
-(RETURN_CODE) getAllTags:(NSDictionary**)data;

/**
 * Get Mask and Encryption
 - BTPay
 
 *
 Retrieves the MSR Mask and Encryption settings
 
 @param data Pointer that will return location of MaskAndEncryption structure.
 
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
-(RETURN_CODE) getMaskAndEncryption:(MaskAndEncryption**)data;


/**
 * Restore Mask and Encryption default settings
 - BTPay
 
 *
 Restores the default values for MSR Mask and Encryption settings
 
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
-(RETURN_CODE) restoreMaskAndEncryptionDefaults;

/**
 * Set PAN masking character
 - BTPay
 
 *
 Sets the character for PAN masking
 
 @param maskChar Masking character.  Default value '*';
 
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
-(RETURN_CODE) setPANMaskingCharacter:(char)maskChar;

/**
 * Set PrePAN Clear Digits
 - BTPay
 
 *
 Sets the number of digits to show in clear text at the beginning of PAN
 
 @param clearDigits Amount of characters to display cleartext at beginning of PAN. Valid range 0-6.  Default value 4.
 
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
-(RETURN_CODE) setPrePANClearDigits:(int)clearDigits;
/**
 * Set PostPAN Clear Digits
 - BTPay
 
 *
 Sets the number of digits to show in clear text at the ending of PAN
 
 @param clearDigits Amount of characters to display cleartext at end of PAN. Valid range 0-4.  Default value 4.
 
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
-(RETURN_CODE) setPostPANClearDigits:(int)clearDigits;


/**
 * Set Expiration Date masking
 - BTPay
 
 *
 Sets the flag to enable Expiratin Date masking
 
 @param mask TRUE = mask expiration date.  FALSE = display expiration date in cleartext
 
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
-(RETURN_CODE) setExpMasking:(BOOL)mask;





/**
 * Set Base Key Type
 - UniPay II
 
 *
 Sets the base key type
 
 @param maskOption Mask Option
 Bit0: T1 mask allowed
 Bit1: T2 mask allowed
 Bit2: T3 mask allowed
 
 
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
-(RETURN_CODE) setMaskingOption:(char)maskOption;

/**
 * Set Encryption Key Type
 - UniPay II
 
 *
 Sets the encryption key type
 
 @param encOption Encryption Option
 Bit 0 : T1 force encrypt
 Bit 1 : T2 force encrypt
 Bit 2 : T3 force encrypt
 Bit 3 : T3 force encrypt when card type is 0
 
 
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
-(RETURN_CODE) setEncryptionOption:(char)encOption;

/**
 * Get Language Type
 - BTPay
 
 *
 Gets the language type
 
 @param response LANGUAGE_TYPE of the BTPay
 ENGLISH: 01
 PORTUGUESE: 02
 SPANISH: 03
 
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
-(RETURN_CODE) getLanguageType:(NSUInteger**)response;
/**
 * Set Language Type
 - BTPay
 
 *
 Sets the language type of BTPay prompts
 
 @param lang LANGUAGE_TYPE
 ENGLISH: 01
 PORTUGUESE: 02
 SPANISH: 03
 
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
-(RETURN_CODE) setLanguageType:(LANGUAGE_TYPE)lang;

/**
 * Set Pre/Post PAN Ctrl Data Length
 - UniPayII
 *
Sets the length of the PAN Pre and Post Ctrl Data
 
 @param pre  Amount of digits Pre Ctrl Data
 @param post  Amount of digits Post Ctrl Data
 
 
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
-(RETURN_CODE) setPrePostPANCtrlData:(Byte)pre post:(Byte)post;

/**
 * Get Pre/Post PAN Ctrl Data Length
 - UniPayII
 *
 Gets the length of the PAN Pre and Post Ctrl Data
 
 @param pre  Return amount of digits Pre Ctrl Data
 @param post  Return amount of digits Post Ctrl Data

 
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
-(RETURN_CODE) getPrePostPANCtrlData:(Byte**)pre post:(Byte**)post;


/**
 * Get Mask Character
 - UniPayII
 *
Gets the PAN Mask Character
 
 @param response  Single character NSString with the mask character.
 
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
-(RETURN_CODE) getASCIIMaskData:(NSString**)mask;

/**
 * Set Mask Character
  - UniPayII
 *
 Sets the PAN Mask Character
 
 @param mask  Single character NSString with the mask character.
 
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
-(RETURN_CODE) setASCIIMaskData:(NSString*)mask;

/**
 * Get ICC Connector
 - UniPayII
 *
 Gets the ICC Connector
 
 @param response ICC Connector
 00: User Card Connector
 01: SAM Connector
 
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
-(RETURN_CODE) getICCConnector:(NSUInteger**)response;

/**
 * Set ICC Connector
 - UniPayII
 *
 Sets the ICC Connector
 
 @param connector ICC Connector
 00: user Car Connector (main module)
 01: SAM Connector
 
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
-(RETURN_CODE) setICCConnector:(int)connector;

/**
 * Get BCD Mask Data
 - UniPayII
 *
 Gets the BCD Mask Data.  Valid values are 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
 
 @param response Mask Value
 
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
-(RETURN_CODE) getBCDMaskData:(NSUInteger**)response;

/**
 * Set BCD Mask Data
 - UniPayII
 *
 Sets the BCD Mask Data.  Valid values are 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
 
 @param mask  Mask value, valid values are 10 - 15  (0x0A - 0x0F)
 
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
-(RETURN_CODE) setBCDMaskData:(int)mask;


/**
 * Set Default ICC Group Settings
 - UniPayII
 *
 Restores ICC Group Settings to defaults;
 
Function Name | Default Value
  -------- | --------
ICC Connector | User Card Connector (main module)
PrePANCtlDataLen | 4
PostPANCtlDataLen | 4
AsciiMaskData | 0x2A
BCDMaskData | 0x0C
 

 
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
-(RETURN_CODE) setDefaultICCGroupSettings;

/**
 * Get Account DUKPT Key Variant
 - UniPayII
 
 *
 Gets the Key Variant for DUKPT
 
 @param response Key Variant
 DUKPT Data Key: 00
 DUKPT PIN Key: 01
 
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
-(RETURN_CODE) getDUKPTKeyVariant:(NSUInteger**)response;
/**
 * Set Account DUKPT Key Variant
 - UniPayII
 
 *
 Sets the response Key Variant
 
 @param type Key Variant
 DUKPT Data Key: 00
 DUKPT PIN Key: 01
 
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
-(RETURN_CODE) setDUKPTKeyVariant:(int)type;

/**
 * Get Account DUKPT Key Variant
 - UniPayII
 
 *
 Gets the DUKPT Key encryption and decryption modes
 
 @param response Current Encryption/Decryption Mode
 TDES: 00
 AES: 01
 
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
-(RETURN_CODE) getDUKPTKeyEncryption:(NSUInteger**)response;
/**
 * Get Account DUKPT Key Encryption/Decryption mode
 - UniPayII
 
 *
 Sets the DUKPT Key encryption and decryption modes
 
 @param type Encryption/Decryption Mode
 TDES: 00
 AES: 01
 
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
-(RETURN_CODE) setDUKPTKeyEncryption:(int)type;

/**
 * Get Function Key
 - UniPayII
 *
@param response Returns the key pressed mapped to ASCII according to the following table:
 - "C": Cancel Key
 - "B": Backspace Key
 - "E": Enter Key
 - "F1": F1 Key
 - "F2": F2 Key
 
 
 
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
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - Cancel Command can terminate this command.
 
 */
-(RETURN_CODE) getFunctionKeyUniPay:(NSString**)response;

/**
 * Get Function Key
 - BTPay 200
 *
 Returns function key value of pressed key in deviceDelegate::pinpadData:keySN:event:() with event EVENT_PINPAD_FUNCTION_KEY.  Value passed as NSData in pinpadData with one of the following values
  - 0x43: Cancel Key
  - 0x42: Backspace Key
  - 0x45: Enter Key
  - 0x23: # Key
  - 0x2A: * Key
 
 
 
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
 
 \par Notes
 - If there is no any enter in 3 minutes, this command will time out.
 - Cancel Command can terminate this command.
 
 */
-(RETURN_CODE) getFunctionKey;

/**
 Is Device Connected
 
Returns the connection status of the requested device
 
 @param device Check connectivity of device type
 */
-(bool) isConnected:(IDT_DEVICE_Types)device;

/**
 Is Any Device Connected
 
 Returns if and device is connected
 
 */
-(bool) anyConnected;

/**
 * Set ICC Notifications
 *
 Determins if card insert/remove events are captured and sent to delegate UniPay_EventFunctionICC

 
 @param turnON  TRUE = monitor ICC card events, FALSE = ignore ICC card events.  Default value is FALSE/OFF.
 
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
-(RETURN_CODE) setICCNotification:(BOOL)turnON;


/**
 * Set Minimum Wait Time
 *
 Sets the minimum wait time when waiting for a response.  Used by system when sequence of commands are being received out of sync.
 
 
 @param time  Time, in seconds, to wait for response.
 
 
 */

-(void) minWaitTime:(float)time;

/**
 * Get Current Device
 *
 returns the current device that is communicating with this class
 
 
 @param type IDT_DEVICE_Types.
 
 
 */
-(IDT_DEVICE_Types) getCurrentDevice;

/**
 * Polls device for next KSN on UniMag
 
 *
 * @param response  Returns next KSN on UniMag
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) getNextKSN:(NSData**)response;


/**
 * Send UniMag Command
 
 *
 * @param command  A command to execute from UNIMAG_COMMAND_Types enumeration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) sendUniMagCommand:(UNIMAG_COMMAND_Types)command;

/**
 * Send UniMag Command
 
 *
 * @param command  A command to execute
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) sendUniMagCmd:(NSString*)command;


/**
 * Send NEO IDG Command
 Send a NEO IDG ViVOtech 2.0 command
 *
 * @param command  One byte command as per NEO IDG Reference Guide
 * @param subCommand  One byte sub-command as per NEO IDG Reference Guide
 * @param data  Command data (if applicable)
* @param response  Returns next Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) sendIDGCommand:(unsigned char)command subCommand:(unsigned char)subCommand data:(NSData*)data response:(NSData**)response;

/**
 * Send NEO IDG Command
 Send a NEO IDG ViVOtech 3.0 command
 *
 * @param command  One byte command as per NEO IDG Reference Guide
 * @param subCommand  One byte sub-command as per NEO IDG Reference Guide
 * @param data  Command data (if applicable)
 * @param response  Returns next Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) sendIDGCommandV3:(unsigned char)command subCommand:(unsigned char)subCommand data:(NSData*)data response:(NSData**)response;

/**
 * Device Is Connected
 *
 @return returns value if any IDT Device is connected.
 
 
 
 */

-(BOOL) deviceIsConnected;

/**
 * UniPay Extended Firmware Version
 *
 @return returns value of UniPay Exteneded Firmware. Internal use only.
 

 */
-(NSString*) getFirmwareVersionExtendedUnipay;


/**
 * Get Multi APDU
 *
 @return returns value of UniPay Multi APDU request. Internal use only.
 
 
 */
-(NSArray*) getMultiAPDU;

/**
 * Disable Auto Authenticate Transaction
 *
 If auto authenticate is DISABLED, authenticateTransaction must be called after a successful startTransaction execution.
 
 @param disable  FALSE = auto authenticate ENABLED, TRUE = auto authenticate DISABLED
 
 */
-(void) disableAutoAuthenticateTransaction:(BOOL)disable;

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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) authenticateTransaction:(NSData*)tags;

/**
 * Get Level and Baud
  *
 @param response  The Baud Rate and Audio Level.
 
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
-(RETURN_CODE) getLevelAndBaud:(NSString**)response;


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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) callbackResponseLCD:(int)mode selection:(unsigned char) selection;

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
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) callbackResponsePIN:(EMV_PIN_MODE_Types)mode KSN:(NSData*)KSN PIN:(NSData*)PIN;

/**
 Sets the terminal major configuration in ICS .
 @param configuration A configuration value, range 1-5
 - 1 = 1C
 - 2 = 2C
 - 3 = 3C
 - 4 = 4C
 - 5 = 5C
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) setTerminalMajorConfiguration:(int)configuration;

/**
 Gets the terminal major configuration in ICS .
 @param configuration A configuration value, range 1-5
 - 1 = 1C
 - 2 = 2C
 - 3 = 3C
 - 4 = 4C
 - 5 = 5C
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) getTerminalMajorConfiguration:(NSUInteger**)configuration;

/**
 * Assign Delegate 2
 *
 Assigns a second delegate. Internal use only.
 
 @param del  IDT_Device_Delegate delegate.
 

 
 */

-(void) assignDelegate2:(id<IDT_Device_Delegate>)del;

/**
 * Set Bypass Delegate
 *
 Sets the bypass delegate. .
 
 @param del  IDT_Device_Delegate delegate.
 

 */

-(void) assignBypassDelegate:(id<IDT_Device_Delegate>)del;

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
-(RETURN_CODE) exchangeMultiAPDU:(NSArray*)dataAPDU response:(NSData**)response;

/**
 * Get Resource Bundle Version
 *
 Retrieves the Resource Version
 
 @return Version - the resource version
 
  */
+(NSString*) getResourceBundleVersion;

/**
 * Attempt connection
 *
 Reserved for system use
  */
-(void) attemptConnect;

/**
 * Loop for card insertion/removal
 *
 Reserved for system use
 */
-(void) loopCardCheck;

/**
 * Sends ping command to VivoTech reader
 *
 Reserved for system use
 */
-(RETURN_CODE) pingVivoTech;

/**
 * VP3300 Command
 *
 Reserved for system use
 */
-(NSData*) grsiP2Command:(unsigned char)command statusCode:(unsigned char)status data:(NSData*)d1;

/**
 * Update NEO 2 Firmware
 *
 Reserved for system use
 */
-(RETURN_CODE) updateFirmwareNeo2:(FIRMWARE_TYPE)type data:(NSData*)firmwareData;

/**
 * VP3300 Command
 *
 Reserved for system use
 */
-(NSData*) grsiP3Command:(unsigned char)command statusCode:(unsigned char)status data:(NSData*)d1;

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
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) setPassThrough:(BOOL)enablePassThrough;





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

 @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
*/

-(RETURN_CODE) pollForToken:(Byte)seconds card:(Byte**)card serialNumber:(NSData**)serialNumber;

/**
* Set Special Function or Feature Configuration Command
*
 The Set Special Function or Feature Configuration command sets a specific special configuration.

 @param feature  Function/Feature ID
 @param addRequirement  Additional Requirement

 @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
*/
-(RETURN_CODE) device_setSpecialFunctionOrFeature:(NSData*)feature addRequirement:(NSData*)addRequirement;


/**
* Get Special Function or Feature Configuration Command
*
 The Get Special Function or Feature Configuration command returns the specific special configuration.

 @param feature  Function/Feature ID
 @param addRequirement  Additional Requirement

 @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
*/
-(RETURN_CODE) device_getSpecialFunctionOrFeature:(NSData**)feature addRequirement:(NSData**)addRequirement;

/**
 * Buzzer OnOff
 *
 
 Causes the buzzer to beep once
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_buzzerOnOff;

/**
 * Log Clear
 *
 
 Instructs the device to delete all log data
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_logClear;
/**
 * Log Read
 *
 Instructs the device to output all log data
 
 @param response Full response received from NEO2 device, including log
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_logRead:(NSData**)response;
/**
 * Enable Log
 * 
 Instructs the device to enable/disable the log
 
 @param enable True = enable log, False = disable log
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_logEnable:(BOOL)enable;

/**
* Antenna Control
*
 The Antenna Control command turns the RF Antenna ON or OFF.

 @param turnON  TRUE = ON, FALSE = OFF

 @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
*/
-(RETURN_CODE) antennaControl:(bool)turnON;

/**
* Exchange Contactless Data
*
 The Echange Contactless Data command allows the host device to send, via the ViVOpay reader,
 application-level APDUs to a PICC that supports ISO 14443-4 Protocol. The reader sends the PICC
 response back to the host

 @param sendData  APDU Out
 @param receiveData  APDU response

 @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
*/
-(RETURN_CODE) exchangeContactlessData:(NSData*)sendData  receiveData:(NSData**)receiveData;

/**
 * Multi App Selection
 *
 Reserved for system use
 */
-(NSMutableArray*) multiAPP:(NSArray*)AIDS;

/**
 * Multi File Selection
 *
 Reserved for system use
 */
-(NSMutableArray*) multiFile:(NSArray*)apduArray;


/**
 * Get Encrypted UniPay Tags
 *
 Reserved for system use
 */
-(NSDictionary*) getEncryptedUniPayTags;

/**
 * Get Masked UniPay Tags
 *
 Reserved for system use
 */
-(NSDictionary*) getMaskedUniPayTags;

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
-(RETURN_CODE) startRKI;

/**
 * Set Encrypted Unipay
 *
 Informs SDK of UniPay encryption capabilities to EMV kernel can format/interpret correct APDU packets
 
 @param isEncrypted  TRUE = encrypted UniPay, FALSE = unencrypted Unipay.  Default is TRUE
 */
-(void) setEncryptedUniPay:(BOOL)isEncrypted;

/**
 * Send 2nd Gen Command
 *
 Informs SDK to format command for EMV Gen2 Device Communication
 
 @param tlv  TLV Command
 @param response  TLV Response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()

 */
-(RETURN_CODE) device_sendGen2Cmd:(NSData*)tlv response:(NSData**)response;






/**
 * Multi App Selection
 *
 Reserved for system use
 */
+(IDTEMVData*) checkCID82:(IDTEMVData*)card;

/**
 * Cancel multi-APDU
 *
 Reserved for system use
 */
+(void) cancelM_APDU;


/**
 * Get All Configuration Groups
 *
 Retrieves all Configuration Groups.
 
 @param response Groups returned as Data stream
 
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
 * Get Configuration Group
 *
 Retrieves the Configuration for the specified Group. Group 0 = terminal settings.
 
 @param group Configuration Group
 @param response Group TLV returned as a dictionary
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 
 */
-(RETURN_CODE) ctls_getConfigurationGroup:(int)group response:(NSDictionary**)response;

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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 
 */

-(RETURN_CODE) ctls_updateBalance:(NSData*)statusCode authCode:(NSData*)authCode date:(NSData*)date time:(NSData*)time;

/**
 * Remove All Certificate Authority Public Key
 *
 Removes all the CAPK for CTLS
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_removeAllCAPK;

/**
 * Remove Application Data by AID
 *
 Removes the Application Data for CTLS as specified by the AID name passed as a parameter
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010") Must be between 5 and 16 bytes
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_removeApplicationData:(NSString*)AID;

/**
 * Remove Certificate Authority Public Key
 *
 Removes the CAPK for CTLS as specified by the RID/Index
 
 @param capk 6 byte CAPK =  5 bytes RID + 1 byte INDEX
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeCAPK:(NSData*)capk;

/**
 * Remove Configuration Group
 *
 Removes the Configuration as specified by the Group.  Must not by group 0
 
 @param group Configuration Group
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeConfigurationGroup:(int)group;


/**
 * Retrieve AID list
 *
 Returns all the AID names installed on the terminal for CTLS.
 
 @param response  array of AID name as NSData*
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_retrieveAIDList:(NSArray**)response;

/**
 * Retrieve Application Data by AID
 *
 Retrieves the CTLS Application Data as specified by the AID name passed as a parameter.
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010"). Must be between 5 and 16 bytes
 @param response  The TLV elements of the requested AID
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveApplicationData:(NSString*)AID response:(NSDictionary**)response;
/**
 * Retrieve Application Data by AID
 * 
 Retrieves the CTLS Application Data for system default group 0 as specified by the AID name passed as a parameter
 
 @param AID Name of the Application ID in bytes. Must be between 5 and 16 bytes
 @param tlv The TLV elements of the requested AID in bytes
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPK:(NSData*)capk key:(NSData**)key;


/**
 * Retrieve the Certificate Authority Public Key list
 *
 Returns all the CAPK RID and Index installed on the terminal for CTLS.
 
 @param keys NSArray of NSData CAPK name  (RID + Index)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPKList:(NSArray**)keys;

/**
 * Retrieve Terminal Data
 *
 Retrieves the Terminal Data for CTLS. This is configuration group 0 (Tag FFEE - > FFEE0100).  The terminal data
 can also be retrieved by ctls_getConfiguraitonGroup(0).
 
 @param tlv Response returned as a TLV
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 
 */
-(RETURN_CODE)  ctls_retrieveTerminalData:(NSData**)tlv;

/**
 * Set Application Data by AID
 *
 Sets the Application Data for CTLS as specified by TLV data
 
 @param tlv  Application data in TLV format
 The first tag of the TLV data must be the group number (FFE4).
 The second tag of the TLV data must be the AID (9F06)
 
 Example valid TLV, for Group #2, AID a0000000035010:
 "ffe401029f0607a0000000051010ffe10101ffe50110ffe30114ffe20106"
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_setCAPK:(NSData*)key;


/**
 * Set Configuration Group
 *
 Sets the Configuration Group for CTLS as specified by the TLV data
 
 @param tlv  Configuration Group Data in TLV format
 The first tag of the TLV data must be the group number (FFE4).
 A second tag must exist
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_setConfigurationGroup:(NSData*)tlv;


/**
 * Set Terminal Data
 *
 Sets the Terminal Data for CTLS as specified by the TLV.  The first TLV must be Configuration Group Number (Tag FFE4).  The terminal global data
 is group 0, so the first TLV would be FFE40100.  Other groups can be defined using this method (1 or greater), and those can be
 retrieved with ctls_getConfigurationGroup(), and deleted with ctls_removeConfigurationGroup(int group).  You cannot
 delete group 0.
 
 @param tlv TerminalData configuration data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_setTerminalData:(NSData*)tlv;




/**
 * Start a CTLS Transaction Request
 *
 Authorizes the CTLS transaction for an CTLS card
 
 The tags will be returned in the callback routine.
 
 @param amount Transaction amount value  (tag value 9F02)
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param tags Any other tags to be included in the request.
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
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
 * Set Default Configuration Group
 * 
 Resets the device to default CTLS configuration group settins
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) ctls_setDefaultConfiguration;
/**
 * Display Online Authorization Result Extended
 * 
 Use this command to display the status of an online authorization request on the reader's display (OK, NOT OK, or ARC). Use this command after the reader sends an online request to the issuer
 
 @param statusCode 0 = NOT OK, 1 = OK, 2 = ARC
 @param TLV Optional TLV data for AOSA, can be empty
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) ctls_displayOnlineAuthResult_ext:(Byte)statusCode TLV:(NSData*)TLV;
/**
 * Display Online Authorization Result
 * 
 Use this command to display the status of an online authorization request on the reader's display (OK or NOT OK). Use this command after the reader sends an online request t the issuer
 @param isOK True = OK, False = NOT OK
 @param TLV Optional TLV for the AOSA, can be empty
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
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
 Cancels Transaction request (EMV).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) emv_cancelTransaction;

/**
 * Set Merchant Record
 *
 Sets the merchant record for ApplePay VAS
 
 @param index Merchant Record Index, valid values: 1-6
 @param enabled Merchant Enabled / valid flag
 @param merchantID Merchant unique identifier registered with Apple. Example: ID TECH
 @param merchantURL Merchant URL, when applicable Example: com.idtechproducts.applePay
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_setMerchantRecord:(int)index enabled:(BOOL)enabled merchantID:(NSString*)merchantID merchantURL:(NSString*)merchantURL;

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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCAPK:(NSData*)key;

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
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveCAPK:(NSData*)capk key:(NSData**)key;
/**
 * Retrieve the Certificate Authority Public Key list
 * 
 Returns all the CAPK RID and Index installed on the terminal for EMV Kernel
 
 @param keys The list of keys with 6 bytes each
        - 5 bytes RID + 1 byte index
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveCAPKList:(NSData**)keys;
/**
 * Remove All Certificates Revocation List Entries
 * 
 Removes all CRL entries
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_removeAllCRL;
/**
 * Set Terminal Data
 * 
 Sets the terminal data
 
 @param tlv Terminal tlv data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_setTerminalData:(NSData*)tlv; //Added
/**
 * Set Terminal Data VP8800
 * 
 Sets the terminal data for the VP8800
 
 @param tlv Terminal tlv data
 @param config Config verification, valid values 1-4
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_setTerminalDataVP8800:(NSData*)tlv config:(int)config;
/**
 * Retrieve the EMV Exception List
 * 
 Returns the EMV Exception entries on the terminal
 
 @param list The list of exception entries ([Exeption1], [Exception2],...[ExceptionN] where each is 12 bytes:
 *      - [1 byte Len][10 bytes PAN][1 byte sequence number]
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveExceptionList:(NSData**)list;
/**
 * Remove All EMV Exceptions
 * 
 Removes all the entries from the EMV Exception List
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_removeAllExceptions;
/**
 * Remove EMV Exception
 * 
 Removes an exception EMV entry containing the PAN and Sequence number where:
 @param exception  [Exception] is 12 bytes:
 *      - [1 byte len][10 bytes PAN][1 byte sequence number]
 *      -PAN, in compressed numeric format, is padded with 0xff if required. For example 0x5413339000001596FFFF)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:() 
 */
-(RETURN_CODE) emv_removeException:(NSData*)exception;
/**
 * Set EMV Exception
 * 
 Adds an entry to the EMV Exception List
 
 @param exception EMV exception entry containing the PAN and sequence number where:
 *      -Exception is 12 bytes
 *      -[1 byte len][10 bytes PAN][1 byte sequence number]
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_setException:(NSData*)exception;
/**
 * Get EMV Configuration Check Value
 * 
 Polls device for the EMV Configuration Check Value
 
 @param response Response returned of the Check Value of the Configuration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVConfigurationCheckValue:(NSString**)response;
/**
 Cancels Transaction request (EMV, swipe or CTLS).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_cancelTransaction;

/**
 * Get Kernel Check Value
 *
 Polls the device for the Kernel Check Value
 
 @param response Response returned of the Check Value of the Kernel
 * @return RETURN_CODE: Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelCheckValue:(NSString**)response;

/**
 * Generate DUKPT IK using KEK
 *
 Use this command to send the encrypted KEK and signature generated by the ETC.
    NEO2 returns the DUKPT IK in TR-31 format encrypted with the KEK and signature
 
 @param cert ETC certificate for signature verification
 @param signature Signature of (KEK || NONCE_ETC) with PKCS1-v1_5 padding
 @param key ASN.1 structure of DUKPT IK used between NEO2 and ETC
 * @return RETURN_CODE: Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_generateDUKPT:(NSData*)cert signature:(NSData*)signature key:(NSData**)key;

/**
 * Battery Voltage
 *
 Polls the device for the current battery voltage
 
 @param response Returns battery voltage string representing millivolts
 * @return RETURN_CODE: Values can be parsed with IDT_Device::getResponseCodeString:()
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
 * Get CRL Status
 * 
 This command returns information about the EMV Certificate Revocation List. The version number, record size, and number of records contained in the file are returned.
 
 @param status 12 bytes returned
    - bytes 0-3: Version number
    - bytes 4-7: Number of records
    - bytes 8-11: Size of record
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_getCRLStatus:(NSData**)status;
/**
 * Retrieve the Certificate Revocation List
 * 
 Returns the CRL entries on the EMV terminal
 
 @param list [CRL1][CRL2]...[CRLn], each CRL 9 bytes where CRL = 5 bytes RID + 1 byte index + 3 bytes serial number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveCRLList:(NSData**)list;
/**
 * Get Transaction Log Status
 * 
 This command returns information about the EMV transaction log. The version number, record size, and number of records contained in the file are returned.
 
 @param status 12 bytes returned
    - bytes 0-3: Version number
    - bytes 4-7: Number of records
    - bytes 8-11: Size of record
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_getTransactionLogStatus:(NSData**)status;

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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_getTransactionLogRecord:(NSData**)record remaining:(int*)remaining;
/**
 * Remove Certificate Authority Public Key
 * 
 Removes the CAPK for EMV Kernel as specified by they RID/Index
 
 @param capk 6 byte CAPK, 5 bytes RID + 1 byte index
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_removeCAPK:(NSData*)capk;
/**
 * Remove All Application Data
 * 
 Removes all the application data for the EMV kernel
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE)emv_removeAllApplicationData;
/**
 * Exchange Certificates, Nonces, and Keys
 * 
 Use this command to send the ETC certificate, nonce, and signature
 The returned data is the NEO2 certificate, nonce, and signature
 
 @param cert Send ETC certificate for signature verification, Receives NEO2 certificate for signature verification
 @param nonce Send ETC random nonce, Receives NEO2 random nonce
 @param signature Send ETC signature, Receives NEO2 signature. Signature of (CertETC_SV || NONCE_ETC) with PKCS1-v1_5 padding
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_exchangeCerts:(NSData**)cert nonce:(NSData**)nonce signature:(NSData**)signature;
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_callbackResponseKSN:(NSData*)KSN;
/**
 * Callback Response MSR Entry
 * 
 Provides MSR information to kernel after a callback was received with type EMV_CALLBACK_MSR
 
 @param MSR Swiped track data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_callbackResponseMSR:(NSData*)MSR;
/**
 * Callback Response LCD Display
 * 
 Provides menu selection responses to the kernel after a acallback was received with DeviceState.EMVCallback and with the following callback types:
 *      - EMV_CALLBACK_TYPE.EMV_CALLBACK_TYPE_LCD
 and the folllowing lcd display modes:
 *      - EMV_LCD_DISPLAY_MODE_MENU, EMV_LCD_DISPLAY_MODE_PROMPT, or EMV_LCD_DISPLAY_MODE_LANGUAGE_SELECT
 
 @param type If Cancel key is pressed during menu selection, then the value is EMV_LCD_DISPLAY_MODE_CANCEL, otherwise it is one of the above display modes
 @param selection If the type is either display mode MENU or LANGUANGE SELECT, provide the selection ID line number, otherwise if the mode is PROMPT, supply either 0x43 for cancel or 0x45 for enter/accept
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_callbackResponseLCD:(EMV_LCD_DISPLAY_MODE)type selection:(Byte)selection;
/**
 * Remove Transaction Amount Log
 * 
 This command can delete transaction amount log in the reader. (When EMV transaction is offline approved, or online, transaction amout log saves to the reader)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_continueTransactionForCV:(int)result pinblock:(NSData*)pinblock;
/**
 * Verify DUKPT IK Loaded on ETC
 * 
 Use this command to verify the DUKPT IK isloaded into the ETC. NEO2 is activated and it can request PIN from ETC after this command
 
 @param KCV ASN.1 structure of KCV
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_verifyDUKPTLoaded:(NSData*)KCV;
/**
 * Get EMV Kernel Version
 * 
 Polls the device for the EMV Kernel Version
 
 @param response The kernel version response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersion:(NSString**)response;
/**
 * Get Extended EMV Kernel Version
 * 
 Polls the device for the extended EMV kernel version
 
 @param response The extended kernel verion response in a string
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_getEMVKernelVersionExt:(NSString**)response;
/**
 * Clear Transaction Log
 * 
 Clears the transaction log on the VP8800
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_clearTransactionLog;
/**
 * Get EMV Exemption Log Status
 * 
 This command returns information about the EMV Exemption log. The version number, record size, and number of records contained in the file are returned
 
 @param status 12 bytes are returned
 *  - bytes 0-3: Version Number
 *  - bytes 4-7: Number of records
 *  - bytes 8-11: Size of record
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_getExemptionLogStatus:(NSData**)status;
/**
 * Get Bootloader Version
 *
 Polls the device for the bootloader version
 *
 * @param response Response return of the bootloader version from the device
 * @return RETURN_CODE: Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getBootloaderVersion:(NSString**)response;

/**
 * Get TransArmor ID
 *
 Gets the TransArmor ID from the device
 *
 * @param TID TransArmor ID
 * @return RETURN_CODE: Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getTransArmorID:(NSString**)TID;

/**
 * Extended Error Condition
 *
 Enables/disables extended error condition for commands 02-40, 61-xx, 62-xx, 83-41 when error is 0xd0a or 0xd0b
 *
 * @param enable TRUE = enable log, FALSE = disable log
 * @return RETURN_CODE: Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_extendedErrorCondition:(BOOL)enable;
/**
 * Set TransArmor ID
 *
 Sets the TransArmor ID
 
 @param TID TransArmor ID
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setTransArmorID:(NSString*)TID;
/**
 * Set TransArmor Encryption
 * 
 Sets the TransArmor encryption from the given certificate
 
 @param cert Certificate in PEM format or DER format. PEM format must be string data (converted to binary) starting with "---". DER format is binary data.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_setTransArmorEncryption:(NSData*)cert;
/**
 * Listen for Notifcations
 *
 Instructs SDK to listen for unsolicited data
 
 @param enable True = Listen, False = Don't listen
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_controlIndicator:(int)indicator enable:(BOOL)enable;
/**
 * Device Certificate Type
 *
 Returns the device certificate type
 
 @param type 0 = Unknown, 1 = Demo, 2 = Production
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_certificateType:(int*)type;
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
 */
-(RETURN_CODE) device_getDRS:(NSData**)codeDRS;
/**
 * Self Check
 *
 If Self-Test function failed, then work into De-activation state
 If device work into De-activation state, all sensitive data will be erased and it needs to be fixed by manufacturer.
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_selfCheck;
/**
 * Delete File
 *
 This command deletes a file or group of files

 @param filename Complete path and file name of the file you want to delete. You do not need to specify the root directory. Indicate subdirectories with a foward slash (/)
 @param isSD True = Delete from SD card, False = Delete from Flash
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
 */
-(RETURN_CODE) device_deleteFile:(NSString*)filename isSD:(BOOL)isSD;
/**
 * Delete Directory
 *
 This command deletes an empty directory
 
 @param filename Complete path of the directory you want to delete.
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
 */
-(RETURN_CODE) device_listDirectory:(NSString*)directoryName recursive:(BOOL)recursive onSD:(BOOL)onSD directory:(NSString**)directory;
/**
 * Create Directory
 *
 This command adds a subdirectory to the indicated path
 
 @param directoryName The directory name. The data for this command is an ASCII string with the complete path and directory name you want to create. You do not need to specify the root directory. Indicate subdirectories with a forward slash (/)
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
 */
-(RETURN_CODE) device_createDirectory:(NSString*)directoryName;
/**
 * Drive Free Space
 *
 This command returns the free and used disk space on the flash drive
 
 @param free Free bytes available on device
 @param used Used bytes on device
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
 */
-(RETURN_CODE) device_setBuzzerLED:(Byte)buzzer led:(Byte)LED ledON:(BOOL)ledON;
/**
 * Enter Low Power Mode
 *
 Puts the terminal in sleep or stop mode, with the option to wake on swipe/tap
 
 @param stopMode True = Stop Mode (POR required), False = Sleep Mode (resume from last instruction)
 @param wakeOnTrans True = Swipe/Tap will wake from low power, False = Will not wake from swipe/tap
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
   
 */
-(RETURN_CODE) device_controlUserInterface:(NSData*)values;
/**
 * Get Cash Transaction Reader Risk Parameters
 *
 Returns the TTQ and reader risk parameters that will be used for cash transactions, if enabled
 
 @param tlv TLV data objects
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
   
 */
-(RETURN_CODE) device_getCashTranRiskPara:(NSData**)tlv;
/**
 * Get DRL Reader Risk Parameters
 *
 Gets the index, application program ID, and reader risk parameters for the DRL settings
 
 @param index DRL index (01-04)
 @param tlv TLV data objects
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
   
 */
-(RETURN_CODE) device_getDrlReaderRiskPara:(Byte)index tlv:(NSData**)tlv;
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
* RRC Disonnect
*
The RRC Disonnect command allows a host to terminate the RRC connection to a reader.


* @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()

*/
-(RETURN_CODE) device_rrcDisconnect;

/**
* RRC Connect
*
The RRC Connect command allows a host to establish an RRC connection to a reader. A host
must first establish an RRC connection to the reader before issuing other RRC IDG commands.


* @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()

*/
-(RETURN_CODE) device_rrcConnect;


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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
   
 */
-(RETURN_CODE) device_getMsrSecurePara:(BOOL)b0 b1:(BOOL)b1 b2:(BOOL)b2 b3:(BOOL)b3 tlv:(NSData**)tlv;
/**
 * Get Module Version Information
 *
 Gets the 16 byte UID of the MCU
 
 @param uid The string representation of the UID
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
   
 */
-(RETURN_CODE) device_getModuleVer:(NSString**)moduleVer;
/**
 * Calibrate reference parameters
 * 
 Calibrates parameters with a given delta value
 
 @param delta Delta value (0x02 is the standard value)
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_calibrateParameters:(Byte)delta;
/**
 * Disable Blue LED Sequence
 * 
 Stop the blue LEDs on the ViVOpay Vendi reader from flashing in the left to right sequence and the LEDs off. Contactless function is also disabled at the same time
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_enaBlueLED:(NSData*)dataCmd;
/**
 * Turn On Yellow LED
 * 
 Turn on the ViVOpay Vendi reader yellow LED. This LED is located below the three blue LEDs
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_onYellowLED;
/**
 * Turn Off Yellow LED
 * 
 Turn off the ViVOpay Vendi reader yellow LED. This LED is located below the three blue LEDs
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_offYellowLED;
/**
 * Enter standby mode
 * 
 Puts unit into low power standby mode
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_enterStandbyMode;
/**
 * Get Light Sensor Value
 * 
 Gets the value from the sensor
 
 @param lightVal Value of the light sensor
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getLightSensorVal:(UInt16*)lightVal;
/**
 * Get RT1050 Firmware Version
 * 
 Gets the version for the RT1050 Firmware
 
 @param response Response returned of the RT1050 firmware
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_getRT1050FirmwareVersion:(NSString**)response;
/**
 * Enable L80 Passthrough
 * 
 Enables Passthrough mode for direct communicaiton to L80 hooked up to NEO2 device
 
 @param enablePassThrough True = passthrough ON, False = passthrough OFF
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_enableL80PassThrough:(BOOL)enablePassThrough;
/**
 * Set QuickChip HID Mode
 * 
 Puts the device into QuickChip KB Output Mode while in HID
 
 @param mode
        - 0 = Disable
        - 2 = CT + MSR + CL
        - 3 = CT + MSR
        - 4 = CL + MSR
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_getProcessorType:(NSData**)type;
/**
 * Get UID of MCU
 * 
 Returns the UID of the device
 
 @param response The module UID information
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_getUIDofMCU:(NSString**)response; //Added-MAR
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_getHardwareInfo:(NSString**)response;
/**
 * Ping Device
 * 
 Pings the reader. If it is connected, returns success, otherwise returns timeout
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_pingDevice;
/**
 * Enable L100 Passthrough
 * 
 Enable passthrough mode for direct communication to L100 hooked up to NEO2 device
 
 @param enablePassThrough True = passthrough ON, False = passthrough OFF
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_enableL100PassThrough:(BOOL)enablePassThrough;
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_getDateTime:(NSData**)dateTime;
/**
 * Set Date Time
 * 
 Sets the date and time of the device to the current date and time of the iOS device
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_setDateTime;
/**
 * Send Payment Application Engine Command
 * 
 Executes a PAE command
 
 @param command ASCII command string, should be started with "*PAE"
 @param response Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_sendPAE:(NSString*)command response:(NSString**)response;

/**
 * Send Raw Command
 *
 Executes a Raw command
 
 @param command Command data
 @param response Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_sendRaw:(NSData*)command response:(NSData**)response timeout:(float)timeout;

/**
 * Read File from SD card
 * 
 Reads a file from the SD card
 
 @param directory The file directory to read from. If empty, the root directory is used
 @param filename The name of the file to retrieve
 @param fileData The contents of the file if it exists
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_readFileFromSD:(NSString*)directory filename:(NSString*)filename fileData:(NSData**)fileData;
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) device_queryFile:(NSString*)directory filename:(NSString*)filename isSD:(BOOL)isSD exists:(BOOL*)exists timestamp:(NSDate**)timestamp fileSize:(int*)fileSize;
/**
 * Get Transaction Results
 * 
 Gets the transaction results when the reader is functioning in "Auto Poll" mode
 
 @param results The transaction results
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::getResponseCodeString:()
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
 * Get Module Bytes
 * 
 Retrieves the first 64 bytes of the module information running in an ADF environment
 
 @param type The ADF type
    - ADF_TYPE_SDK = SDK
    - ADF_TYPE_APP = Application
 @param adfInfo List of modules information
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_getModuleInfo:(ADF_TYPE)type adfInfo:(NSArray**)adfInfo;
/**
 * Erase ADF Flash
 * 
 Erases the ADF flash memory
 
 @param type The ADF type
    - ADF_TYPE_SDK = SDK
    - ADF_TYPE_APP = Application
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_eraseFlash:(ADF_TYPE)type;
/**
 * Set JTAG
 * 
 Enables/Disables the JTAG pin in an ADF environment
 
 @param enable True = JTAG enabled, False = JTAG disabled
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_setJTAG:(BOOL)enable;
/**
 * Set ADF Mode
 * 
 Enables/Disables the ADF environment
 
 @param enable True = ADF enabled, False = ADF disabled
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_setADFMode:(BOOL)enable;
/**
 * Get ADF Mode
 * 
 Retrieves the state of the ADF environment
 
 @param enable True = ADF is enabled, False = ADF is disabled
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) adf_getADFMode:(BOOL*)enable;
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_getMerchantRecord:(int)index record:(NSData**)record;
/**
 * FeliCa Authentication
 *
 Provides a key to be used in a follow up FeliCa Read with MAC (3 blocks max) or Write with MAC (1 block max).
 This command must be executed before each Read w/MAC or Write w/MAC command
 
 NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param key 16 byte key used for MAC generation of Read or Write with MAC
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_authentication:(NSData*)key;

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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getMSRTrack:(int*)val;

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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setMSRTrack:(int)val;
/**
 * Set Swipe Mask Option
 *
 Sets the swipe mask/clear data sending option
 
 @param track1 Mask track 1 allowed
 @param track2 Mask track 2 allowed
 @param track3 Mask track 3 allowed
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setSwipeMaskOption:(BOOL)track1 track2:(BOOL)track2 track3:(BOOL)track3;
/**
 * Set Swipe Force Encryption
 *
 Sets the swipe force encryption options
 
 @param track1 Force encrypt track 1
 @param track2 Force encrypt track 2
 @param track3 Force encrypt track 3
 @param track3card0 Force encrypt track 3 when card type is 0
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setSwipeForcedEncryptionOption:(BOOL)track1 track2:(BOOL)track2 track3:(BOOL)track3 track3card0:(BOOL)track3card0;
/**
 * Set Swipe Data Encryption
 *
 Sets the swipe encryption method
 
 @param encryption 1 = TDES, 2 = AES
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setSwipeEncryption:(Byte)encryption;
/**
 * Set Expiration Masking
 *
 Sets the flag to mask the expiration date
 
 @param mask True = mask expiration
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setExpirationMask:(BOOL)mask;
/**
 * Set Clear PAN Digits
 *
 Sets the amount of digits shown in the clear (not masked) at the beginning of the returned PAN value
 
 @param val Number of digits to show in the clear from 0 - 6 digits
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setClearPANID:(Byte)val;
/**
 * Set Multi MSR setting value
 *
 Sets the MSR setting to the value given
 
 @param setting MSR setting to set
 @param value MSR setting value
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setSetting:(Byte)setting value:(Byte)val;
/**
 * Get Swipe Mask Option
 *
 Gets the swipe mask/clear data sending option
 
 @param option Byte using lower three bits as flags. 0 = Mask Option OFF, 1 = Mask Option ON
 bit0 = Track 1
 bit1 = Track 2
 bit2 = Track 3
 
 Example: option = 0x03 -> Track1 & 2 Masked Option ON, Track 3 Masked Option OFF.
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getSwipeMaskOption:(Byte*)option;
/**
 * Get Expiration Masking
 *
 Get the flag that determines if to mask the expiration date
 
 @param value masked = 0, unmasked = 1
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getExpirationMask:(Byte*)value;
/**
 * Get Clear PAN Digits
 *
 Returns the number of digits that begin the PAN that will be in the clear
 
 @param value Number of digits in clear. Values are between 0 - 6
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getClearPANID:(Byte*)value;
/**
 * Get Multi MSR setting values
 *
 Returns the settings used for swipe data
 
 @param setting MSR setting to retrieve
 @param value MSR setting values
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getSettings:(Byte)setting value:(NSData**)value;
/**
 * Retrieve MSR White List
 *
 Retrieves the whitelist
 
 @param value The whitelist data which is in ASN.1 block format
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_retrieveWhiteList:(NSData**)value;
/**
 * Set MSR Configuration
 *
 Sets MSR configuration data
 
 @param config Configuration data to send
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setConfiguration:(NSData*)config;
/**
 * Get MSR Configuration
 *
 Gets the MSR configuration data
 
 @param config Configuration data retrieved
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getConfiguration:(NSData**)config;
/**
 * Flush Track Data
 *
 Clears any track data being retained in memory by future PIN Block request
 
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_flushTrackData;
/**
 * FeliCa Read with MAC Generation
 *
 Reads up to 3 blocks with MAC Generation.  FeliCa Authentication must be performed first
 
 NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param numBlocks Number of blocks
 @param blockList Block to read. Each block in blockList   Maximum 3 block requests
 @param blocks  Blocks read.  Each block 16 bytes.
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_readWithMac:(int)numBlocks blockList:(NSData*)blockList blocks:(NSData**)blocks;



/**
 * FeliCa Send Command
 *
 Send a Felica Command
 
 NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param command Command data from settlement center to be sent to felica card
 @param response Response data from felica card
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_SendCommand:(NSData*)command response:(NSData**)response;


/**
 * FeliCa Write with MAC Generation
 *
 Writes a block with MAC Generation.  FeliCa Authentication must be performed first
 
 NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param blockNumber Number of block
 @param data  Block to write.  Must be 16 bytes.
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
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
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_read:(NSData*)serviceCode numBlocks:(int)numBlocks blockList:(NSData*)blockList blocks:(NSData**)blocks;


/**
 * FeliCa Write
 *
 Writes a block
 
  @param serviceCode Service Code list.  Each service code must be be 2 bytes
  @param blockCount Block Count
  @param blockList Block list.
  @param data  Block to write.  Must be 16 bytes.
  @param statusFlag  Status flag response as explained in FeliCA Lite-S User's Manual, Section 4.5
  * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
  
  */
 -(RETURN_CODE) felica_write:(NSData*)serviceCode blockCount:(int)blockCount  blockList:(NSData*)blockList data:(NSData*)data statusFlag:(NSData**)statusFlag;


/**
* FeliCa Poll Card
*
Perform functions a Felica Card Poll
 
 NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param systemCode System Code
 @param response  Response as explained in FeliCA Lite-S User's Manual
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_nfcCommand:(NSData*)systemCode response:(NSData**)response;


/**
 * FeliCa Request Service
 *
 Perform functions a Felica Request Service
 
  NOTE: The reader must be in Pass Through Mode for FeliCa commands to work.
 
 @param nodeCode Node Code
 @param response  Response as explained in FeliCA Lite-S User's Manual
 * @return RETURN_CODE:  Values can be parsed with IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) felica_requestService:(NSData*)nodeCode response:(NSData**)response;

-(void) notifyDataReceived: (NSData*) RevData;
+(bool) dataHasPrefix:(NSData*)data str:(NSString*)str;

+(bool) dataEquals:(NSData*)data str:(NSString*)str;
-(void) setReaderAttached:(BOOL)attached;

+ (NSString*) externalAccessoryProtocol;

+ (void) setExternalAccessoryProtocol:(NSString*)newValue;

+(NSData*) getLastData;

@end
