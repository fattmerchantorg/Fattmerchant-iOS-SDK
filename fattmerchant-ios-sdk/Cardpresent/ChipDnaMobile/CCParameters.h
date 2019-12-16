/**
 * @author Creditcall Ltd
 * @brief Parameters used in communication with ChipDNA Mobile.
 * @class CCParameters
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

#import <Foundation/Foundation.h>

/**
 * @defgroup CCParameterValues
 * @{
 */
/**
 * @memberof CCParameters
 * Parameter values used in communication with ChipDNA Mobile
 */
typedef NSString* CCParameterValue;
extern CCParameterValue const CCValueTrue; /**< @memberof CCParameters Parameter value representation of TRUE. */
extern CCParameterValue const CCValueFalse; /**< @memberof CCParameters Parameter value representation of FALSE. */

extern CCParameterValue const CCValueBluetooth; /**< @memberof CCParameters Parameter value representation of BLUETOOTH connection type. */
extern CCParameterValue const CCValueTcpIp; /**< @memberof CCParameters Parameter value representation of TCP IP connection type..*/

extern CCParameterValue const CCValueEnvironmentLive; /**< @memberof CCParameters Parameter value representation of LIVE environment. */
extern CCParameterValue const CCValueEnvironmentTest; /**< @memberof CCParameters Parameter value representation of STAGING/TEST environment. */

extern CCParameterValue const CCValueConnectionAndConfigureStarted; /**< @memberof CCParameters Parameters value indicating that {@link ChipDnaMobile#connectAndConfigure:} has started. */
extern CCParameterValue const CCValueConnecting; /**< @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is about to connect to the PIN pad. */
extern CCParameterValue const CCValueRegistering; /**< @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is about to perform a register query. */
extern CCParameterValue const CCValuePerformingTmsUpdate; /**< @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is about to perform a TMS Update. */
extern CCParameterValue const CCValueCheckingPinPadConfig; /**< @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is checking the configuration on the PIN pad. */
extern CCParameterValue const CCValueUpdatingPinPadConfig; /**< @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is updating the configuration on the PIN pad. */
extern CCParameterValue const CCValueUpdatingPinPadFirmware; /** @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is updating the firmware on the PIN pad. */
extern CCParameterValue const CCValueVerifyingPinPad; /**< @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is verifying the PIN pad. */
extern CCParameterValue const CCValueRebootingPinPad; /**< @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is rebooting the PIN pad. */
extern CCParameterValue const CCValueUpdatingSecurityConfig; /**< @memberof CCParameters Parameter value indicating that {@link ChipDnaMobile#connectAndConfigure:} is updating security configuration on the PIN pad. */

extern CCParameterValue const CCValueEndOfDayTipping; /**< @memberof CCParameters Parameter value indicating whether end of day tipping is supported or should be used. */
extern CCParameterValue const CCValueOnDeviceTipping; /**< @memberof CCParameters Parameter value indicating whether on device tipping is supported or should be used. */
extern CCParameterValue const CCValueBothTipping; /**< @memberof CCParameters Parameter value indicating the whether both on device and end of day tipping are supported or should be used. */

extern CCParameterValue const CCValueSale; /**< @memberof CCParameters Parameter value indicating that the transaction is a sale. */
extern CCParameterValue const CCValueRefund; /**< @memberof CCParameters Parameter value indicating that the transaction is a refund. */
extern CCParameterValue const CCValueCash; /**< @memberof CCParameters Parameter value indicating that the transaction will be processed as a cash transaction. */
extern CCParameterValue const CCValueCheque; /**< @memeberof CCParameters Parameter value indicating that the transaction will be processed as cheque transaction. */
extern CCParameterValue const CCValueCard; /**< @memberof CCParameters Parameter value indicating that the transaction will be processed with a card. */
extern CCParameterValue const CCValueAmountTypeActual; /**< @memberof CCParameters Parameter value indicating that {@link CCParameters#CCParamAmount} is the actual amount. */
extern CCParameterValue const CCValueAmountTypeEstimate; /**< @memberof CCParameters Parameter value indicating that {@link CCParameters#CCParamAmount} is the estimated amount. */

extern CCParameterValue const CCValueApproved; /**< @memberof CCParameters Parameter value returned when a transaction is approved. */
extern CCParameterValue const CCValueDeclined; /**< @memberof CCParameters Parameter value returned when a transaction is declined. */
extern CCParameterValue const CCValueRetryRequest; /**< @memberof CCParameters Parameter value returned when a request is required to be retried by the integrating application. */
extern CCParameterValue const CCValuePartiallyApproved; /**< @memberof CCParameters Parameter value returned when a transaction is partially approved. */
extern CCParameterValue const CCValueCommitted; /**< @memberof CCParameters Parameter value returned when the state of a transaction is committed. */
extern CCParameterValue const CCValueUncommitted; /**< @memberof CCParameters Parameter value returned when the state of a transaction is uncommitted. */
extern CCParameterValue const CCValueVoided; /**< @memeberof CCParameters Parameter value returned when the state of a transaction is void. */
extern CCParameterValue const CCValueUncommittedVoid; /**< @memberof CCParameters Parameter value returned when the state of a transaction is uncommitted void. */
extern CCParameterValue const CCValuePartiallyCommitted; /** @memberof CCParameter Parameter value returned when the state of a transaction is partially committed. */

extern CCParameterValue const CCValueOfflineFailed; /**< @memeberof CCParameters Parameter value to indicate that there is an offline request which has failed to send. */
extern CCParameterValue const CCValueOfflinePending; /**< @memeberof CCParameters Parameter value to indicate that there is an offline request pending for the transaction. */

extern CCParameterValue const CCValueDeferredAuthReasonConnectionFailed; /**< @memberof CCParameters Parameter value to indicate that a deferred authorization is being offered because of a connection failure.*/
extern CCParameterValue const CCValueDeferredAuthReasonCommunicationFailed; /**< @memberof CCParameters Parameter value to indicate that a deferred authorization is being offered because of a communication error during request processing. */
extern CCParameterValue const CCValueDeferredAuthReasonProcessingFailed; /**< @memberof CCParameters Parameter value to indicate that a deferred authorization is being offered because the platform failed to connect to the processor. */

extern CCParameterValue const CCValueOfflineCompletionStateProcessed; /**< @memberof CCParameters Parameter value indicating that the offline request was successfully completed and sent online. */
extern CCParameterValue const CCValueOfflineCompletionStateDeleted; /**< @memberof CCParameters Parameter value indicating that the offline request was deleted (and not sent online). */

/**
 * @}
 */

/**
 * @defgroup CCParameterKeys
 * @{
 */
/**
 * @memberof CCParameters
 * Parameter keys used in communication with ChipDNA Mobile
 */
typedef NSString* CCParameterKey;
extern CCParameterKey const CCParamResult; /**< @memberof CCParameters Parameter key for the result value. */
extern CCParameterKey const CCParamError; /**< @memberof CCParameters Parameter key used when returning error codes.  Multiple error codes may be returned in a comma separated list. */
extern CCParameterKey const CCParamErrorDescription; /**< @memberof CCParameters Parameter key for returning text description of an error. */

extern CCParameterKey const CCParamPassword; /**< @memberof CCParameters Parameter key for attempting a password during initialization. */
extern CCParameterKey const CCParamChangePassword; /**< @memberof CCParameters Parameter key for changing the password after initialization. */
extern CCParameterKey const CCParamDestroy; /**< @memberof CCParameters Parameter key to wipe all SDK app data and delete the configuration and offline database when value is @{link #CCValueTrue TRUE}.*/
extern CCParameterKey const CCParamRemainingAttempts; /**< @memberof CCParameters Parameter key for remaining attempts for initialization password before SDK app data is destroyed. */

extern CCParameterKey const CCParamTerminalId; /**< @memberof CCParameters Parameter key for setting terminal ID. */
extern CCParameterKey const CCParamTransactionKey; /**< @memberof CCParameters Parameter key for setting transaction key. */
extern CCParameterKey const CCParamEnvironment; /**< @memberof CCParameters Parameter key for setting and receive the current environment of ChipDNA Mobile. The value can be {@link CCParameters#CCValueEnvironmentLive production} or {@link CCParameters#CCValueEnvironmentTest test}. */
extern CCParameterKey const CCParamWebMisUsername; /**< @memberof CCParameters Parameter key for setting WebMIS username */
extern CCParameterKey const CCParamWebMisPassword; /**< @memberof CCParameters Parameter key for setting WebMIS password */
extern CCParameterKey const CCParamApplicationIdentifier; /**< @memberof CCParameters Parameter key for setting application identifier. */
extern CCParameterKey const CCParamApiKey; /**< @memberof CCParameters Parameter key for setting Api Key used for communicating with the gateway. */

extern CCParameterKey const CCParamPinPadName; /**< @memberof CCParameters Parameter key for setting the PIN pad name of the PIN pad to connect to. */
extern CCParameterKey const CCParamPinPadConnectionType; /**< @memberof CCParameters Parameter key for setting the connection type of the PIN pad to connect to. */
extern CCParameterKey const CCParamPinPadIpAddress; /**< @memberof CCParameters Parameter key for setting the IP address of the PIN pad to connect to.*/
extern CCParameterKey const CCParamPinPadPort;/**< @memberof CCParameters Parameter key for setting the IP port number of the PIN pad to connect to.*/

extern CCParameterKey const CCParamFullTmsUpdate; /**< @memberof CCParameters Parameter key to trigger a full TMS update. */
extern CCParameterKey const CCParamForceTmsUpdate; /**< @memberof CCParameters Parameter key to force a TMS update. */

extern CCParameterKey const CCParamVersionInformation; /**< @memberof CCParameters Parameter key for {@link VersionInformation} XML which can be converted using {@link ChipDnaMobileSerializer#deserializeVersionInformation:}. */
extern CCParameterKey const CCParamPaymentPlatformStatus; /**< @memberof CCParameters Parameter key for {@link PaymentPlatformStatus} XML which can be converted using {@link ChipDnaMobileSerializer#deserializePaymentPlatformStatus:}. */
extern CCParameterKey const CCParamRequestQueueStatus; /**< @memberof CCParameters Parameter key for {@link RequestQueueStatus} XML which can be converted using {@link ChipDnaMobileSerializer#deserializeRequestQueueStatus:}. */
extern CCParameterKey const CCParamChipDnaStatus; /**< @memberof CCParameters Parameter key for current status of ChipDNA. */
extern CCParameterKey const CCParamDeviceStatus; /**< @memberof CCParameters Parameter key for {@link DeviceStatus} XML which can be converted using {@link ChipDnaMobileSerializer#deserializeDeviceStatus:}. */
extern CCParameterKey const CCParamTmsStatus; /**< @memberof CCParameters Parameter key for {@link TmsStatus} XML which can be converted using {@link ChipDnaMobileSerializer#deserializeTmsStatus:}. */
extern CCParameterKey const CCParamTerminalStatus; /**< @memberof CCParameters Parameter key for {@link TerminalStatus} XML which can be converted using {@link ChipDnaMobileSerializer#deserailizeTerminalStatus:}. */
extern CCParameterKey const CCParamLinkedRefundsSupported; /**< @memberof CCParameters Parameter key indicating if linked refunds are supported. */
extern CCParameterKey const CCParamStandaloneRefundsSupported; /**< @memberof CCParameters Parameter key indicating if standalone refunds are supported. */
extern CCParameterKey const CCParamCashTransactionsSupported; /**< @memberof CCParameters Parameter key indicating if cash transactions are supported. */
extern CCParameterKey const CCParamChequeTransactionsSupported; /**< @memberof CCParameters Parameter key indicating if cheque transaction are supported. */
extern CCParameterKey const CCParamTransactionHistorySupported;/**< @memberof CCParameters Parameter key indicating if transaction history information is supported. */
extern CCParameterKey const CCParamOfflineProcessingSupported; /**< @memberof CCParameters Parameter key indicating if offline processing is supported. */
extern CCParameterKey const CCParamRefundOperatorPinSupported; /**< @memberof CCParameters Parameter key indicating if an operator PIN must be supplied in order to perform standalone refunds. */
extern CCParameterKey const CCParamEmailReceiptSupported; /**< @memberof CCParameters Parameter key indicating if receipting via email is supported. */
extern CCParameterKey const CCParamSmsReceiptSupported; /**< @memberof CCParameters Parameter key indicating if receipting via SMS is supported. */
extern CCParameterKey const CCParamTippingSupported; /**< @memberof CCParameters Parameter key indicating which version of tipping is supported. */

extern CCParameterKey const CCParamPaymentPlatformTest; /**< @memberof CCParameters Parameter key used to request the return of a {@link PaymentPlatformStatus} object. This will require online connectivity. */

extern CCParameterKey const CCParamAvailablePinPads; /**< @memberof CCParameters Parameter key for available PIN pad XML string. XML can be converted into an object using {@link ChipDnaMobileSerializer#deserializeAvailablePinPadsString: */
extern CCParameterKey const CCParamAccessoryConnectionType; /**< @memberof CCParameters Parameter key for connection type selected for accessory. */
extern CCParameterKey const CCParamAccessoryName; /**< @memberof CCParameters Parameter key for accessory name. */
extern CCParameterKey const CCParamTmsUpdateUtc; /**< @memberof CCParameters Parameter key for date/time of last TMS update time in UTC. */
extern CCParameterKey const CCParamConfigurationUpdate; /** @memberof CCParameters Parameter key for updates on the progress of {@link ChipDnaMobile#connectAndConfigure:}. */
extern CCParameterKey const CCParamDeviceStatusUpdate; /** @memberof CCParameters Parameter key for current device status XML string. XML can be converted into an {@link DeviceStatus} object using @{link ChipDnaMobileSerializer#deserializeDeviceStatus}. */

extern CCParameterKey const CCParamAutomaticReboot; /**< @memberof CCParameters Parameter key indicating if the PIN pad reboot is automatic or manual. Values can be {@link CCParameters#CCValueTrue TRUE} for automatic or {@link CCParameters#CCValueFalse False} for manual. */

extern CCParameterKey const CCParamAmount; /**< @memberof CCParameter Parameters key for the transaction amount. */
extern CCParameterKey const CCParamTransactionType; /**< @memberof CCParameters Parameter key for the transaction type. Can be a value of {@link CCParameter#CCValueSale SALE} or {@link CCParameter#CCValueRefund REFUND}. */
extern CCParameterKey const CCParamAmountType; /**< @memberof CCParameters Parameter key for the amount type. Can be a value of {@link CCParameter#CCValueAmountTypeActual ACTUAL} or {@link CCParameter#CCValueAmountTypeEstimate ESTIMATE}. */
extern CCParameterKey const CCParamUserReference; /**< @memeberof CCParameters Parameter key for the user reference. User reference must be unique per transaction. */
extern CCParameterKey const CCParamCurrency; /**< @memberof CCParameters Parameter key for the currency for a transaction. If only one currency is available ChipDNA Mobile will default to that currency. */

extern CCParameterKey const CCParamTippingType; /**< @memberof CCParameters Parameter key for tipping type being used for a transaction. Values can be {@link CCParameters#CCValueEndOfDayTipping END_OF_DAY}, {link CCParameters#CCValueOnDeviceTipping ON_DEVICE_TIPPING} or {@link CCParameters#CCValueBothTipping BOTH}. */
extern CCParameterKey const CCParamPaymentMethod; /**< @memberof CCParameters Parameter key to indicate how a transaction will be processed. Values can be {@link CCParameters#CCValueCash CASH}, {@link CCParameters#CCValueCheque CHEQUE} or {@link CCParameters#CCValueCard CARD}. */
extern CCParameterKey const CCParamOperatorPin; /**< @memeberof CCParameters Parameter key for the operator pin.  */
extern CCParameterKey const CCParamCustomerVaultId; /**< @memberof CCParameters Parameter key for customer vault identifier */
extern CCParameterKey const CCParamCustomerVaultCommand; /**< @memberof CCParameters Parameter key for customer vault command */
extern CCParameterKey const CCParamMerchantDefinedField01; /**< @memberof CCParameters Parameter key for merchant defined field 1. */
extern CCParameterKey const CCParamMerchantDefinedField02; /**< @memberof CCParameters Parameter key for merchant defined field 2. */
extern CCParameterKey const CCParamMerchantDefinedField03; /**< @memberof CCParameters Parameter key for merchant defined field 3. */
extern CCParameterKey const CCParamMerchantDefinedField04; /**< @memberof CCParameters Parameter key for merchant defined field 4. */
extern CCParameterKey const CCParamMerchantDefinedField05; /**< @memberof CCParameters Parameter key for merchant defined field 5. */
extern CCParameterKey const CCParamMerchantDefinedField06; /**< @memberof CCParameters Parameter key for merchant defined field 6. */
extern CCParameterKey const CCParamMerchantDefinedField07; /**< @memberof CCParameters Parameter key for merchant defined field 7. */
extern CCParameterKey const CCParamMerchantDefinedField08; /**< @memberof CCParameters Parameter key for merchant defined field 8. */
extern CCParameterKey const CCParamMerchantDefinedField09; /**< @memberof CCParameters Parameter key for merchant defined field 9. */
extern CCParameterKey const CCParamMerchantDefinedField10; /**< @memberof CCParameters Parameter key for merchant defined field 10. */
extern CCParameterKey const CCParamMerchantDefinedField11; /**< @memberof CCParameters Parameter key for merchant defined field 11. */
extern CCParameterKey const CCParamMerchantDefinedField12; /**< @memberof CCParameters Parameter key for merchant defined field 12. */
extern CCParameterKey const CCParamMerchantDefinedField13; /**< @memberof CCParameters Parameter key for merchant defined field 13. */
extern CCParameterKey const CCParamMerchantDefinedField14; /**< @memberof CCParameters Parameter key for merchant defined field 14. */
extern CCParameterKey const CCParamMerchantDefinedField15; /**< @memberof CCParameters Parameter key for merchant defined field 15. */
extern CCParameterKey const CCParamMerchantDefinedField16; /**< @memberof CCParameters Parameter key for merchant defined field 16. */
extern CCParameterKey const CCParamMerchantDefinedField17; /**< @memberof CCParameters Parameter key for merchant defined field 17. */
extern CCParameterKey const CCParamMerchantDefinedField18; /**< @memberof CCParameters Parameter key for merchant defined field 18. */
extern CCParameterKey const CCParamMerchantDefinedField19; /**< @memberof CCParameters Parameter key for merchant defined field 19. */
extern CCParameterKey const CCParamMerchantDefinedField20; /**< @memberof CCParameters Parameter key for merchant defined field 20. */

extern CCParameterKey const CCParamBillingAddress1; /**< Parameter key for Address Line One in Billing Information. */
extern CCParameterKey const CCParamBillingAddress2; /**< Parameter key for Address Line Two in Billing Information. */
extern CCParameterKey const CCParamBillingCity; /**< Parameter key for the City in Billing Information. */
extern CCParameterKey const CCParamBillingState; /**< Parameter key for the State in Billing Information */
extern CCParameterKey const CCParamBillingZipCode; /**< Parameter key for the Zip Code in Billing Information. Can be used interchangeably with BILLING_ZIP_CODE */
extern CCParameterKey const CCParamBillingPostalCode; /**< Parameter key for the Zip Code in Billing Information. Can be used interchangeably with BILLING_POSTAL_CODE */
extern CCParameterKey const CCParamBillingCountry; /**< Parameter key for the Country in Billing Information. */
extern CCParameterKey const CCParamBillingEmailAddress; /**< Parameter key for the Email Address in Billing Information. */
extern CCParameterKey const CCParamBillingPhoneNumber; /**< Parameter key for the Phone Number in Billing Information. */

extern CCParameterKey const CCParamPurchaseOrderNumber; /**< Parameter key for the Purchase Order Number. */
extern CCParameterKey const CCParamTaxAmount; /**< Parameter key for the Tax Amount. */

extern CCParameterKey const CCParamTransactionResult; /**< @memberof CCParameters Parameter key for result of a transaction. Value can be {@link CCParameters#CCValueApproved APPROVED}, {@link CCParameters#CCValueDeclined DECLINED} or {@link CCParameters#CCValuePartiallyApproved}. */
extern CCParameterKey const CCParamTransactionState; /**< @memeber of CCParameters Parameter key for transaction state. Value can be {@link CCParameters#CCValueCommitted COMMITTED}, {@link CCParameters#CCValueUncommitted UNCOMMITTED}, {@link CCParameters#CCValueVoided VOID} or {@link CCParameters#CCValueUncommittedVoid UNCOMMITTED_VOID}. */
extern CCParameterKey const CCParamAuthCode; /**< @memberof CCParameters Parameter key for authorization code of a transaction. */
extern CCParameterKey const CCParamCardEaseReference; /**< @memberof CCParameters Parameter key for CardEase reference of a transaction. */
extern CCParameterKey const CCParamTransactionId; /**< @memberof CCParameters Parameter key for transaction identifier of a transaction. */
extern CCParameterKey const CCParamMaskedPan; /**< @memeberof CCParameters Parameter key for a masked PAN. */
extern CCParameterKey const CCParamAmountOnlineAuthorized; /**< @memberof CCParameters Parameter key for the amount of a transaction authorized online. */
extern CCParameterKey const CCParamTransactionDateTime; /**< @memberof CCParameters Parameter key for a transactions local date and time. */
extern CCParameterKey const CCParamTransactionDateTimeUtc; /**< @memberof CCParameters Parameter key for a transactions UTC date time. */
extern CCParameterKey const CCParamReceiptData; /**< @memberof CCParameters Parameter key for the transactions receipt data in XML format. Which can be converted to an array of {@link ReceiptField ReceiptFields} using {@link ChipDnaMobileSerializer#deserializeReceiptData:} method. */
extern CCParameterKey const CCParamCardHash; /**< @memeberof CCParameters Parameter key for the card hash in XML format. Value can be converted to a {@link CCCardHash} object using {@link ChipDnaMobileSerializer#deserializeCardHash:}.*/
extern CCParameterKey const CCParamCardReference; /**< @memberof CCParameters Parameter key for the card reference.*/
extern CCParameterKey const CCParamCardSchemeId; /**< @memberof CCParameters Parameter key for the card scheme ID. */
extern CCParameterKey const CCParamCardHolderFirstName; /**< @memberof CCParameters Parameter key for the card holder first name. */
extern CCParameterKey const CCParamCardHolderLastName; /**< @memberof CCParameters Parameter key for the card holder last name. */
extern CCParameterKey const CCParamAquirerResponseCode; /**< @memeberof CCParameters Parameter key for the acquirer response code. */
extern CCParameterKey const CCParamDateTimeFormat; /**< @memebreof CCParameters Parameter key for the format of the date/times returned. */
extern CCParameterKey const CCParamPreformattedMerchantReceipt; /**< @memberof CCParameters Parameter key for the preformatted merchant receipt text. */
extern CCParameterKey const CCParamPreformattedCustomerReceipt; /**< @memberof CCParameters Parameter key for the preformatted customer receipt text. */

extern CCParameterKey const CCParamTransactionUpdate; /**< @member CCParameters Parameter key for {@link TransactionUpdate} returned; */
extern CCParameterKey const CCParamUserNotification; /**< @member CCParameters Parameter key for {@link UserNotification} returned; */

extern CCParameterKey const CCParamResponseRequired; /**< @memberof CCParameters Parameter key indicating if a response is required to the triggered action. Value will be either {@link CCParameters#CCValueTrue TRUE}, indicating a response is required, or {@link CCParameters#CCValueFalse FALSE} if not. */
extern CCParameterKey const CCParamDigitalSignatureSupported; /**< @memeberof CCParameters Parameter key indicating if digital signature is supported. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE}. If not supplied, value is {@link CCParameters#CCValueFalse FALSE}.*/
extern CCParameterKey const CCParamOperatorPinRequired; /**< @memberof CCParameters Parameter key indicating if an operator pin must be supplied when responding to an action. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE}. If not supplied, value is {@link CCParameters#CCValueFalse FALSE}. */
extern CCParameterKey const CCParamSignatureData; /**< @memberof CCParameters Parameter key to supply digital signature data as a base64 string. */

extern CCParameterKey const CCParamReferralNumber; /**< @memberof CCParameters Parameter key to for the phone number required for voice referral. */
extern CCParameterKey const CCParamOnlinePinPresent; /**< @memberof CCParameters Parameter key indicating that an online pin block is present. */

extern CCParameterKey const CCParamTrack2ClearText; /**< @memberof CCParameters Parameter key to for the unencrypted clear text track2 of a card returned by the pinpad. */
extern CCParameterKey const CCParamMaskedTrack2; /**< @memberof CCParameters Parameter key to for the masked track2 of a card returned by the PIN pad. */
extern CCParameterKey const CCParamExpiryDate; /**< @memberof CCParameters Parameter key to for the expiry date of a card returned by the PIN pad. */

extern CCParameterKey const CCParamSendEmailReceipt; /**< @memeberof CCParameters Parameter key for requesting that a receipt should be sent via email. */
extern CCParameterKey const CCParamSendSmsReceipt; /**< @membeberof CCParameters Parameter key for requesting that a receipt should be send via SMS. */
extern CCParameterKey const CCParamPrintReceipt; /**< @memberof CCParameters Parameter key for requesting that a receipt should be printed from the device if supported. */
extern CCParameterKey const CCParamReceiptText; /**< @memberof CCParameters Parameter key for the receipt text. */
extern CCParameterKey const CCParamEmailAddress; /**< @memberof CCParameters Parameter key for an email address. */
extern CCParameterKey const CCParamPhoneNumber; /**< @memberof CCParameters Parameter key for a telephone number. */
extern CCParameterKey const CCParamReceiptSourceName; /**< @memeberof CCParameters Parameter key for the receipts source name. */
extern CCParameterKey const CCParamReceiptSourceEmail; /**< @memberof CCParameters Parameter key for the receipts source email address. */
extern CCParameterKey const CCParamReceiptEmailSubject; /**< @memeberof CCParameters Parameter key for the emails subject field of receipts. */

extern CCParameterKey const CCParamReceiptEmailResult; /**< @memberof CCParameters Parameter key for the result of sending a email receipt. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE}. */
extern CCParameterKey const CCParamReceiptSmsResult; /**< @memberof CCParameters Parameter key for the result of sending a SMS receipt. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE} */
extern CCParameterKey const CCParamReceiptPrintResult; /**< @memberof CCParameters Parameter key for the result of printing a receipt from the device. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE} */

extern CCParameterKey const CCParamIdleMessage; /**< @memberof CCParameters Parameter key for the idle message to be displayed. */
extern CCParameterKey const CCParamDisplayTime; /**< @memberof CCParameters Parameter key for the length of time a message should be displayed in seconds. If not present time will be infinite.*/

extern CCParameterKey const CCParamCloseTransaction; /**< @memeberof CCParameters Parameter key indicating whether a transaction should be closed. */
extern CCParameterKey const CCParamTipAmount; /**< @memberof CCParameters Parameter key for the tip amount. */

extern CCParameterKey const CCParamSaleReference; /**< @memberof CCParameters Parameter key for the user reference of a linked sale transaction. */

extern CCParameterKey const CCParamOfflineStatus; /**< @memberof CCParameters Parameter key indicating the offline status of a transaction. */

extern CCParameterKey const CCParamLinkedRefundReferences; /**< @memberof CCParameters Parameter key for the linked refund references linked to the transaction. */

extern CCParameterKey const CCParamAvailableCurrencies; /**< @memberof CCParameters Parameter key for the currencies available XML for the terminal. The data converted using {@link ChipDnaMobileSerializer#deserializedAvailableCurrencies:}*/

extern CCParameterKey const CCParamDeferredAuthReason; /**< @memberof CCParameters Parameter key indicating the reason a deferred authorization is being offered. */

extern CCParameterKey const CCParamClearIdleMessage; /**< @memberof CCParameters Parameter key to clear the current status message and return display to default. */

extern CCParameterKey const CCParamOfflineCompletionState; /**< @memberof CCParameters Parameter key indicating how a transaction from the offline queue was completed. This can be either {@link CCValueOfflineCompletionStateProcessed} or {@link CCValueOfflineCompletionStateDeleted}. */
extern CCParameterKey const CCParamOfflineCompletionDateTime; /**< @memberof CCParameter Parameter for the date and time that an offline request was completed. */

extern CCParameterKey const CCParamOfflineRequestErrors; /**< @memberof CCParameter Parameter for the errors associated with an offline request. */

extern CCParameterKey const CCParamAvailableCardApplications; /**< @memberof CCParameter Parameter for the card applications XML to be presented to the Cardholder during application selection. The data converted using {@link ChipDnaMobileSerializer#deserializedAvailableCardApplications:}*/

extern CCParameterKey const CCParamSelectedCardApplication; /**< @memberof CCParameter Parameter for the card application selected by the Cardholder during application selection. */

extern CCParameterKey const CCParamPANKeyEntry; /**< @memberof CCParameter Parameter key for PAN key entry. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE}. */
extern CCParameterKey const CCParamAutoConfirm; /**< @memberof CCParameter Parameter key to trigger auto confirmation of a transaction. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE}.*/

/**
 * @}
 */

@interface CCParameters : NSObject
/**
 * Set a value for a {@link CCParameterKey}.
 *
 * @param value Value of the parameter.
 * @param aKey {@link CCParameterKey} for the value.
 */
-(void)setValue:(NSString *)value forKey:(CCParameterKey)aKey;

/**
 * Remove a value for a {@link CCParameterKey}.
 *
 * @param akey {@link CCParameterKey} for the value being removed.
 */
-(void)removeValueForKey:(CCParameterKey)aKey;

/**
 * Retrieve the value for a {@link CCParameterKey}.
 *
 * @param aKey {@link CCParameterKey} for value to retrieve.
 */
-(NSString *)valueForKey:(CCParameterKey)aKey;

/**
 * Remove all objects from the Parameters collection.
 */
-(void)removeAllObjects;

/**
 * Returns all keys currently within the the parameter collection.
 */
-(NSArray *)allKeys;

/**
 * Returns all values currently within the parameter collection.
 */
-(NSArray *)allValues;

@end

