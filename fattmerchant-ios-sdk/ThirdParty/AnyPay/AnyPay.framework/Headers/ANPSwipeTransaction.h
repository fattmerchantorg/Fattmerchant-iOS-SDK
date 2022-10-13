//
//  ANPSwipeTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 20/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPCardTransaction.h"

@interface ANPSwipeTransaction : ANPCardTransaction

@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *encryptedSwipe;

@property (nonatomic, copy) NSString *ksn;
@property (nonatomic, copy) NSString *trackEncoding;
@property (nonatomic, copy) NSString *data;
@property (nonatomic, copy) NSString *firstSubsidiaryAccountNumber;
@property (nonatomic, copy) NSString *track1Length;
@property (nonatomic, copy) NSString *track2Length;
@property (nonatomic, copy) NSString *finalMessage;
@property (nonatomic, copy) NSString *track2Status;
@property (nonatomic, copy) NSString *encTracks;
@property (nonatomic, copy) NSString *track3Status;
@property (nonatomic, copy) NSString *PAN;
@property (nonatomic, copy) NSString *partialTrack;
@property (nonatomic, copy) NSString *bID;
@property (nonatomic, copy) NSString *posEntryMode;
@property (nonatomic, copy) NSString *formatID;
@property (nonatomic, copy) NSString *serviceCode;
@property (nonatomic, copy) NSString *productType;
@property (nonatomic, copy) NSString *track1Status;
@property (nonatomic, copy) NSString *secondSubsidiaryAccountNumber;
@property (nonatomic, copy) NSString *track3Length;
@property (nonatomic, copy) NSString *encWorkingKey;
@property (nonatomic, copy) NSString *serialNumber;
@property (nonatomic, copy) NSString *encTrack1;
@property (nonatomic, copy) NSString *encTrack2;
@property (nonatomic, copy) NSString *encTrack3;
@property (nonatomic, copy) NSString *mac;
@property (nonatomic, copy) NSString *cardDataMacAlgorithmIdAndMacDOL;

@end
