//
//  ANPAccountType.h
//  AnyPay
//
//  Created by Ankit Gupta on 27/12/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#ifndef ANPAccountType_h
#define ANPAccountType_h


typedef NS_ENUM(NSUInteger, ANPAccountType) {
    ANPAccountType_UNSPECIFIED = 0,
    ANPAccountType_CHECKING,
    ANPAccountType_SAVINGS,
    ANPAccountType_OTHER
};

#endif /* ANPAccountType_h */
