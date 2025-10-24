/**
 * @author Creditcall Ltd
 * @brief A parser which returns the coresponding CEXAuthorisationEntity field for a given string value.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXAuthorisationEntityParser
 */
#import <Foundation/Foundation.h>

/// @file

/**
 * @brief The entities that can authorise transactions.
 **/
typedef enum
{
    AuthorisationEntityEmpty,           ///< Internal use to indicate no result set.
    AuthorisationEntityAcquirer,        ///< The acquirer authorised the transaction.
    AuthorisationEntityCardAcceptor,    ///< The card acceptor authorised the transaction.
    AuthorisationEntityCardIssuer,      ///< The card issuer authorised the transaction.
    AuthorisationEntityCardScheme,      ///< The card scheme authorised the transaction.
    AuthorisationEntityUnknown,         ///< An unknown entity authorised the transaction.
} CEXAuthorisationEntity;

/**
 * @brief An array containing the string representations
 *        for all known values of CEXAuthorisationEntity.
 *
 * @property AuthorisationEntityToString
 */
extern NSString *AuthorisationEntityToString[6];

@interface CEXAuthorisationEntityParser : NSObject

/**
 * @brief Returns the coresponding CEXAuthorisationEntity field for a given
 *        string value.
 *
 * @param code The string respresentation of a CEXAuthorisationEntity field.
 * @return The coresponding CEXAuthorisationEntity field.
 * @exception InvalidArgument Will be raised if the string code given cannot be matched.
 */
+(CEXAuthorisationEntity)parse:(NSString *)code;
@end
