//  UniPay.h
//  UniPay SDK
/* Copyright 2013 ID TECH. All rights reserved. 2013-10-28
 */

//#import <UIKit/UIKit.h>

//Versioning: MRC STD ver

#define UMSDK_VERSION_UP @"1.4.01"
#define UMSDK_CUSTOMIZATION2 1
#import "uniMag.h"


//async task methods return value
//Description               |Applicable task
//                          |Connect|Swipe|Cmd
typedef NS_ENUM(NSInteger, RDStatus) {              //--------------------------+-------+-----+---+------
    RDS_SUCCESS,            //no error, beginning task  | *     | *   | * |
    RDS_FAILED,             //err response or data      | *     | *   | * |
    RDS_NOT_ATTACHED,       //no reader attached        | *     | *   | * |
    RDS_SDK_BUSY,           //SDK is doing another task | *     | *   | * |
    RDS_MONO_AUDIO,         //mono audio is enabled     | *     |     | * |
    RDS_CONNECTED,          //did connection            | *     |     |   |
    RDS_LOW_VOLUME,         //audio volume is too low   | *     |     | * |
    RDS_DISCONNECTED,       //did not do connection     | *     | *   | * |
    RDS_INVALID_PARAMETER,  //wrong parameter           |       |     | * |
    RDS_CANCELED,           //task or CMD be canceled   | *     | *   |   |
    RDS_TIMEDOUT,           //time out for task or CMD  | *     | *   | * |
} ;

static inline NSString* RDStatus_lookup(RDStatus c) {
    /*
#define URLOOK(a) case a: return @#a;
    switch (c) {
            URLOOK(RDS_SUCCESS          )
            URLOOK(RDS_FAILED        )
            URLOOK(RDS_NOT_ATTACHED        )
            URLOOK(RDS_SDK_BUSY         )
            URLOOK(RDS_MONO_AUDIO       )
            URLOOK(RDS_CONNECTED)
            URLOOK(RDS_LOW_VOLUME       )
            URLOOK(RDS_DISCONNECTED    )
            URLOOK(RDS_INVALID_PARAMETER   )
            URLOOK(RDS_CANCELED       )
            URLOOK(RDS_TIMEDOUT  )
    }
#undef URLOOK
     */
    switch (c) {
        case RDS_SUCCESS:
            return @"RETURN_CODE_DO_SUCCESS";
            break;
        case RDS_FAILED:
            return @"RETURN_CODE_FAILED";
            break;
        case RDS_NOT_ATTACHED:
            return @"RETURN_CODE_NOT_ATTACHED";
            break;
        case RDS_SDK_BUSY:
            return @"RETURN_CODE_SDK_BUSY_MSR";
            break;
        case RDS_MONO_AUDIO:
            return @"RETURN_CODE_MONO_AUDIO";
            break;
        case RDS_CONNECTED:
            return @"RETURN_CODE_CONNECTED";
            break;
        case RDS_LOW_VOLUME:
            return @"RETURN_CODE_LOW_VOLUME";
            break;
        case RDS_DISCONNECTED:
            return @"RETURN_CODE_ERR_DISCONNECT";
            break;
        case RDS_INVALID_PARAMETER:
            return @"RETURN_CODE_ERR_INVALID_PARAMETER";
            break;
        case RDS_CANCELED:
            return @"RETURN_CODE_CANCELED";
            break;
        case RDS_TIMEDOUT:
            return @"RETURN_CODE_ERR_TIMEDOUT";
            break;
            

            
    }
}

//InjectKeyType
/*
typedef NS_ENUM(NSInteger, InjectDUKPTKeyType) {
    DUKPT_KEY_MSR = 0x00,
    DUKPT_KEY_ICC = 0x01,
    DUKPT_KEY_Admin = 0x10,
    DUKPT_KEY_Paireing_PinPad = 0x20,
} DUKPT_KEY_Type;
*/

typedef struct {
    RDStatus status;
    __unsafe_unretained NSMutableData *data;
} RDResult;

@protocol UniPay_Delegate <NSObject>

@optional
-(void) UniPay_EventFunctionAttachment;
-(void) UniPay_EventFunctionDetachment;
- (void) dataInOutMonitor:(NSData*)data  incoming:(BOOL)isIncoming;
- (void) dataInOutMonitorDate:(NSData*)data  incoming:(BOOL)isIncoming timestamp:(NSDate*)timestamp;
//!<All incoming/outgoing data going to the device can be monitored through this delegate.
//!< @param data The serial data represented as a NSData object
//!< @param isIncoming The direction of the data
//!<- <c>TRUE</c> specifies data being received from the device,
//!<- <c>FALSE</c> indicates data being sent to the device.
//!< @param timestamp The timestamp of the data

-(void) UniPay_EventFunctionConnect:(Byte) nType;
-(void) UniPay_EventFunctionMessage:(NSString *) someString;

-(void) UniPay_EventFunctionMSR:(Byte)nType card:(NSData *)cardData;
-(void) UniPay_EventFunctionICC: (Byte) nICC_Attached;
-(void) UniPay_ReceivedData: (NSData*) data;



@end

@interface UniPay : NSObject{
    id<UniPay_Delegate> delegate;
}

@property(strong) id<UniPay_Delegate> delegate;

//version
+(void) isKeyLoad:(BOOL)keyload;
+(void) disableAudioDetection;
+(NSString*) SDK_version;
+(UniPay*) start:(id<UniPay_Delegate>)del;
//+(void) resetSharedController;
// troubleshooting
+(void) enableLogging:(BOOL) enable;
+(UniPay*) sharedController;
+(void) isVP3300:(BOOL)VP3300;
+(void) isUniMag:(int)uniMag;
-(void) setVivoAlive:(BOOL)val;
-(void) clearTask;
-(void) restart;
-(void) stopConnection;
//connect
-(BOOL) cmut_isReaderConnected;
-(void) setReaderAttached:(BOOL)attached;
-(RDStatus) cmut_StartConnect_Task;
-(RDStatus) cmut_CancelConnect_Task;
-(RDStatus) startUniPay:(BOOL)start;
//commands
-(void) setDisconnect;
-(void) setConnect;
-(BOOL) isNoWait;
+(void) removeCommandDelay;
-(void) noWaitNextCommand:(BOOL)wait;
-(RDResult) comn_GetFirmVersion;
-(RDResult) comn_GetSerialNumber;
-(RDResult) comn_SetSerialNumber: (NSString*) strSN;
-(RDResult) comn_GetModelNumber;
-(RDResult) comn_ResetReader;
-(RDResult) comn_directIO: (NSData *) dataCMD;

-(RDResult) smart_GetICC_Status;
-(RDResult) smart_ICC_PowerOn;
-(RDResult) smart_ICC_PowerOff;
-(RDResult) smart_ExchangeAPDU: (NSData *) dataAPDU;
-(RDResult) smart_ExchangeAPDU_Encrypted: (NSData *) dataAPDU;
-(RDResult) smart_GetEncryptionMode;
-(RDResult) smart_SetEncryptionMode: (unsigned char) nMode;
-(RDResult) smart_Get_KeyType_DUKPT;
-(RDResult) smart_Set_KeyType_DUKPT: (unsigned char) nType;
-(RDResult) smart_GetAllSetting;
-(RDResult) smart_SetAllSetting_Default;
-(RDResult) smart_GetKSN;
-(RDResult) smart_ExchangeAPDU_DUKPT_Fully: (NSData *) dataDUKPT_CAPDU;
-(void) registerAttachmentCallback:(BOOL)reg;
-(RDResult) EnableMSR;
-(RDResult) CancelMSR;
-(RDResult) GetEncryptionMode;
-(RDResult) SetEncryptionMode: (unsigned char) nMode;
-(RDResult) GetSecurityLevel;
-(RDResult) GetCommonSetting: (unsigned char) nFunctionID;
-(RDResult) SetCommonSetting: (unsigned char) nFunctionID : (unsigned char) nValue;
-(RDResult) GetAllSetting;
-(RDResult) SetAllSetting_Default;
-(void) playAudioCommand:(NSData*)cmdString;
-(void) playVivoCommand:(NSData*)cmdString;
//audio jack communication
//config
-(BOOL) cmut_setCmdTimeoutDuration:(NSInteger) seconds;
-(void) cmut_setAutoAdjustVolume:(BOOL) b;
-(int) cmut_setVolume: (float) fVolume;

-(NSString *) comn_GetErrorString: (NSData *) errorCode;
-(NSData *) debug_GetReceivedWave;

//old API, just for backward compatibility
-(RDStatus) StartMSR_Task;
-(void) CancelMSR_Task;
-(int)umRetToRC:(UmRet)val;

- (void)uniMagNotifEnable:(BOOL) enable;
+(void) usePreamble:(BOOL)pre;
-(void) startTask_hook ;
/**
* Reset Singleton
-  All Devices
*
Resets the singleton instance of the SDK

*/

+(void) resetSingleton;

@end

