/**
 * @author Creditcall Ltd
 * @brief This class represents a single field which may appear on a receipt.
 * @class ReceiptField
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

#import <Foundation/Foundation.h>

/**
 @defgroup kCCReceiptField
 @{
 */
/**
 @memberof ReceiptData
 Keys for retrieving {@link ReceiptField}s from the returned NSDictionarys.
 */
typedef NSString kCCReceiptField;
extern NSString * const kCCReceiptFieldHeader; /**< Dictionary key to retrieve the receipt header receipt field.*/
extern NSString * const kCCReceiptFieldMerchantNameAndAddress; /**< Dictionary key to retrieve the merchants name and address receipt field.*/
extern NSString * const kCCReceiptFieldApplicationId; /**< Dictionary key to retrieve the application id receipt field.*/
extern NSString * const kCCReceiptFieldAppPreferredName;/**< Dictionary key to retrieve the card applications preferred name field.*/
extern NSString * const kCCReceiptFieldMaskedCardNumber;/**< Dictionary key to retrieve the masked card number field.*/
extern NSString * const kCCReceiptFieldPANSequenceNumber;/**< Dictionary key to retrieve the pan sequence number field.*/
extern NSString * const kCCReceiptFieldTransactionSource;/**< Dictionary key to retrieve the transaction source field (eg. cash, cheque, ICC, swipe).*/
extern NSString * const kCCReceiptFieldTransactionType;/**< Dictionary key to retrieve the transaction type field (eg. sale, refund).*/
extern NSString * const kCCReceiptFieldGratuityAmount;/**< Dictionary key to retrieve the gratuity field. When printing this field this should display the currency symbol or code*/
extern NSString * const kCCReceiptFieldTransactionTotal;/**< Dictionary key to retrieve the transaction partially approved amount receipt field. When printing this field it should display the currency symbol or code*/
extern NSString * const kCCReceiptFieldTransactionAvailableOfflineSpendingAmount;/**< Dictionary key to retrieve the transaction available offline spending amount (balance) receipt field. When printing this field it should display the currency symbol or code. This field should only be printed on cardholder's receipt.*/
extern NSString * const kCCReceiptFieldTransactionPartialAmountAuthorised;/**< Dictionary key to retrieve the transaction total receipt field. When printing this field it should display the currency symbol or code*/
extern NSString * const kCCReceiptFieldTransactionCurrency; /**< Dictionary key to retrieve the currency receipt field. The value of this field is the 3 character code in ISO 4217*/
extern NSString * const kCCReceiptFieldCVM;/**< Dictionary key to retrieve the card verification method field.*/
extern NSString * const kCCReceiptFieldTransactionResult;/**< Dictionary key to retrieve the transactions result field.*/
extern NSString * const kCCReceiptFieldAuthCode;/**< Dictionary key to retrieve the transactions authorisation code field.*/
extern NSString * const kCCReceiptFieldCardEaseReference; /**< Dictionary key to retrieve the card ease reference field*/
extern NSString * const kCCReceiptFieldMerchantNumber; /**< Dictionary key to retrieve the merchant number field.*/
extern NSString * const kCCReceiptFieldTerminalID;/**< Dictionary key to retrieve the terminalID field.*/
extern NSString * const kCCReceiptFieldDateTime;/**< Dictionary key to retrieve the date time field.*/
extern NSString * const kCCReceiptFieldRetentionReminder; /**< Dictionary key to retrieve the retention reminder field.*/
extern NSString * const kCCReceiptFieldFooter;/**< Dictionary key to retrieve the receipt footer field.*/
extern NSString * const kCCReceiptFieldAuthMode;/**< Dictionary key to retrieve the Authorisation Mode field.*/
extern NSString * const kCCReceiptFieldTerminalVerificationResult;/**< Dictionary key to retrieve the Terminal Verification Result field.*/
extern NSString * const kCCReceiptFieldIssuerApplicationData;/**< Dictionary key to retrieve the Issuer Application Data field.*/
extern NSString * const kCCReceiptFieldTransactionStatusInformation; /**< Dictionary key to retrieve the Transaction Status Information field*/
extern NSString * const kCCReceiptFieldAuthorisationResponseCode; /**< Dictionary key to retrieve the Authorisation Response Code field.*/
extern NSString * const kCCReceiptFieldApplicationTransactionCounter;/**< Dictionary key to retrieve the Application Transaction Counter field.*/
extern NSString * const kCCReceiptFieldApplicationCryptogram;/**< Dictionary key to retrieve the Application Cryptogram field.*/
extern NSString * const kCCReceiptFieldCardScheme; /**< Dictionary key to retrieve the Card Scheme field.*/
extern NSString * const kCCReceiptFieldDescription;/**< Dictionary key to retrieve the Description field.*/
extern NSString * const kCCReceiptFieldExpiryDate;/**< Dictionary key to retrieve the Expiry Date field.*/
extern NSString * const kCCReceiptFieldValidationCode;/**< Dictionary key to retrieve the Validation Code field.*/
extern NSString * const kCCReceiptFieldAcquirerTransactionReference;/**< Dictionary key to retrieve the Acquirer Transaction Reference field.*/
extern NSString * const kCCReceiptFieldPaymentType; /**< Dictionary key to retrieve the Payment Type field*/
extern NSString * const kCCReceiptFieldBatchNumber; /**< Dictionary key to retrieve the Batch Number field.*/
extern NSString * const kCCReceiptFieldItemNumber;/**< Dictionary key to retrieve the Item Number field.*/
extern NSString * const kCCReceiptFieldVoiceReferral;/**< Dictionary key to retrieve the Voice referral field.*/
extern NSString * const kCCReceiptFieldTransactionSequenceCounter;/**< Dictionary key to retrieve the Transaction Sequence Counter.*/
extern NSString * const kCCReceiptFieldAgreement;/** < Dictionary key to retrieve the Agreement field.*/

//Offline decline data
extern NSString * const kCCReceiptFieldEmvTag5F2A; /**< Dictionary key to retrieve EMV tag 5F2A. */
extern NSString * const kCCReceiptFieldEmvTag82; /**< Dictionary key to retrieve EMV tag 82. */
extern NSString * const kCCReceiptFieldEmvTag84; /**< Dictionary key to retrieve EMV tag 84. */
extern NSString * const kCCReceiptFieldEmvTag9A; /**< Dictionary key to retrieve EMV tag 9A. */
extern NSString * const kCCReceiptFieldEmvTag9C; /**< Dictionary key to retrieve EMV tag 9C. */
extern NSString * const kCCReceiptFieldEmvTag9F1A; /**< Dictionary key to retrieve EMV tag 9F1A. */
extern NSString * const kCCReceiptFieldEmvTag9F1E; /**< Dictionary key to retrieve EMV tag 9F1E. */
extern NSString * const kCCReceiptFieldEmvTag9F02; /**< Dictionary key to retrieve EMV tag 9F02. */
extern NSString * const kCCReceiptFieldEmvTag9F03; /**< Dictionary key to retrieve EMV tag 9F03. */
extern NSString * const kCCReceiptFieldEmvTag9F09; /**< Dictionary key to retrieve EMV tag 9F09. */
extern NSString * const kCCReceiptFieldEmvTag9F10; /**< Dictionary key to retrieve EMV tag 9F10. */
extern NSString * const kCCReceiptFieldEmvTag9F26; /**< Dictionary key to retrieve EMV tag 9F26. */
extern NSString * const kCCReceiptFieldEmvTag9F27; /**< Dictionary key to retrieve EMV tag 9F27. */
extern NSString * const kCCReceiptFieldEmvTag9F33; /**< Dictionary key to retrieve EMV tag 9F33. */
extern NSString * const kCCReceiptFieldEmvTag9F34; /**< Dictionary key to retrieve EMV tag 9F34. */
extern NSString * const kCCReceiptFieldEmvTag9F35; /**< Dictionary key to retrieve EMV tag 9F35. */
extern NSString * const kCCReceiptFieldEmvTag9F36; /**< Dictionary key to retrieve EMV tag 9F36. */
extern NSString * const kCCReceiptFieldEmvTag9F37; /**< Dictionary key to retrieve EMV tag 9F37. */
extern NSString * const kCCReceiptFieldEmvTag95; /**< Dictionary key to retrieve EMV tag 95. */
extern NSString * const kCCReceiptFieldEmvTag50; /**< Dictionary key to retrieve EMV tag 50. */
extern NSString * const kCCReceiptFieldEmvTag5A; /**< Dictionary key to retrieve EMV tag 5A. */
extern NSString * const kCCReceiptFieldEmvTag5F34; /**< Dictionary key to retrieve EMV tag 5F34. */
extern NSString * const kCCReceiptFieldEmvTag9F21; /**< Dictionary key to retrieve EMV tag 9F21. */
extern NSString * const kCCReceiptFieldEmvTag9F07; /**< Dictionary key to retrieve EMV tag 9F07. */
extern NSString * const kCCReceiptFieldEmvTag9F0D; /**< Dictionary key to retrieve EMV tag 9F0D. */
extern NSString * const kCCReceiptFieldEmvTag9F0E; /**< Dictionary key to retrieve EMV tag 9F0E. */
extern NSString * const kCCReceiptFieldEmvTag9F0F; /**< Dictionary key to retrieve EMV tag 9F0F. */
extern NSString * const kCCReceiptFieldEmvTag9F12; /**< Dictionary key to retrieve EMV tag 9F12. */


extern NSString * const kCCReceiptFieldTerminalActionCodeDefault; /**< Dictionary key to retrieve the Default TAC. */;
extern NSString * const kCCReceiptFieldTerminalActionCodeDenial; /**< Dictionary key to retrieve the Denial TAC. */
extern NSString * const kCCReceiptFieldTerminalActionCodeOnline; /**< Dictionary key to retrieve the Online TAC. */

/**@}*/

@interface ReceiptField : NSObject

/**
 * An represents whether the ReceiptField should be inclused on the receipt.
 */
typedef enum {
    InclusionMandatory = 1, /**< ReceiptField InclusionManditory means a field must be displayed on the receipt*/
    InclusionOptional /**< ReceiptField InclusiontOptional means a field may or may not be included on a receipt*/
} Inclusion;

/**
 Gets the {@link Inclusion} of the receipt field
 
 @return {@link Inclusion} of the receiver.
 */
@property (nonatomic, readonly)Inclusion inclusion;

/**
 Gets the value of the receipt field.
 
 @return string representation of the receipt fields value
 */
@property (nonatomic, strong, readonly)NSString *value;

/**
 Gets the recommended label for the receipt field. This value may be nil.
 
 @return string representation of the recommended label or nil.
 */
@property (nonatomic, strong, readonly)NSString *labelString;

/**
 The placement priority of this ReceiptField.
 
 @return integer representing the order of placement for this receipt field.
 */
@property (nonatomic, readonly)NSInteger priority;

@end
