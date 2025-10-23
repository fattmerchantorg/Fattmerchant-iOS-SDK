/**
 * @author Creditcall Ltd
 * @brief A class holding all of the data that constitutes a Response from CardEaseXML.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXResponse
 * @details The necessary components of the request should be retrieved (using the "getters").
 * 
 * The response can only be obtained from the CEXClient in response to a CEXRequest.
 *
 * @see CEXClient
 * @see CEXRequest
 */
#import <Foundation/Foundation.h>

#import "CEXResultCode.h"
#import "CEXVerificationResult.h"
#import "CEXAuthorisationEntity.h"
#import "CEXXmlEncoding.h"
#import "CEXCurrencyData.h"
#import "CEXName.h"
#import "CEXFundingCard.h"

/// @file

@interface CEXResponse : NSObject
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 40000
	<NSXMLParserDelegate>
#endif
{
	NSMutableString *tagContent;
	NSMutableArray *tagStack;
}

/**
 * @brief The raw response data received from the address verification with the issuer.
 *
 * The content of this is dependant upon the acquirer, country, protocol etc. This is an
 * alphanumeric string. This will be available if required in the original request.
 *
 * @property addressResponseData
 **/
@property (strong, readonly) NSString *addressResponseData;

/**
 * @brief The result of the address verification with the issuer.
 *
 * This will be available if required in the original request.
 *
 * @property addressResult
 **/
@property (readonly) CEXVerificationResult addressResult;

/**
 * @brief The amount approved for this transaction. This might be partially approved.
 *
 * This will be in major units.
 *
 * @property amountApproved
 **/
@property (readonly) NSString *amountOnlineApproved;

/**
 * @brief The acquirer response code for the transaction.
 *
 * @property acquirerResponseCode.
 */
@property (readonly) NSString *acquirerResponseCode;

/**
 * @brief The authorisation code found in this response.
 *
 * This will only be present if the transaction was approved. This is an
 * alphanumeric string with a maximum length of 12 characters.
 *
 * @property authCode
 **/
@property (readonly) NSString *authCode;

/**
 * @brief The entity that authorised the transaction.
 *
 * @property authorisationEntity
 * @see CEXAuthorisationEntity
 */
@property (readonly) CEXAuthorisationEntity authorisationEntity;

/**
 * @brief The CardEaseXML reference found in this response.
 *
 * This is a unique reference that can be used with CardEaseXML during follow-up requests
 * related to the original such as confirmations, refunds and voids.  This is
 * an alphanumeric string with a fixed length of 36 characters.
 *
 * @property cardEaseReference
 **/
@property (readonly) NSString *cardEaseReference;

/**
 * @brief The card hash found in the response that can be used to reference the card in a follow-up transaction.
 *
 * This is an alphanumeric string with a fixed length of 24 characters.
 * Used in conjunction with the CardReference property.
 * The benefit of being able to reference a previously used cards is that
 * an integrator need not store actual card details on their system for
 * repeat transactions. This reduces the risk of card infomation
 * being compromised, and reduces the integrators PCI requirements.
 *
 * @property cardHash
 * @see cardReference
 **/
@property (readonly) NSString *cardHash;

/**
 * @brief The card reference found in the response that can be used to reference the card in a follow-up
 *        transaction.
 *
 * This is an alphanumeric string with a fixed length of 36 characters.
 * Used in conjunction with the CardHash property.
 * The benefit of being able to reference a previously used cards is that
 * an integrator need not store actual card details on their system for
 * repeat transactions. This reduces the risk of card infomation
 * being compromised, and reduces the integrators PCI requirements.
 *
 * @property cardReference
 * @see cardHash
 */
@property (readonly) NSString *cardReference;

/**
 * @brief The description of the card scheme used in the request.
 *
 * This can be used on a receipt.  This is an alphanumeric string with a maximum length
 * of 50 characters.
 *
 * @property cardScheme
 **/
@property (readonly) NSString *cardScheme;

/**
 * @brief The identifier for the card scheme used in the request.
 *
 * @property cardSchemeId
 */
@property (readonly) NSString *cardSchemeId;

/**
 * @brief The account type of the card used in the request.
 *
 * @property cardSchemeAccountType
 */
@property (readonly) NSString *cardSchemeAccountType;

/**
 * @breif The card holder's name
 *
 * @property cardHolderName
 */
@property (readonly) CEXName *cardHolderName;

/**
 * @brief The raw response data received from the security code verification with the issuer.
 *
 * This is also referred to as CVV, CVC and CV2. The content of this is dependant upon the
 * acquirer, country, protocol etc. This is an alphanumeric string. This will be available if
 * required in the original request. If the CSC validation fails the authorisation is
 * automatically declined.
 *
 * @property cscResponseData
 **/
@property (strong, readonly) NSString *cscResponseData;

/**
 * @brief The result of the security code verification with the issuer.
 *
 * This will be available if required in the original request.
 * If the CSC validation fails the authorisation is automatically declined.
 *
 * @property cscResult
 */
@property (readonly) CEXVerificationResult cscResult;

/**
 * @brief The currency of the transaction, if available.
 *
 * This will be available in some transasction responses, eg Refund by reference (Linked Refund).
 *
 * @property currencyData
 */
@property (readonly) CEXCurrencyData *currencyData;

/**
 * @brief The Customer Vault ID associated with this response for the Omni Gateway.
 *
 * If the Customer Vault Command is add-customer this is optional, if the Customer Vault ID is not supplied it will be generated.
 *
 * If the Customer Vault Command is update-customer then this field is mandatory. The value must be one that was previously
 * used with an add-customer Customer Vault Command.
 *
 * @property customerVaultId
*/
@property (readonly) NSString *customerVaultId;

/**
 * @brief Whether the transaction has been declined and the issuer has indicated that the same transaction should not be re-attemetped with the same parameters.
 *
 * @property doNotReauthorize.
 */
@property (readonly) BOOL doNotReauthorize;

/**
 * @brief The reason the issuer has indicated that the same transaction should not be re-attempted with the same parameters.
 *
 * @property doNotReauthorize.
 */
@property (readonly) NSString *doNotReauthorizeReason;

/**
 * @brief Whether the transaction was recognised as a duplicate.
 *
 * @property duplicate
 **/
@property (readonly) BOOL duplicate;

/**
 * @brief The expiry date associated with the card in this response.
 *
 * This will match the expiry date format. This is a character string with a
 * maximum length of 10 characters.
 *
 * @property expiryDate
 * @see expiryDateFormat
 **/
@property (readonly) NSString *expiryDate;

/**
 * @brief The expiry date format associated with the card in the response.
 *
 *
 * This will match the format of the expiry date and can include separators such as - and /.
 * This is a character string with a maximum length of 10 characters.
 * The available options are shown in the following table:
 * <table>
 * <tr>
 *   <td>Format</td>
 *   <td>Description</td>
 *   <td>Example</td>
 * </tr>
 * <tr>
 *   <td>yyyy</td>
 *   <td>Year with century</td>
 *   <td>2004</td>
 * </tr>
 * <tr>
 *   <td>yy</td>
 *   <td>Year without century</td>
 *   <td>04</td>
 * </tr>
 * <tr>
 *   <td>MM</td>
 *   <td>Month of year</td>
 *   <td>01</td>
 * </tr>
 * <tr>
 *   <td>dd</td>
 *   <td>Day of month</td>
 *   <td>27</td>
 * </tr>
 * </table>
 *
 * @property expiryDateFormat
 * @see expiryDate
 *
 **/
@property (strong, readonly) NSString *expiryDateFormat;

/**
* @brief If the transaction needs to fallfoward to contact.
*
* This is an BOOL and will indicate if it is necessary to Fallfoward To
* Contact
*
* @property fallforwardToContact
*/
@property (readonly) BOOL fallforwardToContact;

/**
 * @brief The type of ICC transaction associated with this response.
 *
 * This is an alphanumeric string.  This is mandatory for ICC authorisations
 * and by default is "EMV".  An EMV transaction must have associated ICC tags.
 *
 * @see CEXIccTag
 * @see iccTags
 * @property iccType
 */
@property (strong, readonly) NSString *iccType;

/**
 * @brief The issue number associated with the card in this response.
 *
 * This is dependant upon the card scheme associated with the card and will be
 * exactly as found on the card (including and leading 0's). This is a numeric
 * string with a maximum length of 2 characters.
 *
 * @property issueNumber
 **/
@property (readonly) NSString *issueNumber;

/**
 * @brief The Funding Card details that have been obtained from the CardEase platform when processing the source request.
 *
 * @property fundingCard
 **/
@property (readonly) CEXFundingCard *fundingCard;

/**
 * @brief The date and time at the terminal's location.
 *
 * This can be used on a receipt and will match the local date and time format.
 * This is a character string.
 *
 * @property localDateTime
 * @see localDateTimeFormat
 **/
@property (readonly) NSString *localDateTime;

/**
 * @brief The local date and time format at the terminal's location.
 *
 * This will match the format of the local date and time and can include separators such as :, - and /.
 * This is a character string. The available options are shown in the following table:
 * <table>
 * <tr>
 *   <th>Format</th>
 *   <th>Description</th>
 *   <th>Example</th>
 * </tr>
 * <tr>
 *   <td>yyyy</td>
 *   <td>Year with century</td>
 *   <td>2004</td>
 * </tr>
 * <tr>
 *   <td>yy</td>
 *   <td>Year without century</td>
 *   <td>04</td>
 * </tr>
 * <tr>
 *   <td>MM</td>
 *   <td>Month of year</td>
 *   <td>01</td>
 * </tr>
 * <tr>
 *   <td>dd</td>
 *   <td>Day of month</td>
 *   <td>27</td>
 * </tr>
 * <tr>
 *   <td>HH</td>
 *   <td>Hour of day (24 hour)</td>
 *   <td>17</td>
 * </tr>
 * <tr>
 *   <td>mm</td>
 *   <td>Minute of hour</td>
 *   <td>53</td>
 * </tr>
 * <tr>
 *   <td>ss</td>
 *   <td>Second of minute</td>
 *   <td>43</td>
 * </tr>
 * </table>
 *
 * @property localDateTimeFormat
 * @see localDateTime
 */
@property (strong, readonly) NSString *localDateTimeFormat;

/**
 * @brief The identifier for transactions using the gateway.
 *
 * @property transactionId
 **/
@property (strong, readonly) NSString *transactionId;

/**
* @brief If the transaction needs to require online pin.
*
* This is an BOOL and will indicate if it is necessary to require
* online pin.
*
* @property onlinePinRequired
*/
@property (readonly) BOOL onlinePinRequired;

/**
 * @brief Gets the city of the originating IP address found in this response.
 *
 * The city of the originating IP address found in this response. This will be null
 * if no city was found in this response.
 *
 * @property originatingIPAddressCity
 **/
@property (readonly) NSString *originatingIPAddressCity;

/**
 * @brief Gets the continent of the originating IP address found in this response.
 *
 * The continent of the originating IP address found in this response. This will be
 * null if no continent was found in this response.
 *
 * @property originatingIPAddressContinent
 **/
@property (readonly) NSString *originatingIPAddressContinent;

/**
 * @brief Gets the ISO 3166 continent alpha-2 of the originating IP address found in this response.
 *
 * The ISO 3166 continent alpha-2 of the originating IP address found in this response.
 * This will be null if no ISO 3166 continent alpha-2 was found in this response.
 *
 * @property originatingIPAddressContinentAlpha2
 **/
@property (strong, readonly) NSString *originatingIPAddressContinentAlpha2;

/**
 * @brief Gets the country of the originating IP address found in this response.
 *
 * The country of the originating IP address found in this response. This will be
 * null if no country was found in this response.
 *
 * @property originatingIPAddressCountry
 **/
@property (readonly) NSString *originatingIPAddressCountry;

/**
 * @brief Gets the ISO 3166 country alpha-2 of the originating IP address found in this response.
 *
 * The ISO 3166 country alpha-2 of the originating IP address found in this response.
 * This will be null if no ISO 3166 country alpha-2 was found in this response.
 *
 * @property originatingIPAddressCountryAlpha2
 */
@property (strong, readonly) NSString *originatingIPAddressCountryAlpha2;

/**
 * @brief Gets the ISO 3166 country code of the originating IP address found in this response.
 *
 * The ISO 3166 country code of the originating IP address found in this response.
 * This will be null if no ISO 3166 country code was found in this response.
 *
 * @property originatingIPAddressCountryCode
 **/
@property (strong, readonly) NSString *originatingIPAddressCountryCode;

/**
 * @brief Gets whether the originating IP address is black listed.
 *
 * Whether the originating IP address is black listed. This will be false if this was
 * not found in the response.
 *
 * @property originatingIPAddressIsBlackListed
 **/
@property (readonly) BOOL originatingIPAddressIsBlackListed;

/**
 * @brief Gets whether the originating IP address is a known proxy.
 *
 * Whether the originating IP address is a known proxy. This will be false if this was
 * not found in the response.
 *
 * @property originatingIPAddressIsKnownProxy
 **/
@property (readonly) BOOL originatingIPAddressIsKnownProxy;

/**
 * @brief Gets the region of the originating IP address found in this response.
 *
 * @property originatingIPAddressRegion
 * @return The region of the originating IP address found in this response.
 *         This will be null if no region was found in this response.
 **/
@property (readonly) NSString *originatingIPAddressRegion;

/**
 * @brief Gets the region code of the originating IP address found in this response.
 *
 * The region code of the originating IP address found in this response. This will be null
 * if no region code was found in this response.
 *
 * @property originatingIPAddressRegionCode
 **/
@property (strong, readonly) NSString *originatingIPAddressRegionCode;

/**
 * @brief Gets the zip code of the originating IP address found in this response.
 *
 * The zip code of the originating IP address found in this response. This will be null
 * if no zip code was found in this response.
 *
 * @property originatingIPAddressZipCode
 **/
@property (readonly) NSString *originatingIPAddressZipCode;

/**
 * @brief The masked PAN (Primary Account Number) found in this response.
 *
 * The PAN is masked with x's for security. This is an alphanumeric string with a
 * minimum length of 13 characters and a maximum length of 19 characters.
 *
 * @property pan
 */
@property (readonly) NSString *pan;

/**
 * @brief The PAR (Primary Account Reference) found in this response.
 *
 * The PAR (Primary Account Reference) found in this response. This is an alphanumeric
 * string, if no PAR is found nil is returned.
 *
 * @property par
 */
@property (readonly) NSString *par;

/**
 * @brief The result code that has been obtained from the CardEase platform when processing the source request.
 *
 * @property resultCode
 **/
@property (readonly) CEXResultCode resultCode;

/**
 * @brief The name of the server software.
 *
 * @property serverName
 * @see serverVersion
 **/
@property (strong, readonly) NSString *serverName;

/**
 * @brief The version of the server software.
 *
 * @property serverVersion
 * @see serverName
 **/
@property (strong, readonly) NSString *serverVersion;

/**
 * @brief The start date associated with the card in this response.
 *
 * This will match the start date format. This is an alphanumeric string with a maximum length of 10 characters.
 *
 * @property startDate
 * @see startDateFormat
 **/
@property (readonly) NSString *startDate;

/**
 * @brief The start date format associated with the card in this response.
 *
 * This will match the format of the start date and can include separators such as - and /.
 * The available options are shown in the following table:
 * <table>
 * <tr>
 *   <th>Format</th>
 *   <th>Description</th>
 *   <th>Example</th>
 * </tr>
 * <tr>
 *   <td>yyyy</td>
 *   <td>Year with century</td>
 *   <td>2004</td>
 * </tr>
 * <tr>
 *   <td>yy</description>
 *   <td>Year without century</td>
 *   <td>04</td>
 * </tr>
 * <tr>
 *   <td>MM</td>
 *   <td>Month of year</td>
 *   <td>01</td>
 * </tr>
 * <tr>
 *   <td>dd</td>
 *   <td>Day of month</td>
 *  <td>27</td>
 * </tr>
 * </table>
 *
 * @property startDateFormat
 * @see startDate
 **/
@property (strong, readonly) NSString *startDateFormat;

/**
 *@brief In response to an authorisation request the issuer has indicated that 3-D Secure authentication is required.
 *
 *@property threeDSecureRequired
 */
@property (readonly) BOOL threeDSecureRequired;

/**
 * @brief The user reference found in this response.
 *
 * This will be the same as that in the original request. This allows a user to attached
 * their own reference against a request and verify it against this response.
 * This is an alphanumeric string with a maximum length of 50 characters. Use of the user
 * reference is optional for all requests.
 *
 * @property userReference
 **/
@property (readonly) NSString *userReference;

/**
 * @brief The UTC.
 *
 * This is a character string.
 *
 * @property utc
 * @see utcFormat
 **/
@property (readonly) NSString *utc;

/**
 * @brief The format of the UTC.
 *
 * This will match the format of the UTC and can include separators such as :, - and /.
 * This is a character string. The available options are shown in the following table:
 * <table>
 * <tr>
 *   <th>Format</th>
 *   <th>Description</th>
 *   <th>Example</th>
 * </tr>
 * <tr>
 *   <td>yyyy</td>
 *   <td>Year with century</td>
 *   <td>2004</td>
 * </tr>
 * <tr>
 *   <td>yy</td>
 *   <td>Year without century</td>
 *   <td>04</td>
 * </tr>
 * <tr>
 *   <td>MM</td>
 *   <td>Month of year</td>
 *   <td>01</td>
 * </tr>
 * <tr>
 *   <td>dd</td>
 *   <td>Day of month</td>
 *   <td>27</td>
 * </tr>
 * <tr>
 *   <td>HH</td>
 *   <td>Hour of day (24 hour)</td>
 *   <td>17</td>
 * </tr>
 * <tr>
 *   <td>mm</td>
 *   <td>Minute of hour</td>
 *   <td>53</td>
 * </tr>
 * <tr>
 *   <td>ss</td>
 *   <td>Second of minute</td>
 *   <td>43</td>
 * </tr>
 * </table>
 *
 * @property utcFormat
 * @see utc
 **/
@property (strong, readonly) NSString *utcFormat;

/**
 * @brief If provided this is the number the operator should use to call
 *        their acquiring bank to perform a voice referral. If this is not
 *        supplied in the response they should use the default numbers
 *        given to them by the acquiring bank.
 *
 * @property voiceReferralTelephoneNumber
 **/
@property (readonly) NSString *voiceReferralTelephoneNumber;

/**
 * @brief The raw response data received from the zip code/post code verification with the issuer.
 *
 * The content of this is dependant upon the acquirer, country, protocol etc. This is an
 * alphanumeric string. This will be available if required in the original request.
 *
 * @property zipCodeResponseData
 **/
@property (strong, readonly) NSString *zipCodeResponseData;

/**
 * @brief The result of the zip code/post code verification with the issuer.
 *
 * This will be available if required in the original request.
 *
 * @property zipCodeResult
 **/
@property (readonly) CEXVerificationResult zipCodeResult;

/**
 * @brief The type of encoding used.
 *
 * @property encoding
 **/
@property CEXXmlEncoding encoding;

/**
 * @brief The payment provider error code.
 *
 * @property paymentProviderErrorCode
 **/
@property (readonly) NSString *paymentProviderErrorCode;

/**
 * @brief The payment provider error description.
 *
 * @property paymentProviderErrorDescription
 **/
@property (readonly) NSString *paymentProviderErrorDescription;

/**
 * @brief The default init.
 *
 * @return An initialised CEXResponse instance.
 **/
- (CEXResponse *)init;

/**
 * @brief Parses the XML response.
 *
 * @param xml The XML to parse.
 **/
-(void)parseResponseXml:(NSString *)xml error:(NSError **)error;

/**
 * @brief The Card Tokens associated with the card.
 *
 * @details This will be nil if no card tokens were found in the response.
 *
 * @return The list of cardTokens
 **/
- (NSArray *)cardTokens;

/**
 * @brief A list of errors that were encountered when trying to process the request.
 *
 * @details Each error contains an error code and an error message.
 *
 * @return The list of errors
 * @see CEXError
 */
- (NSArray *)errors;

/**
 * @brief The list of extended properties found in the response.
 *
 * @details This will be nil if no extended properties were found in the response.
 *
 * @return The list of extendedProperties
 **/
- (NSArray *)extendedProperties;

/**
 * @brief The list of ICC tags found in this response.
 *
 * @details Each ICC tag has an tag, type and value.
 * For example, a tag of 0x9f02/AsciiHex/000000000100 is using to specify the
 * transaction amount. These are mandatory for an EMV transaction.
 *
 * @return The list of iccTags
 * @see CEXIccTag
 **/
- (NSArray *)iccTags;

@end
