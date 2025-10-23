//
//  CEXParameterKeys.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CEXParameterKeys : NSObject

/**
 * @brief Parameter key for attended type
 **/
extern NSString * const CEXKEYAttendedType;

/**
 * @brief Parameter key for device type
 **/
extern NSString * const CEXKEYDeviceType;

/**
 * @brief Parameter key for mobile pos.
 **/
extern NSString * const CEXKEYMobilePos;

/**
 * @brief Parameter key for premises.
 **/
extern NSString * const CEXKEYPremises;

/**
 * @brief Parameter key to set Cardholder Activated Terminal Level to Automated Dispensing Machine
 **/
extern NSString * const CEXKEYCatLevelAutomatedDispensingMachine;

/**
 * @brief Parameter key to set Cardholder Activated Terminal Level to Self Service
 **/
extern NSString * const CEXKEYCatLevelSelfService;

/**
 * @brief Parameter key for keyed.
 **/
extern NSString * const CEXKEYKeyed;

/**
 * @brief Parameter key for if Magstripe is supported.
 **/
extern NSString * const CEXKEYMagstripe;

/**
 * @brief Parameter for if contact EMV is supported.
 **/
extern NSString * const CEXKEYContactEmv;

/**
 * @brief Parameter for if contactless EMV is supported.
 **/
extern NSString * const CEXKEYContactlessEmv;

/**
 * @brief Parameter key for visa being supported.
 **/
extern NSString * const CEXKEYVisa;

/**
 * @brief Parameter key for mastercard being supported.
 **/
extern NSString * const CEXKEYMastercard;

/**
 * @brief Parameter key for amex being supported.
 **/
extern NSString * const CEXKEYAmex;

/**
 * @brief Parameter key for if 3-D Secure Version 1 is supported.
 **/
extern NSString * const CEXKEYThreeDSecureVersion1;

/**
 * @brief Parameter key for if 3-D Secure Version 2 is supported.
 **/
extern NSString * const CEXKEYThreeDSecureVersion2;

/**
 * @brief Parameter key for if signature is supported.
 **/
extern NSString * const CEXKEYSignature;

/**
 * @brief Parameter key for if online pin is supported.
 **/
extern NSString * const CEXKEYOnlinePin;

/**
 * @brief Parameter key for if pin retry is supported.
 **/
extern NSString * const CEXKEYPinRetry;

/**
 * @brief Parameter key for if offline pin is supported.
 **/
extern NSString * const CEXKEYOfflinePin;

/**
 * @brief Parameter key for if pin bypass is supported.
 **/
extern NSString * const CEXKEYPinBypass;

/**
 * @brief Parameter key for Consumer Device Cardholder Verification Method.
 **/
extern NSString * const CEXKEYCdcvm;

/**
 * @brief Parameter key if fall back is supported.
 **/
extern NSString * const CEXKEYFallBack;

/**
 * @brief Parameter key for if fall forward is supported.
 **/
extern NSString * const CEXKEYFallForward;

/**
 * @brief Parameter key for if single tap is supported.
 **/
extern NSString * const CEXKEYSingleTap;

/**
 * @brief Parameter key for if address verification is supported.
 **/
extern NSString * const CEXKEYAvs;

/**
 * @brief Parameter key for if card security code is supported
 **/
extern NSString * const CEXKEYCsc;

/**
 * @brief Parameter key for ecommerce URL.
 **/
extern NSString * const CEXKEYEcomUrl;

/**
 * @brief Parameter key for card capture being supported.
 **/
extern NSString * const CEXKEYCardCapture;

/**
 * @brief Parameter for if dynamic currency conversion is supported.
 **/
extern NSString * const CEXKEYDcc;

/**
 * @brief Parameter key for if multi currency is supported.
 **/
extern NSString * const CEXKEYMultiCurrency;

/**
 * @brief Parameter key for if partial auth is supported.
 **/
extern NSString * const CEXKEYPartialAuth;

/**
 * @brief Parameter key for if partial reversal is supported.
 **/
extern NSString * const CEXKEYPartialReversal;

/**
 * @brief Parameter key for if voice referral is supported.
 **/
extern NSString * const CEXKEYVoiceReferral;

/**
 * @brief Parameter key for if on device tipping is supported.
 **/
extern NSString * const CEXKEYOnDeviceTipping;

/**
 * @brief Parameter key for if end of day tipping is supported.
 **/
extern NSString * const CEXKEYEndOfDayTipping;

/**
 * @brief Parameter key for digital signature being supported.
 **/
extern NSString * const CEXKEYDigitalSignature;

/**
 * @brief Parameter key for the terminal having a screen.
 **/
extern NSString * const CEXKEYScreen;

/**
 * @brief Parameter key for the terminal having a printed.
 **/
extern NSString * const CEXKEYPrinter;

/**
 * @brief Parameter key for the terminal having a receipt.
 **/
extern NSString * const CEXKEYReceipt;


@end

NS_ASSUME_NONNULL_END
