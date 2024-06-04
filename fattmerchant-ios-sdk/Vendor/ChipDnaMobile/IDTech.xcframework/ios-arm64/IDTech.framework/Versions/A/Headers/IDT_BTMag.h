//
//  IDT_BTMag.h
//  IDTech
//
//  Created by Randy Palermo on 8/28/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "IDT_Device.h"


typedef enum{
    EPT_NOKEY = 0x30,
    EPT_TDES,
    EPT_AES
}Encryption_Type;

typedef enum{
    PRE_AMBLE =0x02,
    POST_AMBLE
}PostPreamble;

typedef enum{
    BOTH_DIRECTION=0x31,
    HEAD_DIRECTION,
    HEAD_AGAINST,
    RAW_DATA
    
}DecodingMethod;

typedef enum{
    KEY_FIXED=0x30,
    KEY_DUKPT
}KeyManagementType;

typedef enum{
    TRACK_1=0x31,
    TRACK_2,
    TRACK_1_and_2,
    TRACK_3,
    TRACK_1_and_3,
    TRACK_2_and_3,
    TRACK_ALL,
    TRACK_ANY1,
    TRACK_ANY2
}MagneticTrack;

typedef enum{
    TRACK1_ID= 1,
    TRACK2_ID,
    TRACK3_ID
}TrackID;

/** Protocol methods established for IDT_BTPay class  **/
@protocol IDT_BTMag_Delegate <NSObject>

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

@end

/**
 Class to drive the IDT_BTMag device
 */
@interface IDT_BTMag : NSObject<IDT_Device_Delegate>{
    id<IDT_BTMag_Delegate> delegate;
}

@property(strong) id<IDT_BTMag_Delegate> delegate;  //!<- Reference to IDT_BTPay_Delegate.



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
 Establishes an singleton instance of IDT_BTMag class.
 
 @return  Instance of IDT_BTMag
 */
+(IDT_BTMag*) sharedController;



/**
 * Sets the OSX Connection Method
 *
 When using BTPay on OSX, the device can connect either via Bluetooth or USB-HID.  Default is Bluetooth.  Use this function to change the connection method to USB-HID
 
 @param isUSB  TRUE = connect via USB-HID, FALSE = connect via BlueTooth.
 
 
 */
+(void) connectUSB:(BOOL)isUSB;


/**
 * Polls device for Firmware Version
 
 *
 * @param response Response returned of Firmware Version
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTMag::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;

/**
 * Send a NSData object to device
 *
 * Sends a command represented by the provide NSData object to the device through the accessory protocol.
 *
 * @param cmd NSData representation of command to execute
 * @param lrc If <c>TRUE</c>, this will wrap command with start/end/CheckLRC:  '{STX} data {ETX} {CheckLRC}'
 @param response Response data
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 */
-(RETURN_CODE) device_sendDataCommand:(NSData*)cmd calcLRC:(BOOL)lrc response:(NSData**)response;

/**
 * Get Clear PAN Digits
 *
 Returns the number of digits that begin the PAN that will be in the clear
 
 @param value Number of digits in clear. Values are between 0 - 6
 * @return RETURN_CODE:  Values can be parsed with IDT_BTMag::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getClearPANID:(Byte*)value;

/**
 * Get Expiration Masking
 *
 Get the flag that determines if to mask the expiration date
 
 @param value masked = 0, unmasked = 1
 * @return RETURN_CODE:  Values can be parsed with IDT_BTMag::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getExpirationMask:(Byte*)value;

/**
 * Get Swipe Mask Option
 *
 Gets the swipe mask/clear data sending option
 
 @param option Byte using lower three bits as flags. 0 = Mask Option OFF, 1 = Mask Option ON
 bit0 = Track 1
 bit1 = Track 2
 bit2 = Track 3
 
 Example: option = 0x03 -> Track1 & 2 Masked Option ON, Track 3 Masked Option OFF.
 * @return RETURN_CODE:  Values can be parsed with IDT_BTMag::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_getSwipeMaskOption:(Byte*)option;

/**
 * Set Multi MSR setting value
 *
 Sets the MSR setting to the value given
 
 @param setting MSR setting to set
 @param value MSR setting value
 * @return RETURN_CODE:  Values can be parsed with IDT_BTMag::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setSetting:(Byte)setting value:(Byte)val;

/**
 * Set Clear PAN Digits
 *
 Sets the amount of digits shown in the clear (not masked) at the beginning of the returned PAN value
 
 @param val Number of digits to show in the clear from 0 - 6 digits
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) msr_setClearPANID:(Byte)val;

/**
 * Set Expiration Masking
 *
 Sets the flag to mask the expiration date
 
 @param mask True = mask expiration
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) msr_setExpirationMask:(BOOL)mask;

/**
 * Set Swipe Data Encryption
 *
 Sets the swipe encryption method
 
 @param encryption 1 = TDES, 2 = AES
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) msr_setSwipeEncryption:(Byte)encryption;

/**
 * Set Swipe Force Encryption
 *
 Sets the swipe force encryption options
 
 @param track1 Force encrypt track 1
 @param track2 Force encrypt track 2
 @param track3 Force encrypt track 3
 @param track3card0 Force encrypt track 3 when card type is 0
 * @return RETURN_CODE:  Values can be parsed with IDT_BTMag::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) msr_setSwipeForcedEncryptionOption:(BOOL)track1 track2:(BOOL)track2 track3:(BOOL)track3 track3card0:(BOOL)track3card0;

/**
 Is Device Connected
 
 Returns the connection status of the BTMag
 
 */
-(bool) isConnected;

/**
 * Get Response Code String
 *
 Interpret a IDT_UniMag response code and return string description.
 
 @param errorCode Error code, range 0x0000 - 0xFFFF, example 0x0300
 
 
 * @return Verbose error description
 
 
 */
-(NSString *) device_getResponseCodeString: (int) errorCode;



/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_BTMag::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getSerialNumber:(NSString**)response;

/**
 *Close Device
 */

-(void) close;

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
