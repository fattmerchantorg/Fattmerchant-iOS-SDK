/**
 * @author Creditcall Ltd
 * @brief Parameters used in communication with ChipDNA Mobile.
 * @class CardApplication
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 */

#import <Foundation/Foundation.h>

@interface CardApplication : NSObject
/**
 * Gets the preferred name of the card application.
 *
 * @return The application name.
 */
@property (nonatomic, readonly)NSString *name;

/**
 * Gets the priority index of the card application.
 *
 * @return The priority index.
 */
@property (nonatomic, readonly)int priority;

@end
