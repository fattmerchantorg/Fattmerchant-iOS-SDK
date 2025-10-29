//
//  CEXPurchaseLevel2Data.h
//  CardEaseXmlClient
//
//  Created by Mariya Marinova on 09/04/2019.
//  Copyright © 2019 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CEXPurchaseLevel2Data : NSObject <NSCoding>

/**
 * @brief  The PO number.
 * @property PONumber
 **/
@property NSString * PONumber;

/**
 * @brief  The tax amount.
 * @property taxAmount
 **/
@property NSString * taxAmount;

/**
 * @brief Creates a new purchase level 2 data object
 *
 * @param thePONumber The PO number in the purchase level 2 data.
 * @param theTaxAmount The tax amount in the purchase level 2 data.
 *
 * @return An initialised CEXPurchaseLevel2Data instance.
 **/
-(CEXPurchaseLevel2Data*) init : (NSString*) thePONumber : (NSString*) theTaxAmount;

@end
