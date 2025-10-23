//
//  CEXTransactionLinkType.h
//  CardEaseXmlClient
//
//  Created by Jacob Durbin on 01/12/2020.
//  Copyright © 2020 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

/** @brief A class to represent the type of Transaction Link. **/
typedef enum
{
    /**
     * @brief Indicates that the TransactionLink links to a previous contactless transaction
     * whose response specifies that the transaction should be reran as a contact transaction
     **/
    FallForwardToContactInitiator,
    
    /**
     * @brief Indicates that the TransactionLink links to a previous contactless transaction
     * whose response specifies that the transaction should be reran and sent with a PIN block required
     * for Online PIN verification
     **/
    OnlinePinRequiredInitiator,        
} CEXTransactionLinkType;



@interface CEXTransactionLinkTypeParser : NSObject

+(NSString *)parse:(CEXTransactionLinkType)linkType;
@end
