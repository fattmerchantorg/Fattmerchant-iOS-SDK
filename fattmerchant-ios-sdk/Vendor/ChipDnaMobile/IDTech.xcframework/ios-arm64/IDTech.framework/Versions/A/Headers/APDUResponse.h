//
//  APDUResponse.h
//  IDTech
//
//  Created by Randy Palermo on 5/16/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//


@class APDUResponse;

/**  Used in IDT_BTpay::icc_exchangeAPDU:encrypted:ksn:response:() IDT_UniPay::icc_exchangeAPDU:encrypted:response:()
 */
@interface APDUResponse : NSObject{
    @public 
    BOOL hasKSN; //!< KSN data read
    BOOL hasEncryption;  //!< APDU response is encrypted
    NSData* response;  //!< APDU Response excluding SW1 and SW2
    NSData* ksn;    //!< Key Seral Number
    unsigned char SW1;     //!< Status Word Byte 1
    unsigned char SW2;     //!< Status Word Byte 2
    int apduLength; //!< Length of valid R-APDU
}


/**
 * clears all APDUResponse properties
 */
-(void)clear;
/**
 * Singleton instance of APDUResponse
 */
+ (APDUResponse *)sharedController;



@property unsigned char SW1;
@property unsigned char SW2;
@property BOOL hasKSN;
@property BOOL hasEncryption;
@property int apduLength;
@property (nonatomic, strong) NSData* response;
@property (nonatomic, strong) NSData* ksn;

@end
