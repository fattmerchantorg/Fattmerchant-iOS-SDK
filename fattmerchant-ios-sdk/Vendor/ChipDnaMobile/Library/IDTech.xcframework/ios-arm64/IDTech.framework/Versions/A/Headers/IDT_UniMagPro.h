//
//  IDT_UniMagPro.h
//  IDTech
//
//  Created by Randy Palermo on 2/5/15.
//  Copyright (c) 2015 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "APDUResponse.h"
#import "IDT_Device.h"





/** Protocol methods established for IDT_UniMagPro class  **/
@protocol IDT_UniMagPro_Delegate <NSObject>

@optional
-(void) deviceConnected; //!<Fires when device connects.  If a connection is established before the delegate is established (no delegate to send initial connection notification to), this method will fire upon establishing the delegate.
-(void) deviceDisconnected; //!<Fires when device disconnects.
- (void) plugStatusChange:(BOOL)deviceInserted; //!<Monitors the headphone jack for device insertion/removal.
//!< @param deviceInserted TRUE = device inserted, FALSE = device removed
- (void) dataInOutMonitor:(NSData*)data  incoming:(BOOL)isIncoming; //!<All incoming/outgoing data going to the device can be monitored through this delegate.
//!<All incoming/outgoing data going to the device can be monitored through this delegate.
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

@interface IDT_UniMagPro : NSObject<IDT_Device_Delegate>{
    id<IDT_UniMagPro_Delegate> delegate;
}

@property(strong) id<IDT_UniMagPro_Delegate> delegate;  //!<- Reference to IDT_UniMagPro_Delegate.

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
 Establishes an singleton instance of IDT_UniMagPro class.
 
 @return  Instance of IDT_UniMagPro
 */
+(IDT_UniMagPro*) sharedController;

/**
 * Get Response Code String
 *
 Interpret a IDT_UniMagPro response code and return string description.
 
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
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMagPro::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;


/**
 * Enable MSR Swipe
 
 *
 Enables MSR, waiting for swipe to occur. Allows track selection. Returns IDTMSRData instance to deviceDelegate::swipeMSRData:()
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMagPro::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe;


/**
 Device Connected
 
 @return isConnected  Boolean indicated if UniMagPro is connected
 
 */

-(bool) isConnected;

@end
