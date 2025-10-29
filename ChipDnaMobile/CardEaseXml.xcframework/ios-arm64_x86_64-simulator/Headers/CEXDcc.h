
/**
 * @author Creditcall Ltd
 * @brief A class holding all of the data that constitutes a Response from CardEaseXML.
 * @copyright Copyright Creditcall Limited 2010 - 2015.
 * @class CEXDcc
 * @details The DCC transaction data.
 */
#import <Foundation/Foundation.h>
#import "CEXAmountUnit.h"

/// @file

@interface CEXDcc: NSObject

@property CEXAmountUnit amountUnit;

@property (strong) NSString *amount;

@property (strong) NSString *amountTip;

@property (strong) NSString *amountOther;

@property (strong) NSString *amountOtherTransactionFee;

@property (strong) NSString *commissionAmount;

@property CEXAmountUnit commissionAmountUnit;

@property (strong) NSString *marginRate;

@property (strong) NSString *exchangeRate;

@property (strong) NSString *currencyCode;

+(NSDecimalNumber *)Truncate:(NSDecimalNumber *)value  decimalPlaces:(NSInteger) decimalPlaces;
+(NSDecimalNumber *)calculateFinalRate:(NSDecimalNumber *)exchangeRate marginRate:(NSDecimalNumber *)marginRate exchangeRatePrecision:(NSInteger) exchangeRatePrecision;
+(NSDecimalNumber *)convertTo:(NSDecimalNumber *)amount rate:(NSDecimalNumber *)rate decimalPlaces:(NSInteger) decimalPlaces;
+(NSDecimalNumber *)convertFrom:(NSDecimalNumber *)amount rate:(NSDecimalNumber *)rate decimalPlaces:(NSInteger) decimalPlaces;
+(BOOL)convert:(NSInteger)currencyExponent amount:(NSDecimalNumber *)amount amountOther:(NSDecimalNumber *)amountOther amountOtherTransactionFee:(NSDecimalNumber *)amountOtherTransactionFee
          amountTip:(NSDecimalNumber *)amountTip exchangeRate:(NSDecimalNumber *)exchangeRate exchangeRatePrecision:(NSInteger)exchangeRatePrecision
         marginRate:(NSDecimalNumber *)marginRate commission:(NSDecimalNumber *)commission
          finalRate:(NSDecimalNumber **)finalRate dccAmount:(NSDecimalNumber **)dccAmount dccAmountOther:(NSDecimalNumber **)dccAmountOther dccAmountOtherTransactionFee:(NSDecimalNumber **)dccAmountOtherTransactionFee
       dccAmountTip:(NSDecimalNumber **)dccAmountTip;

@end