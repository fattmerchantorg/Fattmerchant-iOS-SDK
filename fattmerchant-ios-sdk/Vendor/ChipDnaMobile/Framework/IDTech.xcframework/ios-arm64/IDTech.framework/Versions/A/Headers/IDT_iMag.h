//
//  IDT_iMag.h
//  IDTech
//
//  Created by Randy Palermo on 8/28/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "IDT_Device.h"


/** Protocol methods established for IDT_BTPay class  **/
@protocol IDT_iMag_Delegate <NSObject>

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
 Class to drive the IDT_iMag device
 */
@interface IDT_iMag : NSObject<IDT_Device_Delegate>{
    id<IDT_iMag_Delegate> delegate;
}

@property(strong) id<IDT_iMag_Delegate> delegate;  //!<- Reference to IDT_BTPay_Delegate.



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
 Establishes an singleton instance of IDT_iMag class.
 
 @return  Instance of IDT_iMag
 */
+(IDT_iMag*) sharedController;


/**
 * Set Serial Number
 *
 Set device's serial number
 *
 @param strSN Device serial number must be 8 - 10 characters
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 
 */
-(RETURN_CODE) setSerialNumber:(NSString*)strSN;

/**
 * Polls device for Serial Number
 *
 
 * @return Returns Serial Number

 
 *
 */
-(NSString*) getSerialNumber;

/**
 * Downloads Patch File to iMag
 
 @param file Patch File. Must be in Intel Hex Format.  Example
 
 @code
 :10AC4000FC0800FF00FF00FF00FF01030200040BEF
 :106950000020526561646572205621322E312E313D
 :00000001FF
 @endcode
 *

 *
 */
-(BOOL) downloadPatch:(NSString*)file;


/**
 * Polls device for Firmware Version
 *
 *
 * @return Firmware Version

 *
 */
-(NSString*) getFirmwareVersion;

/**
 * Send a NSData object to device
 *
 * Sends a command represented by the provide NSData object to the device through the accessory protocol.
 *
 * @param cmd NSData representation of command to execute
 * @param lrc If <c>TRUE</c>, this will wrap command with start/length/end/lrc:  '{STX} data {ETX} {CheckLRC}'
 @param response Response data
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 */
-(RETURN_CODE) sendDataCommand:(NSData*)cmd calcLRC:(BOOL)lrc response:(NSData**)response;

/**
 * Send Payment Application Engine Command
 * 
 Executes a PAE command
 
 @param command ASCII command string, should be started with "*PAE"
 @param response Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_Device::device_getResponseCodeString:()
 */
-(RETURN_CODE) device_sendPAE:(NSString*)command response:(NSString**)response;

/**
 Is Device Connected
 
 Returns the connection status of the iMag
 
 */
-(bool) isConnected;

@end
