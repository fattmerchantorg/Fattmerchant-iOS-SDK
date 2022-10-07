//
//  ANPProPayTransaction.h
//  AnyPay
//
//  Created by Ankit Gupta on 16/07/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#import "ANPEmvTransaction.h"

@interface ANPProPayTransaction : ANPEmvTransaction

- (ANPProPayTransaction *)createCapture;


@end
