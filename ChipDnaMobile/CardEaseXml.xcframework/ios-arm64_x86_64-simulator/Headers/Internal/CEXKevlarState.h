//
//  CEXKevlarState.h
//  CardEaseXmlClient
//
//  Created by Sam Sutton on 27/02/2017.
//  Copyright © 2017 CreditCall Ltd. All rights reserved.
//

typedef enum {
    CEXKevlarStateNone,
    CEXKevlarStateTokenRequestSent,
    CEXKevlarStateTokenRequestComplete,
    CEXKevlarStateTransactionRequestSent,
    CEXKevlarStateTransactionResponseReceived,
    CEXKevlarStateResponseNotificationSent,
    CEXKevlarStateResponseNotificationComplete,
} CEXKevlarState;
