/**
 * @author Creditcall Ltd
 * @brief A class holding all of the data that constitutes a request to CardEaseXML.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXRequest
 * @details <p>
 * The necessary components of the request should be specified (using the "setters").
 * The request can then be submitted to the CEXClient in order to obtain a CEXResponse.
 * </p>
 * <p>
 * For each request there are a number of optional and mandatory components
 * depending upon the type of the request.
 *</p><p>
 * In brief, these rules are:
 * </p>
 * <ul type="circle">
 * <li>All requests:</li>
 * <ul type="circle">
 * <li>RequestType - the type of the request</li>
 * <li>SoftwareName - the name of the software using the SDK</li>
 * <li>SoftwareVersion - the version of the software using the SDK</li>
 * <li>TerminalID - the ID of the terminal making the request</li>
 * <li>TransactionKey - the transaction key allocated to a terminal or set of
 * terminals</li>
 * </ul>
 * <li>Auth requests:</li>
 * <ul type="circle">
 * <li>Amount - the transaction amount</li>
 * <li>ICCTags or ManualType or Track2 - to specify the card details</li>
 * </ul>
 * <li>Conf requests:</li>
 * <ul type="circle">
 * <li> CardEaseReference - the reference of the transaction being confirmed</li>
 * </ul>
 * <li>Offline requests:</li>
 * <ul type="circle">
 * <li>Amount - the transaction amount</li>
 * <li>ICCTags or ManualType or Track2 - to specify the card details</li>
 * </ul>
 * <li>Refund requests:</li>
 * <ul type="circle">
 * <li>CardEaseReference - the reference of the transaction being refunded</li>
 * <li>ICCTags or ManualType or Track2 - to specify the card details</li>
 * </ul>
 * <li>Void requests:</li>
 * <ul type="circle">
 * <li>CardEaseReference - the reference of the transaction being made void</li>
 * <li>VoidReason - the reason for which this transaction is being made void</li>
 * </ul>
 * </ul>
 * <p>
 * If a manual request is being made the PAN, expiry date and expiry date format
 * should be present as a minimum.
 * </p>
 * @see CEXClient
 * @see CEXResponse
 */
#import <Foundation/Foundation.h>

#import "CEXAmountUnit.h"
#import "CEXIccTag.h"
#import "CEXRequestType.h"
#import "CEXVoiceReferralResult.h"
#import "CEXVoidReason.h"
#import "CEXAddress.h"
#import "CEXEmailAddress.h"
#import "CEXName.h"
#import "CEXPhoneNumber.h"
#import "CEXProduct.h"
#import "CEXExtendedProperty.h"
#import "CEXSubType.h"
#import "CEXThreeDSecureCardHolderEnrolled.h"
#import "CEXThreeDSecureTransactionStatus.h"
#import "CEXXIDFormat.h"
#import "CEXIAVFormat.h"
#import "CEXAmountType.h"
#import "CEXDcc.h"
#import "CEXPointOfInteraction.h"
#import "CEXPurchaseLevel2Data.h"
#import "CEXBillingInformation.h"
#import "CEXCredentialOnFile.h"
#import "CEXFeatureToken.h"
#import "CEXTerminalCapabilities.h"
#import "CEXTransactionLink.h"

#define CEXRequestValidationError   -1

@class CEXXmlWriter;

/// @file

@interface CEXRequest : NSObject
{
	NSString *cardEaseXmlVersion;
}

/**
 * @brief The address details associated with the card in this request.
 *
 * This
 * can be used for additional verification of the card details with the
 * issuer. The content of this is dependant upon the country in which
 * authorisation is being performed. Typically it is the first line of the
 * address where the card is registered.  This is an alphanumeric string.
 * It is optional.
 *
 * @property address
 **/
@property (strong) NSString *address;

/**
 * @brief The amount associated with this request.
 *
 * This may be in major or
 * minor units. For example 1.23 GBP (Major) == 123 GBP (Minor). The amount is
 * mandatory for Auth and Offline requests.
 *
 * @property amount
 **/
@property (strong) NSString *amount;

/**
 * @brief The cashback amount associated with this request.
 *
 * This must use the same units as Amount.
 *
 * @property amount
 **/
@property (strong) NSString *amountOther;

/**
 * @brief The cashback fee amount associated with this request.
 *
 * This must use the same units as Amount.
 *
 * @property amount
 **/
@property (strong) NSString *amountOtherTransactionFee;


/**
 * @brief The tip amount associated with this request.
 *
 * This must use the same units as Amount.
 *
 * @property amountTip
 * @see amount
 **/
@property (strong) NSString *amountTip;

/**
 * @brief The amount type.
 *
 * Specify whether the amount is Actual or Estimate
 *
 * @property amountType
 * @see CEXAmountType
 **/
@property CEXAmountType amountType;

/**
 * @brief The units in which the amount associated with this request is specified.
 *
 * This may be Major or Minor. For example 1.23 GBP (Major) == 123 GBP (Minor).
 * The default is Minor.
 *
 * @property amountUnit
 * @see CEXAmountUnit
 **/
@property CEXAmountUnit amountUnit;

/**
 * @brief AuthCode for a transaction that has been approved by some other means.
 *
 * Optional for Offline requests and VoiceReferral requests where transaction
 * has been approved by the merchant offline.
 *
 * Mandatory for VoiceReferral requests where the transaction has been
 * approved by the acquiring bank over the phone.
 *
 * @property authCode
 **/
@property (strong) NSString *authCode;

/**
 * @brief Whether an authorisation request is automatically confirmed without
 *        a confirmation request.
 *
 * By default is false, a confirmation request will be required for this transaction.
 *
 * @property autoConfirm
 **/
@property BOOL autoConfirm;

/**
 * @brief Whether the platform will allow multiple confirms.
 *
 * By default is true, multiple confirms will be allowed.
 *
 * @property allowMultipleConfirms
 **/
@property BOOL allowMultipleConfirms;

/**
 * @brief The batch reference associated with this request.
 *
 * This allows the user to attach a reference to a transaction
 * to help group similar transactions.
 *
 * @property batchReference
 **/
@property (strong) NSString* batchReference;

/**
 * @brief The CardEase reference associated with this request.
 *
 * This is a unique reference that has been obtained from the CardEase platform during
 * previous requests.
 *
 * This is an alphanumeric string with a fixed length of 36 characters.
 * This is mandatory for Conf, Refund and Void requests.
 *
 * @property cardEaseReference
 **/
@property (strong) NSString *cardEaseReference;

/**
 * @brief The card hash returned from a previous transaction that references the card
 *        details that should also be used for this transaction.
 *
 * This is an alphanumeric string with a fixed length of 28 characters.
 * Used in conjunction with the CardReference property.
 *
 * The benefit of being able to reference a previously used card is that an integrator
 * need not store actual card details on their system for repeat transactions.
 *
 * This reduces the risk of card infomation being compromised, and reduces the integrators
 * PCI requirements.
 *
 * @property cardHash
 * @see CardReference
 **/
@property (strong) NSString *cardHash;

/**
 * @brief The card holder's address.
 *
 * @property cardHolderAddress
 **/
@property (strong) CEXAddress *cardHolderAddress;

/**
 * @brief The card holder's email addresses.
 *
 * @property cardHolderEmailAddresses
 **/
@property (strong) NSMutableArray *cardHolderEmailAddresses;

/**
 * @brief The card holder's name.
 * @property cardHolderName
 **/
@property (strong) CEXName *cardHolderName;

/**
 * @brief The card holder's phone numbers.
 * @property cardHolderPhoneNumbers
 **/
@property (strong) NSMutableArray *cardHolderPhoneNumbers;

/**
 * @brief The reference of an existing card to use for manual payment in place of the PAN, ExpiryDate etc.
 *
 * The card reference returned from a previous transaction that references the card details
 * that should also be used for this transaction.
 *
 * This is an alphanumeric string with a fixed length of 36 characters.
 * Used in conjunction with the CardHash property.
 *
 * The benefit of being able to reference a previously used card is that an integrator need
 * not store actual card details on their system for repeat transactions.
 *
 * This reduces the risk of card infomation being compromised, and reduces the
 * integrators PCI requirements.
 *
 * @property cardReference
 * @see CardHash
 **/
@property (strong) NSString *cardReference;

/**
 * @brief Whether the transaction was a contactless transaction.
 *
 * Default is false.
 *
 * @property contactless
 **/
@property BOOL contactless;

/**
 * @brief The security code associated with the card in this request.
 *
 * This can be used for additional verification with the issuer. This is also
 * referred to as CVV, CVC and CV2.
 *
 * This is an numeric string with a minimum length of 3 characters and a maximum
 * length of 4 characters.  This is optional.
 * If the CSC validation fails the authorisation is automatically declined.
 *
 * On Visa and MasterCard this is the last three digits of the signature strip.
 *
 * On Amex this is the four digits printed above the PAN.
 *
 * @property csc
 **/
@property (strong) NSString *csc;

/**
 * @brief The ISO currency code or mnemonic associated with this request amount.
 * <p>
 * For example, GBP or USD. If this is not specified the currency code held against
 * the terminal ID in the CardEase platform is assumed.
 *
 * This is an alphanumeric string with a fixed length of 3 characters.
 * </p><p>
 * Examples of recognised currency codes and mnemonics:
 * </p>
 * <table>
 * <tr>
 *   <th>Currency Code</th>
 *   <th>Mnemonic</th>
 *   <th>Description</th>
 * </tr>
 * <tr>
 *   <td>826</td>
 *   <td>GBP</td>
 *   <td>United Kingdom, Pound</td>
 * </tr>
 * <tr>
 *   <td>840</td>
 *   <td>USD</td>
 *   <td>United States, Dollar</td>
 * </tr>
 * <tr>
 *   <td>978</td>
 *   <td>EUR</td>
 *   <td>European Euro</td>
 * </tr>
 * <tr>
 *   <td>124</td>
 *   <td>CAD</td>
 *   <td>Canada, Dollar</td>
 * </tr>
 * <tr>
 *   <td>392</td>
 *   <td>JPY</td>
 *   <td>Japan, Yen</td>
 * </tr>
 * <tr>
 *   <td>208</td>
 *   <td>DKK</td>
 *   <td>Denmark, Krone</td>
 * </tr>
 * <tr>
 *   <td>756</td>
 *   <td>CHF</td>
 *   <td>Switzerland, Franc</td>
 * </tr>
 * <tr>
 *   <td>752</td>
 *   <td>SEK</td>
 *   <td>Sweden, Krona</td>
 * </tr>
 * </table>
 *
 * @property currencyCode
 **/
@property (strong) NSString *currencyCode;

/**
 * @brief  Whether the transaction is a debt repayment. Defaults to false.
 * @property debtRepayment
 **/
@property BOOL debtRepayment;

/**
 * @brief The delivery address.
 * @property deliveryAddress
 **/
@property (strong) CEXAddress *deliveryAddress;

/**
 * @brief The delivery email addresses.
 * @property deliveryEmailAddresses
 **/
@property (strong) NSMutableArray *deliveryEmailAddresses;

/**
 * @brief The delivery name.
 * @property deliveryName
 **/
@property (strong) CEXName *deliveryName;

/**
 * @brief The delivery phone numbers.
 * @property deliveryPhoneNumbers
 **/
@property (strong) NSMutableArray *deliveryPhoneNumbers;

/**
 * @brief The expiry date associated with the card in this request.
 *
 * This is a character string with a maximum length of 10 characters.
 * This is mandatory for manual authorisation requests (such as Card Not Present).
 * This should match the expiry date format.
 *
 * @property expiryDate
 * @see expiryDateFormat
 * @see startDate
 * @see startDateFormat
 * @see manualType
 **/
@property (strong) NSString *expiryDate;

/**
 * @brief The expiry date format associated with the card in this request.
 *
 * This is a character string with a maximum length of 10 characters.
 * This is mandatory for manual authorisation requests (such as Card Not Present).
 * By default this is "yyMM". This should match the format of the expiry date and
 * can include separators such as - and /.
 *
 * The available options are shown in the following table:
 *
 * <table>
 * <tr>
 *   <th>Format</th>
 *   <th>Description</th>
 *   <th>Example</th>
 * </tr>
 * <tr>
 *   <td>yyyy</td>
 *   <td>Year with century</td>
 *   <td>2004</td>
 * </tr>
 * <tr>
 *   <td>yy</td>
 *   <td>Year without century</td>
 *   <td>04</td>
 * </tr>
 * <tr>
 *   <td>MM</td>
 *   <td>Month of year</td>
 *   <td>01</td>
 * </tr>
 * <tr>
 *   <td>dd</td>
 *   <td>Day of month</td>
 *   <td>27</td>
 * </tr>
 * </table>
 *
 * @property expiryDateFormat
 * @see expiryDate
 * @see manualType
 **/
@property (strong) NSString *expiryDateFormat;

/**
 * @brief The list of Feature Tokens associated with this request.
 *
 * Default is nil.
 *
 * @see CEXFeatureToken
 * @see addFeatureToken
 * @property featureTokens
 **/
@property (strong) NSMutableArray *featureTokens;

/**
 * @brief Whether the transaction was a fallback from EMV to magnetic strip.
 *
 * Default is false.
 *
 * @property iccFallback
 **/
@property BOOL iccFallback;

/**
 * @brief The type of ICC transaction associated with this request.
 *
 * This is an alphanumeric string.  This is mandatory for ICC authorisations
 * and by default is "EMV".
 *
 * An ICC transaction must have associated ICC tags.
 *
 * @see CEXIccTag
 * @see addIccTag
 * @see iccTags
 * @property iccType
 **/
@property (strong) NSString *iccType;

/**
 * @brief The invoice address.
 * @property invoiceAddress
 **/
@property (strong) CEXAddress *invoiceAddress;

/**
 * @brief The invoice email addresses.
 * @property invoiceEmailAddresses
 **/
@property (strong) NSMutableArray *invoiceEmailAddresses;

/**
 * @brief The invoice name.
 * @property invoiceName
 **/
@property (strong) CEXName *invoiceName;

/**
 * @brief The invoice phone numbers.
 * @property invoicePhoneNumbers
 **/
@property (strong) NSMutableArray *invoicePhoneNumbers;

/**
 * @brief The issue number associated with the card in this request.
 *
 * This is a numeric string with a maximum length of 2 characters. The requirement
 * for this is dependant upon the card scheme associated with the card and must be
 * exactly as found on the card (including and leading 0's).
 *
 * @property issueNumber
 **/
@property (strong) NSString *issueNumber;

/**
 * @brief The machine reference associated with this request.
 *
 * This is mandatory if the TerminalID is a Master Terminal ID used to represent
 * multiple terminals.
 *
 * This is an alphanumeric string with a maximum length of 50 characters.
 *
 * @see terminalId
 * @property machineReference
 **/
@property (strong) NSString *machineReference;

/**
 * @brief The type of manual authorisation being used for this request.
 *
 * By default this is "cnp" (i.e. Card Not Present).
 * This is an alphanumeric string.
 * This is mandatory for manual authorisations.
 *
 * @property manualType
 **/
@property (strong) NSString *manualType;

/**
 * @brief The date and/or time of the transaction if processed offline.
 * @property offlineDateTime
 **/
@property (strong) NSString *offlineDateTime;

/**
 * @brief The format of the date and/or time of the transaction if processed offline.
 *
 * This is a character string with a maximum length of 16 characters.
 *
 * By default this is "ddMMyy hhmmss".
 * This should match the format of the offline date/time and can include separators such as - and /.
 * The available options are shown in the following table:
 * <table>
 * <tr>
 *   <th>Format</th>
 *   <th>Description</th>
 *   <th>Example</th>
 * </tr>
 * <tr>
 *   <td>yyyy</td>
 *   <td>Year with century</td>
 *   <td>2004</td>
 * </tr>
 * <tr>
 *   <td>yy</td>
 *   <td>Year without century</td>
 *   <td>04</td>
 * </tr>
 * <tr>
 *   <td>MM</td>
 *   <td>Month of year</td>
 *   <td>01</td>
 * </tr>
 * <tr>
 *   <td>dd</td>
 *   <td>Day of month</td>
 *   <td>27</td>
 * </tr>
 * <tr>
 *   <td>hh</td>
 *   <td>Hour</td>
 *   <td>12</td>
 * </tr>
 * <tr>
 *   <td>mm</td>
 *   <td>Minute</td>
 *   <td>54</td>
 * </tr>
 * <tr>
 *   <td>ss</td>
 *   <td>Second</td>
 *   <td>22</td>
 * </tr>
 * </table>
 *
 * @see offlineDateTime
 * @property offlineDateTimeFormat
 **/
@property (strong) NSString *offlineDateTimeFormat;

/**
 * @brief The originating IP address of the request (e.g. client browser).
 *
 * @property originatingIpAddress
 **/
@property (strong) NSString *originatingIpAddress;

/**
 * @brief The PAN (Primary Account Number) associated with the card in this request.
 *
 * This is a numeric string with a minimum length of 13 characters and a maximum length of 19 characters.
 *
 * This is a requirement for manual authorisation requests (such as Card Not Present).
 *
 * @property pan
 **/
@property (strong) NSString *pan;

/**
 * @brief The product list associated with this request.
 *
 * @property products
 **/
@property (strong) NSMutableArray *products;

/**
 * @brief The type of the request
 *
 * This can be Auth, Conf, Test and so on.
 *
 * By default this is "Auth".
 * This is mandatory for all requests.
 *
 * @see CEXRequestType
 * @property requestType
 **/
@property CEXRequestType requestType;

/**
 * @brief The name of the software/firmware using the CardEaseXML SDK.
 *
 * This is an alphanumeric string with a maximum length of 50 characters.
 *
 * This is mandatory for all requests.
 *
 * @property softwareName
 * @see softwareVersion
 **/
@property (strong) NSString *softwareName;

/**
 * @brief The version of the software/firmware using the CardEaseXML SDK.
 *
 * This is an alphanumeric string with a maximum length of 20 characters.
 *
 * This is mandatory for all requests.
 *
 * @property softwareVersion
 * @see softwareName
 **/
@property (strong) NSString *softwareVersion;

/**
 * @brief The start date associated with the card in this request.
 *
 * This is a character string with a maximum length of 10 characters.
 *
 * This is optional for manual authorisation requests (such as Card Not Present).
 *
 * This should match the start date format.
 *
 * @see startDateFormat
 * @see expiryDate
 * @see expiryDateFormat
 * @see manualType
 * @property startDate
 **/
@property (strong) NSString *startDate;

/**
 * @brief The start date format associated with the card in this request.
 *
 * This is a character string with a maximum length of 10 characters.
 *
 * This is optional for manual authorisation requests (such as Card Not Present).
 *
 * By default this is "yyMM".
 *
 * This should match the format of the start date and can include separators such as - and /.
 *
 * The available options are shown in the following table:
 *
 * <table>
 * <tr>
 *   <th>Format</th>
 *   <th>Description</th>
 *   <th>Example</th>
 * </tr>
 * <tr>
 *   <td>yyyy</td>
 *   <td>Year with century</td>
 *   <td>2004</td>
 * </tr>
 * <tr>
 *   <td>yy</td>
 *   <td>Year without century</td>
 *   <td>04</td>
 * </tr>
 * <tr>
 *   <td>MM</td>
 *   <td>Month of year</td>
 *   <td>01</td>
 * </tr>
 * <tr>
 *   <td>dd</td>
 *   <td>Day of month</td>
 *   <td>27</td>
 * </tr>
 * </table>
 *
 * @see startDate
 * @see manualType
 * @property startDateFormat
 **/
@property (strong) NSString *startDateFormat;

/**
 * @brief The sub type of the request.
 *
 * Currently applicable to ICCManagement requests.
 *
 * @see requestType
 * @property subType
 **/
@property (strong) NSString *subType;

/**
 * @brief The terminal ID associated with the machine performing this request.
 *
 * This is mandatory for all requests and is supplied by CreditCall Ltd.
 *
 * It is unique across all CardEase products.
 *
 * It is a numeric string with a fixed length of 8 characters.
 *
 * @property terminalId
 **/
@property (strong) NSString *terminalId;

/**
 * @brief The terminal capabilities associated with the terminal.
 */
@property CEXTerminalCapabilities *terminalCapabilities;

/**
 * @brief The 3-D Secure card holder enrollment.
 *
 * This is required for authorisations in which the liability shift is possible
 * due to the integration with a 3-D Secure MPI.
 *
 * @see threeDSecureCardHolderEnrolled
 * @see threeDSecureECI
 * @see threeDSecureIAV
 * @see threeDSecureIAVAlgorithm
 * @see threeDSecureTransactionStatus
 * @see threeDSecureXID
 * @see threeDSecureDirectoryServerTransactionId
 * @see threeDSecureServerTransactionId
 * @property threeDSecureCardHolderEnrolled
 **/
@property CEXThreeDSecureCardHolderEnrolled threeDSecureCardHolderEnrolled;

/**
 * @brief The 3-D Secure Transaction status.
 *
 * This is required for authorisations in which the liability shift is possible due to the
 * integration with a 3-D Secure MPI.
 *
 * @see threeDSecureCardHolderEnrolled
 * @see threeDSecureECI
 * @see threeDSecureIAV
 * @see threeDSecureIAVAlgorithm
 * @see threeDSecureXID
 * @see threeDSecureDirectoryServerTransactionId
 * @see threeDSecureServerTransactionId
 * @property threeDSecureTransactionStatus
 **/
@property CEXThreeDSecureTransactionStatus threeDSecureTransactionStatus;

/**
 * @brief The 3-D Secure Directory Server Transaction ID.
 *
 * This is required for 3-D Secure Version 2.
 *
 * This is formatted as a GUID.
 *
 * @see threeDSecureVersion
 * @see threeDSecureServerTransactionId
 * @property threeDSecureDirectoryServerTransactionId
 **/
@property NSString *threeDSecureDirectoryServerTransactionId;

/**
 * @brief The 3-D Secure Server Transaction ID.
 *
 * This is formatted as a GUID.
 *
 * @see threeDSecureVersion
 * @see threeDSecureDirectoryServerTransactionId
 * @property threeDSecureServerTransactionId
 **/
@property NSString *threeDSecureServerTransactionId;

/**
 * @brief The 3-D Secure Electronic Commerce Indicator.
 *
 * This is required for authorisations in which a liability shift is possible
 * due to the integration with a 3-D Secure MPI.
 *
 * It is a numeric string with a length of 2 characters.
 *
 * @see threeDSecureECI
 * @see threeDSecureCardHolderEnrolled
 * @see threeDSecureIAV
 * @see threeDSecureIAVAlgorithm
 * @see threeDSecureTransactionStatus
 * @see threeDSecureXID
 * @see threeDSecureDirectoryServerTransactionId
 * @see threeDSecureServerTransactionId
 * @property threeDSecureECI;
 **/
@property (strong) NSString *threeDSecureECI;

/**
 * @brief The 3-D Secure Authentication Verification value.
 *
 * This is required for authorisations in which the liability shift is possible
 * due to the integration with a 3-D Secure MPI.
 *
 * It is an alphanumeric string with a maximum size of 32 characters.
 *
 * With Verified by Visa this is called CAVV.
 *
 * With MasterCard SecureCode this is called AAV.
 *
 * @see threeDSecureCardHolderEnrolled
 * @see threeDSecureECI
 * @see threeDSecureIAVAlgorithm
 * @see threeDSecureIAVFormat
 * @see threeDSecureTransactionStatus
 * @see threeDSecureXID
 * @see threeDSecureDirectoryServerTransactionId
 * @see threeDSecureServerTransactionId
 * @property threeDSecureIAV;
 **/
@property (strong) NSString *threeDSecureIAV;

/**
 * @brief The 3-D Secure Authentication Verification algorithm.
 *
 * This is required for authorisations in which the liability shift is possible
 * due to the integration with a 3-D Secure MPI.
 *
 * @see threeDSecureCardHolderEnrolled
 * @see threeDSecureECI
 * @see threeDSecureIAV
 * @see threeDSecureIAVFormat
 * @see threeDSecureTransactionStatus
 * @see threeDSecureXID
 * @see threeDSecureDirectoryServerTransactionId
 * @see threeDSecureServerTransactionId
 * @property threeDSecureIAVAlgorithm
 **/
@property (strong) NSString *threeDSecureIAVAlgorithm;

/**
 * @brief The 3-D Secure Authentication Verification value format.
 *
 * This can be either Base64 or AsciiHex.
 *
 * The default is Base64.
 *
 * @see threeDSecureIAV
 * @property threeDSecureIAVFormat
 **/
@property CEXIAVFormat threeDSecureIAVFormat;

/**
 * @brief The 3-D Secure Version being used.
 *
 * This value can be retrieved from the 3-D Secure Server.
 * It is required for 3-D Secure version 2 and above.
 *
 * @see threeDSecureDirectoryServerTransactionId
 * @see threeDSecureServerTransactionId
 * @property threeDSecureVersion
 **/
@property NSString *threeDSecureVersion;

/**
 * @brief The 3-D Secure Transaction Identifier.
 *
 * This is required for authorisations in which the liability shift is possible due to the
 * integration with a 3-D Secure MPI.
 *
 * It is an alphanumeric string with a maximum length of of 28 characters.
 *
 * @see threeDSecureCardHolderEnrolled
 * @see threeDSecureECI
 * @see threeDSecureIAV
 * @see threeDSecureIAVAlgorithm
 * @see threeDSecureTransactionStatus
 * @see threeDSecureXIDFormat
 * @see threeDSecureDirectoryServerTransactionId
 * @see threeDSecureServerTransactionId
 * @property threeDSecureXID
 **/
@property (strong) NSString *threeDSecureXID;

/**
 * @brief The 3-D Secure Transaction Identifier format.
 *
 * This can be either Base64, Ascii or AsciiHex.
 *
 * The default is Ascii.
 * @see threeDSecureXID
 * @property threeDSecureXIDFormat
 **/
@property CEXXIDFormat threeDSecureXIDFormat;

/**
 * @brief The track 1 associated with the card in a magnetic stripe authorisation.
 *
 * This is an alphanumeric string with a maximum length of 79 characters.
 *
 * This is optional.
 *
 * @property track1
 **/
@property (strong) NSString *track1;

/**
 * @brief The track 2 associated with the card in a magnetic stripe authorisation.
 *
 * This is an alphanumeric string with a maximum length of 40 characters.
 *
 * This should include start and end sentinels (and separator character if provided).
 *
 * It is mandatory for magnetic stripe authorisation.
 *
 * @property track2
 **/
@property (strong) NSString *track2;

/**
 * @brief The track 3 associated with the card in a magnetic stripe authorisation.
 *
 * This is an alphanumeric string with a maximum length of 107 characters.
 *
 * This is optional.
 *
 * @property track3
 **/
@property (strong) NSString *track3;

/**
 * @brief The Encrypted PAN key entry data associated with this request.
 *
 * This is an alphanumeric string with no maximum length.
 *
 * This is optional.
 *
 * @property encryptedPANKeyEntryData
 **/
@property (strong) NSString *encryptedPANKeyEntryData;

/**
 * @brief The transaction key associated with this request.
 *
 * This is mandatory for all requests and is supplied by CreditCall Ltd for a
 * terminal or number of terminals.
 *
 * It must be in exactly the same case as provided by CreditCall.
 *
 * This is an alphanumeric string with a maximum length of 20 characters.
 *
 * @property transactionKey
 **/
@property (strong) NSString *transactionKey;

/**
 * @brief List of TransactionLinks associated with the auth request.
 *
 * It is optional.
 *
 * @property transactionKey
 **/
@property (strong) NSMutableArray *transactionLinks;

/**
 * @brief The user reference associated with this request.
 *
 * This allows a user to attach their own reference against a request.
 *
 * This is an alphanumeric string with a maximum length of 50 characters.
 *
 * This is optional for all requests.
 *
 * @property userReference.
 **/
@property (strong) NSString *userReference;

/**
 * @brief The result of a voice referral call.
 *
 * This is mandatory for voice referral requests.
 *
 * @property voiceReferralResult
 **/
@property CEXVoiceReferralResult voiceReferralResult;

/**
 * @brief The reason for which a void request is being made.
 *
 * This is mandatory for void requests.
 *
 * @property voidReason
 **/
@property CEXVoidReason voidReason;

/**
 * @brief The zip code/post code details associated with the card in this request.
 *
 * This can be used for additional verification with the issuer.
 *
 * The content of this is dependant upon the country in which authorisation is being performed.
 *
 * This is an alphanumeric string.
 *
 * It is optional.
 *
 * @property zipCode
 **/
@property (strong) NSString *zipCode;

/**
 * @brief The poin of interaction information
 * @property pointOfInteraction
 */
@property (strong) CEXPointOfInteraction *pointOfInteraction;

/**
 * @brief Sets the API Key associateed with the register request.
 *
 * This is mandatory only for Register requests.
 *
 * It must be a valid API Key supplied by the Gateway
 *
 * This is a 32 character string.
 *
 * @property apiKey
 */
@property (strong) NSString *apiKey;

/**
 * @brief Sets the POS GUID associated with the register request
 *
 * This is mandatory only for Register requests.
 *
 * It is suppled with the terminal installation.
 *
 * @property posGuid
 */
@property (strong) NSString *posGuid;

/**
 * @brief Sets the application ID associated with the register request
 *
 * This is mandatory only for Register requests.
*
 * @property applicationId
 */
@property (strong) NSString *applicationId;

/**
 * @brief Sets the Gateway Transaction ID associated with this request.
 *
 * This unique transaction ID is obtained from the CardEase platform as part of the auth response.
 *
 * This is mandatory for Conf and Void requests using an API Key.
 *
 * This is an int32 format.
 *
 * @property transactionId
 */
@property (strong) NSString *transactionId;

/**
 * @brief Sets the merchant defined values associated with this request
 *
 * This dictionary must have a key and value of type NSString
 *
 * @property merchantDefinedFields
 */
@property (strong) NSDictionary<NSString*, NSString*> *merchantDefinedFields;

/**
 * @brief Sets the billing information associated with this request
 *
 * @property billingInformation
 */
@property CEXBillingInformation *billingInformation;

/**
 * @brief Sets the purchase level 2 data associated with this request
 *
 * @property purchaseLevel2Data
 */
@property CEXPurchaseLevel2Data *purchaseLevel2Data;

/**
 * @brief The DCC data for the transaction.
 *
 * It is optional.
 *
 * @property cexDcc
 **/
@property CEXDcc *cexDcc;

/**
 * @brief The Credential-on-File associated with the request.
 *
 *
 * @property credentialOnFile
 **/
@property CEXCredentialOnFile *credentialOnFile;

/**
 * @brief The Lane ID associated with this request for the Omni Gateway.
 *
 * This can be used to uniquely identify the Point of Sale in order to be compliant with Mastercard requirements.
 * The format varies from processor to processor.
 *
 * @property laneId
*/
@property (strong) NSString *laneId;

/**
 * @brief The Merchant Processor ID associated with this request for the Omni Gateway.
 *
 * Multiple merchant processors can be configured against an account. This can be used to specify which one
 * of those merchant processors should be used for this request.
 *
 * This must be a numeric string.
 *
 * @property merchantProcessorId
*/
@property (strong) NSString *merchantProcessorId;

/**
 * @brief The Customer Vault Command associated with this request for the Omni Gateway.
 *
 * Currently valid values are add-customer or update-customer
 *
 * @property customerVaultCommand
*/
@property (strong) NSString *customerVaultCommand;

/**
 * @brief The Customer Vault ID associated with this request for the Omni Gateway.
 *
 * If the Customer Vault Command is add-customer this is optional, if the Customer Vault ID is not supplied it will be generated.
 *
 * If the Customer Vault Command is update-customer then this field is mandatory. The value must be one that was previously
 * used with an add-customer Customer Vault Command.
 *
 * @property customerVaultId
*/
@property (strong) NSString *customerVaultId;

//leaving uncommented intentionally;
@property BOOL omniSale;

/**
 * @brief The certification identifier for the transactions being processed.
 *
 * The certification identifier indicate which certification is being used for the transaction.
 */
@property (strong) NSString *certificationIdentifier;

/**
 * @brief The platform associated with the terminal
 *
 * The platform that the terminal is running.
 */
@property (strong) NSString *platform;

/**
 * @brief The platform version number associated with the terminal
 *
 * The version of the platform being used by the terminal.
 */
@property (strong) NSString *platformVersion;
/**
 * @brief Initialises a new CEXRequest instance.
 *
 * @return An initialised CEXRequest instance.
 **/
-(CEXRequest *)init;

/**
 * @brief Adds an email address to the list of email addresses associated with the card holder.
 *
 * @param emailAddress The email address to add.  Should not be null.
 * @see cardHolderEmailAddresses
 * @see CEXEmailAddress
 **/
- (void)addCardHolderEmailAddress:(CEXEmailAddress*) emailAddress;

/**
 * @brief Adds a phone number to the list of phone numbers associated with the card holder.
 *
 * @param phoneNumber The phone number to add.
 * @see cardHolderPhoneNumbers
 * @see CEXPhoneNumber
 */
- (void)addCardHolderPhoneNumber:(CEXPhoneNumber*) phoneNumber;

/**
 * @brief Adds an email address to the list of email addresses associated with the delivery address.
 *
 * @param emailAddress The email address to add.
 * @see CEXEmailAddress
 * @see deliveryEmailAddresses
 **/
- (void)addDeliveryEmailAddress:(CEXEmailAddress*) emailAddress;

/**
 * @brief Adds a phone number to the list of phone numbers associated with the delivery address.
 *
 * @param phoneNumber The phone number to add.
 * @see deliveryPhoneNumbers
 * @see CEXPhoneNumber
 **/
- (void)addDeliveryPhoneNumber:(CEXPhoneNumber*) phoneNumber;

/**
 * @brief Adds a phone number to the list of phone numbers associated with the invoice address.
 *
 * @param phoneNumber The phone number to add.  Should not be null.
 * @see invoicePhoneNumbers
 * @see CEXPhoneNumber
 **/
- (void)addInvoicePhoneNumber:(CEXPhoneNumber*) phoneNumber;

/**
 * @brief Adds an email address to the list of email addresses associated with the invoice address.
 *
 * @param emailAddress The email address to add.
 * @see invoiceEmailAddresses
 * @see CEXEmailAddress
 **/
- (void)addInvoiceEmailAddress:(CEXEmailAddress*) emailAddress;

/**
 * @brief Adds a product to the list of products associated with this request.
 *
 * @param product The product to add.  Should not be null.
 * @see products
 * @see CEXProduct
 **/
- (void)addProduct:(CEXProduct*) product;

/**
 * @brief Adds an extended property to the list of extended properties associated with this request.
 *
 * @param extendedProperty The extended property to add to the list of extended properties
 *                         associated with this request.
 *                         This should not be null.
 *
 * @see extendedProperties
 * @see CEXExtendedProperty
 **/
- (void)addExtendedProperty:(CEXExtendedProperty*) extendedProperty;

/**
 * @brief Adds a feature token to the list of feature tokens associated with this request.
 *
 * @param featureToken The feature token to add to the list of feature tokens
 *                      associated with this request.
 *
 * @see featureTokens
 * @see CEXFeatureToken
 **/
- (void)addFeatureToken:(CEXFeatureToken*) featureToken;

/**
 * @brief Adds an ICC tag to the list of ICC tags associated with this request.
 *
 * Each ICC tag has a tag, type and value.
 *
 * For example, a tag of 0x9f02/AsciiHex/000000000100 is using to specify the transaction amount.
 *
 * These are mandatory for an EMV transaction.
 *
 * @param tag The ICC tag to add to the list of ICC tags associated with this request.
 *            This should not be null.
 *
 * @see iccTags
 * @see CEXIccTag
 **/
- (void)addIccTag:(CEXIccTag *)tag;

/**
 * @brief Adds a transaction link to the list of transaction link associated with this request.
 *
 * @param transactionLink The transaction link to add to the list of transaction links
 *                      associated with this request.
 *
 * @see transactionLinks
 * @see CEXTransactionLink
 **/
- (void)addTransactionLink:(CEXTransactionLink*) transactionLink;

/**
 * @brief The list of extended properties associated with this transaction.
 *
 * @return The list of extended properties
 * @see addExtendedProperty
 * @see CEXExtendedProperty
 **/
- (NSArray *)extendedProperties;

/**
 * @brief The list of ICC tags associated with this request.
 *
 * Each ICC tag has a tag, type and value.
 * For example, a tag of 0x9f02/AsciiHex/000000000100 is using to specify
 * the transaction amount. These are mandatory for an EMV transaction.
 *
 * @see CEXIccTag
 * @see addIccTag
 * @see iccType
 * @return The list of iccTags
 **/
- (NSArray *)iccTags;

/**
 * @brief The list of extended properties associated with this transaction.
 *
 * @param theExtendedProperties The list of extended properties to use in the request.
 * @see addExtendedProperty
 * @see CEXExtendedProperty
 **/
- (void)setExtendedProperties:(NSArray *)extendedProperties;

/**
 * @brief The list of ICC tags associated with this request.
 *
 * Each ICC tag has a tag, type and value.
 * For example, a tag of 0x9f02/AsciiHex/000000000100 is using to specify
 * the transaction amount. These are mandatory for an EMV transaction.
 *
 * @see CEXIccTag
 * @see addIccTag
 * @see iccType
 * @property iccTags
 **/
- (void)setIccTags:(NSArray *)iccTags;
- (void)parseTerminalCapabilities:(CEXXmlWriter *)writer;
- (NSString *)generateRequestXml:(CEXXmlWriter *)writer;

@end
