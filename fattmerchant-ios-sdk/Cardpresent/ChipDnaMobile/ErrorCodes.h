/**
 * @author Creditcall Ltd
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 * @file ErrorCodes.h
 */

#import <Foundation/Foundation.h>

/**
 * @defgroup ChipDnaMobileError
 * @{
 *
 * The possible error codes that can be returned for a ChipDnaMobie Error type.
 */
extern NSString * const JailbrokenDevice;      /**< The device appears to be jailbroken. */

extern NSString * const UnableToOpenURL;                        /**< Unable to open the specified URL. */
extern NSString * const UnableToSpecifyCommunicationProtocol;   /**< Unable to specify communication protocol. */
extern NSString * const UnableToSendRequest;                    /**< Unable to send request. */
extern NSString * const UnableToGetHttpResponseCode;            /**< Unable to retrieve HTTP response code. */
extern NSString * const UnsuccessfulHttpRequest;                /**< The response to the HTTP request was not equal to 200(OK). */
extern NSString * const UnableToReadResponse;                   /**< Unable to read HTTP response data. */
extern NSString * const UnableToRetrieveResponse;               /**< Unable to retrieve server response. */
extern NSString * const UnableToWriteRequestXml;                /**< Unable to write request XML. */
extern NSString * const UnableToParseResponseXml;               /**< Unable to parse response XML. */
extern NSString * const UnableToReadResponseXml;                /**< Unable to read HTTP response data. */
extern NSString * const ErrorParsingResponseXml;                /**< Error parsing response XML. */
extern NSString * const ResponseXmlNotFound;                    /**< Response XML not found. */
extern NSString * const EmptyPropertySet;                       /**< No properties returned by TMS. */
extern NSString * const ErrorParsingPropertyXML;                /**< Error parsing properties in Property element in response XML. */
extern NSString * const SpecifiedUrlMalformed;                  /**< Specified URL is malformed. */

extern NSString * const CardEaseTerminalId;    /**< There is a format error with the CardEase Terminal ID. */
extern NSString * const TransactionKey;        /**< There is a format error with the Transaction Key. */
extern NSString * const PinpadNotPaired;           /**< The selected PIN pad is not paired. */
extern NSString * const PrinterNotPaired;          /**< The selected Printer is not paired. */
extern NSString * const WebMISUsername;        /**< There is a format error with the WebMIS Username. */
extern NSString * const WebMISPassword;        /**< There is a format error with the Test WebMIS Password. */
extern NSString * const ApiKey;              /**< There is a format error with the Api Key. */
extern NSString * const AmbiguousIdentifier;   /**< An attempt to set more than one identifier has been made. */
extern NSString * const RegistrationFailed;    /**< The registration request has failed. */
extern NSString * const RegistrationFailedToRetrieveTerminalId;    /**< Registration failed to retrieve a terminal ID. */
extern NSString * const RegistrationFailedToRetrieveTransactionKey;    /**< Registration failed to retrieve a transaction key. */
extern NSString * const MerchantConfigurationError;             /**< Merchant's configuration is incorrect. */
extern NSString * const MissingField;                           /**< Request is missing a required field. */
extern NSString * const InvalidField;                           /**< Field within the request is invalid. */
extern NSString * const InvalidHTTPMethod;                      /**< Invalid HTTP method. */
extern NSString * const AccessDenied;                           /**< Access to the gateway has been denied. */
extern NSString * const DeviceConfiguration;                    /**< The configuration on the device is incorrect. */
extern NSString * const UnexpectedField;                        /**< There was an unexpected field within the request. */
extern NSString * const DataTypeError;                          /**< An incorrect data type has been used. */
extern NSString * const ParseError;                             /**< Failed to parse the request. */
extern NSString * const ServerError;                            /**< There has been an error with the server. */
extern NSString * const GeneralError;                           /**< There has been a general error with the gateway. */
extern NSString * const GatewayGeneralError;                    /**< There has been a general error with the gateway. */
extern NSString * const GatewayProcessError;                    /**< There has been a process error with the gateway. */
//TODO: replace generic errors
extern NSString * const ParameterMissing;          /**< A required parameter is missing. */
extern NSString * const ParameterValueInvalid;     /**< The value of a parameter is invalid. */
extern NSString * const UserReferenceMissing;      /**< No user reference found. */
extern NSString * const InvalidUserReference;      /**< User reference entered wasn't valid. Must be a unique per transaction and between 1 and 50 characters long. */
extern NSString * const OperatorPinMissing;        /**< Operator pin required and not found. */
extern NSString * const InvalidOperatorPin;        /**< Operator pin entered wasn't valid. Must be a 4 numeric characters long. */
extern NSString * const InvalidPaymentMethod;      /**< Invalid payment method parameter value. */
extern NSString * const TransactionTypeMissing;    /**< Transaction type parameter method missing. */
extern NSString * const InvalidTransactionType;    /**< Invalid transaction type parameter value. */
extern NSString * const InvalidTippingType;        /**< Invalid tipping type parameter value. */
extern NSString * const InvalidAuthCode;           /**< Invalid authorization code parameter value. */
extern NSString * const AuthCodeMissing;           /**< Authorization code parameter missing. */
extern NSString * const PhoneNumberMissing;        /**< Phone number parameter is missing. */
extern NSString * const InvalidPhoneNumber;        /**< Invalid phone number parameter value. */
extern NSString * const EmailAddressMissing;       /**< Email address is missing. */
extern NSString * const InvalidEmailAddress;       /**< Invalid emails address parameter value. */
extern NSString * const SourceNameMissing;         /**< Source name is missing. */
extern NSString * const SourceEmailMissing;        /**< Source email address is missing. */
extern NSString * const InvalidSourceEmail;        /**< Source email address is invalid. */
extern NSString * const ReceiptTextMissing;        /**< Receipt text is missing. */
extern NSString * const InvalidReceiptType;        /**< The receipt type is invalid. */
extern NSString * const ReceiptSubjectMissing;     /**< The receipt subject text is missing. */
extern NSString * const IdleMessageTextMissing;    /**< The idle message text is missing. */
extern NSString * const SaleReferenceMissing;      /**< The sale reference is missing. */
extern NSString * const InvalidPinPadName;         /**< Invalid PIN pad name value. */
extern NSString * const InvalidConnectionType;     /**< Invalid connection type value. */
extern NSString * const InvalidPinPadIpAddress;    /**< Invalid PIN pad IP address value. */
extern NSString * const InvalidPinPadPort;         /**< Invalid PIN pad port value. */
extern NSString * const PinPadNameMismatch;        /**< Configured PIN pad name does not match that of the connected PIN pad.*/

extern NSString * const FailedToCreate;         /**< Unable to create a database on the mobile device. */
extern NSString * const FailedToOpen;           /**< Unable to open the database on mobile device storage. */
extern NSString * const FailedToRead;           /**< Unable to read from database on the mobile device storage. */
extern NSString * const FailedToWrite;          /**< Unable to write to the database on mobile device storage. */
extern NSString * const FailedToClose;          /**< Unable to close the database on mobile device storage. */
extern NSString * const FailedToUpgrade;        /**< Unable to upgrade the database on the mobile device. */
extern NSString * const FailedToMigrate;        /**< Unable to migrate the database on mobile device storage. */
extern NSString * const FailedToChangePassword; /**< Unable to change database password. */
extern NSString * const IncorrectPassword;      /**< The password given was incorrect. */

extern NSString * const ProvidedSignatureUnsupportedFormat;         /**< The signature format is unsupported. */
extern NSString * const ProvidedSignatureInvalidFormat;             /**< The signature format in invalid. */
extern NSString * const ProvidedSignatureOnlyContainsWhiteSpace;    /**< The signature only contains white space. */
extern NSString * const ProvidedSignatureTooSmall;                  /**< The signature is too small. */
extern NSString * const CroppedSignatureTooSmall;                   /**< The signature is too small after cropping out white space. */
extern NSString * const ResizedSignatureTooSmall;                   /**< The signature is too small after cropping out white space and resizing. */
extern NSString * const SignatureDataMissing;                       /**< Signature data is missing. */

extern NSString * const ReceiptSourceSMSNotSupported;       /**< SMS receipting is not supported. */
extern NSString * const ReceiptSourceEmailNotSupported;     /**< Email receipting is not supported. */
extern NSString * const ProcessReceiptNotSupported;         /**< Receipt processing is not supported. */

extern NSString * const SOAPError;                     /**< The SOAP error occurred. */
extern NSString * const InvalidTimeZoneOffset;         /**< The time zone offset is invalid. */
extern NSString * const LogOnDenied;                   /**< Log on is denied. */
extern NSString * const LogOnManuallyDisabled;         /**< Log on is manually disabled. */
extern NSString * const LogOnAutomaticallyDisabled;    /**< Log on is automatically disabled. */
extern NSString * const LogOnLocked;                   /**< Log on is locked. */
extern NSString * const InvalidLoginPlatform;          /**< The invalid login platform is used. */
extern NSString * const DataNotAvailable;              /**< The request did not return any results. This might be because there are no results for the given data range or because the provided data does not generate any results (for example, incorrect guid for retrieving transaction details). */
extern NSString * const UnexpectedChipDnaError;        /**< ChipDNA encountered an unexpected error.*/

/**
 * @defgroup ConfigurationError
 * @{
 *
 * The possible error codes that can be returned for a Configuration Error.
 */
extern NSString * const ApplicationUpdateRequired;                /**< The current version of the ChipDna Mobile software is invalid and an update is */
extern NSString * const InvalidHomeNetworkIdentity;               /**< The Home Network Identity (HNI) is invalid. */
extern NSString * const TerminalDisabled;                         /**< The terminal is disabled. */
extern NSString * const TransactionTypeNotSupported;              /**< The type of transaction is not supported by the terminal. */
extern NSString * const NoCompletedTransaction;                   /**< A transaction has not been completed. */
extern NSString * const AnotherOperationBeingProcessed;            /**< Another operation is being processed. */
extern NSString * const PinPadAlreadyConnected;                    /**< The PIN pad is already connected. */
extern NSString * const PinPadNotConnected;                        /**< The PIN pad is not connected. */
extern NSString * const UpdateNotConfigured;                       /**< The update has not been configured. */
extern NSString * const TMSUpdateRequired;                         /**< An update from the TMS is required. */
extern NSString * const StandaloneRefundNotSupported;              /**< Standalone refunds are not supported by the terminal. */
extern NSString * const LinkedRefundNotSupported;                  /**< Linked refunds are not supported by the terminal. */
extern NSString * const CashTransactionsNotSupported;              /**< Cash transactions are not supported by this terminal. */
extern NSString * const ChequeTransactionsNotSupported;            /**< Cheque transactions are not supported by this terminal. */

extern NSString * const GratuityNotSupported;                      /**< Gratuity not supported. */
extern NSString * const CurrencyNotFound;                          /**< Currency not found in configuration data. */
extern NSString * const CurrencyNotSet;                            /**< Currency is not set. */
extern NSString * const CurrencyNotSupportedForPaymentMethod;      /**< Currency is not supported for payment method. */
extern NSString * const EndOfDayTippingNotSupported;               /**< End of Day Tipping is not supported */
extern NSString * const OnDeviceTippingNotSupported;               /**< On Device Tipping is not supported */
extern NSString * const PinPadCheckRequired;                       /**< A PIN pad check is required. */
extern NSString * const SREDKeyRequired;                           /**< The SRED Key has failed to inject and is missing. */
extern NSString * const DeviceNotPresentOnWhitelist;               /**< The device is not present on the whitelist. */
extern NSString * const IncorrectPassword;                         /**< The password entered was incorrect. */
extern NSString * const ChipDnaMobileNotInitialized;               /**< ChipDNA Mobile has not been initialized. */
extern NSString * const TerminalIdNotConfigured;                   /**< Terminal ID has not been configured. */
extern NSString * const TransactionKeyNotConfigured;               /**< Transaction key has not been configured. */
extern NSString * const ApplicationIdNotConfigured;                /**< Application Identifier has not been configured. */
extern NSString * const PinPadNotConfigured;                       /**< PIN pad has not been configured. */
extern NSString * const PinPadConnectionTimeout;                   /**< Connection to the PIN pad timed out. */
extern NSString * const FailedToConfigurePinPad;                   /**< Failed to configure PIN pad. */
extern NSString * const TmsUpdateFailed;                           /**< The requested update from TMS has failed. */
extern NSString * const DeviceRegistrationFailed;                  /**< The attempt to register the device has failed. */

extern NSString * const P2PEFailedToReadRKIFiles;                       /**< The P2PE key injection process failed to read RKI files. */
extern NSString * const P2PEFailedToGetHSMFiles;                        /**< The P2PE key injection process failed to get HSM files. */
extern NSString * const P2PEStatus;                                     /**< The P2PE key injection process failed to process status response. */
extern NSString * const P2PEStatusRootCertificate;                      /**< The P2PE key injection process failed to process root certificate. */
extern NSString * const P2PEStatusProductCertificate;                   /**< The P2PE key injection process failed to process product certificate.*/
extern NSString * const P2PEStatusTerminalCertificate;                  /**< The P2PE key injection process failed to process terminal certificate. */
extern NSString * const P2PEStatusKeySigningKey;                        /**< The P2PE key injection process failed to process key signing key. */
extern NSString * const P2PEStatusInternalError;                        /**< The P2PE key injection process failed to process status with internal error. */
extern NSString * const P2PEInitialise;                                 /**< The P2PE key injection process failed to initialise. */
extern NSString * const P2PEInitialiseSystemFileOrCertificateMissing;   /**< The P2PE key injection process failed to initialise with system file or certificate missing. */
extern NSString * const P2PEInitialiseFailedToGenerateRSAKey;           /**< The P2PE key injection process failed to initialise with failing to generate RSA key. */
extern NSString * const P2PEInitialiseFailedToLoadRSAKey;               /**< The P2PE key injection process failed to initialise with loading RSA key. */
extern NSString * const P2PEInitialiseFailedToCreateRSACertificate;     /**< The P2PE key injection process failed to initialise with creating RSA certificate. */
extern NSString * const P2PEInitialiseFailedToPrepareOutputFiles;       /**< The P2PE key injection process failed to initialise with preparing output files. */
extern NSString * const P2PEInitialiseInternalError;                    /**< The P2PE key injection process failed to initialise with internal error. */
extern NSString * const P2PEImport;                                     /**< The P2PE key injection process failed to import. */
extern NSString * const P2PEImportHSMFileMissing;                       /**< The P2PE key injection process failed to import because of missing HSM file. */
extern NSString * const P2PEImportFailedToValidateHSMcrt;               /**< The P2PE key injection process failed to import because of failed HSM certificate validation. */
extern NSString * const P2PEImportFailedToLoadRSAKey;                   /**< The P2PE key injection process failed to import because of failed RSA key loading. */
extern NSString * const P2PEImportFailedToValidateTransportKey;         /**< The P2PE key injection process failed to import because of failed transport key validation. */
extern NSString * const P2PEImportFailedToInstallDUKPTKey;              /**< The P2PE key injection process failed to import because of failed DUKPT key installation. */
extern NSString * const P2PEImportFailedToInstallDUKPTInitialKSN;       /**< The P2PE key injection process failed to import because of failed DUKPT initial KSN installation. */
extern NSString * const P2PEImportInternalError;                        /**< The P2PE key injection process failed to import with internal error. */

extern NSString * const FailedLoadingIncompatiblePinPadSoftware;    /**< The software to be loaded onto the PIN pad was not compatible with the PIN pad. */
extern NSString * const ConnectionClosed;                           /**< The Bluetooth connection to the PIN pad was closed when files where transferred to it. */
extern NSString * const CheckNotComplete;                           /**< The PIN pad configuration check was not completed successfully. */
extern NSString * const IncompatibleOSWithAppMode;                  /**< The PIN pad OS is not compatible with the application mode.  */
extern NSString * const CommunicationsFailed;                       /**< The failure in communications has occurred.  */
extern NSString * const KeysNotLoaded;                              /**< The PIN pad encryption keys are not loaded (when the remote key injection is not supported).  */
extern NSString * const IncompatibleDeviceConfiguration;            /**< The device configuration is invalid.  */
extern NSString * const DeviceRebooting;                            /**< The device is rebooting. */
extern NSString * const DeviceBusy;                                 /**< The device is busy. */
extern NSString * const FailedToRetrieveFirmwareUpdateSessionKeys;  /**< The session keys for the firmware update could not be retrieved. */
extern NSString * const FirmwareUpdateSessionKeyInvalid;            /**< The session key for the firmware update is invalid. */
extern NSString * const FirmwareFileParsingFailed;                  /**< Failed to parse firmware file. */
extern NSString * const FirmwareFileDataMissing;                    /**< The firmware file is missing data required for the firmware update. */

extern NSString * const TerminalRiskManagementDataFormatError;               /**< There is a format error with the Terminal Risk Management Data. */
extern NSString * const CountryCodeFormatError;                              /**< There is a format error with the Country Code. */
extern NSString * const MerchantCategoryCodeFormatError;                     /**< There is a format error with the Merchant Category Code. */
extern NSString * const MerchantNameAddressFormatError;                      /**< There is a format error with the Merchant Name and Address. */
extern NSString * const ReceiptCopyToSelfFormatError;                        /**< There is a format error with the Receipt Copy to Self. */
extern NSString * const WebmisEmailAddressFormatError;                       /**< There is a format error with the WebMIS Email Address. */
extern NSString * const ServerTimeoutFormatError;                            /**< There is a format error with the Server Timeout. */
extern NSString * const TerminalSerialNumberFormatError;                     /**< There is a format error with the Terminal Serial Number. */
extern NSString * const TerminalTypeFormatError;                             /**< There is a format error with the Terminal Type. */
extern NSString * const TerminalCapabilitiesFormatError;                     /**< There is a format error with the Terminal Capabilities. */
extern NSString * const TerminalAdditionalCapabilitiesFormatError;           /**< There is a format error with the Terminal Additional Capabilities. */
extern NSString * const TransactionCategoryCodeFormatError;                  /**< There is a format error with the Transaction Category Code. */
extern NSString * const TimeOut1FormatError;                                 /**< There is a format error with the Time Out 1. */
extern NSString * const TimeOut2FormatError;                                 /**< There is a format error with the Time Out 2. */
extern NSString * const TimeOut3FormatError;                                 /**< There is a format error with the Time Out 3. */
extern NSString * const AcquirerIdentifierFormatError;                       /**< There is a format error with the Acquirer Identifier. */
extern NSString * const MerchantIdentifierFormatError;                       /**< There is a format error with the Merchant Identifier. */
extern NSString * const TerminalIdentifierFormatError;                       /**< There is a format error with the Terminal Identifier. */
extern NSString * const MerchantAccountFormatError;                          /**< There is a format error with the Merchant Account. */
extern NSString * const AmexAcquirerIdentifierFormatError;                   /**< There is a format error with the Amex Acquirer Identifier. */
extern NSString * const AmexMerchantIdentifierFormatError;                   /**< There is a format error with the Amex Merchant Identifier. */
extern NSString * const AmexTerminalIdentifierFormatError;                   /**< There is a format error with the Amex Terminal Identifier. */
extern NSString * const AmexMerchantAccountFormatError;                      /**< There is a format error with the Amex Merchant Account. */
extern NSString * const DinersAcquirerIdentifierFormatError;                 /**< There is a format error with the Diners Acquirer Identifier. */
extern NSString * const DinersMerchantIdentifierFormatError;                 /**< There is a format error with the Diners Merchant Identifier. */
extern NSString * const DinersTerminalIdentifierFormatError;                 /**< There is a format error with the Diners Terminal Identifier. */
extern NSString * const DinersMerchantAccountFormatError;                    /**< There is a format error with the Diners Merchant Account. */
extern NSString * const IccTacSetFormatError;                                /**< There is a format error with the ICC Tac Set. */
extern NSString * const AidFormatError;                                      /**< There is a format error with the AID. */
extern NSString * const ApplicationVersionNumberFormatError;                 /**< There is a format error with the Application Version Number. */
extern NSString * const TacDefaultFormatError;                               /**< There is a format error with the TAC Default. */
extern NSString * const TacDenialFormatError;                                /**< There is a format error with the TAC Denial. */
extern NSString * const TacOnlineFormatError;                                /**< There is a format error with the TAC Online. */
extern NSString * const ForceOnlineFormatError;                              /**< There is a format error with the Force Online. */
extern NSString * const TargetPercentageRandomSelectionFormatError;          /**< There is a format error with the Target Percentage used in Random Selection. */
extern NSString * const MaxTargetPercentageRandomSelectionFormatError;       /**< There is a format error with the Maximum Target Percentage used in Random Selection. */
extern NSString * const ThresholdValueRandomSelectionFormatError;            /**< There is a format error with the Threshold Value used in Random Selection. */
extern NSString * const FloorLimitFormatError;                               /**< There is a format error with the Floor Limit. */
extern NSString * const TdolFormatError;                                     /**< There is a format error with the TDOL. */
extern NSString * const DdolFormatError;                                     /**< There is a format error with the DDOL. */
extern NSString * const PosModeEntryCodeFormatError;                         /**< There is a format error with the POS Mode Entry Code. */
extern NSString * const ProcessTypeFormatError;                              /**< There is a format error with the Process Type. */
extern NSString * const OutputDebuggingFormatError;                          /**< There is a format error with the Output Debugging. */
extern NSString * const SimulateCommsFailureFormatError;                     /**< There is a format error with the Simulate Comms Failure. */
extern NSString * const SignatureCaptureSupportedFormatError;                /**< There is a format error with the Signature Capture Supported. */
extern NSString * const SignatureCheckRequiredFormatError;                   /**< There is a format error with the Signature Check Required. */
extern NSString * const SignatureCheckRequiredNoCvmFormatError;              /**< There is a format error with the Signature Check Required when no CVM. */
extern NSString * const ConstraintChecksFormatError;                         /**< There is a format error with the Constraints Checks. */
extern NSString * const AdditionalIccConstraintChecksFormatError;            /**< There is a format error with the Additional ICC Constraint Checks. */
extern NSString * const AdditionalSwipeConstraintChecksFormatError;          /**< There is a format error with the Additional Swipe Constraint Checks. */
extern NSString * const PinpadKernelFormatError;                             /**< There is a format error with the PIN pad Kernel. */
extern NSString * const OperatorPinFormatError;                              /**< There is a format error with the Operator PIN. */
extern NSString * const LinkedRefundsSupportedFormatError;                   /**< There is a format error with the Linked Refunds Supported. */
extern NSString * const StandaloneRefundsSupportedFormatError;               /**< There is a format error with the Standalone Refunds Supported. */
extern NSString * const PinpadCaKeysFormatError;                             /**< There is a format error with the PIN pad CaKeys. */
extern NSString * const PinpadIccAppsFormatError;                            /**< There is a format error with the PIN pad IccApps.*/
extern NSString * const HomeNetworkIdentityFormatError;                      /**< There is a format error with the Home Network Identity. */
extern NSString * const MaximumDaysBetweenConfigurationUpdatesFormatError;   /**< There is a format error with the Maximum Number of Days Between Configuration Updates. */
extern NSString * const ApplicationStateFormatError;                         /**< There is a format error with the Application State. */
extern NSString * const MinimumMinutesBetweenStatusUpdatesFormatError;       /**< There is a format error with the Minimum Minutes Between Status Updates. */
extern NSString * const TerminalStateFormatError;                            /**< There is a format error with the Terminal State. */
extern NSString * const FileSizeTooLarge;                                    /**< The file size is too large. */
extern NSString * const ReceiptSourceEmailFormatError;                       /**< There is a format error with the Receipt Source Email. */
extern NSString * const ReceiptSourceSmsFormatError;                         /**< There is a format error with the Receipt Source SMS. */
extern NSString * const FileDecodeError;                                     /**< The file could not be decoded. */
extern NSString * const FullIccRefundsSupportedFormatError;                  /**< There is a format error with the Full Icc Refunds Supported. */
extern NSString * const TerminalLanguageFormatError;                         /**< There is a format error with the Terminal Language. */
extern NSString * const InternationalDiallingCodeFormatError;                /**< There is a format error with the International Dialling Code. */
extern NSString * const GratuitySupportedFormatError;                        /**< There is a format error with the Gratuity Supported. */
extern NSString * const NoCvmRefundsSupportedFormatError;                    /**< There is a format error with the No CVM Refunds Supported. */
extern NSString * const RefundSignatureCheckRequiredFormatError;             /**< There is a format error with the Refunds Signature Check Required. */
extern NSString * const OperatorPinRequiredForVerificationFormatError;       /**< There is a format error with the Operator Pin Required For Verification. */
extern NSString * const OperatorPinRequiredForRefundsFormatError;            /**< There is a format error with the Operator Pin Required For Refunds. */
extern NSString * const CashSupportedFormatError;                            /**< There is a format error with the Cash Supported. */
extern NSString * const ChequeSupportedFormatError;                          /**< There is a format error with the Cheque Supported. */
extern NSString * const SOAPUsernameFormatError;                             /**< There is a format error with the SOAP Username. */
extern NSString * const SOAPPasswordFormatError;                             /**< There is a format error with the SOAP Password. */
extern NSString * const OfflineSupportFormatError;                           /**< There is a format error with the Offline Support. */
extern NSString * const FloorLimitsFormatError;                              /**< There is a format error with the Floor Limits. */
extern NSString * const OfflineTransactionVolumeLimitsFormatError;           /**< There is a format error with the Offline Transaction Volume Limits. */
extern NSString * const OfflineRequestRetryMaximumTimeIntervalFormatError;   /**< There is a format error with the Offline Request Retry Maximum Time Interval. */
extern NSString * const OfflineRequestRetryMinimumTimeIntervalFormatError;   /**< There is a format error with the Offline Request Retry Minimum Time Interval. */
extern NSString * const OfflineAuthsSupportedFormatError;                    /**< There is a format error with the Offline Auths Supported. */
extern NSString * const ForcedAcceptanceSupportedFormatError;                /**< There is a format error with the Forced Acceptance Supported. */
extern NSString * const MaximumAmountFormatError;                            /**< There is a format error with the MaximumAmount. */
extern NSString * const MinimumAmountFormatError;                            /**< There is a format error with the MinimumAmount. */
extern NSString * const CardDataInputCapabilityFormatError;                  /**< There is a format error with the Card Data Input Capability. */
extern NSString * const MagstripeAccountTypesSupportedFormatError;           /**< There is a format error with the Magstripe Account Types Supported. */
extern NSString * const PartialApprovalsSupportedFormatError;                /**< There is a format error with the Partial Approvals Supported. */
extern NSString * const PartialApprovalsCompletionSupportFormatError;        /**< There is a format error with the Partial Approvals Completion Support. */
extern NSString * const BinRangeWhitelistSupportedFormatError;               /**< There us a format error with the BinRangeWhitelistSupported. */
extern NSString * const MerchantAccountsFormatError;                         /**< There is a format error with the MerchantAccounts. */
extern NSString * const TestReceiptSelectorOverrideFormatError;              /**< There is a format error with the TestReceiptSelectorOverride. */
extern NSString * const TippingSupportedFormatError;                         /**< There is a format error with the TippingSupported. */
extern NSString * const TerminalConfigurationFormatError;                    /**< There is a format error with the TerminalConfiguration. */
extern NSString * const CAPublicKeysFormatError;                             /**< There is a format error with the CAPublicKeys. */
extern NSString * const PinPadConnectionTimeoutFormatError;                  /**< There is a format error with the PinPadConnectionTimeout. */
extern NSString * const USCommonDebitFormatError;                            /**< There is a format error with the USCommonDebitMode. */
extern NSString * const RefundAmountConfirmationRequiredFormatError;         /**< There is a format error with RefundAmountConfirmationRequired. */
extern NSString * const SaleAmountConfirmationRequiredFormatError;           /**< There is a format error with SaleAmountConfirmationRequired. */
extern NSString * const RkiKeyAvailableFormatError;                          /**< There is a format error with the RkiKeyAvailable. */
extern NSString * const DeferredAuthsSupportedFormatError;                   /**< There is a format error with the DeferredAuthsSupported. */
extern NSString * const DeferredAuthRequestPeriodFormatError;                /**< There is a format error with the DeferredAuthRequestPeriod. */
extern NSString * const DeferredAuthDeclineRetryPeriodFormatError;           /**< There is a format error with the DeferredAuthDeclineRetryPeriod. */
extern NSString * const FileDataNotFound;                                    /**< The file data is missing. */
extern NSString * const MobilePosFormatError;                                /**< There is a format error with the MobilePOS flag. */
extern NSString * const RequestQueueMaximumTimeIntervalFormatError;          /**< There is a format error with RequestQueueMaximumTimeInterval. */
extern NSString * const RequestQueueMinimumTimeIntervalFormatError;          /**< There is a format error with RequestQueueMinimumTimeInterval. */
extern NSString * const RequestQueueRandomTimeIntervalFormatError;           /**< There is a format error with RequestQueueRandomTimeInterval. */
extern NSString * const OperationHeadTimeoutFormatError;                     /**< There is a format error with OperationHeadTimeout. */
extern NSString * const OperationPostTimeoutFormatError;                     /**< There is a format error with OperationPostTimeout. */
extern NSString * const OperationPollTimeoutFormatError;                     /**< There is a format error with OperationPollTimeout. */
extern NSString * const PciP2PEConfigFormatError;                            /**< There is a format error with PciP2PEConfig. */
extern NSString * const PciP2PESupportedFormatError;                         /**< There is a format error with PciP2PESupported. */
extern NSString * const RegisterMaintenanceSettingsFormatError;              /**< There is a format error with RegisterMaintenanceSettings. */
extern NSString * const PaymentPlatformStatusIntervalFormatError;            /**< There is a format error with PaymentPlatformStatusInterval. */
extern NSString * const ManualCardDataInputCapabilityFormatError;            /**< There is a format error with ManualCardDataInputCapability. */
extern NSString * const PanKeyEntrySupportedFormatError;                     /**< There is a format error with PanKeyEntrySupported. */
extern NSString * const LogFileStoreCountLimitFormatError;                   /**< There is a format error with LogFileStoreCountLimit. */
extern NSString * const AutoConfirmSupportedFormatError;                     /**< There is a format error with AutoConfirmSupported. */
/**
 * @}
 */


/**
 * @defgroup PaymentDeviceError
 * @{
 * The possible error codes that can be returned when for Payment Device errors.
 */

extern NSString * const DeviceNotPaired;                     /**< The selected Bluetooth device is not paired. */
extern NSString * const BluetoothNotSupported;               /**< Bluetooth is not supported on this mobile device. */
extern NSString * const NoDeviceSelected;                    /**< No supported Bluetooth device has been selected for connection. */
extern NSString * const ConnectionFailed;                    /**< Connection failed. */
extern NSString * const BluetoothNotEnabled;                 /**< Bluetooth is not enabled on this mobile device. */
extern NSString * const DeviceNotPairedAndConnected;         /**< Bluetooth device is not paired and connected. */
extern NSString * const UnsupportedCommunicationProtocol;    /**< Bluetooth/EA Framework communication protocol is unsupported */

extern NSString * const InvalidState;                       /**< The command can not be processed at this point in the transaction. */
extern NSString * const DisplayTextTooLong;                 /**< The text to display is too long to fit on screen. */
extern NSString * const DisplayTextFormatInvalid;           /**< The text to display contains invalid characters. */
extern NSString * const NilDisplayText;                     /**< The text to display can not be nil. */
extern NSString * const UnableToStartGetCardDetails;        /**< Get card details could not be initialised. */
extern NSString * const ForcedAcceptanceNotSuported;        /**< Forced acceptance is not supported. */
extern NSString * const DeferredAuthorizationNotSupported;  /**< Deferred authorization is not supported. */
extern NSString * const PartialApprovalNotSupported;        /**< Partial approval is not supported. */
extern NSString * const DisplayTextNotSupported;            /**< Display text is not supported. */
extern NSString * const GetCardDetailsNotSupported;         /**< Get card details is not supported. */
extern NSString * const TerminateTransactionNotSupported;   /**< Terminate transaction not supported. */
extern NSString * const IdVerificationNotSupported;         /**< ID verification is not supported. */

extern NSString * const Unmapped;                        /**< Unmapped error occurred. */
extern NSString * const General;                         /**< General error has occurred. */
extern NSString * const InvalidCommandOrMode;            /**< The Bluepad PIN pad is currently in a mode which doesn't accept that command. */
extern NSString * const InvalidParameter;                /**< Invalid parameter sent to Bluepad50 PIN pad. */
extern NSString * const InvalidAddress;                  /**< Invalid Address. */
extern NSString * const InvalidValue;                    /**< Invalid value sent to Bluepad50 PIN pad. */
extern NSString * const InvalidLength;                   /**< Invalid length. */
extern NSString * const UnauthorisedCommandOrMode;       /**< The command is not authorised in this mode. */
extern NSString * const DataMissing;                     /**< There is data missing. */
extern NSString * const TimeOut;                         /**< Action timed out. */
extern NSString * const InvalidKeyID;                    /**< Invalid key ID. */
extern NSString * const InvalidKeyAttribute;             /**< Invalid key attribute. */
extern NSString * const InvalidDevice;                   /**< Invalid device. */
extern NSString * const UnsupportedFirmwareVersion;      /**< Action not supported on this firmware version. */
extern NSString * const PinLimit;                        /**< PIN limit, reserved for future use. */
extern NSString * const FlashError;                      /**< Internal flash error. */
extern NSString * const HardwareError;                   /**< Hardware error. */
extern NSString * const InvalidChecksum;                 /**< Invalid checksum, reserved for future use. */
extern NSString * const ActionCancelled;                 /**< Action aborted with cancel key. */
extern NSString * const InvalidSignature;                /**< Invalid signature. */
extern NSString * const SWSInvalidHeader;                   /**< Invalid header. */
extern NSString * const InvalidPassword;                 /**< Invalid password. */
extern NSString * const InvalidKeyFormat;                /**< Invalid security key format. */
extern NSString * const SCRHardwareError;                /**< SCR Hardware error. */
extern NSString * const HALHardwareError;                /**< HAL Hardware error. */
extern NSString * const InvalidKeyData;                  /**< Invalid key data supplied. */
extern NSString * const InvalidPinData;                  /**< Invalid PIN data in the internal buffer. */
extern NSString * const InvalidReminderData;             /**< Invalid reminder data not provided during ICC process. */
extern NSString * const InitError;                       /**< Init error, reserved for future use. */
extern NSString * const LimitError;                      /**< Limit error, Reserved for future use. */
extern NSString * const InvalidCommandSequence;          /**< Invalid command sequence, reserved for future use. */
extern NSString * const ActionNotPermitted;              /**< Action not permitted. */
extern NSString * const TMKNotLoaded;                    /**< Terminal Master Key not loaded. */
extern NSString * const InvalidKEK;                      /**< Invalid KEK provided. */
extern NSString * const DuplicatedKey;                   /**< Duplicate key. */
extern NSString * const GeneralKeyboardError;            /**< General keyboard error. */
extern NSString * const KeyboardNotCalibrated;           /**< Keyboard not calibrated. */
extern NSString * const KeyboardBugDetected;             /**< Defect in keyboard detected. */
extern NSString * const BluepadBusy;                     /**< Bluepad50 is busy. */
extern NSString * const TamperedModeActive;              /**< Tamper protection has been triggered on the Bluepad50 and the device cannot be used. */
extern NSString * const EncryptedHeaderError;            /**< Error in encrypted header. */
extern NSString * const AcceptedButtonPressed;           /**< Accept button pressed. */
extern NSString * const PINPadOutOfMemory;               /**< PIN pad out of memory. */
extern NSString * const DevicePrinterNotSupported;       /**< The PIN pad does not support printing of receipts. */
extern NSString * const PrinterPaperEmpty;               /**< The device printer is out of paper. */
extern NSString * const PrinterOverheat;                 /**< The device printer has overheated. */
extern NSString * const PrinterError;                    /**< There has been a printer error. */
extern NSString * const PinpadResponseInvalid;           /**< The response recieved from the PINpad is invalid. */

/**
 * @}
 */


/**
 * @defgroup TransactionError
 * @{
 * The possible error codes that can be returned when a transaction is terminated.
 */
extern NSString * const Empty;                                      /**< The transaction failed due to an unknown error. */
extern NSString * const ServerCommunicationError;                   /**< The transaction failed due to a communication error with the server. */
extern NSString * const ServerRequestError;                         /**< The transaction failed due to an error in the server request. */
extern NSString * const ServerResponseError;                        /**< The transaction failed due to an error in the server response. */
extern NSString * const NoResponseFromServer;                       /**< The transaction failed as no response was received from the server. */
extern NSString * const ExceededOfflineStorageLimits;               /**< The transaction failed because the offline storage limits were exceeded. */
extern NSString * const AmountInvalid;                              /**< The amount is invalid. */
extern NSString * const AmountMissing;                              /**< The amount is missing. */
extern NSString * const AmountTooLarge;                             /**< The merchant is not set up to accept a payment this high. */
extern NSString * const AmountTooSmall;                             /**< The merchant is not set up to accept a payment this low. */
extern NSString * const CardBanned;                                 /**< The card has been banned for use on this payment system. */
extern NSString * const CardDetailsNotFound;                        /**< The card details have not been found. */
extern NSString * const CardDetailsUnavailable;                     /**< The card details are unavailable. */
extern NSString * const CardEaseReferenceInvalid;                   /**< The internal system reference is invalid. */
extern NSString * const CardEaseReferenceMissing;                   /**< The internal system reference is missing. */
extern NSString * const CardHashInvalid;                            /**< The card hash is invalid. */
extern NSString * const CardReferenceInvalid;                       /**< The card reference is invalid. */
extern NSString * const CardSchemeNotSupported;                     /**< The merchant is not set up to accept a payment from this card scheme. */
extern NSString * const WhitelistCardPresented;                     /**< E2EE encryption can't be enabled due to missing DUKPT key. */
extern NSString * const CardUsageExceeded;                          /**< The card has been used too many times on this payment system. */
extern NSString * const ExpiredCard;                                /**< The expiry date is invalid; it cannot be in the past. */
extern NSString * const ExpiryDateInvalid;                          /**< The expiry date is invalid. */
extern NSString * const ExpiryDateMissing;                          /**< The expiry date is missing. */
extern NSString * const InvalidData;                                /**< The transaction failed due to invalid data. */
extern NSString * const InvalidMessageType;                         /**< The transaction failed due to an invalid message type. */
extern NSString * const InvalidXMLRequest;                          /**< The transaction failed due to an invalid XML request. */
extern NSString * const IssueNoInvalid;                             /**< The issue number is incorrect for this card. */
extern NSString * const IssueNoMissing;                             /**< The issue number must be supplied for this card. */
extern NSString * const MessageTypeInvalid;                         /**< The transaction failed due to an invalid message type. */
extern NSString * const MessageTypeMissing;                         /**< The transaction failed due to a missing message type. */
extern NSString * const NotAllowed;                                 /**< The transaction request was not permitted. */
extern NSString * const PANFailsLuhnCheck;                          /**< The card number is invalid. */
extern NSString * const PANInvalid;                                 /**< The card number is invalid. */
extern NSString * const PANMissing;                                 /**< The card number is missing. */
extern NSString * const PANTooLong;                                 /**< The card number is too long. */
extern NSString * const PANTooShort;                                /**< The card number is too short. */
extern NSString * const PreValidCard;                               /**< The start date is invalid; it cannot be in the future. */
extern NSString * const StartDateInvalid;                           /**< The start date must be specified for this card. */
extern NSString * const StartDateMissing;                           /**< The start date is missing. */
extern NSString * const TemporarilyUnavailable;                     /**< The payment system is temporarily unavailable. */
extern NSString * const TerminalIDDisabled;                         /**< The Terminal ID is disabled. */
extern NSString * const TerminalIDInvalid;                          /**< The Terminal ID is invalid. */
extern NSString * const TerminalIDMissing;                          /**< The Terminal ID is missing. */
extern NSString * const TerminalIDUnknown;                          /**< The Terminal ID is unknown. */
extern NSString * const TerminalUsageExceeded;                      /**< The terminal has been used too many times on this payment system. */
extern NSString * const TransactionAlreadyRefunded;                 /**< The transaction has already been refunded or refund is greater than original amount. */
extern NSString * const TransactionAlreadySettled;                  /**< The transaction has already been settled. */
extern NSString * const TransactionAlreadyVoided;                   /**< The transaction has already been voided. */
extern NSString * const TransactionKeyIncorrect;                    /**< The transaction key is incorrect. */
extern NSString * const TransactionKeyInvalid;                      /**< The transaction key is invalid. */
extern NSString * const TransactionKeyMissing;                      /**< The transaction key is missing. */
extern NSString * const TransactionNotFound;                        /**< The transaction has not been found. */
extern NSString * const TransactionOriginallyDeclined;              /**< The transaction was originally declined. */
extern NSString * const TransactionNotCommitted;                    /**< The transaction was approved but not committed. */
extern NSString * const XMLDecryptionError;                         /**< The transaction failed due to an XML decryption error. */
extern NSString * const XMLElementMissing;                          /**< The transaction failed due to a missing element. */
extern NSString * const UnableToStartPinpadTransaction;             /**< The transaction could not be started on the PIN pad. */
extern NSString * const UnableToInitialisePinpad;                   /**< Unable to initialise the PIN pad for operation. */
extern NSString * const UnableToStartPinpadProcess;                 /**< The transaction process could not be started on the PIN pad. */
extern NSString * const FailedToTransmitPinpadMessage;              /**< Failed to transmit message to PIN pad. */
extern NSString * const PinpadConnectionClosed;                     /**< The Bluetooth connection to the PIN pad is closed. */
extern NSString * const UnknownRequestFromPinpad;                   /**< The request received from the PIN pad is unknown. */
extern NSString * const UnexpectedRequestFromPinpad;                /**< The request received from the PIN pad is not expected. */
extern NSString * const TerminalConfigurationError;                 /**< Terminal has not been configured correctly to perform a transaction (a property is missing). */
extern NSString * const InvalidIssuerResponse;                      /**< The issuer response has an invalid format. */
extern NSString * const InvalidPinpadRefundResult;                  /**< Unknown transaction result received from PIN pad. */
extern NSString * const PinpadTransactionTerminated;                /**< (PIN pad) Transaction Terminated. */
extern NSString * const ChipApplicationSelectionFailure;            /**< (Chip) Application Selection Failure. */
extern NSString * const ChipInitiateApplicationProcessingFailure;   /**< (Chip) Initiate Application Processing Failure. */
extern NSString * const ChipReadApplicationDataFailure;;             /**< (Chip) Read Application Data Failure. */
extern NSString * const ChipOfflineDataAuthenticationFailure;       /**< (Chip) Offline Data Authentication Failure. */
extern NSString * const ChipProcessRestrictionsFailure;             /**< (Chip) Process Restrictions Failure. */
extern NSString * const ChipTerminalRiskManagementFailure;          /**< (Chip) Terminal Risk Management Failure. */
extern NSString * const ChipCardholderVerificationMethodFailure;    /**< (Chip) Cardholder Verification Method Failure. */
extern NSString * const ChipTerminalActionAnalysisFailure;          /**< (Chip) Terminal Action Analysis. */
extern NSString * const ChipCardActionAnalysisFailure;              /**< (Chip) Card Action Analysis Failure. */
extern NSString * const ChipCompletionFailure;                      /**< (Chip) Completion Failure. */
extern NSString * const EposTransactionTerminated;                  /**< (EPOS) Transaction Terminated. */
extern NSString * const ChipNoAnswerToReset;                        /**< (Chip) No Answer to Reset. */
extern NSString * const SwipeReadFailure;                           /**< (Swipe) Read Failure. */
extern NSString * const ChipCardRemoved;                            /**< (Chip) Card Removed. */
extern NSString * const PinpadUserCancelled;                        /**< (PIN pad) User Cancelled. */
extern NSString * const ChipNoSupportedApplications;                /**< (Chip) No Supported Applications. */
extern NSString * const ChipCardBlocked;                            /**< (Chip) Card Blocked. */
extern NSString * const ChipReadFailure;                            /**< (Chip) Read Failure. */
extern NSString * const ApplicationNotSupported;                    /**< Terminal has not been configured to support the selected application. */
extern NSString * const VoiceReferralNotificationError;             /**< An error occurred while notifying the server of the voice referral result. */
extern NSString * const OfflineIccAuthorisationError;               /**< An error occurred during authorisation of an off-line ICC transaction. */
extern NSString * const AtmCashOnlyCard;                            /**< The third digit of the service code indicates this is an ATM or cash only card and cannot be used for a transaction. */
extern NSString * const CardHolderActionTimedOut;                   /**< (PIN pad) CardHolder Action Timed Out. */
extern NSString * const PinpadDukptKeyFailure;                      /**< (PIN pad) DUKPT Key Failure. */
extern NSString * const InvalidDataInCommandError;                  /**< Invalid Data in command APDU. */
extern NSString * const TerminalNotReadyError;                      /**< Terminal not ready (Continue before Start). */
extern NSString * const NoSmartcardInSlotError;                     /**< No smartcard in slot. */
extern NSString * const InvalidCardResponseError;                   /**< Invalid card, card responded incorrectly, no MSR fall back allowed. */
extern NSString * const TransactionAlreadyInProgressError;          /**< Transaction already in progress. */
extern NSString * const MissingDataInCommandError;                  /**< Data missing from command APDU (data field should list missing object(s)). */
extern NSString * const MissingFileError;                           /**< Missing file (data field should list missing file). */
extern NSString * const InvalidIssuerPublicKeyError;                /**< Invalid issuer public key, hash check failed. */
extern NSString * const TestCheckError;                             /**< Test check failed with an unknown error.*/
extern NSString * const GeneralEMVError;                            /**< General EMV error occurred.*/
extern NSString * const CardBlockedError;                           /**< Card blocked.*/
extern NSString * const TimeOutError;                               /**< The transaction has timed out.*/
extern NSString * const TransactionAbortedError;                    /**< The transaction was aborted.*/
extern NSString * const ConfigurationError;                         /**< Configuration files are not loaded.*/
extern NSString * const InvalidApplicationError;                    /**< Invalid application error.*/
extern NSString * const PINError;                                   /**< An error occurred during the PIN entry.*/
extern NSString * const OperatorPinIncorrect;                       /**< An operator PIN in incorrect. */
extern NSString * const OfflineRequestTooOld;                       /**< Offline stored request too old to be processed. */
extern NSString * const TimeOutErrorContactless;                    /**< Contactless transaction timed out. */
extern NSString * const ContactlessHardwareError;                   /**< Contactless hardware error. */
extern NSString * const NoInterfacesEnabled;                        /**< There are no enabled interfaces for processing transactions. */
extern NSString * const MagstripeInterfaceNotEnabled;               /**< Magstripe card has been swiped and magstripe interface is not enabled. */
extern NSString * const EMVInterfaceNotEnabled;                     /**< EMV interface is not enabled and the card cannot be processed using magstripe interface. */
extern NSString * const MagstripeInterfaceNotEnabledEMVFallbackNotPossible;                  /**< EMV card requires a fallback to magstripe and magstripe interface is not enabled. */
extern NSString * const EMVAndMagstripeInterfacesNotEnabledContactlessFallForwardNotPossible;   /**< Contactless card requires EMV or magstripe, EMV and magstripe interfaces are not enabled. */
extern NSString * const EMVInterfaceNotEnabledContactlessFallbackNotPossible;                /**< Contactless card requires EMV, EMV interface is not enabled. */
extern NSString * const OnlinePINInternalError1;                    /**< Online PIN internal error 1. */
extern NSString * const OnlinePINInternalError2;                    /**< Online PIN internal error 2. */
extern NSString * const OnlinePINInternalError3;                    /**< Online PIN internal error 3. */
extern NSString * const OnlinePINKeyMissingForMagstripeDebit;       /**< Online PIN key is missing, magstripe debit transaction cannot be carried out. */
extern NSString * const MerchantTerminateTransaction;               /**< Merchant Terminated Transaction. */
extern NSString * const TransactionAlreadyCommitted;                /**< Transaction has already been committed */
extern NSString * const TippingNotAllowed;                          /**< Tipping is not allowed for this transaction. */
extern NSString * const AmountModificationNotAllowed;               /**< Amount modification is not allowed for this transaction. */
extern NSString * const EndOfDayTippingNotEnabled;                  /**< End of day tipping is not enabled for this transaction. */
extern NSString * const ApiKeyMissing;                              /**< The Api Key is missing. */
extern NSString * const ApiKeyInvalid;                              /**< The Api Key is invalid. */
extern NSString * const PosGuidMissing;                             /**< The POS GUID is missing. */
extern NSString * const PosGuidInvalid;                             /**< The POS GUID is invalid. */
extern NSString * const ApplicationIdMissing;                       /**< The application identifier is missing. */
extern NSString * const GatewayRejectedTransaction;                 /**< The gateway has rejected the transaction. */
extern NSString * const RegistrationRequired;                       /**< A request to register the device is required */
extern NSString * const CustomerVaultIdInvalid;                     /**< The customer vault id is invalid. */
extern NSString * const CustomerVaultCommandInvalid;                /**< The customer vault command is invalid. */
extern NSString * const ApplicationIdInvalid;                       /**< The application identifier is invalud. */
extern NSString * const PinPadInternalModuleError;                  /**< Incorrect Module Error */
extern NSString * const PinPadIncorrectP2PEStatus;                  /**< Incorrect P2PE status for this command, status include "SRED Ready" */
extern NSString * const PinPadEncryptionError;                      /**< Encryption Error */
extern NSString * const UnsupportedCommand;                         /**< The PIN pad doesn't support this command. */
extern NSString * const MerchantDefinedField01Invalid;              /**< Merchant Defined Field 01 is Invalid. */
extern NSString * const MerchantDefinedField02Invalid;              /**< Merchant Defined Field 02 is Invalid. */
extern NSString * const MerchantDefinedField03Invalid;              /**< Merchant Defined Field 03 is Invalid. */
extern NSString * const MerchantDefinedField04Invalid;              /**< Merchant Defined Field 04 is Invalid. */
extern NSString * const MerchantDefinedField05Invalid;              /**< Merchant Defined Field 05 is Invalid. */
extern NSString * const MerchantDefinedField06Invalid;              /**< Merchant Defined Field 06 is Invalid. */
extern NSString * const MerchantDefinedField07Invalid;              /**< Merchant Defined Field 07 is Invalid. */
extern NSString * const MerchantDefinedField08Invalid;              /**< Merchant Defined Field 08 is Invalid. */
extern NSString * const MerchantDefinedField09Invalid;              /**< Merchant Defined Field 09 is Invalid. */
extern NSString * const MerchantDefinedField10Invalid;              /**< Merchant Defined Field 10 is Invalid. */
extern NSString * const MerchantDefinedField11Invalid;              /**< Merchant Defined Field 11 is Invalid. */
extern NSString * const MerchantDefinedField12Invalid;              /**< Merchant Defined Field 12 is Invalid. */
extern NSString * const MerchantDefinedField13Invalid;              /**< Merchant Defined Field 13 is Invalid. */
extern NSString * const MerchantDefinedField14Invalid;              /**< Merchant Defined Field 14 is Invalid. */
extern NSString * const MerchantDefinedField15Invalid;              /**< Merchant Defined Field 15 is Invalid. */
extern NSString * const MerchantDefinedField16Invalid;              /**< Merchant Defined Field 16 is Invalid. */
extern NSString * const MerchantDefinedField17Invalid;              /**< Merchant Defined Field 17 is Invalid. */
extern NSString * const MerchantDefinedField18Invalid;              /**< Merchant Defined Field 18 is Invalid. */
extern NSString * const MerchantDefinedField19Invalid;              /**< Merchant Defined Field 19 is Invalid. */
extern NSString * const MerchantDefinedField20Invalid;              /**< Merchant Defined Field 20 is Invalid. */
extern NSString * const BillingAddress1Invalid;                     /**< Billing Address Line One in Billing Information is invalid. */
extern NSString * const BillingAddress2Invalid;                     /**< Billing Address Line Two in Billing Information is invalid. */
extern NSString * const BillingCityInvalid;                         /**< Billing City in Billing Information is invalid. */
extern NSString * const BillingStateInvalid;                        /**< Billing State in Billing Information is invalid. */
extern NSString * const BillingZipCodeInvalid;                      /**< Billing ZipCode in Billing Information is invalid. */
extern NSString * const BillingPostalCodeInvalid;                   /**< Billing Postal Code in Billing Information is invalid. */
extern NSString * const BillingCountryInvalid;                      /**< Billing Country in Billing Information is invalid. */
extern NSString * const BillingEmailAddressInvalid;                 /**< Billing Email Address in Billing Information is invalid. */
extern NSString * const BillingPhoneNumberInvalid;                  /**< Billing Telephone Number in Billing Information is invalid. */
extern NSString * const AmbiguousBillingInformation;                /**< Billing Information contains ambiguous parameter values. They should be checked for consistency. */
extern NSString * const PONumberInvalid;                            /**< The Purchase Order Number is invalid. */
extern NSString * const TaxAmountInvalid;                           /**< The Tax Amount is invalid. */
extern NSString * const PANKeyEntryTransactionsNotSupported;        /**< PAN key entry has not been enabled.. */
extern NSString * const PANKeyEntryTippingNotAllowed;               /**< PAN key entry with tipping is not allowed. */
extern NSString * const PANKeyEntryWithoutCardNotAllowed;           /**< PAN key entry without payment type card is not allowed. */
extern NSString * const PANKeyEntryOfflineNotAllowed;               /**< PAN key entry transactions cannot be authorized offline. */
extern NSString * const VoiceReferralResponseNotExpected;           /**< Unexpected Voice Referral response. */
extern NSString * const AutoConfirmNotSupported;                    /**< Auto confirmation of transaction is not supported.*/
extern NSString * const VoidRequestFailed;                          /**< Confirmation request failed to go online and wasn't able to be stored offline. Confirmation requests can be retried using {@link ChipDnaMobile#confirmTransaction:}.*/
extern NSString * const ConfirmRequestFailed;                       /**< Void request failed to go online and wasn't able to be stored offline. Void requests can be retried using {@link ChipDnaMobile#voidTransaction:}. */
extern NSString * const AmbiguousReference;                         /**< Ambiguous reference parameters supplied. Multiple identifiers for a transaction are not supported. */

/**
 * @}
 */

@interface ErrorCodes : NSObject

@end
