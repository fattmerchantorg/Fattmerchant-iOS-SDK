/**
 * @author Creditcall Ltd
 * @brief Returns the coresponding CEXVoiceReferralResult field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXVoiceReferralResultParser
 */
#import <Foundation/Foundation.h>

/// @file

/** @brief The result of a voice referral obtained for a transaction. **/
typedef enum
{
    /**
     * @brief Internal use to indicate no result set.
     **/
	VoiceReferralResultEmpty,
    
    /**
     * @brief Transaction was approved by the acquirer over the phone.
     *
     * An AuthCode will have been supplied which should be sent in the
     * AuthCode property.
     **/
	VoiceReferralResultApproved,
    
    /**
     * @brief Transaction was declined by the acquirer over the phone.
     **/
	VoiceReferralResultDeclined,
    
    /**
     * @brief Transaction was approved by the merchant without calling their
     *        acquiring bank.
     **/
	VoiceReferralResultApprovedOffline,
    
    /**
     * @brief Transaction was declined by the merchant without calling their acquiring
     *        bank.
     **/
	VoiceReferralResultDeclinedOffline,
    
    /**
     * @brief Transaction was cancelled by the merchant.
     **/
	VoiceReferralResultCancelled,
    
} CEXVoiceReferralResult;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXVoiceReferralResult.
 *
 * @property VoiceReferralResultToString
 */
extern NSString *VoiceReferralResultToString[6];

@interface CEXVoiceReferralResultParser : NSObject

/**
 * @brief Returns the coresponding CEXVoiceReferralResult field for a given string value.
 *
 * @param code The string respresentation of an CEXVoiceReferralResult field.
 * @return The coresponding CEXVoiceReferralResult field value.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXVoiceReferralResult)parse:(NSString *)code;
@end

