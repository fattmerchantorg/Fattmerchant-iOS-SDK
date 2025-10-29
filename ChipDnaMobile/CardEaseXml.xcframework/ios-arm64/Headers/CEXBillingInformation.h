//
//  CEXBillingInformation.h
//  CardEaseXmlClient
//
//  Created by Mariya Marinova on 05/04/2019.
//  Copyright © 2019 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CEXBillingInformation : NSObject

/**
 * @brief The first line of the address.
 * @property address1
 **/
@property NSString * line1;

/**
 * @brief The second line of the address.
 * @property address2
 **/
@property NSString * line2;

/**
 * @brief The city in the address.
 * @property city
 **/
@property NSString * city;

/**
 * @brief The state in the address.
 * @property state
 **/
@property NSString * state;

/**
 * @brief The zip code in the address.
 * @property zipCode
 **/
@property NSString * zipCode;

/**
 * @brief The country in the address.
 * @property country
 **/
@property NSString * country;

/**
 * @brief The email address in the billing information.
 * @property emailAddress
 **/
@property NSString * emailAddress;

/**
 * @brief The phone number in the billing information.
 * @property telephoneNumber
 **/
@property NSString * telephoneNumber;

/**
 * @brief Creates a new billing information object
 *
 * @param theAddress1 The first line of the address.
 * @param theAddress2 The second line of the address.
 * @param theCity The city in the address.
 * @param theState The state in the address.
 * @param theZipCode The zip code in the address.
 * @param theCountry The country in the address.
 * @param emailAddress The email address in the billing information.
 * @param telephoneNumber The phone number in the billing information.
 *
 * @return An initialised CEXBillingInformation instance.
 **/
-(CEXBillingInformation*) init : (NSString*) theLine1 : (NSString*) theLine2 : (NSString*) theCity : (NSString*) theState : (NSString*) theZipCode : (NSString*) theCountry : (NSString*) theEmailAddress : (NSString*) theTelephoneNumber;

@end
