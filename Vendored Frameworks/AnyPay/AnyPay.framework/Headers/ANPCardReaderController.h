//
//  ANPCardReaderController.h
//  AnyPay
//
//  Created by Ankit Gupta on 12/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayCardReader.h"

@class AnyPayCardReader, ANPMeaningfulError, ANPBluetoothDevice, BBPOSDeviceEventDispatch, BBDeviceController, ANPBBPOSOTACredential, BBPOSDeviceOTAEventDispatch, ANPBBPOSOTACredential, ANPBBPOSOTADeviceConfiguration;
@interface ANPCardReaderController : NSObject

@property (nonatomic, readonly, nullable) AnyPayCardReader *connectedReader;
@property (nonatomic, readonly, nullable) BBPOSDeviceEventDispatch *eventDispatch;
@property (nonatomic, readonly, nullable) BBPOSDeviceOTAEventDispatch *otaEventDispatch;
@property (nonatomic, readonly, nullable) ANPBBPOSOTACredential *otaCredential;

- (void)subscribeOnCardReaderConnected:(void (^ _Nonnull)(AnyPayCardReader * _Nullable cardReader))connectionHandler;
- (void)subscribeOnCardReaderDisConnected:(void (^ _Nonnull)(void))handler;
- (void)subscribeOnCardReaderConnectionFailed:(void (^ _Nonnull)(ANPMeaningfulError * _Nullable error))errorHandler;
- (void)subscribeOnCardReaderError:(void (^ _Nonnull)(ANPMeaningfulError * _Nullable error))errorHandler;

- (void)unsubscribeOnCardReaderConnected:(void (^ _Nonnull)(AnyPayCardReader * _Nullable cardReader))connectionHandler;
- (void)unsubscribeOnCardReaderDisConnected:(void (^ _Nonnull)(void))handler;
- (void)unsubscribeOnCardReaderConnectionFailed:(void (^ _Nonnull)(ANPMeaningfulError * _Nullable error))errorHandler;
- (void)unsubscribeOnCardReaderError:(void (^ _Nonnull)(ANPMeaningfulError * _Nullable cardReader))errorHandler;

+ (instancetype _Nonnull )sharedController;

- (BOOL)isReaderConnected;
- (NSArray<Class> *)supportedReaders;
- (NSDictionary<NSString *, Class> *)readerClasses;

#pragma mark Audio
- (void)connectAudioReader;

#pragma mark Bluetooth
- (void)connectBluetoothReader:(void (^ _Nullable)(NSArray<ANPBluetoothDevice *> *_Nullable))availableBTReadersToConnectHandler;
- (void)connectToBluetoothReader:(ANPBluetoothDevice * _Nonnull)reader;
- (void)connectToBluetoothReaderWithSerial:(NSString * _Nonnull)serialNumber;

#pragma mark USB
- (void)connectUSBReader;

- (void)disconnectReader;

- (BBDeviceController *_Nullable)getBBDeviceControllerInstance;
- (void)enableReaderLogs:(BOOL)enable;

#pragma mark OTA
- (void)setOTACredential:(ANPBBPOSOTACredential * _Nonnull)credential;
- (void)getTargetVersion:(void (^ _Nonnull)(NSDictionary *setConfig, ANPMeaningfulError *error))completionHandler;
- (void)getTargetFirmwareVersionList:(void (^ _Nonnull)(NSArray *list, ANPMeaningfulError *error))completionHandler;
- (void)getTargetConfigVersionList:(void (^ _Nonnull)(NSArray *list, ANPMeaningfulError *error))completionHandler;
- (void)setOTAConfiguration:(ANPBBPOSOTADeviceConfiguration *)config completionHandler:(void (^ _Nonnull)(BOOL success, ANPMeaningfulError *error))completionHandler;
- (void)updateFirmwareToTargetVersion:(NSString *)targetVersion completionHandler:(void (^ _Nonnull)(float percentUpdate, BOOL updated, ANPMeaningfulError *))completionHandler;
- (void)updateFirmwareCompletionHandler:(void (^ _Nonnull)(float percentUpdate, BOOL updated, ANPMeaningfulError *))completionHandler;
- (void)updateConfigToTargetVersion:(NSString *)deviceSettingVersion terminalSettingVersion:(NSString *)terminalSettingVersion completionHandler:(void (^ _Nonnull)(float percentUpdate, BOOL updated, ANPMeaningfulError *))completionHandler;
- (void)updateConfigCompletionHandler:(void (^ _Nonnull)(float percentUpdate, BOOL updated, ANPMeaningfulError *))completionHandler;

@end
