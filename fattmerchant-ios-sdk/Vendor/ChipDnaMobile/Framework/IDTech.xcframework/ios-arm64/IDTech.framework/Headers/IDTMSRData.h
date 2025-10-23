//
//  IDTMSRData.h
//  IDT
//
//  Created by Randy Palermo on 4/25/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import "IDTCommon.h"

@class IDTMSRData;






/**
 * Encapsulating data class for MSR data capture
 */
@interface IDTMSRData : NSObject{
    @public
    /**
     Event type. Uses enumeration EVENT_MSR_Types
     
     @code
     typedef enum{
        EVENT_MSR_UNKNOWN = 31,
        EVENT_MSR_CARD_DATA,
        EVENT_MSR_CANCEL_KEY,
        EVENT_MSR_BACKSPACE_KEY,
        EVENT_MSR_ENTER_KEY,
        EVENT_MSR_DATA_ERROR,
        EVENT_MSR_ICC_START,
        EVENT_BTPAY_CARD_DATA,
        EVENT_UNIPAYII_EMV_NO_ICC_MSR_DATA,
        EVENT_UNIPAYII_EMV_FALLBACK_DATA,
        EVENT_UNIPAY_KEYLOADING,
        EVENT_MSR_TIMEOUT
     }EVENT_MSR_Types;
     @endcode
     */
    EVENT_MSR_Types  event;
    /**
     Encode Type of captured MSR Data.
     
     Uses enumeration CAPTURE_ENCODE_TYPE:
     
     @code
     typedef enum{
        CAPTURE_ENCODE_TYPE_ISOABA=0,
        CAPTURE_ENCODE_TYPE_AAMVA=1,
        CAPTURE_ENCODE_TYPE_Other=3,
        CAPTURE_ENCODE_TYPE_Raw=4
     } CAPTURE_ENCODE_TYPE;

     @endcode
     */
    CAPTURE_ENCODE_TYPE captureEncodeType;
    /**
     Encrypt Type of captured MSR Data.
     
     Uses enumeration CAPTURE_ENCODE_TYPE:
     
     @code
     typedef enum{
        CAPTURE_ENCRYPT_TYPE_TDES=0,
        CAPTURE_ENCRYPT_TYPE_AES=1
     } CAPTURE_ENCRYPT_TYPE;
     
     @endcode
     */
    CAPTURE_ENCRYPT_TYPE captureEncryptType;
    CAPTURE_CARD_TYPE captureCardType;
    KEY_VARIANT_TYPE msr_keyVariantType;
    NSData* cardData;   //!< Complete unparsed swipe data as received from MSR
    NSString* track1;   //!< Track 1 masked if encryption enabled or cleartext if encryption disabled
    NSString* track2;   //!< Track 2 masked if encryption enabled or cleartext if encryption disabled
    NSString* track3;   //!< Track 3 masked if encryption enabled or cleartext if encryption disabled
    int track1Length;   //!< Length of track 1 masked/clear text data
    int track2Length;   //!< Length of track 2 masked/clear text data
    int track3Length;   //!< Length of track 3 masked/clear text data
    NSData* encTrack1;   //!< Track 1 encoded data OR all encoded track data if encryption method combines all tracks into single blob
    NSData* encTrack2;   //!< Track 2 encoded
    NSData* msr_extendedField;   //!< MSR Extended Field
    NSData* encTrack3;   //!< Track 3 encoded
    NSData* hashTrack1;   //!< Sha-256 hash of Track 1 encoded data
    NSData* hashTrack2;   //!< Sha-256 hash of Track 2 encoded data
    NSData* hashTrack3;   //!< Sha-256 hash of Track 3 encoded data
    NSString* RSN;   //!< Reader Serial Number
    NSData* KSN;   //!< Key Serial Number
    NSData* sessionID;   //!< Session ID - Security level 4 only
    NSData* mac;   //!< mac
    NSData* macKSN;   //!< macKSN
    NSData* rawData;   //!< rawData
    /**
     Track Read Status
     - Bit 0: 1=Track 1 decode success, 0=Track 1 decode fail
     - Bit 1: 1=Track 2 decode success, 0=Track 2 decode fail)
     - Bit 2: 1=Track 3 decode success, 0=Track 3 decode fail)
     - Bit 3: 1=Track 1 sampling data exists, 0=Track 1 sampling data does not exist
     - Bit 4: 1=Track 2 sampling data exists, 0=Track 2 sampling data does not exist
     - Bit 5: 1=Track 3 sampling data exists, 0=Track 3 sampling data does not exist
     - Bit 6: reserved for future use
     - Bit 7: reserved for future use
     */
    unsigned char readStatus;
    int errorCode; //!<Contains error code when data is not returned
    bool iccPresent; //!<Card contains ICC
    NSDictionary* unencryptedTags; //!<Unencrypted card data provided via TLV
    NSDictionary* encryptedTags; //!<Encrypted card data provided via TLV
    NSDictionary* maskedTags; //!<Masked card data provided via TLV
	NSString* kbOutput; //!< Contains output converted to KB format


}

/**
 * clears all IDTMSRData properties
 */
-(void)clear;
/**
 * Singleton instance of IDTMSRData
 */
+ (IDTMSRData *)sharedController;


@property KEY_VARIANT_TYPE  msr_keyVariantType;
@property EVENT_MSR_Types  event;
@property CAPTURE_ENCODE_TYPE captureEncodeType;
@property CAPTURE_ENCRYPT_TYPE captureEncryptType;
@property CAPTURE_CARD_TYPE captureCardType;
@property (nonatomic, strong) NSData* mac;
@property (nonatomic, strong) NSData* macKSN;
@property (nonatomic, strong) NSData* cardData;
@property (nonatomic, strong) NSString* kbOutput;
@property (nonatomic, strong) NSString* track1;
@property (nonatomic, strong) NSString* track2;
@property (nonatomic, strong) NSString* track3;
@property int track1Length;
@property int track2Length;
@property int track3Length;
@property (nonatomic, strong) NSData* rawData;
@property (nonatomic, strong) NSData* msr_extendedField;
@property (nonatomic, strong) NSData* encTrack1;
@property (nonatomic, strong) NSData* encTrack2;
@property (nonatomic, strong) NSData* encTrack3;
@property (nonatomic, strong) NSData* hashTrack1;
@property (nonatomic, strong) NSData* hashTrack2;
@property (nonatomic, strong) NSData* hashTrack3;
@property (nonatomic, strong) NSString* RSN;
@property (nonatomic, strong) NSData* KSN;
@property (nonatomic, strong) NSData* sessionID;
@property unsigned char readStatus;
@property int errorCode;
@property bool iccPresent;
@property (nonatomic, strong) NSDictionary* unencryptedTags;
@property (nonatomic, strong) NSDictionary* encryptedTags;
@property (nonatomic, strong) NSDictionary* maskedTags;


@end
