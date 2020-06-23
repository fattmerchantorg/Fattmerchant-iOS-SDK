//
//  ANPPriorityPaymentsReferenceTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 08/08/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPReferenceTransaction.h"

@interface ANPPriorityPaymentsReferenceTransaction : ANPReferenceTransaction

@property (nonatomic) BOOL forceRefund; //Force a refund if transaction has already settled

@end
