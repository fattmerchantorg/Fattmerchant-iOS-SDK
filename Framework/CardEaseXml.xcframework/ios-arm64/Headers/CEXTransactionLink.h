/**
* @author NMI
* @brief A class used to hold the transaction link.
* @copyright Copyright NMI 2010 - 2020.
* @class CEXFeatureToken
*/
#import <Foundation/Foundation.h>
#import "CEXTransactionLinkType.h"

@interface CEXTransactionLink : NSObject

/**
 * @brief The type of the TransactionLink
 * @property type
 **/
@property CEXTransactionLinkType type;

/**
 * @brief The GUID of the transaction which the TransactionLink refers to
 **/
@property (strong) NSString *guid;

/**
 * @brief Creates a new TransactionLink with no values
 *
 * The values default to nil.
 *
 * @return The initialised CEXTransactionLink instance.
 **/
-(CEXTransactionLink *)init;

/**
 * @brief Creates a new TransactionLink with the specified type and guid.
 *
 * @param theType The type of the TransactionLink
 * @param theGuid The GUID of the transaction which the TransactionLink refers to
 *
 * @return The initialised CEXTransactionLink instance.
 **/
-(CEXTransactionLink *)initWithType:(CEXTransactionLinkType)theType guid:(NSString *)theGuid;

@end
