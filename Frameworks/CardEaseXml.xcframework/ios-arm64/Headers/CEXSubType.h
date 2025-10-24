/**
 * @author Creditcall Ltd
 * @brief
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 */

#import <Foundation/Foundation.h>

/// @file

/**
 * @brief The public key update sub type for ICCManagement request.s
 * @see CEXRequestType
 **/ 
extern NSString * const SubTypeICCManagementPublicKeyUpdate;

/**
 * @brief The query for the card's scheme and type.
 * @see CEXRequestType
 **/
extern NSString * const SubTypeQueryCardScheme;

/**
 * @brief The query for the fraud state.
 * @see CEXRequestType
 **/
extern NSString * const SubTypeQueryFraudState;

/**
 * @brief The query for the settlement state.
 * @see CEXRequestType
 **/
extern NSString * const SubTypeQuerySettlementState;

/**
 * @brief The query for the supported card schemes.
 * @see CEXRequestType
 **/
extern NSString * const SubTypeQuerySupportedCardScheme;

/**
 * @brief The fraud message for update requests.
 * @see CEXRequestType
 **/
extern NSString * const SubTypeUpdateFraud;

/**
 * @brief The subtype for cash requests.
 * @see CEXRequestType
 **/
extern NSString * const SubTypeCash;

/**
 * @brief The subtype for cheque requests.
 * @see CEXRequestType
 **/
extern NSString * const SubTypeCheque;

/**
 * @brief The subtype for gw-register requests.
 * @see CEXRequestType
 **/
extern NSString * const SubTypeRegister;
