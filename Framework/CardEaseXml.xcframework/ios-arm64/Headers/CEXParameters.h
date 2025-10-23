//
//  CEXParameters.h
//  CardEaseXmlClient
//
//  Created by William Jones on 30/06/2021.
//  Copyright © 2021 CreditCall Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CEXParameterValues.h"

NS_ASSUME_NONNULL_BEGIN

@interface CEXParameters : NSObject

/**
 * @brief Adds the value with the specified key.
 *
 * @param key
 *            Key.
 * @param value
 *            Value.
 */
-(void) addValue:(NSString *) value forKey:(NSString *) key;

/**
 * @brief Adds the value with the specified key.
 *
 * @param key
 *            Key.
 * @param value
 *            Value.
 */
-(void) addIntValue:(NSInteger *) value forKey:(NSString *) key;

/**
 * @brief Adds the values with the specified key.
 *
 * @param key
 *            Key.
 * @param values
 *            Values.
 */

-(void) addBoolValue:(BOOL *) value forKey:(NSString *) key;

/**
 * @brief Removes the value with the specified key.
 *
 * @param key
 *            Key.
 */
-(void) remove:(NSString *) key;

/**
 * @brief The number of items held.
 *
 * @return Size of the data structure.
 */
-(NSUInteger) count;

/**
 * @brief Determines wheter the {@link CEXParameters} contains any items.
 *
 * @return true if empty
 */
-(BOOL) isEmpty;

/**
 * @brief Determines whether the {@link CEXParameters} contains the specified key.
 *
 * @param key
 *            Key value.
 * @return true if the key is held within the data structure
 */
-(BOOL) containsKey:(NSString *) key;

/**
 * @brief Gets the value associated with the specified key {@link CEXParameterKeys} for possible key values.
 *
 * @param key
 *            Value
 * @return String of the value if contained within {@link Parameters}, else nil.
 */
-(NSString *) getValue:(NSString *) key;

@end

NS_ASSUME_NONNULL_END
