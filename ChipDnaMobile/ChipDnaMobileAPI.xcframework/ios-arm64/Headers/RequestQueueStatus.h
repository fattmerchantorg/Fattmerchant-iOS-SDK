/**
 * @author Creditcall Ltd
 * @brief The current status of the offline request queue.
 * @class RequestQueueStatus
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

/**
 * CEMOfflineRequestType An enumeration used to represent the type of request.
 */
typedef NSString* CEMOfflineRequestType;
extern NSString* const CEMOfflineRequestTypeSale;           /*!< A sale request. */
extern NSString* const CEMOfflineRequestTypeSaleConf;       /*!< A sale confirmation request. */
extern NSString* const CEMOfflineRequestTypeSaleVoid;       /*!< A sale void request. */
extern NSString* const CEMOfflineRequestTypeRefund;         /*!< A refund request. */
extern NSString* const CEMOfflineRequestTypeRefundConf;     /*!< A refund confirmation request. */
extern NSString* const CEMOfflineRequestTypeRefundVoid;     /*!< A refund void request. */

#import <Foundation/Foundation.h>

@interface RequestQueueStatus : NSObject


/**
 * Format of datatime strings.
 */
@property (nonatomic, strong, readonly)NSString *dateTimeFormat;

/**
 * Total number of requests in the offline pending queue.
 */
@property (nonatomic, readonly)NSInteger totalNumberOfPendingRequests;

/**
 * Total number of requests in the offline failed queue.
 */
@property (nonatomic, readonly)NSInteger totalNumberOfFailedRequires;

/**
 * The number of offline requests allowed in the offline pending queue.
 */
@property (nonatomic, readonly)NSInteger pendingRequestsStorageLimit;

/**
 * Total value of all transactions stored in the offline pending queue.
 */
@property (nonatomic, readonly)NSInteger totalPendingValueStored;

/**
 * Total value of all transaction stored in the offline failed queue.
 */
@property (nonatomic, readonly)NSInteger totalFailedValueStored;

/**
 * The total value allowed to be stored in the offline pending queue.
 */
@property (nonatomic, readonly)NSInteger pendingRequestsValueLimit;

/**
 * The date and time of the oldest offline requests currently in the offline queue.
 */
@property (nonatomic, strong, readonly)NSString *oldestPendingRequestDateTime;

/**
 * A dictionary mapping CEMOfflineRequestType to an NSNumber. NSNumber value represents the number of the requests of that type in the pending queue.
 */
@property (nonatomic, strong, readonly)NSDictionary *numberOfPendingRequests;

/**
 * A dictionary mapping CEMOfflineRequestType to an NSNumber. NSNumber value represents the number of requests of that type in the failed queue.
 */
@property (nonatomic, strong, readonly)NSDictionary *numberOfFailedRequests;

/**
 * A dictionary mapping CEMOfflineRequestType to an NSNumber. NSNumber value represents the value of all requests of that type in the pending queue.
 */
@property (nonatomic, strong, readonly)NSDictionary *valueOfPendingRequests;

/**
 * A dictionary mapping CEMOfflineReqyestType to an NSNumber. NSNumber value represents the value of all requests of that type in the pending queue.
 */
@property (nonatomic, strong, readonly)NSDictionary *valueOfFailedRequests;

/**
 * A array of user reference for offline request which have failed to go online. 
 */
@property (nonatomic, strong, readonly)NSArray *failedOfflineRequests;

@end
