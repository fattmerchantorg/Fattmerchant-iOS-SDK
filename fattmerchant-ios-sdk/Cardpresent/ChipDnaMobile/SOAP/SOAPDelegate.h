/**
 * @author Creditcall Ltd
 * @brief This interface should be used to listen during interaction with {@link SOAPTransactionHistory}.
 * @copyright Copyright Creditcall Limited 2010 - 2017.
 * @details The listener is informed when logins are completed, transaction as retrieved, etc and returns the requested data.
 * @protocol SOAPDelegate
 */

#import <Foundation/Foundation.h>
#import "SOAPExtendedTransaction.h"

@protocol SOAPDelegate <NSObject>

@required

/**
 *  Invoked when the list of the transactions has been retrieved.
 *
 *  @param transactions An NSArray containing {@link SOAPTransaction} objects. An array will be nil if an error occurred.
 *  @param error Error code generated during the request. It will contain error if the SOAP request was unsuccessful or some other error occurred. It will be nil otherwise. Error codes are defined in {link ConnectionErrorCode} and {@link SOAPErrorCode}.
 */
- (void)retrievedTransactions:(NSArray *)transactions error:(NSString *)error;

/**
 *  Invoked when the transaction has been retrieved.
 *
 *  @param transaction Transaction returned from the request. It will be nil if an error occurred.
 *  @param error Error code generated during the request. It will contain error if the SOAP request was unsuccessful or some other error occurred. It will be nil otherwise. Error codes are defined in {link ConnectionErrorCode} and {@link SOAPErrorCode}.
 */
- (void)retrievedTransaction:(SOAPExtendedTransaction *)transaction error:(NSString *)error;

/**
 *  Invoked when the transaction statistics have been retrieved.
 *
 *  @param transactionStatistics An NSArray containing {@link SOAPTransactionStatistics} objects. An array will be nil if an error occurred.
 *  @param error Error code generated during the request. It will contain error if the SOAP request was unsuccessful or some other error occurred. It will be nil otherwise. Error codes are defined in {link ConnectionErrorCode} and {@link SOAPErrorCode}.
 */
- (void)retrievedTransactionStatistics:(NSArray *)transactionStatistics error:(NSString *)error;

@end
