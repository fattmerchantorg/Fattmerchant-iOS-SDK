//
//  ANPCardReaderConnectionMethods.h
//  AnyPay
//
//  Created by Ankit Gupta on 17/01/18.
//  Copyright © 2018 Dan McCann. All rights reserved.
//

#ifndef ANPCardReaderConnectionMethods_h
#define ANPCardReaderConnectionMethods_h



typedef NS_ENUM(NSInteger, ANPCardReaderConnectionMethod) {
    ANPCardReaderConnectionMethodUnspecified = 0,
    ANPCardReaderConnectionMethodAudio = 1,
    ANPCardReaderConnectionMethodBluetooth,
    ANPCardReaderConnectionMethodUSB,
    ANPCardReaderConnectionMethodWIFI
};

#endif /* ANPCardReaderConnectionMethods_h */

