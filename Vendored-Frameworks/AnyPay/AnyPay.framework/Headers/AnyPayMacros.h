//
//  AnyPayMacros.h
//  AnyPay
//
//  Created by Ankit Gupta on 25/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#ifndef AnyPayMacros_h
#define AnyPayMacros_h

#import "AnyPay.h"

#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

#define StringResourceFromTable(key, tbl) \
NSLocalizedStringFromTableInBundle(key, tbl, [NSBundle bundleForClass:AnyPay.class], key)

#endif /* AnyPayMacros_h */
