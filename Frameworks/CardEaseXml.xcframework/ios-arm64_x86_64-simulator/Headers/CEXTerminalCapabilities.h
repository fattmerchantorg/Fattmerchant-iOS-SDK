//
//  CEXTerminalCapabilities.h
//  CardEaseXmlClient
//
//  Created by Mariya Marinova on 09/09/2020.
//  Copyright © 2020 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CEXTerminalCapabilitiesGeneric.h"
#import "CEXTerminalCapabilitiesCardInput.h"
#import "CEXTerminalCapabilitiesThreeDSecure.h"
#import "CEXTerminalCapabilitiesCatLevel.h"
#import "CEXTerminalCapabilitiesCardholderAuthentication.h"
#import "CEXTerminalCapabilitiesContactEMV.h"
#import "CEXTerminalCapabilitiesContactless.h"
#import "CEXTerminalCapabilitiesEcomCnp.h"
#import "CEXTerminalCapabilitiesFeatures.h"
#import "CEXParameters.h"
#import "CEXParameterKeys.h"
#import "CEXParameterValues.h"


/**
 *
 * Supplementary information for both the terminal and integration used to complete this transaction,
 * such as the device type and card authentication method.  The supply of this information is optional
 * for existing integrations and certifications, however newer integrations should populate this data.
 *
 */

@interface CEXTerminalCapabilities : NSObject

@property (strong) CEXTerminalCapabilitiesGeneric *terminalCapabilitiesGeneric;
@property (strong) CEXTerminalCapabilitiesCardInput *terminalCapabilitiesCardInput;
@property (strong) CEXTerminalCapabilitiesThreeDSecure *terminalCapabilitiesThreeDSecure;
@property (strong) CEXTerminalCapabilitiesCatLevel *terminalCapabilitiesCatLevel;
@property (strong) CEXTerminalCapabilitiesCardholderAuthentication *terminalCapabilitiesCardholderAuthentication;
@property (strong) CEXTerminalCapabilitiesContactEMV *terminalCapabilitiesContactEmv;
@property (strong) CEXTerminalCapabilitiesContactless *terminalCapabilitiesContactless;
@property (strong) CEXTerminalCapabilitiesEcomCnp *terminalCapabilitiesEcomCnp;
@property (strong) CEXTerminalCapabilitiesFeatures *terminalCapabilitiesFeatures;

/**
 * @brief Adds the generic properties for Terminal Capabilities.
 *
 * @param genericParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYAttendedType} (Optional) If the terminal is an Attended or Unattended terminal, with an EMV terminal this can be based on an EMV tag 9F35 Terminal Type. Values can be {@link CEXParameterValues#CEXVALUEAttendedTypeAttended} or {@link CEXParameterValues#CEXVALUEAttendedTypeUnattended}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYDeviceType} (Optional) If the device being used is merchant or consumer driven. This would be consumer if the transaction was completed via ecommerce. Values can be {@link CEXParameterValues#CEXVALUEDeviceTypeConsumer} or {@link CEXParameterValues#CEXVALUEDeviceTypeMerchant}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYMobilePos} (Optional) If the device is a Mobile Point Of Sale. If the terminal being used is connected to a mobile device e.g. phones, smartphones, tablets. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYPremises} (Optional) The type of merchant premises. If the terminal being used for this transaction is on the merchants property or not. Values can be {@link CEXParameterValues#CEXVALUEPremisesTypeOnPremises} or {@link CEXParameterValues#CEXVALUEPremisesTypeOffPremises}.</p>
 */
-(void) addGenericCapabilities:(CEXParameters *) genericParameters;

/**
 * @brief Adds the 3-D Secure properties for Terminal Capabilities.
 *
 * @param threeDSecureParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYThreeDSecureVersion1} (Optional) If 3-D secure version 1 is supported by the integration. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}}</p>
 * <p>{@link CEXParameterKeys#CEXKEYThreeDSecureVersion2} (Optional) If 3-D secure version 2 is supported by the integration. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}}</p>
 * <p>{@link CEXParameterKeys#CEXKEYVisa} (Optional) If Visa's 3-D Secure solution is supported by the integration. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYMastercard} (Optional) If Mastercard's 3-D Secure solution is supported by the integration. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYAmex} (Optional) If American Express's 3-D is supported by the integration. Values can be {@link CEXParameterValues#True} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 */
-(void) addThreeDSecureCapabilities:(CEXParameters *) threeDSecureParameters;

/**
 * @brief Adds the Cardholder Activated Terminal properties for Terminal Capabilities.
 *
 * @param catParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYCatLevelAutomatedDispensingMachine} (Optional) Unattended terminal that only supports PIN and doesn't support NoCVM. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}}</p>
 * <p>{@link CEXParameterKeys#CEXKEYCatLevelSelfService} (Optional) Unattended terminal that supports only NoCVM. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}}</p>
 * <p>A CAT device may have dual capability as a CAT 1 and a CAT 2 - Contactless only unattended terminal or Contact/Contactless unattended terminal that supports PIN and NoCVM. To indicate the device has dual capability, set both CatLevelAutomatedDispensingMachine and CatLevelSelfService to {@link ParameterValues#CEXVALUETrue}</p>
 */
-(void) addCardHolderActivatedTerminal:(CEXParameters *) catParameters;

/**
 * @brief Adds the Card Input properties for Terminal Capabilities.
 *
 * @param cardInputParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYKeyed} (Optional) If Keyed transactions are supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYDeviceType} (Optional) If Magstripe transactions are supported by the terminal. Values can be {@link CEXParameterValues#TCEXVALUErue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYContactEMV} (Optional) If ContactEMV transactions are supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYContactlessEMV} (Optional) If Contactless EMV transactions are supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 */
-(void) addCardInputCapabilities:(CEXParameters *) cardInputParameters;

/**
 * @brief Adds the Cardholder Authentication properties for Terminal Capabilities.
 *
 * @param cardholderAuthenticationParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYCdcvm} (Optional) If Consumer Device Cardholder Verification Method input is supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYPinBypass} (Optional) If PIN Bypass is supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYOfflinePin} (Optional) If Offline PIN is supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYPinRetry} (Optional) If PIN Retry is supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYOnlinePin} (Optional) If Online PIN is supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYSignature} (Optional) If Signature is supported by the terminal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 */
-(void) addCardHolderAuthenticationCapabilities:(CEXParameters *) cardholderAuthenticationParameters;

/**
 * @brief Adds the Card Input properties for Terminal Capabilities.
 *
 * @param contactEmvParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYFallBack} (Optional) If the terminal will allow fallback to Magstripe when EMV read fails. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 */
-(void) addContactEmvCapabilities:(CEXParameters *) contactEmvParameters;

/**
 * @brief Adds the Contactless properties for Terminal Capabilities.
 *
 * @param contactlessParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYMagstripe} (Optional) If Magstripe is supported by the terminal being used. Values can be {@link CEXParameterValues#True} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYSingleTap} (Optional) If Single Tap is supported by the terminal being used. For this to be true the terminal must support contactless Online PIN and be deployed within the EEA. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYFallForward} (Optional) If the terminal being used is able fall forward to contact. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 */
-(void) addContactlessCapabilities:(CEXParameters *) contactlessParameters;

/**
 * Adds the Card Input properties for Terminal Capabilities.
 *
 * @param ecomCnpParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYAvs} (Optional) If the integration supports the sending of Address Verification Service details. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYCsc} (Optional) If the integration supports the sending of Card Security Code details. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYEcomUrl} (Optional) The Ecommerce URL. This is the URL of the ecommerce website that has initiated this transaction. This value is a String.</p>
 */
-(void) addEcomCnpCapabilites:(CEXParameters *) ecomCnpCapabilities;

/**
 * @brief Adds the Card Input properties for Terminal Capabilities.
 *
 * @param featuresParameters {@link CEXParameters} collection which can contain:
 * <p>{@link CEXParameterKeys#CEXKEYPrinter} (Optional) If the terminal being used has a Printer. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYScreen} (Optional) If the terminal being used has a Screen. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYDigitalSignature} (Optional) Does the terminal support electronic signature on the device. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYEndOfDayTipping} (Optional) If End Of Day Tipping is supported. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYOnDeviceTipping} (Optional) If On Device Tipping is supported by the terminal being used and tip entry is possible at the point of sale. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYVoiceReferral} (Optional) Can the terminal support voice referral response and confirmation. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYPartialReversal} (Optional) Can the device perform a partial reversal. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYPartialAuth} (Optional) If Partial Authentication is supported. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYMultiCurrency} (Optional) If the terminal can support more than one currency. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYCardCapture} (Optional) If the terminal has the capability to capture and retain a payment card. For EMV terminals this is specified in EMV tag 9F33 Byte 3 Bit 6. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYDcc} (Optional) If Dynamic Currency Conversion is supported by the terminal being used. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 * <p>{@link CEXParameterKeys#CEXKEYReceipt} (Optional) If the terminal has a can print or send receipts. Values can be {@link CEXParameterValues#CEXVALUETrue} or {@link CEXParameterValues#CEXVALUEFalse}.</p>
 */
-(void) addFeaturesProperties:(CEXParameters *) featuresParameters;


@end

