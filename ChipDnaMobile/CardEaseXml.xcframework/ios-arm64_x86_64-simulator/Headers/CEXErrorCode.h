/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXEmailAddressType field for a given
 *        string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXErrorCodeParser
 */
#import <Foundation/Foundation.h>

/// @file

/** @brief The possible error codes that can be returned from a CardEaseXML request. **/
typedef enum
{
	ErrorCodeEmpty,                                     /*! No error code. */
	ErrorCodeAmountInvalid,                             /*! The specified amount in the request is invalid. */
	ErrorCodeAmountMissing,                             /*! The request does not contain an amount. */
	ErrorCodeAmountTooLarge,                            /*! The specified amount in the request is too large. */
	ErrorCodeAmountTooSmall,                            /*! The specified amount in the request is too small. */
	ErrorCodeCardBanned,                                /*! The specified card in the request has been banned. */
	ErrorCodeCardDetailsNotFound,                       /*! The card details referenced by the CardReference and 
                                                             CardHash could not be found. */
    
	ErrorCodeCardDetailsUnavailable,                    /*! The card details referenced by the CardReference and           
                                                             CardHash are unavailable. */
    
	ErrorCodeCardEaseReferenceInvalid,                  /*! The specified CardEase reference in the request is 
                                                             invalid. */
    
	ErrorCodeCardEaseReferenceMissing,                  /*! The request does not contain a CardEase reference. */
	ErrorCodeCardHashInvalid,                           /*! The specified card hash in the request is not 
                                                             valid. */
	ErrorCodeCardReferenceInvalid,                      /*! The specified card reference in the request is not 
                                                             valid. */
    
	ErrorCodeCardSchemeNotSupported,                    /*! The specified card scheme in the request is not 
                                                            supported. */
    
	ErrorCodeCardUsageExceeded,                         /*! The specified card usage in the request has been 
                                                            exceeded. */
    
	ErrorCodeExpiredCard,                               /*! The specified card in the request has expired. */
	ErrorCodeExpiryDateInvalid,                         /*! The specified expiry date in the request is invalid.*/
	ErrorCodeExpiryDateMissing,                         /*! The request does not contain an expiry date. */
	ErrorCodeInvalidData,                               /*! An invalid piece of information was sent in the 
                                                            request. */
    
	ErrorCodeInvalidMessageType,                        /*! The specified request type is invalid. */
	ErrorCodeInvalidXmlRequest,                         /*! The request XML is invalid. */
	ErrorCodeIssueNoInvalid,                            /*! The specified issuee number in the request is 
                                                            invalid. */
    
	ErrorCodeIssueNoMissing,                            /*! The request does not contain an issue number. */
	ErrorCodeMessageTypeMissing,                        /*! The message type in the request is missing. */
	ErrorCodeMessageTypeInvalid,                        /*! The specified message type in the request is 
                                                            invalid.*/
    
	ErrorCodeNotAllowed,                                /*! The specified transaction in the request is not 
                                                         allowed. */
    
	ErrorCodePanFailsLuhnCheck,                         /*! The specified PAN in the request fails the Luhn 
                                                            check.*/
    
	ErrorCodePanInvalid,                                /*! The specified PAN in the request is invalid. */
	ErrorCodePanMissing,                                /*! The request does not contain a PAN. */
	ErrorCodePanTooLong,                                /*! The specified PAN in the request is too long. */
	ErrorCodePanTooShort,                               /*! The specified PAN in the request is too short. */
	ErrorCodePreValidCard,                              /*! The specified card in the request is not yet 
                                                            effective. */
    
	ErrorCodeStartDateInvalid,                          /*! The specified start date in the request is invalid. */
	ErrorCodeStartDateMissing,                          /*! The request does not contain a start date. */
	ErrorCodeTemporarilyUnavailable,                    /*! The CardEase platform is temporarily unavailable. */
	ErrorCodeTerminalIdDisabled,                        /*! The specified termainl id in the request is 
                                                            disabled. */
	
    ErrorCodeTerminalIdInvalid,                         /*! The specified terminal id in the request is 
                                                            invalid. */

	ErrorCodeTerminalIdMissing,                         /*! The request does not contain a terminal id. */
	ErrorCodeTerminalIdUnknown,                         /*! The specified terminal id in the request is unknown.*/
	ErrorCodeTerminalUsageExceeded,                     /*! The specified terminal id usage in the request has 
                                                            been exceeded. */
    
	ErrorCodeTransactionAlreadyRefunded,                /*! The transaction has already been refunded in full. */
	ErrorCodeTransactionAlreadySettled,                 /*! The transaction in the request has already been 
                                                            settled. */
    
	ErrorCodeTransactionAlreadyVoided,                  /*! The specified transaction in the request has already 
                                                            been voided. */
    
	ErrorCodeTransactionKeyIncorrect,                   /*! The specified transaction key in the request is 
                                                            incorrect. */
    
	ErrorCodeTransactionKeyInvalid,                     /*! The specified transaction key in the request is 
                                                            invalid. */
    
	ErrorCodeTransactionKeyMissing,                     /*! The specified transaction key in the request is 
                                                            missing. */
    
	ErrorCodeTransactionNotFound,                       /*! The specified transaction in the request was not 
                                                            found.*/
    
    ErrorCodeApiKeyMissing,                             /*! The ApiKey is missing from the register query
                                                            request.*/
    
    ErrorCodeApiKeyInvalid,                             /*! The ApiKey is invalid. */
    
    ErrorCodePosGuidMissing,                            /*! The POS GUID is missing from the register query
                                                            request.*/
    
    ErrorCodePosGuidInvalid,                            /*! The POS GUID is invalid.*/
    
    ErrorCodeApplicationIdMissing,                      /*! The application Id is missing from the register
                                                            query request.*/
    
    ErrorCodeGatewayRejectedTransaction,                /*! Transaction rejected by the gateway.*/
    
    ErrorCodeTMSUpdateForced,                           /*! Terminal requires a TMS update.*/
    
    ErrorCodeGatewayProcessError,                       /*! The gateway had a processing error.*/
    
    ErrorCodeGatewayGeneralError,                       /*! There has been a general error with the gateway.*/
    
    ErrorCodeDeviceRegistrationFailed,                  /*! Failed to register POI information.*/
    
    ErrorCodeMerchantConfigurationError,                /*! Merchant's configuration is incorrect.*/
    ErrorCodeMissingField,                              /*! Request is missing a field.*/
    ErrorCodeInvalidField,                              /*! Field within the request is invalid.*/
    ErrorCodeInvalidHTTPMethod,                         /*! Invalid HTTP method.*/
    ErrorCodeAccessDenied,                              /*! Access to the gateway has been denied.*/
    ErrorCodeDeviceConfiguration,                       /*! The configuration on the device is incorrect.*/
    ErrorCodeUnexpectedField,                           /*! There was an unexpected field in the request.*/
    ErrorCodeDataTypeError,                             /*! Incorrect data type used.*/
    ErrorCodeParseError,                                /*! Failed to parse request.*/
    ErrorCodeServerError,                               /*! Failed to connect to server.*/
    ErrorCodeGeneralError,                              /*! General error.*/
    
	ErrorCodeTransactionOriginallyDeclined,             /*! The specified transaction in the request was 
                                                            originally declined. */
    
	ErrorCodeXmlDecryptionError,                        /*! It is not possible to decrypt the XML. */
	ErrorCodeXmlElementMissing,                         /*! The request does not contain all of the expected XML 
                                                            elements. */
} CEXErrorCode;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXErrorCode.
 *
 * @property ErrorCodeToString
 */
extern NSString *ErrorCodeToString[70];

@interface CEXErrorCodeParser : NSObject

/**
 * @brief Returns the coresponding CEXErrorCode field for a given string value.
 *
 * @param code The string respresentation of an CEXErrorCode field.
 * @return The coresponding CEXErrorCode field.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXErrorCode)parse:(NSString *)code;
@end
