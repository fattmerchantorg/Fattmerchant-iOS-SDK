//
//  ANPConfiguration.h
//  AnyPay
//
//  Created by Ankit Gupta on 13/09/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "AnyPayModel.h"

@class ANPLoggingConfiguration;
@interface ANPConfiguration : AnyPayModel

@property (nonatomic, copy) NSString *defaultDateFormat;
@property (nonatomic, strong) NSNumber *registerCardReaderWhenConnected;
@property (nonatomic, strong) NSNumber *cloudSyncEnabled;
@property (nonatomic, strong) NSNumber *allowDynamicTerminalCreation;
@property (nonatomic, strong) NSNumber *requireDigestAuthentication;
@property (nonatomic, strong) NSNumber *allowAnonymousUsers;
@property (nonatomic) BOOL allowClearTransactions;
@property (nonatomic, strong) NSNumber *allowPartialAuthorizations;
@property (nonatomic, strong) NSNumber *allowPartialRefunds;
@property (nonatomic, strong) NSNumber *allowReferencedRefunds;
@property (nonatomic) BOOL allowUnreferencedRefunds;
@property (nonatomic, strong) NSNumber *chequeColorFormatEditable;
@property (nonatomic, strong) NSNumber *chequeFileFormatEditable;
@property (nonatomic, strong) NSNumber *customFieldsEnabled;
@property (nonatomic, strong) NSNumber *customFieldsEnabledEditable;
@property (nonatomic, strong) NSNumber *defaultCardReaderEditable;
@property (nonatomic, strong) NSNumber *driversLicenseRequired;
@property (nonatomic, strong) NSNumber *driversLicenseStateRequiredEditable;
@property (nonatomic, strong) NSNumber *driversLicenseRequiredEditable;
@property (nonatomic, strong) NSNumber *driversLicenseStateRequired;
@property (nonatomic, strong) NSNumber *enableAvs;
@property (nonatomic, strong) NSNumber *enableCvv;
@property (nonatomic, strong) NSNumber *enableFirmwareUpdate;
@property (nonatomic, strong) NSNumber *enableForgotPasswordUrl;
@property (nonatomic, strong) NSNumber *enabledCurrencyEditable;
@property (nonatomic, strong) NSNumber *enabledEntryModesEditable;

@property (nonatomic, copy) NSString *teminalMode;
@property (nonatomic, copy) NSString *enabledEntryModes;
@property (nonatomic, copy) NSString *chequeColorFormat;
@property (nonatomic, copy) NSString *chequeFileFormat;
@property (nonatomic, copy) NSString *defaultCardReader;
@property (nonatomic, copy) NSString *defaultCurrency;
@property (nonatomic, copy) NSString *enabledCurrency;
@property (nonatomic, copy) NSString *enabledPaymentMethods;
@property (nonatomic, copy) NSString *enabledReceiptOptions;
@property (nonatomic, copy) NSString *forgotPasswordUrl;
@property (nonatomic, copy) NSString *id;

@property (nonatomic, strong) NSNumber *enabledPaymentMethodsEditable;
@property (nonatomic, strong) NSNumber *enabledReceiptOptionsEditable;
@property (nonatomic, strong) NSNumber *enforce;
@property (nonatomic, strong) NSNumber *enforceFirmwareCompliance;
@property (nonatomic, strong) NSNumber *geofencingEnabled;
@property (nonatomic, strong) NSNumber *geofencingEnabledEditable;
@property (nonatomic, strong) NSNumber *geolocationEnabled;
@property (nonatomic, strong) NSNumber *geolocationEnabledEditable;
@property (nonatomic, strong) NSNumber *requireAvs;
@property (nonatomic, strong) NSNumber *requireCardPresentRefunds;
@property (nonatomic, strong) NSNumber *requireCardPresentSales;
@property (nonatomic, strong) NSNumber *requireCvv;
@property (nonatomic, strong) NSNumber *requireTermsAndConditions;
@property (nonatomic, strong) NSNumber *sessionTimeoutEnabled;
@property (nonatomic, strong) NSNumber *sessionTimeoutEnabledEditable;
@property (nonatomic) BOOL surchargeEnabled;
@property (nonatomic, strong) NSNumber *surchargeEnabledEditable;
@property (nonatomic, strong) NSNumber *surchargeOptionsEditable;
@property (nonatomic, strong) NSNumber *suspendAndResumeEnabled;
@property (nonatomic, strong) NSNumber *suspendAndResumeEnabledEditable;
@property (nonatomic) BOOL taxEnabled;
@property (nonatomic) BOOL taxEnabledEditable;
@property (nonatomic, strong) NSNumber *taxRatesEditable;
@property (nonatomic, strong) NSNumber *terminalEnabled;
@property (nonatomic) BOOL tipEnabled;
@property (nonatomic) BOOL tipEnabledEditable;
@property (nonatomic, strong) NSNumber *tipOptionsEditable;
@property (nonatomic, strong) NSNumber *verifyCardPresentForKeyedTransactions;
@property (nonatomic) BOOL allowKeyedTransactions;
@property (nonatomic) BOOL allowKeyedTransactionsEditable;

@property (nonatomic, copy) NSString *sessionTimeoutInterval;
@property (nonatomic, copy) NSString *supportedCurrencies;
@property (nonatomic, copy) NSString *supportedEntryModes;
@property (nonatomic, copy) NSString *supportedPaymentMethods;
@property (nonatomic, copy) NSString *supportedReceiptOptions;
@property (nonatomic, copy) NSString *timestamp;
@property (nonatomic, copy) NSString *suspendAndResumeSupportedEntryModes;
@property (nonatomic, copy) NSString *suspendAndResumeSupportedPaymentMethods;

@property (nonatomic, copy) NSString *transactionReportingMode;
@property (nonatomic, copy) NSString *transactionReportingModeEditable;
@property (nonatomic, copy) NSString *type;
@property (nonatomic, copy) NSNumber *unreferencedRefundLimit;
@property (nonatomic, copy) NSString *userAuthenticationMode;
@property (nonatomic, copy) NSString *version;
@property (nonatomic, copy) NSString *CLASS_META_KEY;
@property (nonatomic, copy) NSString *dateModified;

@property (nonatomic, strong) NSMutableArray<ANPTaxLineItem *> <ANPTaxLineItem> *taxRates;
@property (nonatomic, strong) NSMutableArray<ANPTipLineItem *> <ANPTipLineItem> *tipOptions;
@property (nonatomic, strong) NSMutableArray<ANPFeeLineItem *> <ANPFeeLineItem> *surchargeOptions;

@property (nonatomic, strong) AnyPayEndpoint *endpoint;
@property (nonatomic, strong) ANPLoggingConfiguration *logging;

- (void)addTaxRate:(ANPTaxLineItem *)tax;
- (void)addTaxRates:(NSArray<ANPTaxLineItem *> *)taxesToAdd;

- (void)addTipRate:(ANPTipLineItem *)tip;
- (void)addTipRates:(NSArray<ANPTipLineItem *> *)tipsToAdd;

@end
