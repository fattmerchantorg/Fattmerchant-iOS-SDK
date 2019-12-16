#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSError.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSString.h>
#import <Foundation/NSValue.h>

@class SharedTransaction, SharedKotlinThrowable, SharedKotlinArray, SharedKotlinException, SharedTransactionRequest, SharedKtor_client_coreHttpClient, SharedAmount, SharedInvoice, SharedKotlinError, SharedOmniApi, SharedTransactionType, SharedKotlinEnum, SharedMerchant, SharedKtor_client_coreHttpClientConfig, SharedKotlinx_coroutines_coreCoroutineDispatcher, SharedKtor_client_coreHttpClientEngineConfig, SharedKtor_client_coreHttpReceivePipeline, SharedKtor_client_coreHttpRequestPipeline, SharedKtor_client_coreHttpResponsePipeline, SharedKtor_client_coreHttpSendPipeline, SharedKotlinx_coroutines_coreCancellationException, SharedKtor_utilsAttributeKey, SharedKotlinAbstractCoroutineContextElement, SharedKtor_client_coreHttpResponseConfig, SharedKtor_utilsPipelinePhase, SharedKtor_utilsPipeline, SharedKotlinRuntimeException, SharedKotlinIllegalStateException, SharedKotlinx_ioCharset, SharedKotlinx_coroutines_coreAtomicDesc, SharedKotlinx_ioCharsetDecoder, SharedKotlinx_ioCharsetEncoder, SharedKotlinx_coroutines_coreAtomicOp, SharedKotlinx_coroutines_coreOpDescriptor;

@protocol SharedKotlinCoroutineContext, SharedKotlinx_coroutines_coreCoroutineScope, SharedMobileReaderDriverRepository, SharedMobileReader, SharedOmniClient, SharedInvoiceRepository, SharedCustomerRepository, SharedPaymentMethodRepository, SharedTransactionRepository, SharedKotlinx_coroutines_coreJob, SharedKotlinComparable, SharedModel, SharedModelRepository, SharedKotlinCoroutineContextElement, SharedKotlinCoroutineContextKey, SharedKotlinIterator, SharedKotlinx_ioCloseable, SharedKtor_client_coreHttpClientEngine, SharedKtor_utilsAttributes, SharedKotlinx_coroutines_coreChildHandle, SharedKotlinx_coroutines_coreChildJob, SharedKotlinx_coroutines_coreDisposableHandle, SharedKotlinSequence, SharedKotlinx_coroutines_coreSelectClause0, SharedKtor_client_coreHttpClientFeature, SharedKotlinContinuation, SharedKotlinContinuationInterceptor, SharedKotlinx_coroutines_coreRunnable, SharedKotlinSuspendFunction2, SharedKotlinx_coroutines_coreParentJob, SharedKotlinx_coroutines_coreSelectInstance, SharedKotlinSuspendFunction0, SharedKotlinFunction;

NS_ASSUME_NONNULL_BEGIN
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wnullability"

@interface KotlinBase : NSObject
- (instancetype)init __attribute__((unavailable));
+ (instancetype)new __attribute__((unavailable));
+ (void)initialize __attribute__((objc_requires_super));
@end;

@interface KotlinBase (KotlinBaseCopying) <NSCopying>
@end;

__attribute__((objc_runtime_name("KotlinMutableSet")))
__attribute__((swift_name("KotlinMutableSet")))
@interface SharedMutableSet<ObjectType> : NSMutableSet<ObjectType>
@end;

__attribute__((objc_runtime_name("KotlinMutableDictionary")))
__attribute__((swift_name("KotlinMutableDictionary")))
@interface SharedMutableDictionary<KeyType, ObjectType> : NSMutableDictionary<KeyType, ObjectType>
@end;

@interface NSError (NSErrorKotlinException)
@property (readonly) id _Nullable kotlinException;
@end;

__attribute__((objc_runtime_name("KotlinNumber")))
__attribute__((swift_name("KotlinNumber")))
@interface SharedNumber : NSNumber
- (instancetype)initWithChar:(char)value __attribute__((unavailable));
- (instancetype)initWithUnsignedChar:(unsigned char)value __attribute__((unavailable));
- (instancetype)initWithShort:(short)value __attribute__((unavailable));
- (instancetype)initWithUnsignedShort:(unsigned short)value __attribute__((unavailable));
- (instancetype)initWithInt:(int)value __attribute__((unavailable));
- (instancetype)initWithUnsignedInt:(unsigned int)value __attribute__((unavailable));
- (instancetype)initWithLong:(long)value __attribute__((unavailable));
- (instancetype)initWithUnsignedLong:(unsigned long)value __attribute__((unavailable));
- (instancetype)initWithLongLong:(long long)value __attribute__((unavailable));
- (instancetype)initWithUnsignedLongLong:(unsigned long long)value __attribute__((unavailable));
- (instancetype)initWithFloat:(float)value __attribute__((unavailable));
- (instancetype)initWithDouble:(double)value __attribute__((unavailable));
- (instancetype)initWithBool:(BOOL)value __attribute__((unavailable));
- (instancetype)initWithInteger:(NSInteger)value __attribute__((unavailable));
- (instancetype)initWithUnsignedInteger:(NSUInteger)value __attribute__((unavailable));
+ (instancetype)numberWithChar:(char)value __attribute__((unavailable));
+ (instancetype)numberWithUnsignedChar:(unsigned char)value __attribute__((unavailable));
+ (instancetype)numberWithShort:(short)value __attribute__((unavailable));
+ (instancetype)numberWithUnsignedShort:(unsigned short)value __attribute__((unavailable));
+ (instancetype)numberWithInt:(int)value __attribute__((unavailable));
+ (instancetype)numberWithUnsignedInt:(unsigned int)value __attribute__((unavailable));
+ (instancetype)numberWithLong:(long)value __attribute__((unavailable));
+ (instancetype)numberWithUnsignedLong:(unsigned long)value __attribute__((unavailable));
+ (instancetype)numberWithLongLong:(long long)value __attribute__((unavailable));
+ (instancetype)numberWithUnsignedLongLong:(unsigned long long)value __attribute__((unavailable));
+ (instancetype)numberWithFloat:(float)value __attribute__((unavailable));
+ (instancetype)numberWithDouble:(double)value __attribute__((unavailable));
+ (instancetype)numberWithBool:(BOOL)value __attribute__((unavailable));
+ (instancetype)numberWithInteger:(NSInteger)value __attribute__((unavailable));
+ (instancetype)numberWithUnsignedInteger:(NSUInteger)value __attribute__((unavailable));
@end;

__attribute__((objc_runtime_name("KotlinByte")))
__attribute__((swift_name("KotlinByte")))
@interface SharedByte : SharedNumber
- (instancetype)initWithChar:(char)value;
+ (instancetype)numberWithChar:(char)value;
@end;

__attribute__((objc_runtime_name("KotlinUByte")))
__attribute__((swift_name("KotlinUByte")))
@interface SharedUByte : SharedNumber
- (instancetype)initWithUnsignedChar:(unsigned char)value;
+ (instancetype)numberWithUnsignedChar:(unsigned char)value;
@end;

__attribute__((objc_runtime_name("KotlinShort")))
__attribute__((swift_name("KotlinShort")))
@interface SharedShort : SharedNumber
- (instancetype)initWithShort:(short)value;
+ (instancetype)numberWithShort:(short)value;
@end;

__attribute__((objc_runtime_name("KotlinUShort")))
__attribute__((swift_name("KotlinUShort")))
@interface SharedUShort : SharedNumber
- (instancetype)initWithUnsignedShort:(unsigned short)value;
+ (instancetype)numberWithUnsignedShort:(unsigned short)value;
@end;

__attribute__((objc_runtime_name("KotlinInt")))
__attribute__((swift_name("KotlinInt")))
@interface SharedInt : SharedNumber
- (instancetype)initWithInt:(int)value;
+ (instancetype)numberWithInt:(int)value;
@end;

__attribute__((objc_runtime_name("KotlinUInt")))
__attribute__((swift_name("KotlinUInt")))
@interface SharedUInt : SharedNumber
- (instancetype)initWithUnsignedInt:(unsigned int)value;
+ (instancetype)numberWithUnsignedInt:(unsigned int)value;
@end;

__attribute__((objc_runtime_name("KotlinLong")))
__attribute__((swift_name("KotlinLong")))
@interface SharedLong : SharedNumber
- (instancetype)initWithLongLong:(long long)value;
+ (instancetype)numberWithLongLong:(long long)value;
@end;

__attribute__((objc_runtime_name("KotlinULong")))
__attribute__((swift_name("KotlinULong")))
@interface SharedULong : SharedNumber
- (instancetype)initWithUnsignedLongLong:(unsigned long long)value;
+ (instancetype)numberWithUnsignedLongLong:(unsigned long long)value;
@end;

__attribute__((objc_runtime_name("KotlinFloat")))
__attribute__((swift_name("KotlinFloat")))
@interface SharedFloat : SharedNumber
- (instancetype)initWithFloat:(float)value;
+ (instancetype)numberWithFloat:(float)value;
@end;

__attribute__((objc_runtime_name("KotlinDouble")))
__attribute__((swift_name("KotlinDouble")))
@interface SharedDouble : SharedNumber
- (instancetype)initWithDouble:(double)value;
+ (instancetype)numberWithDouble:(double)value;
@end;

__attribute__((objc_runtime_name("KotlinBoolean")))
__attribute__((swift_name("KotlinBoolean")))
@interface SharedBoolean : SharedNumber
- (instancetype)initWithBool:(BOOL)value;
+ (instancetype)numberWithBool:(BOOL)value;
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("PaginatedData")))
@interface SharedPaginatedData : KotlinBase
- (instancetype)initWithData:(NSArray<id> *)data __attribute__((swift_name("init(data:)"))) __attribute__((objc_designated_initializer));
@property (readonly) NSArray<id> *data __attribute__((swift_name("data")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreCoroutineScope")))
@protocol SharedKotlinx_coroutines_coreCoroutineScope
@required
@property (readonly) id<SharedKotlinCoroutineContext> coroutineContext __attribute__((swift_name("coroutineContext")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("ConnectMobileReader")))
@interface SharedConnectMobileReader : KotlinBase <SharedKotlinx_coroutines_coreCoroutineScope>
- (instancetype)initWithCoroutineContext:(id<SharedKotlinCoroutineContext>)coroutineContext mobileReaderDriverRepository:(id<SharedMobileReaderDriverRepository>)mobileReaderDriverRepository mobileReader:(id<SharedMobileReader>)mobileReader __attribute__((swift_name("init(coroutineContext:mobileReaderDriverRepository:mobileReader:)"))) __attribute__((objc_designated_initializer));
@property (readonly) id<SharedKotlinCoroutineContext> coroutineContext __attribute__((swift_name("coroutineContext")));
@property id<SharedMobileReader> mobileReader __attribute__((swift_name("mobileReader")));
@property id<SharedMobileReaderDriverRepository> mobileReaderDriverRepository __attribute__((swift_name("mobileReaderDriverRepository")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("InitializeDrivers")))
@interface SharedInitializeDrivers : KotlinBase <SharedKotlinx_coroutines_coreCoroutineScope>
- (instancetype)initWithMobileReaderDriverRepository:(id<SharedMobileReaderDriverRepository>)mobileReaderDriverRepository args:(NSDictionary<NSString *, id> *)args coroutineContext:(id<SharedKotlinCoroutineContext>)coroutineContext __attribute__((swift_name("init(mobileReaderDriverRepository:args:coroutineContext:)"))) __attribute__((objc_designated_initializer));
@property (readonly) NSDictionary<NSString *, id> *args __attribute__((swift_name("args")));
@property (readonly) id<SharedKotlinCoroutineContext> coroutineContext __attribute__((swift_name("coroutineContext")));
@property (readonly) id<SharedMobileReaderDriverRepository> mobileReaderDriverRepository __attribute__((swift_name("mobileReaderDriverRepository")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("RefundMobileReaderTransaction")))
@interface SharedRefundMobileReaderTransaction : KotlinBase
- (instancetype)initWithOmni:(id<SharedOmniClient>)omni transaction:(SharedTransaction *)transaction __attribute__((swift_name("init(omni:transaction:)"))) __attribute__((objc_designated_initializer));
@property (readonly) id<SharedOmniClient> omni __attribute__((swift_name("omni")));
@property (readonly) SharedTransaction *transaction __attribute__((swift_name("transaction")));
@end;

__attribute__((swift_name("KotlinThrowable")))
@interface SharedKotlinThrowable : KotlinBase
- (instancetype)initWithMessage:(NSString * _Nullable)message __attribute__((swift_name("init(message:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithCause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(cause:)"))) __attribute__((objc_designated_initializer));
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithMessage:(NSString * _Nullable)message cause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(message:cause:)"))) __attribute__((objc_designated_initializer));
- (SharedKotlinArray *)getStackTrace __attribute__((swift_name("getStackTrace()")));
- (void)printStackTrace __attribute__((swift_name("printStackTrace()")));
- (NSString *)description __attribute__((swift_name("description()")));
@property (readonly) SharedKotlinThrowable * _Nullable cause __attribute__((swift_name("cause")));
@property (readonly) NSString * _Nullable message __attribute__((swift_name("message")));
@end;

__attribute__((swift_name("KotlinException")))
@interface SharedKotlinException : SharedKotlinThrowable
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithMessage:(NSString * _Nullable)message __attribute__((swift_name("init(message:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithMessage:(NSString * _Nullable)message cause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(message:cause:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithCause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(cause:)"))) __attribute__((objc_designated_initializer));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("RefundMobileReaderTransaction.RefundException")))
@interface SharedRefundMobileReaderTransactionRefundException : SharedKotlinException
- (instancetype)initWithMessage:(NSString *)message __attribute__((swift_name("init(message:)"))) __attribute__((objc_designated_initializer));
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
+ (instancetype)new __attribute__((unavailable));
- (instancetype)initWithMessage:(NSString * _Nullable)message cause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(message:cause:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
- (instancetype)initWithCause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(cause:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("SearchForReaders")))
@interface SharedSearchForReaders : KotlinBase <SharedKotlinx_coroutines_coreCoroutineScope>
- (instancetype)initWithMobileReaderDriverRepository:(id<SharedMobileReaderDriverRepository>)mobileReaderDriverRepository args:(NSDictionary<NSString *, id> *)args coroutineContext:(id<SharedKotlinCoroutineContext>)coroutineContext __attribute__((swift_name("init(mobileReaderDriverRepository:args:coroutineContext:)"))) __attribute__((objc_designated_initializer));
@property (readonly) NSDictionary<NSString *, id> *args __attribute__((swift_name("args")));
@property (readonly) id<SharedKotlinCoroutineContext> coroutineContext __attribute__((swift_name("coroutineContext")));
@property (readonly) id<SharedMobileReaderDriverRepository> mobileReaderDriverRepository __attribute__((swift_name("mobileReaderDriverRepository")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("TakeMobileReaderPayment")))
@interface SharedTakeMobileReaderPayment : KotlinBase <SharedKotlinx_coroutines_coreCoroutineScope>
- (instancetype)initWithMobileReaderDriverRepository:(id<SharedMobileReaderDriverRepository>)mobileReaderDriverRepository invoiceRepository:(id<SharedInvoiceRepository>)invoiceRepository customerRepository:(id<SharedCustomerRepository>)customerRepository paymentMethodRepository:(id<SharedPaymentMethodRepository>)paymentMethodRepository transactionRepository:(id<SharedTransactionRepository>)transactionRepository request:(SharedTransactionRequest *)request coroutineContext:(id<SharedKotlinCoroutineContext>)coroutineContext __attribute__((swift_name("init(mobileReaderDriverRepository:invoiceRepository:customerRepository:paymentMethodRepository:transactionRepository:request:coroutineContext:)"))) __attribute__((objc_designated_initializer));
@property (readonly) id<SharedKotlinCoroutineContext> coroutineContext __attribute__((swift_name("coroutineContext")));
@property (readonly) id<SharedCustomerRepository> customerRepository __attribute__((swift_name("customerRepository")));
@property (readonly) id<SharedInvoiceRepository> invoiceRepository __attribute__((swift_name("invoiceRepository")));
@property (readonly) id<SharedMobileReaderDriverRepository> mobileReaderDriverRepository __attribute__((swift_name("mobileReaderDriverRepository")));
@property (readonly) id<SharedPaymentMethodRepository> paymentMethodRepository __attribute__((swift_name("paymentMethodRepository")));
@property (readonly) SharedTransactionRequest *request __attribute__((swift_name("request")));
@property (readonly) id<SharedTransactionRepository> transactionRepository __attribute__((swift_name("transactionRepository")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("VoidMobileReaderTransaction")))
@interface SharedVoidMobileReaderTransaction : KotlinBase <SharedKotlinx_coroutines_coreCoroutineScope>
- (instancetype)initWithMobileReaderDriverRepository:(id<SharedMobileReaderDriverRepository>)mobileReaderDriverRepository transactionRepository:(id<SharedTransactionRepository>)transactionRepository transaction:(SharedTransaction *)transaction coroutineContext:(id<SharedKotlinCoroutineContext>)coroutineContext __attribute__((swift_name("init(mobileReaderDriverRepository:transactionRepository:transaction:coroutineContext:)"))) __attribute__((objc_designated_initializer));
@property (readonly) id<SharedKotlinCoroutineContext> coroutineContext __attribute__((swift_name("coroutineContext")));
@property (readonly) id<SharedMobileReaderDriverRepository> mobileReaderDriverRepository __attribute__((swift_name("mobileReaderDriverRepository")));
@property (readonly) SharedTransaction *transaction __attribute__((swift_name("transaction")));
@property (readonly) id<SharedTransactionRepository> transactionRepository __attribute__((swift_name("transactionRepository")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("CoroutineTesting")))
@interface SharedCoroutineTesting : KotlinBase <SharedKotlinx_coroutines_coreCoroutineScope>
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (void)testmebroCompletion:(void (^)(NSString *))completion __attribute__((swift_name("testmebro(completion:)")));
@property (readonly) id<SharedKotlinCoroutineContext> coroutineContext __attribute__((swift_name("coroutineContext")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("OmniApi")))
@interface SharedOmniApi : KotlinBase
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (id _Nullable)fromJsonJson:(NSString *)json __attribute__((swift_name("fromJson(json:)")));
- (BOOL)isTokenExpiredResponse:(NSString *)response __attribute__((swift_name("isTokenExpired(response:)")));
- (NSString *)toJsonObj:(id _Nullable)obj __attribute__((swift_name("toJson(obj:)")));
@property (readonly) SharedKtor_client_coreHttpClient *httpClient __attribute__((swift_name("httpClient")));
@property NSString *token __attribute__((swift_name("token")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("OmniApi.Companion")))
@interface SharedOmniApiCompanion : KotlinBase
+ (instancetype)alloc __attribute__((unavailable));
+ (instancetype)allocWithZone:(struct _NSZone *)zone __attribute__((unavailable));
+ (instancetype)companion __attribute__((swift_name("init()")));
@property (readonly) NSString *baseUrl __attribute__((swift_name("baseUrl")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Amount")))
@interface SharedAmount : KotlinBase
- (instancetype)initWithDollars:(float)dollars __attribute__((swift_name("init(dollars:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithDollars_:(double)dollars __attribute__((swift_name("init(dollars_:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithDollars__:(id)dollars __attribute__((swift_name("init(dollars__:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithCents:(int32_t)cents __attribute__((swift_name("init(cents:)"))) __attribute__((objc_designated_initializer));
- (NSString *)centsString __attribute__((swift_name("centsString()")));
- (int32_t)component1 __attribute__((swift_name("component1()")));
- (SharedAmount *)doCopyCents:(int32_t)cents __attribute__((swift_name("doCopy(cents:)")));
- (NSString *)dollarsString __attribute__((swift_name("dollarsString()")));
- (BOOL)isEqual:(id _Nullable)other __attribute__((swift_name("isEqual(_:)")));
- (NSUInteger)hash __attribute__((swift_name("hash()")));
- (NSString *)description __attribute__((swift_name("description()")));
@property (readonly) int32_t cents __attribute__((swift_name("cents")));
@end;

__attribute__((swift_name("MobileReader")))
@protocol SharedMobileReader
@required
- (NSString *)getName __attribute__((swift_name("getName()")));
@end;

__attribute__((swift_name("MobileReaderDriver")))
@protocol SharedMobileReaderDriver
@required
@end;

__attribute__((swift_name("OmniClient")))
@protocol SharedOmniClient <SharedKotlinx_coroutines_coreCoroutineScope>
@required
- (id<SharedKotlinx_coroutines_coreJob>)connectReaderMobileReader:(id<SharedMobileReader>)mobileReader onConnected:(void (^)(id<SharedMobileReader>))onConnected onFail:(void (^)(NSString *))onFail __attribute__((swift_name("connectReader(mobileReader:onConnected:onFail:)")));
- (id<SharedKotlinx_coroutines_coreJob>)getAvailableReadersOnReadersFound:(void (^)(NSArray<id<SharedMobileReader>> *))onReadersFound __attribute__((swift_name("getAvailableReaders(onReadersFound:)")));
- (void)getInvoicesCompletion:(void (^)(NSArray<SharedInvoice *> *))completion error:(void (^)(SharedKotlinError *))error __attribute__((swift_name("getInvoices(completion:error:)")));
- (void)getTransactionsCompletion:(void (^)(NSArray<SharedTransaction *> *))completion error:(void (^)(SharedKotlinError *))error __attribute__((swift_name("getTransactions(completion:error:)")));
- (id<SharedKotlinx_coroutines_coreJob>)initializeArgs:(NSDictionary<NSString *, id> *)args error:(void (^)(SharedKotlinError *))error __attribute__((swift_name("initialize(args:error:)")));
- (void)refundMobileReaderTransactionTransaction:(SharedTransaction *)transaction completion:(void (^)(SharedTransaction *))completion error:(void (^)(SharedKotlinError *))error __attribute__((swift_name("refundMobileReaderTransaction(transaction:completion:error:)")));
- (void)refundOrVoidMobileReaderTransactionTransaction:(SharedTransaction *)transaction completion:(void (^)(SharedTransaction *))completion __attribute__((swift_name("refundOrVoidMobileReaderTransaction(transaction:completion:)")));
- (void)takeMobileReaderTransactionRequest:(SharedTransactionRequest *)request completion:(void (^)(SharedTransaction *))completion error:(void (^)(SharedKotlinError *))error __attribute__((swift_name("takeMobileReaderTransaction(request:completion:error:)")));
- (void)voidMobileReaderTransactionTransaction:(SharedTransaction *)transaction completion:(void (^)(SharedTransaction *))completion error:(void (^)(SharedKotlinError *))error __attribute__((swift_name("voidMobileReaderTransaction(transaction:completion:error:)")));
@property (readonly) id<SharedCustomerRepository> customerRepository __attribute__((swift_name("customerRepository")));
@property (readonly) id<SharedInvoiceRepository> invoiceRepository __attribute__((swift_name("invoiceRepository")));
@property (readonly) id<SharedMobileReaderDriverRepository> mobileReaderDriverRepository __attribute__((swift_name("mobileReaderDriverRepository")));
@property SharedOmniApi *omniApi __attribute__((swift_name("omniApi")));
@property (readonly) id<SharedPaymentMethodRepository> paymentMethodRepository __attribute__((swift_name("paymentMethodRepository")));
@property (readonly) id<SharedTransactionRepository> transactionRepository __attribute__((swift_name("transactionRepository")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("TransactionRequest")))
@interface SharedTransactionRequest : KotlinBase
- (instancetype)initWithAmount:(SharedAmount *)amount type:(SharedTransactionType *)type __attribute__((swift_name("init(amount:type:)"))) __attribute__((objc_designated_initializer));
- (SharedAmount *)component1 __attribute__((swift_name("component1()")));
- (SharedTransactionType *)component2 __attribute__((swift_name("component2()")));
- (SharedTransactionRequest *)doCopyAmount:(SharedAmount *)amount type:(SharedTransactionType *)type __attribute__((swift_name("doCopy(amount:type:)")));
- (BOOL)isEqual:(id _Nullable)other __attribute__((swift_name("isEqual(_:)")));
- (NSUInteger)hash __attribute__((swift_name("hash()")));
- (NSString *)description __attribute__((swift_name("description()")));
@property (readonly) SharedAmount *amount __attribute__((swift_name("amount")));
@property (readonly) SharedTransactionType *type __attribute__((swift_name("type")));
@end;

__attribute__((swift_name("TransactionResult")))
@interface SharedTransactionResult : KotlinBase
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
@property SharedAmount * _Nullable amount __attribute__((swift_name("amount")));
@property NSString * _Nullable authCode __attribute__((swift_name("authCode")));
@property NSString * _Nullable cardHolderFirstName __attribute__((swift_name("cardHolderFirstName")));
@property NSString * _Nullable cardHolderLastName __attribute__((swift_name("cardHolderLastName")));
@property NSString * _Nullable cardType __attribute__((swift_name("cardType")));
@property SharedTransactionRequest * _Nullable request __attribute__((swift_name("request")));
@property SharedBoolean * _Nullable success __attribute__((swift_name("success")));
@property NSString * _Nullable transactionType __attribute__((swift_name("transactionType")));
@property NSString * _Nullable userReference __attribute__((swift_name("userReference")));
@end;

__attribute__((swift_name("KotlinComparable")))
@protocol SharedKotlinComparable
@required
- (int32_t)compareToOther:(id _Nullable)other __attribute__((swift_name("compareTo(other:)")));
@end;

__attribute__((swift_name("KotlinEnum")))
@interface SharedKotlinEnum : KotlinBase <SharedKotlinComparable>
- (instancetype)initWithName:(NSString *)name ordinal:(int32_t)ordinal __attribute__((swift_name("init(name:ordinal:)"))) __attribute__((objc_designated_initializer));
- (int32_t)compareToOther:(SharedKotlinEnum *)other __attribute__((swift_name("compareTo(other:)")));
- (BOOL)isEqual:(id _Nullable)other __attribute__((swift_name("isEqual(_:)")));
- (NSUInteger)hash __attribute__((swift_name("hash()")));
- (NSString *)description __attribute__((swift_name("description()")));
@property (readonly) NSString *name __attribute__((swift_name("name")));
@property (readonly) int32_t ordinal __attribute__((swift_name("ordinal")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("TransactionType")))
@interface SharedTransactionType : SharedKotlinEnum
+ (instancetype)alloc __attribute__((unavailable));
+ (instancetype)allocWithZone:(struct _NSZone *)zone __attribute__((unavailable));
- (instancetype)initWithName:(NSString *)name ordinal:(int32_t)ordinal __attribute__((swift_name("init(name:ordinal:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
@property (class, readonly) SharedTransactionType *sale __attribute__((swift_name("sale")));
@property (class, readonly) SharedTransactionType *refund __attribute__((swift_name("refund")));
@property (class, readonly) SharedTransactionType *void_ __attribute__((swift_name("void_")));
- (int32_t)compareToOther:(SharedTransactionType *)other __attribute__((swift_name("compareTo(other:)")));
@end;

__attribute__((swift_name("Model")))
@protocol SharedModel
@required
@property NSString * _Nullable id __attribute__((swift_name("id")));
@end;

__attribute__((swift_name("Customer")))
@interface SharedCustomer : KotlinBase <SharedModel>
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
@property NSString * _Nullable address1 __attribute__((swift_name("address1")));
@property NSString * _Nullable address2 __attribute__((swift_name("address2")));
@property NSString * _Nullable addressCity __attribute__((swift_name("addressCity")));
@property NSString * _Nullable addressCountry __attribute__((swift_name("addressCountry")));
@property NSString * _Nullable addressState __attribute__((swift_name("addressState")));
@property NSString * _Nullable addressZip __attribute__((swift_name("addressZip")));
@property SharedBoolean * _Nullable allowInvoiceCreditCardPayments __attribute__((swift_name("allowInvoiceCreditCardPayments")));
@property NSString * _Nullable ccEmails __attribute__((swift_name("ccEmails")));
@property NSString * _Nullable ccSms __attribute__((swift_name("ccSms")));
@property NSString * _Nullable company __attribute__((swift_name("company")));
@property NSString * _Nullable createdAt __attribute__((swift_name("createdAt")));
@property NSString * _Nullable deletedAt __attribute__((swift_name("deletedAt")));
@property NSString * _Nullable email __attribute__((swift_name("email")));
@property NSString * _Nullable firstname __attribute__((swift_name("firstname")));
@property NSString * _Nullable id __attribute__((swift_name("id")));
@property NSString * _Nullable lastname __attribute__((swift_name("lastname")));
@property NSString * _Nullable merchantId __attribute__((swift_name("merchantId")));
@property NSString * _Nullable notes __attribute__((swift_name("notes")));
@property NSString * _Nullable phone __attribute__((swift_name("phone")));
@property NSString * _Nullable reference __attribute__((swift_name("reference")));
@property NSString * _Nullable updatedAt __attribute__((swift_name("updatedAt")));
@end;

__attribute__((swift_name("Invoice")))
@interface SharedInvoice : KotlinBase <SharedModel>
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
@property NSString * _Nullable balanceDue __attribute__((swift_name("balanceDue")));
@property NSString * _Nullable createdAt __attribute__((swift_name("createdAt")));
@property NSString * _Nullable customerId __attribute__((swift_name("customerId")));
@property NSString * _Nullable deletedAt __attribute__((swift_name("deletedAt")));
@property NSString * _Nullable dueAt __attribute__((swift_name("dueAt")));
@property NSString * _Nullable id __attribute__((swift_name("id")));
@property SharedBoolean * _Nullable isMerchantPresent __attribute__((swift_name("isMerchantPresent")));
@property SharedBoolean * _Nullable isPartialPaymentEnabled __attribute__((swift_name("isPartialPaymentEnabled")));
@property SharedBoolean * _Nullable isWebpayment __attribute__((swift_name("isWebpayment")));
@property NSString * _Nullable merchantId __attribute__((swift_name("merchantId")));
@property NSDictionary<NSString *, id> * _Nullable meta __attribute__((swift_name("meta")));
@property NSString * _Nullable paidAt __attribute__((swift_name("paidAt")));
@property SharedBoolean * _Nullable paymentAttemptFailed __attribute__((swift_name("paymentAttemptFailed")));
@property NSString * _Nullable paymentAttemptMessage __attribute__((swift_name("paymentAttemptMessage")));
@property id _Nullable paymentMeta __attribute__((swift_name("paymentMeta")));
@property NSString * _Nullable paymentMethodId __attribute__((swift_name("paymentMethodId")));
@property NSString * _Nullable scheduleId __attribute__((swift_name("scheduleId")));
@property NSString * _Nullable sentAt __attribute__((swift_name("sentAt")));
@property NSString * _Nullable status __attribute__((swift_name("status")));
@property NSString * _Nullable total __attribute__((swift_name("total")));
@property NSString * _Nullable totalPaid __attribute__((swift_name("totalPaid")));
@property NSString * _Nullable updatedAt __attribute__((swift_name("updatedAt")));
@property NSString * _Nullable url __attribute__((swift_name("url")));
@property NSString * _Nullable userId __attribute__((swift_name("userId")));
@property NSString * _Nullable viewedAt __attribute__((swift_name("viewedAt")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Merchant")))
@interface SharedMerchant : KotlinBase <SharedModel>
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (NSString * _Nullable)emvPassword __attribute__((swift_name("emvPassword()")));
@property NSString * _Nullable id __attribute__((swift_name("id")));
@property NSDictionary<NSString *, id> * _Nullable options __attribute__((swift_name("options")));
@end;

__attribute__((swift_name("OmniException")))
@interface SharedOmniException : SharedKotlinThrowable
- (instancetype)initWithMessage:(NSString *)message detail:(NSString *)detail __attribute__((swift_name("init(message:detail:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithMessage:(NSString *)message __attribute__((swift_name("init(message:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithCause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(cause:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
+ (instancetype)new __attribute__((unavailable));
- (instancetype)initWithMessage:(NSString * _Nullable)message cause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(message:cause:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
@property NSString * _Nullable detail __attribute__((swift_name("detail")));
@end;

__attribute__((swift_name("PaymentMethod")))
@interface SharedPaymentMethod : KotlinBase <SharedModel>
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
@property NSString * _Nullable address1 __attribute__((swift_name("address1")));
@property NSString * _Nullable address2 __attribute__((swift_name("address2")));
@property NSString * _Nullable addressCity __attribute__((swift_name("addressCity")));
@property NSString * _Nullable addressCountry __attribute__((swift_name("addressCountry")));
@property NSString * _Nullable addressState __attribute__((swift_name("addressState")));
@property NSString * _Nullable addressZip __attribute__((swift_name("addressZip")));
@property NSString * _Nullable bankHolderType __attribute__((swift_name("bankHolderType")));
@property NSString * _Nullable bankName __attribute__((swift_name("bankName")));
@property NSString * _Nullable bankType __attribute__((swift_name("bankType")));
@property NSString * _Nullable cardExp __attribute__((swift_name("cardExp")));
@property id _Nullable cardExpDatetime __attribute__((swift_name("cardExpDatetime")));
@property NSString * _Nullable cardLastFour __attribute__((swift_name("cardLastFour")));
@property NSString * _Nullable cardType __attribute__((swift_name("cardType")));
@property NSString * _Nullable createdAt __attribute__((swift_name("createdAt")));
@property NSString * _Nullable customerId __attribute__((swift_name("customerId")));
@property NSString * _Nullable deletedAt __attribute__((swift_name("deletedAt")));
@property SharedBoolean * _Nullable hasCvv __attribute__((swift_name("hasCvv")));
@property NSString * _Nullable id __attribute__((swift_name("id")));
@property SharedInt * _Nullable isDefault __attribute__((swift_name("isDefault")));
@property SharedBoolean * _Nullable isUsableInVt __attribute__((swift_name("isUsableInVt")));
@property NSString * _Nullable merchantId __attribute__((swift_name("merchantId")));
@property NSString * _Nullable meta __attribute__((swift_name("meta")));
@property NSString * _Nullable method __attribute__((swift_name("method")));
@property NSString * _Nullable nickname __attribute__((swift_name("nickname")));
@property NSString * _Nullable personName __attribute__((swift_name("personName")));
@property NSString * _Nullable purgedAt __attribute__((swift_name("purgedAt")));
@property NSString * _Nullable spreedlyToken __attribute__((swift_name("spreedlyToken")));
@property SharedBoolean * _Nullable tokenize __attribute__((swift_name("tokenize")));
@property NSString * _Nullable updatedAt __attribute__((swift_name("updatedAt")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Self")))
@interface SharedSelf : KotlinBase
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
@property (readonly) SharedMerchant * _Nullable merchant __attribute__((swift_name("merchant")));
@end;

__attribute__((swift_name("Transaction")))
@interface SharedTransaction : KotlinBase <SharedModel>
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
@property NSString * _Nullable authId __attribute__((swift_name("authId")));
@property id _Nullable childCaptures __attribute__((swift_name("childCaptures")));
@property NSString * _Nullable createdAt __attribute__((swift_name("createdAt")));
@property NSString * _Nullable customerId __attribute__((swift_name("customerId")));
@property NSString * _Nullable gateway __attribute__((swift_name("gateway")));
@property NSString * _Nullable gatewayId __attribute__((swift_name("gatewayId")));
@property NSString * _Nullable gatewayName __attribute__((swift_name("gatewayName")));
@property NSString * _Nullable id __attribute__((swift_name("id")));
@property NSString * _Nullable invoiceId __attribute__((swift_name("invoiceId")));
@property SharedInt * _Nullable isCaptured __attribute__((swift_name("isCaptured")));
@property SharedBoolean * _Nullable isManual __attribute__((swift_name("isManual")));
@property SharedBoolean * _Nullable isMerchantPresent __attribute__((swift_name("isMerchantPresent")));
@property SharedBoolean * _Nullable isRefundable __attribute__((swift_name("isRefundable")));
@property SharedBoolean * _Nullable isVoidable __attribute__((swift_name("isVoidable")));
@property SharedBoolean * _Nullable isVoided __attribute__((swift_name("isVoided")));
@property NSString * _Nullable issuerAuthCode __attribute__((swift_name("issuerAuthCode")));
@property NSString * _Nullable lastFour __attribute__((swift_name("lastFour")));
@property NSString * _Nullable merchantId __attribute__((swift_name("merchantId")));
@property NSString * _Nullable message __attribute__((swift_name("message")));
@property id _Nullable meta __attribute__((swift_name("meta")));
@property NSString * _Nullable method __attribute__((swift_name("method")));
@property NSString * _Nullable parentAuth __attribute__((swift_name("parentAuth")));
@property NSString * _Nullable paymentMethodId __attribute__((swift_name("paymentMethodId")));
@property SharedBoolean * _Nullable preAuth __attribute__((swift_name("preAuth")));
@property NSString * _Nullable receiptEmailAt __attribute__((swift_name("receiptEmailAt")));
@property NSString * _Nullable receiptSmsAt __attribute__((swift_name("receiptSmsAt")));
@property NSString * _Nullable referenceId __attribute__((swift_name("referenceId")));
@property id _Nullable response __attribute__((swift_name("response")));
@property NSString * _Nullable scheduleId __attribute__((swift_name("scheduleId")));
@property NSString * _Nullable settledAt __attribute__((swift_name("settledAt")));
@property NSString * _Nullable source __attribute__((swift_name("source")));
@property NSString * _Nullable sourceIp __attribute__((swift_name("sourceIp")));
@property NSString * _Nullable spreedlyToken __attribute__((swift_name("spreedlyToken")));
@property SharedBoolean * _Nullable success __attribute__((swift_name("success")));
@property NSString * _Nullable total __attribute__((swift_name("total")));
@property NSString * _Nullable totalRefunded __attribute__((swift_name("totalRefunded")));
@property NSString * _Nullable type __attribute__((swift_name("type")));
@property NSString * _Nullable updatedAt __attribute__((swift_name("updatedAt")));
@property NSString * _Nullable userId __attribute__((swift_name("userId")));
@end;

__attribute__((swift_name("ModelRepository")))
@protocol SharedModelRepository
@required
@property SharedOmniApi *omniApi __attribute__((swift_name("omniApi")));
@end;

__attribute__((swift_name("CustomerRepository")))
@protocol SharedCustomerRepository <SharedModelRepository>
@required
@end;

__attribute__((swift_name("InvoiceRepository")))
@protocol SharedInvoiceRepository <SharedModelRepository>
@required
@end;

__attribute__((swift_name("MobileReaderDriverRepository")))
@protocol SharedMobileReaderDriverRepository
@required
@end;

__attribute__((swift_name("PaymentMethodRepository")))
@protocol SharedPaymentMethodRepository <SharedModelRepository>
@required
@end;

__attribute__((swift_name("TransactionRepository")))
@protocol SharedTransactionRepository <SharedModelRepository>
@required
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("JsonParserKt")))
@interface SharedJsonParserKt : KotlinBase
+ (id _Nullable)fromJsonJson:(NSString *)json __attribute__((swift_name("fromJson(json:)")));
+ (NSString *)toJsonObj:(id _Nullable)obj __attribute__((swift_name("toJson(obj:)")));
@end;

__attribute__((swift_name("KotlinCoroutineContext")))
@protocol SharedKotlinCoroutineContext
@required
- (id _Nullable)foldInitial:(id _Nullable)initial operation:(id _Nullable (^)(id _Nullable, id<SharedKotlinCoroutineContextElement>))operation __attribute__((swift_name("fold(initial:operation:)")));
- (id<SharedKotlinCoroutineContextElement> _Nullable)getKey:(id<SharedKotlinCoroutineContextKey>)key __attribute__((swift_name("get(key:)")));
- (id<SharedKotlinCoroutineContext>)minusKeyKey:(id<SharedKotlinCoroutineContextKey>)key __attribute__((swift_name("minusKey(key:)")));
- (id<SharedKotlinCoroutineContext>)plusContext:(id<SharedKotlinCoroutineContext>)context __attribute__((swift_name("plus(context:)")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("KotlinArray")))
@interface SharedKotlinArray : KotlinBase
+ (instancetype)arrayWithSize:(int32_t)size init:(id _Nullable (^)(SharedInt *))init __attribute__((swift_name("init(size:init:)")));
+ (instancetype)alloc __attribute__((unavailable));
+ (instancetype)allocWithZone:(struct _NSZone *)zone __attribute__((unavailable));
- (id _Nullable)getIndex:(int32_t)index __attribute__((swift_name("get(index:)")));
- (id<SharedKotlinIterator>)iterator __attribute__((swift_name("iterator()")));
- (void)setIndex:(int32_t)index value:(id _Nullable)value __attribute__((swift_name("set(index:value:)")));
@property (readonly) int32_t size __attribute__((swift_name("size")));
@end;

__attribute__((swift_name("Kotlinx_ioCloseable")))
@protocol SharedKotlinx_ioCloseable
@required
- (void)close __attribute__((swift_name("close()")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Ktor_client_coreHttpClient")))
@interface SharedKtor_client_coreHttpClient : KotlinBase <SharedKotlinx_coroutines_coreCoroutineScope, SharedKotlinx_ioCloseable>
- (instancetype)initWithEngine:(id<SharedKtor_client_coreHttpClientEngine>)engine userConfig:(SharedKtor_client_coreHttpClientConfig *)userConfig __attribute__((swift_name("init(engine:userConfig:)"))) __attribute__((objc_designated_initializer));
- (void)close __attribute__((swift_name("close()")));
- (SharedKtor_client_coreHttpClient *)configBlock:(void (^)(SharedKtor_client_coreHttpClientConfig *))block __attribute__((swift_name("config(block:)")));
@property (readonly) id<SharedKtor_utilsAttributes> attributes __attribute__((swift_name("attributes")));
@property (readonly) id<SharedKotlinCoroutineContext> coroutineContext __attribute__((swift_name("coroutineContext")));
@property (readonly) SharedKotlinx_coroutines_coreCoroutineDispatcher *dispatcher __attribute__((swift_name("dispatcher"))) __attribute__((unavailable("[dispatcher] is deprecated. Use coroutineContext instead.")));
@property (readonly) id<SharedKtor_client_coreHttpClientEngine> engine __attribute__((swift_name("engine")));
@property (readonly) SharedKtor_client_coreHttpClientEngineConfig *engineConfig __attribute__((swift_name("engineConfig")));
@property (readonly) SharedKtor_client_coreHttpReceivePipeline *receivePipeline __attribute__((swift_name("receivePipeline")));
@property (readonly) SharedKtor_client_coreHttpRequestPipeline *requestPipeline __attribute__((swift_name("requestPipeline")));
@property (readonly) SharedKtor_client_coreHttpResponsePipeline *responsePipeline __attribute__((swift_name("responsePipeline")));
@property (readonly) SharedKtor_client_coreHttpSendPipeline *sendPipeline __attribute__((swift_name("sendPipeline")));
@end;

__attribute__((swift_name("KotlinCoroutineContextElement")))
@protocol SharedKotlinCoroutineContextElement <SharedKotlinCoroutineContext>
@required
@property (readonly) id<SharedKotlinCoroutineContextKey> key __attribute__((swift_name("key")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreJob")))
@protocol SharedKotlinx_coroutines_coreJob <SharedKotlinCoroutineContextElement>
@required
- (id<SharedKotlinx_coroutines_coreChildHandle>)attachChildChild:(id<SharedKotlinx_coroutines_coreChildJob>)child __attribute__((swift_name("attachChild(child:)")));
- (void)cancelCause:(SharedKotlinx_coroutines_coreCancellationException * _Nullable)cause __attribute__((swift_name("cancel(cause:)")));
- (SharedKotlinx_coroutines_coreCancellationException *)getCancellationException __attribute__((swift_name("getCancellationException()")));
- (id<SharedKotlinx_coroutines_coreDisposableHandle>)invokeOnCompletionOnCancelling:(BOOL)onCancelling invokeImmediately:(BOOL)invokeImmediately handler:(void (^)(SharedKotlinThrowable * _Nullable))handler __attribute__((swift_name("invokeOnCompletion(onCancelling:invokeImmediately:handler:)")));
- (id<SharedKotlinx_coroutines_coreDisposableHandle>)invokeOnCompletionHandler:(void (^)(SharedKotlinThrowable * _Nullable))handler __attribute__((swift_name("invokeOnCompletion(handler:)")));
- (id<SharedKotlinx_coroutines_coreJob>)plusOther:(id<SharedKotlinx_coroutines_coreJob>)other __attribute__((swift_name("plus(other:)"))) __attribute__((unavailable("Operator '+' on two Job objects is meaningless. Job is a coroutine context element and `+` is a set-sum operator for coroutine contexts. The job to the right of `+` just replaces the job the left of `+`.")));
- (BOOL)start __attribute__((swift_name("start()")));
@property (readonly) id<SharedKotlinSequence> children __attribute__((swift_name("children")));
@property (readonly) BOOL isActive __attribute__((swift_name("isActive")));
@property (readonly) BOOL isCancelled __attribute__((swift_name("isCancelled")));
@property (readonly) BOOL isCompleted __attribute__((swift_name("isCompleted")));
@property (readonly) id<SharedKotlinx_coroutines_coreSelectClause0> onJoin __attribute__((swift_name("onJoin")));
@end;

__attribute__((swift_name("KotlinError")))
@interface SharedKotlinError : SharedKotlinThrowable
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithMessage:(NSString * _Nullable)message __attribute__((swift_name("init(message:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithMessage:(NSString * _Nullable)message cause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(message:cause:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithCause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(cause:)"))) __attribute__((objc_designated_initializer));
@end;

__attribute__((swift_name("KotlinCoroutineContextKey")))
@protocol SharedKotlinCoroutineContextKey
@required
@end;

__attribute__((swift_name("KotlinIterator")))
@protocol SharedKotlinIterator
@required
- (BOOL)hasNext __attribute__((swift_name("hasNext()")));
- (id _Nullable)next __attribute__((swift_name("next()")));
@end;

__attribute__((swift_name("Ktor_client_coreHttpClientEngine")))
@protocol SharedKtor_client_coreHttpClientEngine <SharedKotlinx_coroutines_coreCoroutineScope, SharedKotlinx_ioCloseable>
@required
- (void)installClient:(SharedKtor_client_coreHttpClient *)client __attribute__((swift_name("install(client:)")));
@property (readonly) SharedKtor_client_coreHttpClientEngineConfig *config __attribute__((swift_name("config")));
@property (readonly) SharedKotlinx_coroutines_coreCoroutineDispatcher *dispatcher __attribute__((swift_name("dispatcher")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Ktor_client_coreHttpClientConfig")))
@interface SharedKtor_client_coreHttpClientConfig : KotlinBase
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (SharedKtor_client_coreHttpClientConfig *)clone __attribute__((swift_name("clone()")));
- (void)engineBlock:(void (^)(SharedKtor_client_coreHttpClientEngineConfig *))block __attribute__((swift_name("engine(block:)")));
- (void)installClient:(SharedKtor_client_coreHttpClient *)client __attribute__((swift_name("install(client:)")));
- (void)installFeature:(id<SharedKtor_client_coreHttpClientFeature>)feature configure:(void (^)(id))configure __attribute__((swift_name("install(feature:configure:)")));
- (void)installKey:(NSString *)key block:(void (^)(SharedKtor_client_coreHttpClient *))block __attribute__((swift_name("install(key:block:)")));
- (void)plusAssignOther:(SharedKtor_client_coreHttpClientConfig *)other __attribute__((swift_name("plusAssign(other:)")));
@property BOOL expectSuccess __attribute__((swift_name("expectSuccess")));
@property BOOL followRedirects __attribute__((swift_name("followRedirects")));
@property BOOL useDefaultTransformers __attribute__((swift_name("useDefaultTransformers")));
@end;

__attribute__((swift_name("Ktor_utilsAttributes")))
@protocol SharedKtor_utilsAttributes
@required
- (id)computeIfAbsentKey:(SharedKtor_utilsAttributeKey *)key block:(id (^)(void))block __attribute__((swift_name("computeIfAbsent(key:block:)")));
- (BOOL)containsKey:(SharedKtor_utilsAttributeKey *)key __attribute__((swift_name("contains(key:)")));
- (id)getKey_:(SharedKtor_utilsAttributeKey *)key __attribute__((swift_name("get(key_:)")));
- (id _Nullable)getOrNullKey:(SharedKtor_utilsAttributeKey *)key __attribute__((swift_name("getOrNull(key:)")));
- (void)putKey:(SharedKtor_utilsAttributeKey *)key value:(id)value __attribute__((swift_name("put(key:value:)")));
- (void)removeKey:(SharedKtor_utilsAttributeKey *)key __attribute__((swift_name("remove(key:)")));
- (id)takeKey:(SharedKtor_utilsAttributeKey *)key __attribute__((swift_name("take(key:)")));
- (id _Nullable)takeOrNullKey:(SharedKtor_utilsAttributeKey *)key __attribute__((swift_name("takeOrNull(key:)")));
@property (readonly) NSArray<SharedKtor_utilsAttributeKey *> *allKeys __attribute__((swift_name("allKeys")));
@end;

__attribute__((swift_name("KotlinAbstractCoroutineContextElement")))
@interface SharedKotlinAbstractCoroutineContextElement : KotlinBase <SharedKotlinCoroutineContextElement>
- (instancetype)initWithKey:(id<SharedKotlinCoroutineContextKey>)key __attribute__((swift_name("init(key:)"))) __attribute__((objc_designated_initializer));
@property (readonly) id<SharedKotlinCoroutineContextKey> key __attribute__((swift_name("key")));
@end;

__attribute__((swift_name("KotlinContinuationInterceptor")))
@protocol SharedKotlinContinuationInterceptor <SharedKotlinCoroutineContextElement>
@required
- (id<SharedKotlinContinuation>)interceptContinuationContinuation:(id<SharedKotlinContinuation>)continuation __attribute__((swift_name("interceptContinuation(continuation:)")));
- (void)releaseInterceptedContinuationContinuation:(id<SharedKotlinContinuation>)continuation __attribute__((swift_name("releaseInterceptedContinuation(continuation:)")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreCoroutineDispatcher")))
@interface SharedKotlinx_coroutines_coreCoroutineDispatcher : SharedKotlinAbstractCoroutineContextElement <SharedKotlinContinuationInterceptor>
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithKey:(id<SharedKotlinCoroutineContextKey>)key __attribute__((swift_name("init(key:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
- (void)dispatchContext:(id<SharedKotlinCoroutineContext>)context block:(id<SharedKotlinx_coroutines_coreRunnable>)block __attribute__((swift_name("dispatch(context:block:)")));
- (void)dispatchYieldContext:(id<SharedKotlinCoroutineContext>)context block:(id<SharedKotlinx_coroutines_coreRunnable>)block __attribute__((swift_name("dispatchYield(context:block:)")));
- (id<SharedKotlinContinuation>)interceptContinuationContinuation:(id<SharedKotlinContinuation>)continuation __attribute__((swift_name("interceptContinuation(continuation:)")));
- (BOOL)isDispatchNeededContext:(id<SharedKotlinCoroutineContext>)context __attribute__((swift_name("isDispatchNeeded(context:)")));
- (SharedKotlinx_coroutines_coreCoroutineDispatcher *)plusOther_:(SharedKotlinx_coroutines_coreCoroutineDispatcher *)other __attribute__((swift_name("plus(other_:)"))) __attribute__((unavailable("Operator '+' on two CoroutineDispatcher objects is meaningless. CoroutineDispatcher is a coroutine context element and `+` is a set-sum operator for coroutine contexts. The dispatcher to the right of `+` just replaces the dispatcher the left of `+`.")));
- (NSString *)description __attribute__((swift_name("description()")));
@end;

__attribute__((swift_name("Ktor_client_coreHttpClientEngineConfig")))
@interface SharedKtor_client_coreHttpClientEngineConfig : KotlinBase
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
@property BOOL pipelining __attribute__((swift_name("pipelining")));
@property (readonly) SharedKtor_client_coreHttpResponseConfig *response __attribute__((swift_name("response")));
@property int32_t threadsCount __attribute__((swift_name("threadsCount")));
@end;

__attribute__((swift_name("Ktor_utilsPipeline")))
@interface SharedKtor_utilsPipeline : KotlinBase
- (instancetype)initWithPhase:(SharedKtor_utilsPipelinePhase *)phase interceptors:(NSArray<id<SharedKotlinSuspendFunction2>> *)interceptors __attribute__((swift_name("init(phase:interceptors:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithPhases:(SharedKotlinArray *)phases __attribute__((swift_name("init(phases:)"))) __attribute__((objc_designated_initializer));
- (void)addPhasePhase:(SharedKtor_utilsPipelinePhase *)phase __attribute__((swift_name("addPhase(phase:)")));
- (void)afterIntercepted __attribute__((swift_name("afterIntercepted()")));
- (void)insertPhaseAfterReference:(SharedKtor_utilsPipelinePhase *)reference phase:(SharedKtor_utilsPipelinePhase *)phase __attribute__((swift_name("insertPhaseAfter(reference:phase:)")));
- (void)insertPhaseBeforeReference:(SharedKtor_utilsPipelinePhase *)reference phase:(SharedKtor_utilsPipelinePhase *)phase __attribute__((swift_name("insertPhaseBefore(reference:phase:)")));
- (void)interceptPhase:(SharedKtor_utilsPipelinePhase *)phase block:(id<SharedKotlinSuspendFunction2>)block __attribute__((swift_name("intercept(phase:block:)")));
- (void)mergeFrom:(SharedKtor_utilsPipeline *)from __attribute__((swift_name("merge(from:)")));
@property (readonly) id<SharedKtor_utilsAttributes> attributes __attribute__((swift_name("attributes")));
@property (readonly) BOOL isEmpty __attribute__((swift_name("isEmpty")));
@property (readonly) NSArray<SharedKtor_utilsPipelinePhase *> *items __attribute__((swift_name("items")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Ktor_client_coreHttpReceivePipeline")))
@interface SharedKtor_client_coreHttpReceivePipeline : SharedKtor_utilsPipeline
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithPhase:(SharedKtor_utilsPipelinePhase *)phase interceptors:(NSArray<id<SharedKotlinSuspendFunction2>> *)interceptors __attribute__((swift_name("init(phase:interceptors:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
- (instancetype)initWithPhases:(SharedKotlinArray *)phases __attribute__((swift_name("init(phases:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Ktor_client_coreHttpRequestPipeline")))
@interface SharedKtor_client_coreHttpRequestPipeline : SharedKtor_utilsPipeline
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithPhase:(SharedKtor_utilsPipelinePhase *)phase interceptors:(NSArray<id<SharedKotlinSuspendFunction2>> *)interceptors __attribute__((swift_name("init(phase:interceptors:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
- (instancetype)initWithPhases:(SharedKotlinArray *)phases __attribute__((swift_name("init(phases:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Ktor_client_coreHttpResponsePipeline")))
@interface SharedKtor_client_coreHttpResponsePipeline : SharedKtor_utilsPipeline
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithPhase:(SharedKtor_utilsPipelinePhase *)phase interceptors:(NSArray<id<SharedKotlinSuspendFunction2>> *)interceptors __attribute__((swift_name("init(phase:interceptors:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
- (instancetype)initWithPhases:(SharedKotlinArray *)phases __attribute__((swift_name("init(phases:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Ktor_client_coreHttpSendPipeline")))
@interface SharedKtor_client_coreHttpSendPipeline : SharedKtor_utilsPipeline
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithPhase:(SharedKtor_utilsPipelinePhase *)phase interceptors:(NSArray<id<SharedKotlinSuspendFunction2>> *)interceptors __attribute__((swift_name("init(phase:interceptors:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
- (instancetype)initWithPhases:(SharedKotlinArray *)phases __attribute__((swift_name("init(phases:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreDisposableHandle")))
@protocol SharedKotlinx_coroutines_coreDisposableHandle
@required
- (void)dispose __attribute__((swift_name("dispose()")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreChildHandle")))
@protocol SharedKotlinx_coroutines_coreChildHandle <SharedKotlinx_coroutines_coreDisposableHandle>
@required
- (BOOL)childCancelledCause:(SharedKotlinThrowable *)cause __attribute__((swift_name("childCancelled(cause:)")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreChildJob")))
@protocol SharedKotlinx_coroutines_coreChildJob <SharedKotlinx_coroutines_coreJob>
@required
- (void)parentCancelledParentJob:(id<SharedKotlinx_coroutines_coreParentJob>)parentJob __attribute__((swift_name("parentCancelled(parentJob:)")));
@end;

__attribute__((swift_name("KotlinRuntimeException")))
@interface SharedKotlinRuntimeException : SharedKotlinException
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithMessage:(NSString * _Nullable)message __attribute__((swift_name("init(message:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithMessage:(NSString * _Nullable)message cause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(message:cause:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithCause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(cause:)"))) __attribute__((objc_designated_initializer));
@end;

__attribute__((swift_name("KotlinIllegalStateException")))
@interface SharedKotlinIllegalStateException : SharedKotlinRuntimeException
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (instancetype)initWithMessage:(NSString * _Nullable)message __attribute__((swift_name("init(message:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithMessage:(NSString * _Nullable)message cause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(message:cause:)"))) __attribute__((objc_designated_initializer));
- (instancetype)initWithCause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(cause:)"))) __attribute__((objc_designated_initializer));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreCancellationException")))
@interface SharedKotlinx_coroutines_coreCancellationException : SharedKotlinIllegalStateException
- (instancetype)initWithMessage:(NSString * _Nullable)message __attribute__((swift_name("init(message:)"))) __attribute__((objc_designated_initializer));
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
+ (instancetype)new __attribute__((unavailable));
- (instancetype)initWithMessage:(NSString * _Nullable)message cause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(message:cause:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
- (instancetype)initWithCause:(SharedKotlinThrowable * _Nullable)cause __attribute__((swift_name("init(cause:)"))) __attribute__((objc_designated_initializer)) __attribute__((unavailable));
@end;

__attribute__((swift_name("KotlinSequence")))
@protocol SharedKotlinSequence
@required
- (id<SharedKotlinIterator>)iterator __attribute__((swift_name("iterator()")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreSelectClause0")))
@protocol SharedKotlinx_coroutines_coreSelectClause0
@required
- (void)registerSelectClause0Select:(id<SharedKotlinx_coroutines_coreSelectInstance>)select block:(id<SharedKotlinSuspendFunction0>)block __attribute__((swift_name("registerSelectClause0(select:block:)")));
@end;

__attribute__((swift_name("Ktor_client_coreHttpClientFeature")))
@protocol SharedKtor_client_coreHttpClientFeature
@required
- (void)installFeature:(id)feature scope:(SharedKtor_client_coreHttpClient *)scope __attribute__((swift_name("install(feature:scope:)")));
- (id)prepareBlock:(void (^)(id))block __attribute__((swift_name("prepare(block:)")));
@property (readonly) SharedKtor_utilsAttributeKey *key __attribute__((swift_name("key")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Ktor_utilsAttributeKey")))
@interface SharedKtor_utilsAttributeKey : KotlinBase
- (instancetype)initWithName:(NSString *)name __attribute__((swift_name("init(name:)"))) __attribute__((objc_designated_initializer));
- (NSString *)description __attribute__((swift_name("description()")));
@property (readonly) NSString *name __attribute__((swift_name("name")));
@end;

__attribute__((swift_name("KotlinContinuation")))
@protocol SharedKotlinContinuation
@required
- (void)resumeWithResult:(id _Nullable)result __attribute__((swift_name("resumeWith(result:)")));
@property (readonly) id<SharedKotlinCoroutineContext> context __attribute__((swift_name("context")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreRunnable")))
@protocol SharedKotlinx_coroutines_coreRunnable
@required
- (void)run __attribute__((swift_name("run()")));
@end;

__attribute__((swift_name("Ktor_client_coreHttpResponseConfig")))
@interface SharedKtor_client_coreHttpResponseConfig : KotlinBase
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
@property SharedKotlinx_ioCharset *defaultCharset __attribute__((swift_name("defaultCharset"))) __attribute__((unavailable("Use [Charsets { responseFallbackCharset }] in [HttpClientConfig] instead.")));
@end;

__attribute__((objc_subclassing_restricted))
__attribute__((swift_name("Ktor_utilsPipelinePhase")))
@interface SharedKtor_utilsPipelinePhase : KotlinBase
- (instancetype)initWithName:(NSString *)name __attribute__((swift_name("init(name:)"))) __attribute__((objc_designated_initializer));
- (NSString *)description __attribute__((swift_name("description()")));
@property (readonly) NSString *name __attribute__((swift_name("name")));
@end;

__attribute__((swift_name("KotlinFunction")))
@protocol SharedKotlinFunction
@required
@end;

__attribute__((swift_name("KotlinSuspendFunction2")))
@protocol SharedKotlinSuspendFunction2 <SharedKotlinFunction>
@required
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreParentJob")))
@protocol SharedKotlinx_coroutines_coreParentJob <SharedKotlinx_coroutines_coreJob>
@required
- (SharedKotlinx_coroutines_coreCancellationException *)getChildJobCancellationCause __attribute__((swift_name("getChildJobCancellationCause()")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreSelectInstance")))
@protocol SharedKotlinx_coroutines_coreSelectInstance
@required
- (void)disposeOnSelectHandle:(id<SharedKotlinx_coroutines_coreDisposableHandle>)handle __attribute__((swift_name("disposeOnSelect(handle:)")));
- (id _Nullable)performAtomicTrySelectDesc:(SharedKotlinx_coroutines_coreAtomicDesc *)desc __attribute__((swift_name("performAtomicTrySelect(desc:)")));
- (void)resumeSelectCancellableWithExceptionException:(SharedKotlinThrowable *)exception __attribute__((swift_name("resumeSelectCancellableWithException(exception:)")));
- (BOOL)trySelectIdempotent:(id _Nullable)idempotent __attribute__((swift_name("trySelect(idempotent:)")));
@property (readonly) id<SharedKotlinContinuation> completion __attribute__((swift_name("completion")));
@property (readonly) BOOL isSelected __attribute__((swift_name("isSelected")));
@end;

__attribute__((swift_name("KotlinSuspendFunction0")))
@protocol SharedKotlinSuspendFunction0 <SharedKotlinFunction>
@required
@end;

__attribute__((swift_name("Kotlinx_ioCharset")))
@interface SharedKotlinx_ioCharset : KotlinBase
- (instancetype)initWith_name:(NSString *)_name __attribute__((swift_name("init(_name:)"))) __attribute__((objc_designated_initializer));
- (SharedKotlinx_ioCharsetDecoder *)doNewDecoder __attribute__((swift_name("doNewDecoder()")));
- (SharedKotlinx_ioCharsetEncoder *)doNewEncoder __attribute__((swift_name("doNewEncoder()")));
- (NSString *)description __attribute__((swift_name("description()")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreAtomicDesc")))
@interface SharedKotlinx_coroutines_coreAtomicDesc : KotlinBase
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (void)completeOp:(SharedKotlinx_coroutines_coreAtomicOp *)op failure:(id _Nullable)failure __attribute__((swift_name("complete(op:failure:)")));
- (id _Nullable)prepareOp:(SharedKotlinx_coroutines_coreAtomicOp *)op __attribute__((swift_name("prepare(op:)")));
@end;

__attribute__((swift_name("Kotlinx_ioCharsetDecoder")))
@interface SharedKotlinx_ioCharsetDecoder : KotlinBase
- (instancetype)initWith_charset:(SharedKotlinx_ioCharset *)_charset __attribute__((swift_name("init(_charset:)"))) __attribute__((objc_designated_initializer));
@end;

__attribute__((swift_name("Kotlinx_ioCharsetEncoder")))
@interface SharedKotlinx_ioCharsetEncoder : KotlinBase
- (instancetype)initWith_charset:(SharedKotlinx_ioCharset *)_charset __attribute__((swift_name("init(_charset:)"))) __attribute__((objc_designated_initializer));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreOpDescriptor")))
@interface SharedKotlinx_coroutines_coreOpDescriptor : KotlinBase
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (id _Nullable)performAffected:(id _Nullable)affected __attribute__((swift_name("perform(affected:)")));
@end;

__attribute__((swift_name("Kotlinx_coroutines_coreAtomicOp")))
@interface SharedKotlinx_coroutines_coreAtomicOp : SharedKotlinx_coroutines_coreOpDescriptor
- (instancetype)init __attribute__((swift_name("init()"))) __attribute__((objc_designated_initializer));
+ (instancetype)new __attribute__((availability(swift, unavailable, message="use object initializers instead")));
- (void)completeAffected:(id _Nullable)affected failure:(id _Nullable)failure __attribute__((swift_name("complete(affected:failure:)")));
- (id _Nullable)performAffected:(id _Nullable)affected __attribute__((swift_name("perform(affected:)")));
- (id _Nullable)prepareAffected:(id _Nullable)affected __attribute__((swift_name("prepare(affected:)")));
- (BOOL)tryDecideDecision:(id _Nullable)decision __attribute__((swift_name("tryDecide(decision:)")));
@property (readonly) BOOL isDecided __attribute__((swift_name("isDecided")));
@end;

#pragma clang diagnostic pop
NS_ASSUME_NONNULL_END
