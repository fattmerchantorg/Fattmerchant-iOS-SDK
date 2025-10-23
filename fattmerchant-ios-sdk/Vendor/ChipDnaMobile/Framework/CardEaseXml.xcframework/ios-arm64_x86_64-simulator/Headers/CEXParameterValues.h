//
//  CEXParameterValues.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CEXParameterValues : NSObject

/**
 * @brief Parameter value representation of True.
 **/
extern NSString * const CEXVALUETrue;

/**
 * @brief Parameter value representation of False.
 **/
extern NSString * const CEXVALUEFalse;

/**
 * @brief Parameter value representation of the Attended Type Attended.
 **/
extern NSString * const CEXVALUEAttendedTypeAttended;

/**
 * @brief Parameter value representation of the Attended Type Unattended
 **/
extern NSString * const CEXVALUEAttendedTypeUnattended;

/**
 * @brief Parameter value representation of the Device Type Merchant.
 **/
extern NSString * const CEXVALUEDeviceTypeMerchant;

/**
 * @brief Parameter value representation of the Device Type Consumer.
 **/
extern NSString * const CEXVALUEDeviceTypeConsumer;

/**
 * @brief Parameter value representation of the Premises Type On Premises.
 **/
extern NSString * const CEXVALUEPremisesTypeOnPremises;

/**
 * @brief Parameter value representation of the Premises Type Off Premises.
 **/
extern NSString * const CEXVALUEPremisesTypeOffPremises;

@end

NS_ASSUME_NONNULL_END
