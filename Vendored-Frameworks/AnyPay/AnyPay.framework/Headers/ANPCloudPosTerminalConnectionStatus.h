//
//  CloudPosTerminalConnectionStatus.h
//  AnyPay
//
//  Created by Ankit Gupta on 01/09/20.
//  Copyright © 2020 Dan McCann. All rights reserved.
//

#ifndef ANPCloudPosTerminalConnectionStatus_h
#define ANPCloudPosTerminalConnectionStatus_h

typedef NS_ENUM(NSUInteger, ANPCloudPosTerminalConnectionStatus) {
    CloudPosTerminalConnection_DISCONNECTED = 0,
    CloudPosTerminalConnection_CONNECTING,
    CloudPosTerminalConnection_CONNECTED,
    CloudPosTerminalConnection_DISCONNECTING,
    CloudPosTerminalConnection_RECONNECTING
};

#endif /* ANPCloudPosTerminalConnectionStatus_h */
