/**
 * @mainpage
 *The ChipDNA Mobile API contains the classes necessary to process Card Present sale and refund transactions on a mobile platform. Processing Card Present transactions requires one of the following PIN pads:
 *
 * <ul>
 * <li>BBPOS WisePad2 or WisePad2 Plus</li>
 * <li>BBPOS WisePad2 Plus</li>
 * <li>Datecs Bluepad50</li>
 * <li>Miura M006, M007, or M010</li>
 * </ul>
 *
 * All external devices should be paired with the mobile device so that Bluetooth connections can be opened as required. Most transactions will require online authorisation, therefore the mobile device should have access to a wireless or mobile data network.
 * The ChipDNA Mobile API uses an encrypted database and needs to be initialised with a password before the API functionality can be used. {@link ChipDnaMobile#initialize: initialize} method is used to start the initialisation process. Once initialized an instance of the {@link ChipDnaMobile} class obtained using the static {@link ChipDnaMobile#sharedInstance sharedInstance} method.
 *
 *  Use {@link ChipDnaMobile#setProperties: setProperties} to set the required properties such as Application Identifier, PIN pad name, Target platform (Live or Test), Terminal ID and Transaction Key.
 *
 * Once the appropriate properties are set, the PINpad can be prepared for processing transactions by calling {@link ChipDnaMobile#connectAndConfigure: connectAndConfigure} . Register listener using {@link ChipDnaMobile#addConnectAndConfigureFinishedTarget:action: addConnectAndConfigureFinisehdTarget} to receive notifcation of the configuration result. Register listener using {@link ChipDnaMobile#addConfigurationUpdateTarget:action: addConfigurationUpdateTarget} to receive notifcation of the configuration progress. Register listener using {@link ChipDnaMobile#addDeviceUpdateTarget:action: addDeviceUpdateTarget} to receive notification of the PIN pad connectivity events.
 *
 * ChipDNA Mobile transactions are processed in two stages, authorization stage and confirm stage. Once configuration is complete, transactions can be authorized using {@link ChipDnaMobile#startTransaction: startTransaction} method. To be notified of the transaction results register a target using {@link ChipDnaMobile#addTransactionFinishedTarget:action: addTransactionFinishedTarget}. To be notified of transaction progress updates registered a target using {@link ChipDnaMobile#addTransactionUpdateTarget:action addTransactionUpdateTarget} method.
 *
 * ChipDNA Mobile does not auto-confirm transactions. Approved transactions must be explicitly confirmed for settlement in order to obtain the funds using method {@link ChipDnaMobile#confirmTransaction: confirmTransaction} . If the funds for an approved transaction are not settled for reasons such as goods issue failure then the transaction should be explicitly voided using method {@link ChipDnaMobile#voidTransaction: voidTransaction}.
 *
 * Tel. +44 (0)117 930 4455
 * Tel. +1 (800) 868-1832
 * Email: support@creditcall.com
 *
 * Creditcall Ltd. Registered Office: Merchants House North, Wapping Road, Bristol BS1 4RW. 0117 930 4455 Registered No: 3295353. VAT No: 713 0076 80.
 * Creditcall Corporation. 1133 Broadway, Suite 706, New York, NY 10010. USA. 800 868 1832.
 **/

/**
 * @author Creditcall Ltd
 * @brief This class should be used for processing Card Present sale and refund transactions on a mobile device.
 * @class ChipDnaMobile
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 * @details It is also provides the necessary functionality for refunding a sale transaction and emailing and sending SMS receipts.
 * {@link #connectAndConfigure: connectAndConfigure},
 * {@link #startTransaction: startTransaction},
 * {@link #linkedRefundTransaction: linkedRefundTransaction},
 * {@link #getCardDetails: getCardDetails} ,
 * {@link #confirmTransaction: confirmTransaction} and
 * {@link #voidTransaction: voidTransaction}
 * cannot be executed simultaneously, the operation that they perform needs to
 * be finished before any of the other methods is executed. The method {@link #dispose:} cannot be used
 * when the above operations are taking place. Otherwise {@link ConfigurationErrorCode#AnotherOperationBeingProcessed
 * AnotherOperationBeingProcessed} error is returned.
 */

#import <Foundation/Foundation.h>
#import "CCParameters.h"


extern NSString * const CCInitialisationException;

@interface ChipDnaMobile : NSObject

/**
 * Returns the instance of ChipDnaMobile. This method should only be called after ChipDnaMobile
 * has been initialised using {@link #initialize: initialize}. {@link #isInitialized isInitialized}
 * can be used to check whether ChipDnaMobile has been initialised already.
 *
 * Ensures only one instance exists.
 *
 * @return The instance of ChipDnaMobile.
 * @exception CCInitialisationException Thrown if the method is accessed before ChipDnaMobile has been initialised.
 **/
+ (ChipDnaMobile *)sharedInstance;

/**
 * Returns whether ChipDnaMobile has been initialized.
 *
 * @return YES if ChipDnaMobile has been initialized, otherwise NO.
 **/
+ (BOOL)isInitialized;

/**
 * Initializes ChipDnaMobile. Ensures an encrypted database is used and migrates the previous versions of the SDK
 * to use encryption, as needed. The specified delegate will be notified when a password is required and whether 
 * the initialisation was successful or not.
 * 
 * The initialization process has to be completed successfully before ChipDnaMobile functionality is enabled.
 * Only static methods can be executed before the initialisation process has finished successfully.
 *
 * Once ChipDnaMobile has been initialise successfully, @{link #dispose:} should be call before the application terminates 
 * to ensure resources are disposed of correctly.
 *
 * @param request with parameter collection containing oneor more of the following:
 * <p>{@link CCParameters#CCParamPassword CCParamPassword} Required during initialization.</p>
 * <p>{@link CCParameters#CCParamChangePassword CCParamChangePassword} Required during password change</p>
 * <p>{@link CCParameters#CCParamDestroy CCParamDestroy} Optional</p>
 *
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 **/
+ (CCParameters *)initialize:(CCParameters *)request;

/**
 * Disposes of ChipDnaMobile. Resources will be disposed off, database will be closed, PIN pad disconnected and functionality will be disabled.
 * To restore the functionality, ChipDnaMobile will need to be reinitialised after this method is called.
 *
 * The use of this method is required in order to close the connection to the database. It must be called before the application is closed.
 *
 * @param request Currently not used and can be nil.
 *
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 **/
+ (CCParameters *)dispose:(CCParameters *)request;

/**
 * Retrieves a list of names of the PIN pads currently paired and
 * connected to the iOS device. Returned to observers subscribed using {@link ChipDnaMobile#addAvailablePinPadsTarget:action:}.
 *
 * Only the following PIN pads are supported:
 * <ul>
 * <li>BBPOS WisePad2 or WisePad2 Plus</li>
 * <li>Datecs Bluepad50</li>
 * <li>Miura M006, M007, or M010</li>
 * </ul>
 *
 * @param request with parameter collection. Currently not used.
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 **/
- (CCParameters *)getAvailablePinPads:(CCParameters *)request;

/**
 * Set properties required to configured ChipDNA Mobile.
 *
 * @param request Parameter collection which can contain the following:
 * <p>{@link CCParameters#CCParamTestTerminalId CCParamTestTerminalId} Set the test environment terminal ID.</p>
 * <p>{@link CCParameters#CCParamTestTransactionKey CCParamTestTransactionKey} Set the test environment transaction key.</p>
 * <p>{@link CCParameters#CCParamLiveTerminalId CCParamLiveTerminalId} Set the live environment terminal ID.</p>
 * <p>{@link CCParameters#CCParamLiveTransactionKey CCParamLiveTransactionKey} Set the live environment transaction key.</p>
 * <p>{@link CCParameters#CCParamApiKey CCParamApiKey} Set the API Key for the Gateway.</p>
 * <p>{@link CCParameters#CCParamEnvironment CCParamEnvironment} Set the environment to {@link CCParameters#CCValueEnvironmentLive LIVE} or {@link CCParameters#CCValueEnvironmentTest TEST}.</p>
 * <p>{@link CCParameters#CCParamPinPadName CCParamPinPadName} Set the PIN pad name retrieved using {@link ChipDnaMobile#getAvailablePinPads: getAvailablePinPads}.</p>
 * <p>{@link CCParameters#CCParamPinPadConnectionType CCParamPinPadConnectionType} Set the connection type of the PIN pad. Currently only Bluetooth supported. Bluetooth will be used by default.</p>
 * <p>{@link CCParameters#CCParamTestWebMisUsername CCParamTestWebMisUsername} Set WebMIS username for test environment.</p>
 * <p>{@link CCParameters#CCParamTestWebMisPassword CCParamTestWebMisPassword} Set WebMIS password for test environment.</p>
 * <p>{@link CCParameters#CCParamLiveWebMisUsername CCParamLiveWebMisUsername} Set WebMIS username for live environment.</p>
 * <p>{@link CCParameters#CCParamLiveWebMisPassword CCParamLiveWebMisPassword} Set WebMIS password for live environment.</p>
 * <p>{@link CCParameters#CCParamApplicationIdentifier CCParamApplicationIdentifier} Set Application Identifier.</p>
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
- (CCParameters *)setProperties:(CCParameters *)request;

/**
 * Retrieve the available currencies for currently configured terminal.
 *
 * @param request Not currently in use. Can be nil.
 * @return A parameter list which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} defining if the call to getAvailableCurrencies was successful using the values {@link CCParameters#CCValueTrue TRUE} and {@link CCParameters#CCValuesFalse FALSE}.</p>
 * <p>{@link CCParameters#CCParamError CCParamError} a comma separated string of errors which have occurred during the call to getAvailableCurrencies.</p>
 * <p>{@link CCParameters#CCParamAvailableCurrencies} An XML representation of the available currencies for each payment method. XML can be de serialized into a NSDictionary of NSArrays containing {@link Currency} objects using {@link ChipDnaMobileSerializer#deserializeAvailableCurrencies}.</p>
 */
- (CCParameters *)getAvailableCurrencies:(CCParameters *)request;

/**
 * Forces an update using the TMS server. This checks whether the current version of
 * ChipDnaMobile is valid to use, whether the Terminal ID is disabled and downloads 
 * the settings configured on the TMS server.
 *
 * The result of a TMS update is returned to observers registered using {@link ChipDnaMobile#addTmsUpdateTarget addTmsUpdateTarget}
 *
 * @param request Parameter collection which can contain the following:
 * <p>{@link CCParameters#CCParamFullTmsUpdate CCParamFullTmsUpdate} Forces a full TMS update. All configuration for the currently connected PIN pad is downloaded regardless of whether it's required.
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 **/
- (CCParameters *)requestTmsUpdate:(CCParameters *)request;

/**
 * Connect to and configure the current device. This method will perform all necessary actions required to get the selected PIN pad ready for transaction. 
 * This includes performing TMS updates if required. {@link CCParameters#CCParamForceTmsUpdate} and {@link CCParameters#CCParamFullTmsUpdate} can be passed to this
 * method to force a TMS updated. When called the result of this method will be returned to observers subscribed to the {@link ChipDnaMobile#addConnectAndConfigureFinishedTarget:action: addConnectAndConfigureFinishedTarget}.

 *
 * This method can be called regardless of the current connection state of the PIN pad. When the PIN pad is already connected TMS checks will be made and a TMS update performed
 * if required. When new configuration is available this method will then re-configure the PIN pad. This method can also perform registration when required.
 *
 * To receive updates on the progress of this method observers can be added using the {@link ChipDnaMobile#addConfigurationUpdateTarget:action: addConfigurationUpdateTarget} method. Observer will receive a {@link CCParameters} collection which will contain a {@link CCParameters#CCParamConfigurationUpdate CCParamConfigurationUpdate} items with the following values:
 * <p>{@link CCParameters#CCValueConnectionAndConfigureStarted CCValueConnectionAndConfigureStarted}</p>
 * <p>{@link CCParameters#CCValueConnecting CCValueConnecting}</p>
 * <p>{@link CCParameters#CCValuePerformingTmsUpdate CCValuePerformingTmsUpdate}</p>
 * <p>{@link CCParameters#CCValueCheckingPinPadConfig CCValueCheckingPinPadConfig}</p>
 * <p>{@link CCParameters#CCValueUpdatingPinPadConfig CCValueUpdatingPinPadConfig}</p>
 *
 * To receive updates on the current connection status of the PIN pad observers can be added using the {@link ChipDnaMobile#addDeviceUpdateTarget:action: addDeviceUpdateTarget} method. Observers will receive a {@link CCParameters} collection which will contain a {@link CCParameters#CCParamDeviceStatusUpdate CCParamDeviceStatusUpdate} item with a serialized {@link DeviceStatus} object value.
 *
 * @param request Parameter collection which can contain the following:
 * <p>{@link CCParameters#CCParamForceTmsUpdate}</p> Force connectAndConfigure to perform a TMS update whether one is required or not.</p>
 * <p>{@link CCParameters#CCParamFullTmsUpdate} Force a full TMS update whether one is required or not. All configuration will be re-downloaded regardless of whether it's required.
 *
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
-(CCParameters *)connectAndConfigure:(CCParameters *)request;

/**
 * Check the current status of different components of ChipDNA Mobile in a single call.
 *
 * @param request Parameter collection which can contain the following:
 * <p>{@link CCParameters#CCParamPaymentPlatformTest} with a value of {@link CCParameters#CCValueTrue} will check the status of the payment platform returning the result in a {@link PaymentPlatformStatus} object in the response.
 *
 * @return Parameter collection containing the following:
 * <p>{@link CCParameters#CCParamVersionInformation CCParamVersionInformation} - An XML representation of a {@link VersionInformation} object. {@link ChipDnaMobileSerializer#deserializeVersionInformation:} can be used to retrieve an object.</p>
 * <p>{@link CCParameters#CCParamChipDnaStatus CCParamChipDnaStatus} - A string representing the current status of ChipDNA Mobile.</p>
 * <p>{@link CCParameters#CCParamDeviceStatus CCParamDeviceStatus} - An XML representation of a {@link DeviceStatus} object. {@link ChipDnaMobileSerializer#deserializeDeviceStatus:} can be used to retrieve an object.</p>
 * <p>{@link CCParameters#CCParamTmsStatus CCParamTmsStatus} - An XML representation of a {@link TmsStatus} object. {@link ChipDnaMobileSerializer#deserializeTmsStatus:} can be used to retrieve an object.</p>
 * <p>{@link CCParameters#CCParamTestTerminalStatus CCParamTestTerminalStatus} - An XML representation of the test {@link TerminalStatus}. {@link ChipDnaMobileSerializer#deserializeTerminalStatus:} can be used to retrieve an object.</p>
 * <p>{@link CCParameters#CCParamLiveTerminalStatus CCParamLiveTerminalStatus} - An XML representation of the live {@link TerminalStatus}. {@link ChipDnaMobileSerializer#deserializeTerminalStatus:} can be used to retrieve an object.</p>
 * <p>{@link CCParameters#CCParamEnvironment CCParamEnvironment} - The current environment ChipDnaMobile is currently running in. Will have a value of either {@link CCParameters#CCValueEnvironmentLive} or {@link CCParameters#CCValueEnvironmentTest}.</p>
 * <p>{@link CCParameters#CCParamRequestQueueStatus CCParamRequestQueueStatus} - An XML representation of a {@link RequestQueueStatus} object. {@link ChipDnaSerializer#deserializeRequestQueueStatus:} can be used to retrieve an object.</p>
 * <p>{@link CCParameters#PaymentPlatformStatus PaymentPlatformStatus} - An XML representation of a {@link PaymentPlatformStatus} object. Only returned when {@link CCParameters#CCParamPaymentPlatformTest CCParamPaymentPlatformTest} is set to {@link CCValueTrue TRUE}.</p>
 * <p>{@link CCParameters#CCParamLinkedRefundSupported CCParamLinkedRefundSupported} - When present with the value {@link CCValueTrue TRUE} indicates that linked refunds are supported with the currently configured terminal.</p>
 * <p>{@link CCParameters#CCParamStandaloneRefundsSupported CCParamStandaloneRefundsSupported} - When present with the value {@link CCValueTrue TRUE} indicates that standalone refunds are supported with the currently configured terminal.</p>
 * <p>{@link CCParameters#CCParamCashTransactionsSupported CCParamCashTransactionsSupported} - When present with the value {@link CCValueTrue TRUE} indicates that cash transactions are supported with the currently configured terminal.</p>
 * <p>{@link CCParameters#CCParamChequeTransactionsSupported CCParamChequeTransactionsSupported} - When present with the value {@link CCValueTrue TRUE} indicates that cheque transactions are supported with the currently configured terminal.</p>
 * <p>{@link CCParameters#CCParamTransactionHistorySupported CCParamTransactionHistorySupported} - When present with the value {@link CCValueTrue TRUE} indicates that transaction history is supported with the currently configured terminal.</p>
 * <p>{@link CCParameters#CCParamEmailReceiptSupported CCParamEmailReceiptSupported} - When present with the value {@link CCValueTrue TRUE} indicates that centralized emails receipting is supported with the currently configured terminal.</p>
 * <p>{@link CCParameters#CCParamSmsReceiptSupported CCParamSmsReceiptSupported} - When present with the value {@link CCValueTrue TRUE} indicates that centralized SMS receipting is supported with the currently configure terminal.</p>
 * <p>{@link CCParameters#CCParamTippingSupported CCParamTippingSupported} - If present indicates the type of tipping supported with the currently configured terminal. Values can be: {@link CCParameters#CCValueEndOfDayTipping CCValueEndOfDayTipping}, {@link CCParameters#CCValueOnDeviceTipping CCValueOnDeviceTipping}, or {@link CCParameters#CCValueBothTipping CCValueBothTipping}</p>
 * <p>{@link CCParameters#CCParamApplicationIdentifier CCParamApplicationIdentifier} - A string representing the current indentifier for the application.</p>
 */
-(CCParameters *)getStatus:(CCParameters *)request;

/**
 * Start a transaction. Objects wishing to know the outcomes of transactions should observe for the {@link ChipDnaMobile#addTransactionFinishedTarget:action: transactionFinished} event. Objects can also register to receive updates about on going transactions by observing for {@link ChipDnaMobile#addTransactionUpdateTarget:action: transactionUpdates}.
 *
 * @param request A parameter collection defining the behaviour of the transaction. It can contain the following parameters:
 * <p>{@link CCParameters#CCParamAmount CCParamAmount} The amount to be used in the transaction.</p>
 * <p>{@link CCParameters#CCParamUserReferrence CCParamUserReference} A unique reference for this transaction.
 * <p>{@link CCParameters#CCParamTransactionType CCParamTransactionType} The transaction type for this transaction. Values can be {@link CCParameters#CCValueSale CCValueSale} or {@link CCParameters#CCValueRefund CCValueRefund}.
 * <p>{@link CCParameters#CCParamCurrency CCParamCurrency} Set the currency for this transaction. Only required when {@link ChipDnaMobile#getAvailableCurrencies:} returns more than one currency. If only a single currency is supported, it will be used by default.</p>
 * <p>@link CCParameters#CCParamPANKeyEntry CCParamPANKeyEntry} Requests a PAN key entry transaction is started for a card not present transaction. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE}.</p>
 *
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
-(CCParameters *)startTransaction:(CCParameters *)request;

/**
 * Display text on the PIN pad for a given amount of time at which point the PIN pad will return to idle. Currently only supported by the Miura M010 and M007
 * 
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamIdleMessage CCParamIdleMessage} Message to be displayed by PIN pad. </p>
 * <p>{@link CCParameters#CCParamClearIdleMessage CCParamClearIdleMessage} Use with a value of {@link CCParameters#CCValueTrue TRUE} to clear message back to default.
 * <p>{@link CCParameters#CCParamDiplayTime CCParamDisplayTime} Amount of time for message to be displayed in seconds. Should be a decimal value. If 0 or missing an infinite time will be used. </p>
 *
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
-(CCParameters *)setIdleMessage:(CCParameters *)request;

/**
 * Terminate the currently processing transaction.
 *
 * @param request Currently not used, can be set to nil.
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
-(CCParameters *)terminateTransaction:(CCParameters *)request;

/**
 * Continue transaction currently waiting for signature verification. This should be called when a targets observing {@link ChipDnaMobile#addSignatureVerificationTarget: SignatureVerification} receive a parameter list containing {@link CCParameters#CCParamResponseRequired} set to {@link CCParameters#CCValueTrue TRUE}. 
 *
 * @param request Parameter list which can contain the following:
 * <p>{@link CCParameters#CCParamResult CCParamResult} indicating the outcome of signature verification. Value can be {@link CCParameters#CCValueTrue TRUE} to indicate that signature verification was successful or {@link CCParameters#CCValueFalse FALSE} to indicate that signature verification has failed.</p>
 * <p>{@link CCParameters#CCParamOperatorPin CCParamOperatorPin} required when {@link CCParameters#CCParamOperatorPinRequired CCParamOperatorPinRequired} is set to {@link CCParameters#CCValueTrue TRUE}. </p>
 * @return parameter collection which can contain:
 *<p>{@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.</p>
 **/
-(CCParameters *)continueSignatureVerification:(CCParameters *)request;

/**
 * Continue transaction currently waiting for signature capture. This should be called with targets observing {@link ChipDnaMobile#addSignatureCaptureTarget: SigantureCapture} receive a parameter list containing {@link CCParameters#CCParamResponseRequired} set to {@link CCParameters#CCValueTrue TRUE}.
 *
 * @param request Parameter list which can contain the following:
 * <p>{@link CCParameters#CCParamSignatureData CCParamSignatureData} containing data for digital signature represented as a base64 string of PNG image data. Signature must be a minimum dimension of 25x150 or 150x25 pixels not including white space. </p>
 * @return parameter collection which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.</p>
 */
-(CCParameters *)continueSignatureCapture:(CCParameters *)request;

/**
 * Continue transaction currently waiting for voice referral response. Informs ChipDnaMobile of the result of voice referral.
 *
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} indicating the outcome of a voice referral. Value can be {@link CCParameters#CCValueTrue TRUE} to indicate that the voice referral has been approved, or {@link CCParameters#CCValueFalse FALSE} to indicate that the voice referral has been declined.</p>
 * <p>{@link CCParameters#CCParamAuthCode CCParamAuthCode} The auth code of the voice referral. For approved voice referral, it must be 1-6 alphanumeric characters. For declined voice referral, auth code will be ignored. </p>
 * <p>{@link CCParameters#CCParamOperatorPin CCParamOperatorPin} required when {@link CCParameters#CCParamOperatorPinRequired CCParamOperatorPinRequired} is set to {@link CCParameters#CCValueTrue TRUE}. </p>
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
- (CCParameters *)continueVoiceReferral:(CCParameters *)request;

/**
 * Continue transaction currently waiting for forced acceptance response. Informs ChipDnaMobile of the result of forced acceptance.
 *
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} indicating the outcome of forced acceptance. Value can be {@link CCParameters#CCValueTrue TRUE} to indicate that the forced acceptance has been approved, or {@link CCParameters#CCValueFalse FALSE} to indicate that the forced acceptance has been declined. </p>
 * <p>{@link CCParameters#CCParamOperatorPin CCParamOperatorPin} required when {@link CCParameters#CCParamOperatorPinRequired CCParamOperatorPinRequired} is set to {@link CCParameters#CCValueTrue TRUE}. </p>
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
-(CCParameters *)continueForcedAcceptance:(CCParameters *)request;

/**
 * Continue transaction currently waiting for deferred authorisation response. Informs ChipDnaMobile of the result of deferred authorization.
 *
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} indicating the outcome of forced acceptance. Value can be {@link CCParameters#CCValueTrue TRUE} to indicate that the deferred authorization has been approved, or {@link CCParameters#CCValueFalse FALSE} to indicate that the deferred authorization has been declined. </p>
 * <p>{@link CCParameters#CCParamOperatorPin CCParamOperatorPin} required when {@link CCParameters#CCParamOperatorPinRequired CCParamOperatorPinRequired} is set to {@link CCParameters#CCValueTrue TRUE}. </p>
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 **/
- (CCParameters *)continueDeferredAuthorization:(CCParameters *)request;

/**
 * Continue transaction currently waiting for partial approval response. Informs ChipDnaMobile of the result of a partial approval.
 *
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} indicating the outcome of a partial approval. Value can be {@link CCParameters#CCValueTrue TRUE} to indicate that the partial approval has been approved, or {@link CCParameters#CCValueFalse FALSE} to indicate that the partial approval has been declined. </p>
 * <p>{@link CCParameters#CCParamOperatorPin CCParamOperatorPin} required when {@link CCParameters#CCParamOperatorPinRequired CCParamOperatorPinRequired} is set to {@link CCParameters#CCValueTrue TRUE}. </p>
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 **/
- (CCParameters *)continuePartialApproval:(CCParameters *)request;

/**
 * Continue transaction currently waiting for card application selection response. Informs ChipDnaMobile of the result of a card application selection.
 *
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} indicating the outcome of a card application selection. Value can be {@link CCParameters#CCValueTrue TRUE} to indicate that the card application selection has been completed, or {@link CCParameters#CCValueFalse FALSE} to indicate that the card application selection has failed. </p>
 * <p>{@link CCParameters#CCParamSelectedCardApplication CCParamSelectedCardApplication} is required to return the selected card application</p>
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 **/
- (CCParameters *)continueCardApplicationSelection:(CCParameters *)request;

/**
 * Continue transaction currently waiting for ID verification response. Informs ChipDnaMobile of the result of ID verification.
 *
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} indicating the outcome of a partial approval. Value can be {@link CCParameters#CCValueTrue TRUE} to indicate that ID verification has been approved, or {@link CCParameters#CCValueFalse FALSE} to indicate that ID verification has been declined. </p>
 * <p>{@link CCParameters#CCParamOperatorPin CCParamOperatorPin} required when {@link CCParameters#CCParamOperatorPinRequired CCParamOperatorPinRequired} is set to {@link CCParameters#CCValueTrue TRUE}. </p>
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.

 **/
- (CCParameters *)continueIdVerification:(CCParameters *)request;

/**
 * Acquire card details without performing a transaction. Card details will be returned to observers registered using {@link ChipDnaMobile#addCardDetailsTarget:action:}
 *
 * @param request Parameter list which can contain:
 * <p>{@link CCParameter#CCParamCurrency} indicating the currency to be set. Only needs to be set if more than one currency is supported.
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
- (CCParameters *)getCardDetails:(CCParameters *)request;

/**
 * Finalizes a transaction after it has been approved so that the transaction will be settled. This method allows the transaction to be committed using a different amount to the authorized amount.
 *
 * @param request A parameter collection which can contain the following:
 * <p>{@link CCParameters#CCParamUserReference CCParamUserReference} The unique user reference used when performing the transaction.</p>
 * <p>{@link CCParameters#CCParamAmount CCParamAmount} The amount to confirm the transaction for.</p>
 * <p>{@link CCParameters#CCParamTipAmount CCParamTipAmount} The tip amount for the transaction. Can only be used for transaction which used end of day tipping.</p>
 * <p>{@link CCParameters#CCParamCloseTransaction CCParamCloseTransaction} Close the current transaction. Only available with end of day tipping by default this parameter is {@link CCParameters#CCValueTrue TRUE}.</p>
 *
 * @return Parameter collection which can contain:
 * <p>{@link CCParameters#CCParamTransactionResult CCParamTransactionResult} indicating if committing the transaction has been {@link CCParameters#CCValueApproved CCValueApproved} or {@link CCParameters#CCValueDeclined CCValueDeclined}.</p>
 * <p>{@link CCParameters#CCParamError CCParamError} Containing a comma separated list of errors which have occurred.<p>
 * <p>{@link CCParameters#CCParamReceiptData CCParamReceiptData} only returned when transaction result is approved. An array of {@ReceiptField} objects serialized in XML format. Can be serialized into an object using {@link ChipDnaMobileSerializer#deserializeReceiptData:}.</p>
 * <p>{@link CCParameters#CCParamPreformattedMerchantReceipt CCParamPreformattedMerchantReceipt} A preformatted merchant receipt string. Only returned on approval.</p>
 * <p>{@link CCParameters#CCParamPreformattedCustomerReceipt CCParamPreformattedCustomerReceipt} A preformatted customer receipt string. Only returned on approval.</p>
 */
-(CCParameters *)confirmTransaction:(CCParameters *)request;

/**
 * Send receipts via email or SMS using the Creditcall platform.
 *
 * Results of calling this method will be sent to observers registered using {@link ChipDnaMobile#addProcessReceiptFinishedTarget:action:}.
 *
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamUserReference CCParamUserReference} User reference for the transaction the receipt is for. This is mandatory.</p>
 * <p>{@link CCParameters#CCParamReceiptingMethod CCParamReceiptingMethod} Defines the method which the receipt will be sent by. The value can be {@link CCParameters#CCValueReceiptingMethodEmail CCValueReceiptingMethodEmail} to email a receipt or {@link CCParameters#CCValueReceiptingMethodSms CCValueReceiptingMethodSms} to send a receipt vai SMS.</p>
 * <p>{@link CCParameters#CCParamReceiptText CCParamReceiptText} Defines the receipt text to be sent.</p>
 * <p>{@link CCParameters#CCParamEmailAddress CCParamEmailAddress} Required when {@link CCParameters#CCParamReceiptingMethod CCParamReceiptingMethod} is {@link CCParameters#CCValueReceiptingMethodEmail CCValueReceiptingMethodEmail}, defines the email to send the receipt to.</p>
 * <p>{@link CCParameters#CCParamPhoneNumber CCParamPhoneNumber}, Required when {@link CCParameters#CCParamReceiptingMethod CCParamReceiptingMethod} is {@link CCParameters#CCValueReceiptingMethodEmail CCValueReceiptingMethodEmail}, defines the phone number to send the receipt to.</p>
 * <p>{@link CCParameters#CCParamReceiptSourceName CCParameters#CCParamReceiptSourceName} Defines the name of the entity sending the receipt.
 * For SMS receipts this field is used for setting the 'SMS Originating Address' - i.e. the sender name shown on mobiles. Support for this field is networks dependent.</p>
 * <p>{@link CCParameters#CCParamReceiptSourceEmail CCParameters#CCParamReceiptSourceEmail} defines the email of the receipt is being sent from. Only required when {@link CCParameters#CCParamReceiptingMethod CCParamReceiptingMethod} is {@link CCParameters#CCValueReceiptingMethodEmail CCValueReceiptingMethodEmail}</p>
 * <p>{@link CCParameters#CCParamReceiptEmailSubject CCParameters#CCParamReceiptEmailSubject} defines the subject of the receipt email. Only required when {@link CCParameters#CCParamReceiptingMethod CCParamReceiptingMethod} is {@link CCParameters#CCValueReceiptingMethodEmail CCValueReceiptingMethodEmail}</p>
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
-(CCParameters *)processReceipt:(CCParameters *)request;

/**
 * Start a linked refund transaction to refund part or all of a previously online approved transaction.
 *
 * @param request A parameter collection which can contain:
 * <p>{@link CCParameters#CCParamAmount CCParamAmount} The amount to be refunded.</p>
 * <p>{@link CCParameters#CCParamUserReference CCParamUserReference} A unique user reference for the linked refund.</p>
 * <p>{@link CCParameters#CCParamSaleReference CCParamSaleReference} The user reference for the original sale transaction. When this parameter is used then original sale {@link CCParameters#CCParamCardEaseReference CCParamCardEaseReference} must not be provided.</p>
 * <p>{@link CCParameters#CCParamCardEaseReference CCParamCardEaseReference} The CardEase reference for the original sale transaction. When this parameter is used then the original sale {@link CCParameters#CCParamSaleReference CCParamSaleReference} must not be provided.</p>
 * @return A parameter collection which can contain:
 * <p>{@link CCParameters#CCParamTransactionResult CCParamTransactionResult} indicating if committing the transaction has been {@link CCParameters#CCValueApproved CCValueApproved} or {@link CCParameters#CCValueDeclined CCValueDeclined}.</p>
 * <p>{@link CCParameters#CCParamError CCParamError} Containing a comma separated list of errors which have occurred.</p>.
 * <p>{@link CCParameters#CCParamReceiptData CCParamReceiptData} only returned when transaction result is approved. An array of {@ReceiptField} objects serialized in XML format. Can be serialized into an object using {@link ChipDnaMobileSerializer#deserializeReceiptData:}.</p>
 * <p>{@link CCParameters#CCParamPreformattedMerchantReceipt CCParamPreformattedMerchantReceipt} A preformatted merchant receipt string. Only returned on approval.</p>
 * <p>{@link CCParameters#CCParamPreformattedCustomerReceipt CCParamPreformattedCustomerReceipt} A preformatted customer receipt string. Only retured on approval.</p>
 */
-(CCParameters *)linkedRefundTransaction:(CCParameters *)request;

/**
 * Void a previously approved but uncommitted transaction.
 *
 * @param request A parameter collection which can contain:
 * <p>{@link CCParameters#CCParamUserReference CCParamUserReference} The unique user reference used during the call to {@link ChipDnaMobile#startTransaction:}.
 * @return A parameter list which can contain:
 * <p>{@link CCParameters#CCParamTransactionResult CCParamTransactionResult} indicating if voiding the transaction has been {@link CCParameters#CCValueApproved CCValueApproved} or {@link CCParameters#CCValueDeclined CCValueDeclined}.</p>
 * <p>{@link CCParameters#CCParamReceiptData CCParamReceiptData} only returned when transaction result is approved. An array of {@ReceiptField} objects serialized in XML format. Can be serialized into an object using {@link ChipDnaMobileSerializer#deserializeReceiptData:}.</p>
 * <p>{@link CCParameters#CCParamPreformattedMerchantReceipt CCParamPreformattedMerchantReceipt} A preformatted merchant receipt string. Only returned on approval.</p>
 * <p>{@link CCParameters#CCParamPreformattedCustomerReceipt CCParamPreformattedCustomerReceipt} A preformatted customer receipt string. Only returned on approval.</p>
 */
-(CCParameters *)voidTransaction:(CCParameters *)request;

/**
 * Get information stored about a transaction.
 *
 * @param request A parameter list which can contain:
 * <p>{@link CCParameters#CCParamUserReference CCParamUserReference} The user reference used during the call to {@link ChipDnaMobile#startTransaction:}.</p>
 * @return A parameter list which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} defining if the call to getTransactionInformation was successful using the values {@link CCParameters#CCValueTrue TRUE} and {@link CCParameters#CCValuesFalse FALSE}.</p>
 * <p>{@link CCParameters#CCParamError CCParamError} a comma separated string of errors which occurred during the call to getTransactionInformation.</p>
 * <p>{@link CCParameters#CCParamUserReference CCParamUserReference} The user reference of the transaction.</p>
 * <p>{@link CCParameters#CCParamAmount CCParamAmount} The amount the transaction was performed for.</p>
 * <p>{@link CCParameters#CCParamTipAmount CCParamTipAmount} The tip amount tip amount for the transaction.</p>
 * <p>{@link CCParameters#CCParamEnvironment CCParamEnvironment} The payment environment which the transaction applies to.</p>
 * <p>{@link CCParameters#CCParamCurrency CCParamCurrency} The currency used during the transaction. Value is the 3 character code passed in during the call to {@link ChipDnaMobile#startTransaction:}.</p>
 * <p>{@link CCParameters#CCParamTransactionState CCParamTransactionState} The current state of the transaction.</p>
 * <p>{@link CCParameters#CCParamTransactionType CCParamTransactionType} Indicating if the transaction was a {@link CCParameters#CCValueSale Sale} or a {@link CCParameters#CCValueRefund Refund}.</p>
 * <p>{@link CCParameters#CCParamTransactionResult CCParamTransactionResult} The current result of the transaction.</p>
 * <p>{@link CCParameters#CCParamOfflineStatus CCParamOfflineStatus} If returned indicates the status of the transaction in the offline queue.</p>
 * <p>{@link CCParameters#CCParamLinkedRefundReferences CCParamLinkedRefundReferences} Comma separated string of user references for linked refunds linked to this transaction.</p>
 * <p>{@link CCParameters#CCParamTransactionDateTime CCParamTransactionDateTime} The date and time the transaction was performed. </p>
 * <p>{@link CCParameters#CCParamDateTimeFormat CCParamDateTimeFormate} The format of the {@link CCParameters#CCParamTransactionDateTime CCParamTransactionDateTime} parameter.</p>
 * <p>{@link CCParameters#CCParamTransactionId CCParamTransactionId} The identifier for transactions using the gateway.</p>
 */
-(CCParameters *)getTransactionInformation:(CCParameters *)request;

/**
 * Retry a transaction which has been moved to the failed offline queue.
 *
 * @param request A parameter collection which contains the {@link CCParameters#CCParamUserReference CCParamUserReference} of the transaction wishing to be retried.
 * @return A parameter collection which can contain:
 * <p>{@link CCParameters#CCParamTransactionResult CCParamTransactionResult} indicating if retrying the transaction has been {@link CCParameters#CCValueApproved CCValueApproved} or {@link CCParameters#CCValueDeclined CCValueDeclined}.</p>
 * <p>{@link CCParameters#CCParamError CCParamError} defining any errors which occurred during the call to {@link ChipDnaMobile#retryFailedOfflineRequest:}.
 * <p>{@link CCParameters#CCParamReceiptData CCParamReceiptData} only returned when transaction result is approved. An array of {@ReceiptField} objects serialized in XML format. Can be serialized into an object using {@link ChipDnaMobileSerializer#deserializeReceiptData:}.</p>
 * <p>{@link CCParameters#CCParamPreformattedMerchantReceipt CCParamPreformattedMerchantReceipt} A preformatted merchant receipt string. Only returned on approval.</p>
 * <p>{@link CCParameters#CCParamPreformattedCustomerReceipt CCParamPreformattedCustomerReceipt} A preformatted customer receipt string. Only returned on approval.</p>
 */
-(CCParameters *)retryFailedOfflineRequest:(CCParameters *)request;

/**
 * Delete a transaction which has been moved to the failed offline queue.
 *
 * @param request A parameter collection containing the {@link CCParameters#CCParamUserReference CCParamUserReference} of the transaction to be deleted.
 * @return parameter collection containing {@link CCParameters#CCParamResult CCParamResult} and if applicable {@link CCParameters#CCParamErrors CCParamErrors}.
 */
-(CCParameters *)deleteFailedOfflineRequest:(CCParameters *)request;

#pragma mark - Observer management

/**
 * Add an observer and action to be called with the response to {@link ChipDnaMobile#getAvailablePinPads:}.
 *
 * When an observer no longer requires the result of calls to {@link ChipDnaMobile#getAvailablePinPads:} it should be removed using the {@link removeAvailablePinPadsTarget:} method.
 *
 * @param target Target observing results of {@link ChipDnaMobile#getAvailablePinPads:}.
 * @param action Selector to be called on observing Target.
 */
+(void)addAvailablePinPadsTarget:(id)target action:(SEL)action;

/**
 * Remove an observer currently listening for results of calls to {@link ChipDnaMobile#getAvailablePinPads:}.
 *
 * @param target Target to be removed.
 */
+(void)removeAvailablePinPadsTarget:(id)target;

/**
 * Add an observer and action to be called with the response to {@link ChipDnaMobile#tmsUpdate: tmsUpdate:}.
 *
 * When an observer no longer requires the result of calls to {@link ChipDnaMobile#tmsUpdate: tmsUpdate:} it should be removed using the {@link removeTmsUpdateTarget:} method.
 * 
 * Observers are returned a {@link CCParameters} collection which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} defining if the tms update was successful using the values {@link CCParameters#CCValueTrue TRUE} and {@link CCParameters#CCValuesFalse FALSE}.</p>
 * <p>{@link CCParameters#CCParamError CCParamError} defining the error which has occurred. Will always be returned if the {@link CCParameters#CCParamResult CCParamResult} is {@link CCParameters#CCValuesFalse FALSE}.</p>
 * <p>{@link CCParameters#CCParamTmsUpdateUtc CCParamTmsUpdateUtc} is returned if {@link CCParameters#CCParamResult CCParamResult} is {@link CCParameters#CCValueTrue TRUE}. Defines the time of the last successful TMS update in UTC.</p>
 *
 * @param target Target observing results of {@link ChipDnaMobile#tmsUpdate:}.
 * @param action Selector to be called on observing target.
 */
+(void)addTmsUpdateTarget:(id)target action:(SEL)action;

/**
 * Remove an observer currently listening for results of calls to {@link ChipDnaMobile#tmsUpdate: tmsUpdate}.
 *
 * @param target Target to be removed.
 */
+(void)removeTmsUpdateTarget:(id)target;

/**
 * Add an observer and action to be called once {@link ChipDnaMobile#connectAndConfigure: connectAndConfigure} has completed.
 *
 * Observers are return a {@link CCParameters} collection which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} defining if the connect and configure was successful using the values {@link CCParameters#CCValueTrue TRUE} and {@link CCParameters#CCValuesFalse FALSE}.</p>
 * <p>{@link CCParameters#CCParamError CCParamError} defining the errors which have occurred. Will always be returned if the {@link CCParameters#CCParamResult CCParamResult} is {@link CCParameters#CCValuesFalse FALSE}.</p>
 * 
 * @param target Target observing results of {@link ChipDnaMobile#connectAndConfigure:}.
 * @param action Selector to be called on observing target.
 */
+(void)addConnectAndConfigureFinishedTarget:(id)target action:(SEL)action;

/**
 * Remove an observer currently listening for results of calls to {@link ChipDnaMobile#connectAndConfigure:}.
 *
 * @param target Target to be removed.
 */
+(void)removeConnectAndConfigureFinishedTarget:(id)target;

/**
 * Add an observer and action to receive updates about device connectivity.
 *
 * When updates are no longer required by an observer a call should be made to {@link ChipDnaMobile#removeDeviceUpdateTarget:} passing in the target to be removed.
 *
 * Observers are returned a {@link CCParameters} collection which will contain:
 * <p>{@CCParameters#CCParamDeviceStatusUpdate CCParamDeviceStatusUpdate} returned with a serialized {@link DeviceStatus} object. {@link DeviceStatus} can be deserialized using {@link ChipDnaMobileSerializer#deserializeDeviceStatus:}.</p>
 *
 * @param target Target wishing to receive device status updates.
 @ @param action Selector to be called on observing target.
 */
+(void)addDeviceUpdateTarget:(id)target action:(SEL)action;

/**
 * Remove an observer currently listening for device status updates.
 *
 * @param target Target to be removed.
 */
+(void)removeDeviceUpdateTarget:(id)target;

/**
 * Add an observer and action to receive updates about device configuration.
 *
 * When updates are no longer required by an observer a call should be made to {@link ChipDnaMobile#removeConfigurationUpdateTarget:} passing in the target to be removed.
 *
 * Observers are returned a {@link CCParameters} collection which will contain:
 * <p>{@CCParameters#CCParamConfigurationUpdate CCParamConfigurationUpdate} containing information about the current progress of @{ChipDnaMobile#connectAndConfigure: connectAndConfigure}.
 * 
 * @param target Target wishing to receive configuration updates.
 * @param action Selector to be called on observing target.
 */
+(void)addConfigurationUpdateTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for configuration updates.
 *
 * @param target Target to be removed.
 */
+(void)removeConfigurationUpdateTarget:(id)target;

/**
 * Add an observer and action to receive the results of calls to {@link ChipDnaMobile#startTransaction: startTransaction}.
 *
 * When updates are no longer required by an observer a call should be made to {@link ChipDnaMobile#removeTransactionFinishedTarget: removeTransactionFinishedTarget}.
 *
 * Observer are returned a {@link CCParameters} collection which will contain:
 * <p>{@link CCParameters#CCParamAquirerResponseCode} The acquirer response code</p>
 * <p>{@link CCParameters#CCParamAmount} Amount in minor units e.g. "123" for GBP1.23</p>
 * <p>{@link CCParameters#CCParamAmountOnlineAuthorized} The amount authorized online, in minor units e.g. "123" for GBP1.23</p>
 * <p>{@link CCParameters#CCParamAuthCode} Alpha numeric authorization code, up to 12 characters.</p>
 * <p>{@link CCParameters#CCParamCardEaseReference} Unique GUID generated for a transaction if the authorization was submitted online to the Creditcall payment platform.</p>
 * <p>{@link CCParameters#CCParamCardSchemeId} The {@link CardSchemeIdEnum}</p>
 * <p>{@link CCParameters#CCParamCurrency} The 3-char currency code</p>
 * <p>{@link CCParameters#CCParamMaskedPan} Masked primary account number showing only the first 6 (if available) and last 4 digits.</p>
 * <p>{@link CCParameters#CCParamPaymentMethod} The payment method; {@link CCParameters#CCValueCard}, {@link CCParameters#CCValueCash}, or {@link CCParameters#CCValueCheque}</p>
 * <p>{@link CCParameters#CCParamPreformattedCustomerReceipt} Preformatted receipt text, customer copy.</p>
 * <p>{@link CCParameters#CCParamPreformattedMerchantReceipt} Preformatted receipt text, merchant copy.</p>
 * <p>{@link CCParameters#CCParamReceiptData} A Map with keys Receipt Item Tag string and values {@link ReceiptField} objects serialized in XML format. Can be deserialized into an object using {@link ChipDnaMobileSerializer#deserializeReceiptData: deserializeReceiptData}.</p>
 * <p>{@link CCParameters#CCParamUserReference} Unique reference of the transaction supplied by the integrating application.</p>
 * <p>{@link CCParameters#CCParamTransactionDateTime} Local date and time of the transaction.</p>
 * <p>{@link CCParameters#CCParamTransactionDateTimeUtc} UTC date and time of the transaction.</p>
 * <p>{@link CCParameters#CCParamDateTimeFormat} Format of the date and time in {@link CCParameters#CCParamTransactionDateTime} or {@link CCParameters#CCParamTransactionDateTimeUtc}</p>
 * <p>{@link CCParameters#CCParamTransactionResult} The result of the transaction authorization.
 *  Can be {@link CCParameters#CCValueApproved}, {@link CCParameters#CCValuePartiallyApproved} or {@link CCParameters#CCValueDeclined}.</p>
 * <p>{@link CCParameters#CCParamTransactionState} Transaction state.
 *   Value can be {@link CCParameters#CCValueUncommitted}, {@link CCParameters#CCValueVoided} or , {@link CCParameters#CCValueUncommittedVoid}</p>
 * <p>{@link CCParameters#CCParamTransactionType} The transaction type. {@link CCParameters#CCValueSale} or {@link CCParameters#CCValueRefund}</p>
 * <p>{@link CCParameters#CCParamMerchantName} Merchant name is returned for Cash and Cheque transactions </p>
 * <p>{@link CCParameters#CCParamCustomerVaultId} The identifier for the customer's vault.</p>
 * <p>{@link CCParameters#CCParamTransactionId} The identifier for transaction using the gateway.</p>
 * <p>{@link CCParameters#CCParamError} Comma separated errors encountered during transaction processing</p>
 * @param target Target wishing to receive transaction finished results.
 * @param action Selector to be call on observing target.
 */
+(void)addTransactionFinishedTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for transaction finished results.
 *
 * @param target Target to be removed.
 */
+(void)removeTransactionFinishedTarget:(id)target;

/**
 * Add an observer and action to receive updates about on going transactions after a call to {@link ChipDnaMobile#startTransaction: startTransaction}.
 *
 * When updates are no longer required by an observer a call should be made to {@link ChipDnaMobile#removeTransactionUpdateTarget: removeTransactionUpdateTarget}.
 *
 * @param target Target wishing to receive transaction updates.
 * @param action Selector to be called on observing target.
 */
+(void)addTransactionUpdateTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for transaction updates.
 *
 * @param target Target to be removed.
 */
+(void)removeTransactionUpdateTarget:(id)target;

/**
 * Add an observer and action to receive updates on notifications which need to be known to the user after a call to {@link ChipDnaMobile#startTransaction: startTransaction}.
 *
 * When updates are no longer required by an observer a call should be made to {@link ChipDnaMobile#removeUserNotificationTarget: removeUserNotificationTarget}.
 *
 * @param target Target wishing to receive transaction updates.
 * @param action Selector to be called on observing target.
 */
+(void)addUserNotificationTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for user notifications.
 *
 * @param target Target to be removed.
 */
+(void)removeUserNotificationTarget:(id)target;

/**
 * Add an observer and action to know when signature verification is required after a call to {@link ChipDnaMobile#startTransaction:}.
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removeSignatureVeririficationTarget:}.
 *
 * Observer are returned a {@link CCParameters} collection which will contain:
 * <p>{@link CCParameters#CCParamSignatureData CCParamSigantureData} The signature data of receipt after resize and validation. Returned as a base 64 encoded string.
 *
 * @param target Target wishing to know when signature verification is required.
 * @param action Action to be called on the observing target.
 */
+(void)addSignatureVerificationTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for signature verification.
 *
 * @param target Target to be removed.
 */
+(void)removeSignatureVerificationTarget:(id)target;

/**
 * Add an observer and action to know when voice referral is required after a call to {@link ChipDnaMobile#startTransaction:}.
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removeVoiceReferralTarget:}.
 *
 * @param target Target wishing to know when voice referral is required.
 * @param action Action to be called on the observing target.
 */
+(void)addVoiceReferralTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for voice referral.
 *
 * @param target Target to be removed.
 */
+(void)removeVoiceReferralTarget:(id)target;

/**
 * Add an observer and action to know when forced acceptance confirmation is required after a call to {@link ChipDnaMobile#startTransaction:}.
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removeFocedAcceptanceTarget:}.
 *
 * @param target Target wishing to know when forced acceptance is required.
 * @param action Action to be called on the observing target.
 */
+(void)addForcedAcceptanceTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for voice referral.
 *
 * @param target Target to be removed.
 */
+(void)removeForcedAcceptanceTarget:(id)target;

/**
 * Add an observer and action to know when deferred authorization confirmation is required after a call to {@link ChipDnaMobile#startTransaction:}.
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removeDeferredAuthorizationTarget:}.
 *
 * @param target Target wishing to know when deferred authorization is required.
 * @param action Action to be called on the observing target.
 */
+(void)addDeferredAuthorizationTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for deferred authorization.
 *
 * @param target Target to be removed.
 */
+(void)removeDeferredAuthorizationTarget:(id)target;

/**
 * Add an observer and action to know when partial approval confirmation is required after a call to {@link ChipDnaMobile#startTransaction:}.
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removePartialApprovalTarget:}
 *
 * @param target Target wishing to know when partial approval is required.
 * @param action Action to be called on the observing target.
 */
+(void)addPartialApprovalTarget:(id)target action:(SEL)action;

/**
 * Remove a observer currently listening for partial approval.
 *
 * @param target Target to be removed.
 */
+(void)removePartialApprovalTarget:(id)target;

/**
 * Add an observer and action to know when ID verification is required after a call to {@link ChipDnaMobile#startTransaction}.
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removeIdVerificationTarget:}.
 * 
 * @param target Target wishing to know when ID verification is required.
 * @param action Action to be called on the observing target.
 */
+(void)addIdVerificationTarget:(id)target action:(SEL)action;

/**
 * Remove an observer currently listening for ID verification.
 *
 * @param target Target to be removed.
 */
+(void)removeIdVerificationTarget:(id)target;

/**
 * Add an observer and action to receive the results of calls to {@link ChipDnaMobile#getCardDetails:}.
 *
 * Observers will receive a {@link CCParameters} collection which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} defining whether card details have successful be retrieved. Value can be either {@link CCParameters#CCValueTrue TRUE} or {@link CCParameters#CCValueFalse FALSE}.</p>
 * <p>{@link CCParameters#CCParamError CCParamError} defining a comma separated string of error strings.</p>
 * <p>{@link CCParameters#CCParamExpiryDate CCParamExpiryDate} defining the expiry date returned by the PIN pad in the format YYMM.</p>
 * <p>{@link CCParameters#CCParamMaskedTrack2 CCParamMaskedTrack2} defining the masked track2 returned by the PIN pad. </p>
 * <p>{@link CCParameters#CCParamMaskedPan CCParamMaskedPan} defining the masked PAN returned by the PIN pad. </p>
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removeCardDetailsTarget:}.
 *
 * @param target Target wishing to receive card details.
 * @param action Action to be called on the observing target.
 */
+(void)addCardDetailsTarget:(id)target action:(SEL)action;

/**
 * Remove an observer currently listening for get card details updates.
 *
 * @param target Target to be removed.
 */
+(void)removeCardDetailsTarget:(id)target;

/**
 * Add an observer and action to receipt results of calls to {@link ChipDnaMobile#processReceipt:}.
 *
 * Observers will receive a {@link CCParameters} collection which can contain:
 * <p>{@link CCParameters#CCParamResult CCParamResult} defining if the receipt was successfully processed. Values can be {@link CCValueTrue TRUE} or {@link CCValueFalse}. </p>
 * <p>{@link CCParameters#CCparamError CCParamError} defining any errors while attempting to process receipt. If not present no errors occurred.</p>
 * 
 * @param target Target wishing to receive notification of results of {@link ChipDnaMobile#processReceipt}.
 * @param action Action called on observing target.
 */
+(void)addProcessReceiptFinishedTarget:(id)target action:(SEL)action;

/**
 * Remove an observer currently listening for results of {@link ChipDnaMobile#processReceipt}.
 *
 * @param target Target to be removed.
 */
+(void)removeProcessReceiptFinishedTarget:(id)target;

/**
 * Add an observer and action to know when card application selection is required after a call to {@link ChipDnaMobile#startTransaction}.
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removeCardApplicationSelectionTarget:}.
 *
 * @param target Target wishing to know when Card Application Selection is required.
 * @param action Action to be called on the observing target.
 */
+(void)addCardApplicationSelectionTarget:(id)target action:(SEL)action;

/**
 * Remove an observer currently listening for card application selection.
 *
 * @param target Target to be removed.
 */
+(void)removeCardApplicationSelectionTarget:(id)target;

/**
 * Add an observer and action to get the event for signature capture. Set observer before calling method {@link ChipDnaMobile#startTransaction}.
 *
 * When updates are no longer required by the observer a call should be made to {@link ChipDnaMobile#removeSignatureCaptureTarget:}.
 *
 * @param target Target wishing to know when signature capture is required.
 * @param action Action to be called on observing target.
 */
+(void)addSignatureCaptureTarget:(id)target action:(SEL)action;

/**
 * Remove an obserber currently listening for signature capture.
 *
 * @param target Target to be removed.
 */
+(void)removeSignatureCaptureTarget:(id)target;
@end
