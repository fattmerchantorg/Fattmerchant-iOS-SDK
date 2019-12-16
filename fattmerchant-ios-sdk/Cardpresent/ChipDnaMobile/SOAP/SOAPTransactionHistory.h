/**
 * @author Creditcall Ltd
 * @brief This class should be used to access the transaction history and statistics.
 * @class SOAPTransactionHistory
 * @copyright Copyright Creditcall Limited 2010 - 2017.
 * @details The first time that the method is invoked on the SOAPTransactionHistory instance, 
 *          the login will be carried out, as such, the first call might take slightly longer 
 *          then successive calls. The login will expire after 15 minutes and if another method 
 *          is invoked on instance 15 minutes after the last method, the login will be carried out again.
 */

#import <Foundation/Foundation.h>
#import "SOAPDelegate.h"

@interface SOAPTransactionHistory : NSObject

/**
 * Returns an instance of SOAPTransactionHistory. This method should only be called after ChipDnaMobile
 * has been initialised using {@link ChipDnaMobile::initialise:applicationIdentifier:}. {@link ChipDnaMobile::isInitialised}
 * can be used to check whether ChipDnaMobile has been initialised already.
 *
 * Transaction history must be supported and set-up, otherwise an exception will be raised. This can be checked using
 * {@link ChipDnaMobileProperties::isTransactionHistorySupported} and  {@link ChipDnaMobileProperties::isTransactionHistorySetUp} methods.
 *
 * @return an instance of SOAPTransactionHistory.
 * @exception CCInitialisationException Thrown if the method is accessed before ChipDnaMobile has been initialised.
 */
+(SOAPTransactionHistory *)sharedInstance;

/**
 * Disposes of the current instance. If a new instance is retrieved after calling this method,
 * the user will have to be logged in again.
 * 
 * This method is also called internally when {@link ChipDnaMobileProperties::save:} is invoked,
 * if application mode or SOAP credentials were changed. It ensures that the instance uses the right credentials. 
 * It means that a new copy of SOAPTransactionHistory instance should be retrieved after changing the ChipDnaMobileProperties.
 */
+(void)dispose;

/**
 * Retrieves the transactions between the given dates. Once the result is retrieved, {@link SOAPDelegate::retrievedTransactions:error:} will be invoked.
 *
 * @param fromDate the date from which the transactions should be retrieved, in UTC, in "dd/MM/yyyy HH:mm:ss" format.
 * @param toDate the date to which the transactions should be retrieved, in UTC, in "dd/MM/yyyy HH:mm:ss" format.
 * @param delegate The delegate to be notified when the result of the query has been retrieved.
 */
-(void)getTransactionsFromDate:(NSString *)fromDate toDate:(NSString *)toDate delegate:(id<SOAPDelegate>)delegate;

/**
 * Retrieves the transaction with specified date and GUID. Once the result is retrieved, {@link SOAPDelegate::retrievedTransaction:error:} will be invoked.
 *
 * @param transactionDate The date of the transaction, in UTC, in "dd/MM/yyyy HH:mm:ss" format.
 * @param guid GUID (CardEase reference) of the transaction.
 * @param delegate The delegate to be notified when the result of the query has been retrieved.
 */
-(void)getTransactionTransactionDate:(NSString *)transactionDate withGuid:(NSString *)guid delegate:(id<SOAPDelegate>)delegate;

/**
 * Retrieves the transaction statistics between the given dates. Once the result is retrieved, {@link SOAPDelegate::retrievedTransactionStatistics:error:} will be invoked.
 *
 * @param fromDate the date from which the transaction statistics should be retrieved, in UTC, in "dd/MM/yyyy HH:mm:ss" format.
 * @param toDate the date to which the transaction statistics should be retrieved, in UTC, in "dd/MM/yyyy HH:mm:ss" format.
 * @param delegate The delegate to be notified when the result of the query has been retrieved.
 */
-(void)getTransactionsStatisticsFromDate:(NSString *)fromDate toDate:(NSString *)toDate delegate:(id<SOAPDelegate>)delegate;

@end
