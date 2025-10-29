/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXRequestType field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXRequestTypeParser
 */
#import <Foundation/Foundation.h>

/// @file

/** 
 * @brief The types of CardEaseXML request to be processed.
 * @see CEXRequest
 * @see CEXResponse
 **/
typedef enum
{
    /** @brief Empty*/
	RequestTypeEmpty,

    /** 
     * @brief An authorisation request. 
     *
     * This is a request for payment. This can include manual, magnetic stripe and EMV transactions.
     *
     * EMV transactions will be processed and any ICC data provided by the issuer will be returned
     * as part of the response data.
     **/
	RequestTypeAuth,
    
    /** 
     * @brief A confirmation request. 
     *
     * This is the second stage to an authorisation request in which the authorisation obtained is
     * confirmed. This is required before settlement can be achieved.
     *
     * The specification of the amount is optional and if required can be less than the authorised 
     * amount.
     *
     * In the case of an EMV request, all of the ICC tags that have changed since authorisation are 
     * required in settlement. These typically include:
     *
     * <ul type="circle">
     *   <li>Application cryptogram</li>
     *   <li>Cryptogram information data</li>
     *   <li>Application transaction counter</li>
     *   <li>Issuer application data</li>
     *   <li>Terminal verification results</li>
     *   <li>Transaction status information</li>
     *   <li>Issuer script results</li>
     * </ul>
     *
     * It should be noted that there is no ICC tag allocated for the Issuer
     * Script Results. It is up to the EMV Kernel/Terminal Integrator to
     * allocate their own. Therefore when the terminal must send the script
     * results they must set the tag of the ICCTag to the value
     * "IssuerScriptResults".
     * 
     * The response to the confirmation request is an acknowledgement of
     * receiving the request.
     *
     * @see CEXIccTag
     **/
	RequestTypeConf,

    /**
     * @brief An ICC management request. 
     *
     * This is used to perform ICC management functionality.
     *
     * A SubType must be specified for this. For example, aquire a list of public keys.
     *
     * @see CEXRequest::subType
     **/
    RequestTypeICCManagement,
    
    /**
     * @brief An offline request. 
     *
     * This is used to settle an EMV transaction using the ICC data provided where the card
     * has approved a transaction offline.
     *
     * Any ICC data provided by the issuer will be returned as part of the response data.
     *
     * @see CEXIccTag
     **/
    RequestTypeOffline,
    
    /**
     * @brief A partial confirmation request.
     *
     * This will update the status of the transaction to Partially Confirmed.
     * This will update the data stored on the platform but will not connect to our bank interfaces. That will 
     * only occur  when the transaction is confirmed and include updated data.
     *
     * Data can still be further update in the Confirm if so required. e.g. Tip adjustment.
     *
     * @see CEXRequest::subType
     **/
    RequestTypePartialConfirm,

    /**
     * @brief A pre-authorisation request. 
     *
     * This is a small request for payment to confirm that the card details are valid. 
     * This can include manual, magnetic stripe and EMV transactions.
     *
     * EMV transactions will be processed and any ICC data provided by the issuer will be 
     * returned as part of the response data.
     **/
	RequestTypePreAuth,
    
    /**
     * @brief A query request.
     *
     * A SubType must be specified for this.
     *
     * @see CEXSubType
     * @see CEXRequest::subType
     **/
    RequestTypeQuery,
    
    /**
     * @brief A refund request. 
     *
     * This is used to refund a transaction that has previously been settled. 
     * A CardEaseReference obtained from the previous transaction is required.
     **/
	RequestTypeRefund,
    
    /**
     * @brief A test request. 
     *
     * This is used to test connectivity to CardEaseXML. This functionality should be
     * used infrequently as excessive misuse may be interpreted as being a Denial-Of-Service
     * attack causing the sender to be blocked from the CardEaseXML servers.
     **/
    RequestTypeTest,
    
    /**
     * @brief An update request.
     *
     * This is used to update details relating to the original transaction.
     * A CardEaseReference obtained from the previous transaction is required.
     **/
    RequestTypeUpdate,
    
    /**
     * @brief A voice referral request. 
     *
     * This is the second stage to an authorisation when the result returned by the 
     * acquirer is a Referral. For a referral an operator must call their acquiring bank 
     * regarding the transaction.
     *
     * They will authorise of decline the transaction over the phone and provide an AuthCode 
     * if the transaction is authorised. This message type is used to convey the response to
     * CardEaseXML so that a transaction is completed for settlement.
     *
     * A CardEaseReference obtained from the previous transaction is required. A new
     * CardEaseReference is provided in the response, this is the transaction that will be 
     * settled.  The original transaction will never settled.
     **/
	RequestTypeVoiceReferralNotification,
    
    /**
     * @brief A void request. 
     *
     * This is used to void the original transaction. The response to the void request
     * is an acknowledgement of whether the void was approved or declined.
     **/
	RequestTypeVoid,
    
    /**
     * @brief An event request.
     *
     * This is used to report events to the platform, for example device disconnections.
     **/
    RequestTypeEvent,
    
    /**
     * @brief A deferred authorization request. 
     *
     * A deferred authorization request. This is used to present a transaction using the
     * card data provided where the transaction has failed to go online.
     */
    RequestTypeDeferredAuth,
    
} CEXRequestType;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXRequestType.
 *
 * @property RequestTypeToString
 */
extern NSString *RequestTypeToString[16];

@interface CEXRequestTypeParser : NSObject

/**
 * @brief Returns the coresponding CEXRequestType field for a given string value.
 *
 * @param code The string respresentation of a CEXRequestType field.
 * @return The coresponding CEXRequestType field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXRequestType)parse:(NSString *)code;
@end
