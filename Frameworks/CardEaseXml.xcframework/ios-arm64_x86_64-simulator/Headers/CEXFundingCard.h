/**
* @author Creditcall Ltd
* @brief A class used to hold Funding Card information.
* @copyright Copyright Creditcall Limited 2010 - 2020.
* @class CEXFundingCard
* @details A class used to hold Funding Card information required for an EMV transaction.
*
* Each Funding Card has an associated Card Reference, Card Hash, masked PAN and Expiry Date.
*
*/
#import <Foundation/Foundation.h>

@interface CEXFundingCard : NSObject

/**
 * @brief The reference of an existing card to use for manual payment in place of the PAN, ExpiryDate etc.
 * @property cardReference
 *
 * This is an alphanumeric string.
 **/
@property (strong) NSString *cardReference;

/**
 * @brief The hash of an existing card to use for manual payment in place of the PAN, ExpiryDate etc.
 * @property cardHash
 *
 * This is an alphanumeric string.
 **/
@property (strong) NSString *cardHash;

/**
 * @brief The PAN (Primary Account Number) associated with the Funding Card in the request.
 *
 * This is a numeric string with a minimum length of 13 characters and a maximum length of 19 characters.
 *
 * @property pan
**/
@property (strong) NSString *pan;

/**
 * @brief The expiry date associated with the funding card in the request.
 *
 *
 * This is a character string with a maximum length of 4 characters.
 * This is in the format mmYY.
 *
**/
@property (strong) NSString *expiryDate;

/**
 * @brief The expiry date format associated with the card found in the Funding Card within the response.
 *
 * This will match the format of the expiry date and can include separators
 * such as - and /. This is a character string with a maximum length of 10
 * characters. The available options are shown in the following table:
 *
 * <table>
 * <tr>
 * <th>Format</th>
 * <th>Description</th>
 * <th>Example</th>
 * </tr>
 * <tr>
 * <td>yyyy</td>
 * <td>Year with century</td>
 * <td>2004</td>
 * </tr>
 * <tr>
 * <td>yy</td>
 * <td>Year without century</td>
 * <td>04</td>
 * </tr>
 * <tr>
 * <td>MM</td>
 * <td>Month of year</td>
 * <td>01</td>
 * </tr>
 * <tr>
 * <td>dd</td>
 * <td>Day of month</td>
 * <td>27</td>
 * </tr>
 * </table>
**/
@property (strong) NSString *expiryDateFormat;

/**
 * @brief Creates a new Funding Card with no values
 *
 * The card reference defaults to nil.
 * The card hash defaults to nil.
 * The PAN defaults to nil.
 * The expiry date defaults to nil.
 * The expiry date format defaults to nil.
 * Card Tokens defaults to nil.
 *
 * @return The initialised CEXFundingCard instance.
 **/
-(CEXFundingCard *)init;



/**
 * @brief Creates a new Funding Card with card reference,
 * card hash, PAN and expiry date
 *
 * @param cardRef Card reference of the funding card
 * @param hash Card hash of the funding card
 * @param fundingPan PAN (Primary Account Number) of the funding card
 * @param expDate Expiry date of the funding card
 * @param expDateFormat Expiry date format of the funding card
 * @param cardTokens   Card Tokens associated with the Funding Card used.
 *
 * @return The allocated and initialised CEXFundingCard object.
 **/
-(CEXFundingCard *)initWithCardReference:(NSString *)cardRef cardHash:(NSString *)hash pan:(NSString *)fundingPan expiryDate:(NSString *)expDate expiryDateFormat:(NSString *)expDateFormat cardTokens:(NSArray *)theCardTokens;


/**
 * @brief The Card Tokens associated with the Funding Card used.
 *
 * @details This will be nil if no card tokens were found in the response.
 *
 * @return The list of cardTokens
 **/
- (NSArray *)cardTokens;

@end
