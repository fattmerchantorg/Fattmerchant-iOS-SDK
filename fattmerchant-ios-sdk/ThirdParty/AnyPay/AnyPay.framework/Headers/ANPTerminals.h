//
//  ANPTerminals.h
//  AnyPay
//
//  Created by Ankit Gupta on 08/03/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#ifndef ANPTerminals_h
#define ANPTerminals_h

typedef NS_ENUM(NSUInteger, ANPTerminals) {
    ANPTerminal_WORLDNET = 0,
    ANPTerminal_ANYWHERECOMMERCE,
    ANPTerminal_DELTA,
    ANPTerminal_WESTJET
};


#define terminalName(enum) [@[@"Worldnet",@"AnywhereCommerce",@"Delta", @"WestJet"] objectAtIndex:enum]

#endif /* ANPTerminals_h */
