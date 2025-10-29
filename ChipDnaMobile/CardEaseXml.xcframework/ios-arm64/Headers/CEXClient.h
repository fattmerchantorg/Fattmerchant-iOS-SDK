/**
 * @mainpage
 *
 * CardEaseXML is a flexible client-server framework for processing credit and debit card transactions
 * over an internet connection.
 *
 * It has the ability to handle manual, magnetic stripe and EMV details for a number of different request
 * types such as authorisations and refunds.
 *
 * To facilitate the use of CardEaseXML, Creditcall manage a set of secure servers which accept XML formatted
 * messages over a TCP/IP connection which describe the required transactions.
 *
 * There are essentially three ways of communicating with the Creditcall CardEaseXML servers;
 *  <ul type="disc">
 *    <li>SSL connection.</li>
 *    <li>HTTPS connection.</li>
 *    <li>Using an established SDK made available by Creditcall.</li>
 *  </ul>
 *
 * These SDKs are supported by detailed protocol documentation and a test environment under which integration
 * can be performed.
 *
 * Using an SDK developed by Creditcall allows for quicker integration as it is using a code base that has been
 * tested to conform to the CardEaseXML protocol.
 *
 * At this time client SDKs are available for the following development platforms:
 *
 * <ul type="disc">
 *   <li>.NET 2.0</li>
 *   <li>Android</li>
 *   <li>Java</li>
 *   <li>Objective C</li>
 *   <li>PHP</li>
 * </ul>
 *
 * This document describes the interfaces for using the Objective C SDK, it does not provide a detailed CardEaseXML
 * protocol description, or documentation regarding other SDKs. These are available separately from Creditcall.
 *
 * The Objective C SDK has the following dependencies:
 *
 * <ul type="disc">
 *   <li>iOS 5.0</li>
 * </ul>
 *
 * Registration to use CardEaseXML can be performed by visiting this web site and completing the necessary form:
 *
 * <a href="https://testwebmis.creditcall.com">https://testwebmis.creditcall.com</a>
 *
 * During the process of registration it will be necessary to supply a valid email address and choose a password.
 * An email will be sent to the supplied email address which will enable confirmation of registration.
 *
 * Registration will provide details of a CardEase Terminal ID and a CardEase Transaction Key.
 * It will be necessary to make a note of these as they will be required for integration, testing and using CardEaseXML.
 *
 * Once registration is complete and all of the necessary details have been obtained, development can proceed.
 *
 *
 * © Copyright Creditcall Limited 2005 - 2020
 *
 * Tel. +44 (0)117 930 4455<br/>
 * Tel. +1 (800) 868-1832<br/>
 * Email: CExmlSupport@creditcall.com<br/>
 *
 * Creditcall Ltd. Registered Office: Merchants House North, Wapping Road, Bristol BS1 4RW. 0117 930 4455 Registered No: 3295353. VAT No: 713 0076 80.<br/>
 * Creditcall Corporation. 1133 Broadway, Suite 706, New York, NY 10010. USA. 800 868 1832.
 **/


/**
 * @author Creditcall Ltd
 * @brief This class is used to process requests by sending them to CardEaseXML servers.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXClient
 * @details This class is used to process requests by sending them to CardEaseXML servers.
 *
 * This client supports iOS 5.0+
 *
 * To use the CEXClient, a CEXRequest object should be created and populated with the necessary
 * transaction information such as the request type and terminal id, as well as any
 * optional information that is required such as a user reference or address verification
 * information.
 *
 * Due to the redundancy available in the CardEase platform, the client can communicate
 * with any number of CardEaseXML servers until a successful connection is made. Therefore,
 * it is necessary to specify the serverURLs with which you would like the CEXClient to
 * communicate with in the order that you would like the communication to occur. Each of the
 * servers will be contacted in turn until a successful request is made.
 *
 * If a connection is successful processing stops and the CEXResponse can be retrieved.
 * If a connection is unsuccessful the next host is tried until there are no more available.
 * If the CEXClient runs out of servers to contact, the request fails.
 *
 * A successful request will obtain a CEXResponse object from the CardEase platform.
 * The response should be examined for error information and the results of the specified
 * transaction.
 *
 * @see CEXRequest
 * @see CEXResponse
 * @see CEXServerUrl
 
 * NOTE:  Proxy connections are currently NOT supported in this version of the SDK.
 **/
#import <Foundation/Foundation.h>

#import "CEXRequest.h"
#import "CEXResponse.h"
#import "CEXServerUrl.h"
#import "CEXXmlEncoding.h"

/// @file

/** @brief The CardEaseXml error domain. **/
extern NSString * const CardEaseXmlErrorDomain;

/** @brief The key for accessing the status code from a CardEaseXml domain error. **/
extern NSString * const CardEaseXmlErrorKeyStatusCode;

/** @brief The key for accessing the validation error from a CardEaseXml domain error. **/
extern NSString * const CardEaseXmlErrorKeyValidationError;

/** @brief The key for accessing the error reason/message from a CardEaseXml domain error. **/
extern NSString * const CardEaseXmlErrorMessageKey;

/** @brief the key for accessing the recovery action from a CardEaseXml domain error. **/
extern NSString * const CardEaseXmlRecoveryActionKey;

/** @brief the key for accessing the operation reference from a CardEaseXml domain error. **/
extern NSString * const CardEaseXmlOperationReferenceKey;

// Config Options

/** todo comments **/
extern NSString * const CardEaseXmlConfigOptionsReliablityMode;
extern NSString * const CardEaseXmlConfigOptionsTransferMode;
extern NSString * const CardEaseXmlConfigOptionsTransferModePullIntervalTimeout;
extern NSString * const CardEaseXmlConfigOptionsOperationPostTimeout;
extern NSString * const CardEaseXmlConfigOptionsOperationHeadTimeout;
extern NSString * const CardEaseXmlConfigOptionsOperationPath;

// Recovery Actions
/** @brief No further action required **/
extern NSString * const CEXRecoveryActionNone;

/** @brief Host application should attempt to void the reference provided in order to clear up a potentially accepted transaction. **/
extern NSString * const CEXRecoveryActionVoid;

/** @brief he host application should retry their failed operation (e.g. void/conf) at a later time. e.g. 10mins, 60mins, 4hrs, 1day, 2day up to a maximum 5 reties. **/
extern NSString * const CEXRecoveryActionRetry;

typedef enum
{
    CEXCommunicationError,
    CEXRequestError,
    CEXResponseError,
    CEXConfigurationError,
} CEXClientErrorCode;

@interface CEXClient : NSObject
{
    /**
     * @private
     * @brief The most recent error to have occured whilst processing.
     **/
    NSError *lastError;
    
    /**
     * @private
     * @brief The xml body content of the recieved response.
     **/
    NSMutableString *responseXml;
}

/**
 * @brief The XML encoding used in communication with the CardEaseXML server.
 * @property encoding
 **/
@property CEXXmlEncoding encoding;

/**
 * @brief The request to send to the CardEaseXML server.
 * @property request
 **/
@property (strong) CEXRequest *request;

/**
 * @brief The response retrieved from the CardEaseXML server.
 * @property response
 **/
@property (strong) CEXResponse *response;

/**
 * @brief The list of servers to communicate with.
 * @property serverUrls
 **/
@property (strong) NSMutableArray *serverUrls;

/**
 * @brief The public key certificate that is used for the encryption of requests for offline storage.
 * @property xmlEncryptionCertificate
 **/
@property SecCertificateRef xmlEncryptionCertificate;

/**
 * @brief The filename of the public key certificate that is used for the encryption of requests for offline storage.
 * @property xmlEncryptionCertificateFile
 **/
@property (strong) NSString *xmlEncryptionCertificateFile;

/**
 * @brief The internal name of this CardEaseXML SDK.
 * @see cardEaseXmlSdkVersion
 **/
+(NSString *)cardEaseXmlSdkName;

/**
 * @brief The internal version of this CardEaseXML SDK.
 * @see cardEaseXmlSdkName
 **/
+(NSString *)cardEaseXmlSdkVersion;

/**
 * @brief Initialises a CEXClient instance.
 *
 * @return An initialised CEXClient instance.
 **/
-(CEXClient *)init;

/**
 * @brief Adds a CardEaseXML server URL to the list used for communication.
 *
 * During processing of a request each of these are used in turn until
 * communication is successful.
 *
 * @param url The server URL to add to the end of the server URL list.  This should not be null.
 * @see serverUrls
 **/
-(void)addServerUrl:(CEXServerUrl *)url;

/**
 * @brief Adds a CardEaseXML server URL to the list used for communication.
 *
 * During processing of a request each of these are used in turn until
 * communication is successful.
 *
 * @param url The actual URL of the server URL. This should be a HTTPS URL and in the form:
 * "https://...".  This must not be null.
 *
 * @param timeout The read timeout for the specified server URL in milliseconds.  For most
 * requests a timeout of 45 seconds (45000) is recommended.
 *
 **/
-(void)addServerUrl:(NSString*) url : (NSUInteger) timeout;

/**
 * @brief This method uses all of the internal settings to process a CardEaseXML request.
 *
 * A request and server URL must be set before processing can begin.
 *
 * In turn each of the CardEaseXML server URLs are connected to and the
 * transaction is attempted. If a communication error occurs the next server
 * URL is used and so on. This continues until the list of server URLs is
 * exhausted. If communication totally fails an error is returned detailing
 * the last connection failure.
 *
 * @return A NSError instance or nil if no error occurred whilst processing the request.
 **/
-(NSError *)processRequest;

/**
 * @brief This method uses all of the internal settings to process a CardEaseXML request which is stored in a string.
 *
 * A server URL must be set before processing can begin.
 *
 * In turn each of the CardEaseXML server URLs are connected to and the
 * transaction is attempted. If a communication error occurs the next server
 * URL is used and so on. This continues until the list of server URLs is
 * exhausted. If communication totally fails an error is returned detailing
 * the last connection failure.
 *
 * @param requestXML XML The XML request as string.
 * @return A NSError instance or nil if no error occurred whilst processing the request.
 **/
-(NSError *)processRequest:(NSString *)requestXML;

/**
 * @brief This method saves a request to a file with sections that may contain encrypted sensitive data.
 *
 * An encrypted request can be retrieved, stored and resubmitted at a later date or time.
 * It is not necessary to perform encryption of requests that can be processed online as they are secured via SSL and not stored.
 *
 * @param error an error returned if the validation or other errors occured.
 * @param fileName The filename to write the encrypted XML to.
 **/
-(void)saveEncryptedRequestAsAFile:(NSString*)fileName error:(NSError **)error;

/**
 * @brief Saves a request to a string with sections that may contain encrypted sensitive data.
 *
 * An encrypted request can be retrieved, stored and resubmitted at a later date or time.
 * It is not necessary to perform encryption of requests that can be processed online as they are secured via SSL and not stored.
 *
 * @param error an error returned if the validation or other errors occured.
 * @return An NSString containing the encrypted XML.
 **/
-(NSString*)saveEncryptedRequestAsAString:(NSError **)error;

/**
 * @brief This method returns XML string of the currently set request.
 *
 * Request is validated before XML string is created. Validation errors might occur.
 *
 * @param error an error returned if the validation or other errors occured.
 * @return A NSError instance or nil if no error occurred whilst converting the request.
 **/
-(NSString *)requestAsString:(NSError **)error;

/**
 * @brief Set configuration options for configuring how the client will process requests.
 *
 * @param key       Key for configuration option.
 * @param value     Value of configuration option.
 *
 * @return An NSError instance if the specified key does not exist.
 */
+(NSError *)setConfigOption:(NSString *)key value:(NSObject *)value;

/**
 * @brief Return a configuration option for how the client will process requests.
 *
 * @param key       Key for configuration option.
 *
 * @return An NSError instance if the specified key does not exist.
 */
+(NSError *)getConfigOption:(NSString *)key;

/**
 * Determines whether the encryption of requests is possible for offline storage.
 *
 * Note: For encryption of requests for offline storage to be possible the public key certificate must be present, loadable and active (i.e. within its start date and expiry date).
 *      The encrypted request can then be retrieved, stored and resubmitted at a later date or time.
 *      It is not necessary to perform encryption of requests that can be processed online as they are secured via SSL and not stored.
 *
 * @return Whether the encryption of request items is possible for offline storage.
 */
-(BOOL)isXmlEncryptionAvailable;

@end
