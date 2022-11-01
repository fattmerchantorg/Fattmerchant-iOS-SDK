//
//  AnyPayCardReader.h
//  AnyPay
//
//  Created by Ankit Gupta on 12/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPPeripheralDevice.h"
#import "ANPCardReaderConnectionMethods.h"
#import "ANPCardReaderConnectionStatuses.h"
#import "ANPCardReaderInterfaces.h"
#import "ANPTransactionWorkflow.h"

@interface AnyPayCardReader : ANPPeripheralDevice

@property (nonatomic, readonly) ANPCardReaderConnectionMethod defaultConnectionMethod;
@property (nonatomic) ANPCardReaderConnectionMethod connectionMethod;
@property (nonatomic) ANPCardReaderConnectionStatus connectionStatus;

@property (nonatomic, readonly) NSString *emvKsn;
@property (nonatomic, readonly) NSString *trackKsn;
@property (nonatomic, readonly) NSString *pinKsn;
@property (nonatomic, readonly) NSString *macKsn;
@property (nonatomic, readonly) NSString *hardwareVersion;
@property (nonatomic, readonly) NSString *formatID;
@property (nonatomic, readonly) NSString *bootloaderVersion;
@property (nonatomic, readonly) NSString *deviceSettingVersion;
@property (nonatomic, readonly) NSString *serialNumber;
@property (nonatomic, readonly) NSString *terminalSettingVersion;
@property (nonatomic, readonly) NSString *uid;
@property (nonatomic, readonly) NSString *vendorID;
@property (nonatomic, readonly) BOOL isCharging;
@property (nonatomic, readonly) BOOL isSupportedNfc;
@property (nonatomic, readonly) BOOL isSupportedTrack1;
@property (nonatomic, readonly) BOOL isSupportedTrack2;
@property (nonatomic, readonly) BOOL isSupportedTrack3;
@property (nonatomic, readonly) BOOL isUsbConnected;
@property (nonatomic) ANPCardReaderInterface selectedCardInterface;
@property (nonatomic, copy) NSString *name;

- (NSString *)name;
- (ANPCardReaderInterface)cardInterface;
- (ANPCardReaderInterface)defaultCardInterface;
- (ANPTransactionWorkflow *)transactionWorkflow:(ANPTransaction *)transaction;

 @end
