//
//  APDUData.h
//  IDTech
//
//  Created by Randy Palermo on 5/16/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//


@class APDUData;

/**  IDT_UniPay::icc_exchangeAPDU:response:()
 */
@interface APDUData : NSObject{
    @public 
    NSData* responseStatus;  //!< APDU response status bytes, 2 bytes
    Byte maskStatus;  //!< Mask/Clear status: 0x00=TDES, 0x10 = AES
    BOOL hasKSN; //!< KSN data read
    BOOL hasHash; //!< Hash data read
    BOOL hasEncryption;  //!< APDU response is encrypted
    NSData* response;  //!< APDU Response
    NSData* ksn;    //!< Key Seral Number
    NSData* hash;   //!< Hash value
}


/**
 * clears all APDUData properties
 */
-(void)clear;

/**
 * Singleton instance of APDUData
 */
+ (APDUData *)sharedController;



@property (nonatomic, strong) NSData* responseStatus;  
@property Byte maskStatus;
@property BOOL hasKSN;
@property BOOL hasHash;
@property BOOL hasEncryption;
@property (nonatomic, strong) NSData* response;
@property (nonatomic, strong) NSData* ksn;
@property (nonatomic, strong) NSData* hash;

@end
