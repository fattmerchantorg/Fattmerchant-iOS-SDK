/**
 * @author Creditcall Ltd
 * @copyright Copyright Creditcall Limited 2010 - 2018.
 * @file TransactionUpdates.h
 */

#import <Foundation/Foundation.h>

/**
 * @typedef TransactionUpdate
 *
 * The possible updates that can occur during a standalone (sale or refund) transaction process.
 */
typedef NSString* TransactionUpdate;
extern TransactionUpdate const TransactionUpdateStartingTransaction;                 /**< Indicates the transaction has started. */
extern TransactionUpdate const TransactionUpdateOnlineAuthorisation;                 /**< Indicates the transaction is going online for authorisation. */
extern TransactionUpdate const TransactionUpdateCardEntryPrompted;                   /**< Indicates card entry has been prompted on the PIN pad. */
extern TransactionUpdate const TransactionUpdateSmartcardInserted;                   /**< Indicates a card has been inserted into the PIN pad. */
extern TransactionUpdate const TransactionUpdateSmartcardRemovePrompted;             /**< Indicates card removal has been prompted on the PIN pad. */
extern TransactionUpdate const TransactionUpdateSmartcardRemoved;                    /**< Indicates the card has been removed from the PIN pad. */
extern TransactionUpdate const TransactionUpdateCardEntryBypassed;                   /**< Indicates card entry on the PIN pad has been bypassed. */
extern TransactionUpdate const TransactionUpdateCardEntryTimedOut;                   /**< Indicates card entry on the PIN pad has timed out. */
extern TransactionUpdate const TransactionUpdateCardEntryAborted;                    /**< Indicates card entry on the PIN pad has been aborted. */
extern TransactionUpdate const TransactionUpdateCardSwiped;                          /**< Indicates a card has been swiped on the PIN pad. */
extern TransactionUpdate const TransactionUpdateCardSwipeError;                      /**< Indicates an error occurred when swiping a card on the PIN pad. */
extern TransactionUpdate const TransactionUpdateCardTapped;                          /**< Indicates a card has been tapped on the PIN pad. */
extern TransactionUpdate const TransactionUpdateApplicationSelectionStarted;         /**< Indicates application selection on the PIN pad has been started. */
extern TransactionUpdate const TransactionUpdateApplicationSelectionCompleted;       /**< Indicates application selection on the PIN pad has been completed. */
extern TransactionUpdate const TransactionUpdatePinEntryStarted;                     /**< Indicates PIN entry on the PIN pad has been started. */
extern TransactionUpdate const TransactionUpdatePinEntryCompleted;                   /**< Indicates PIN entry on the PIN pad has been completed. */
extern TransactionUpdate const TransactionUpdatePinEntryAborted;                     /**< Indicates PIN entry on the PIN pad has been aborted. */
extern TransactionUpdate const TransactionUpdatePinEntryBypassed;                    /**< Indicates PIN entry on the PIN pad has been bypassed. */
extern TransactionUpdate const TransactionUpdatePinEntryTimedOut;                    /**< Indicates PIN entry on the PIN pad has timed out. */
extern TransactionUpdate const TransactionUpdatePinEntryIncorrect;                   /**< Indicates PIN entry on the PIN pad was incorrect. */
extern TransactionUpdate const TransactionUpdateLastPinEntry;                        /**< Indicates PIN entry on the PIN pad was the last one. */
extern TransactionUpdate const TransactionUpdateAmountConfirmationStarted;           /**< Indicates amount confirmation on the PIN pad has been started. */
extern TransactionUpdate const TransactionUpdateAmountConfirmationCompleted;         /**< Indicates amount confirmation on the PIN pad has been completed. */
extern TransactionUpdate const TransactionUpdateAmountConfirmationAborted;           /**< Indicates amount confirmation on the PIN pad has been aborted. */
extern TransactionUpdate const TransactionUpdateAmountConfirmationBypassed;          /**< Indicates amount confirmation on the PIN pad has been bypassed. */
extern TransactionUpdate const TransactionUpdateAmountConfirmationTimedOut;          /**< Indicates amount confirmation on the PIN pad has timed out. */
extern TransactionUpdate const TransactionUpdateSignatureVerificationStarted;        /**< Indicates signature verification started. */
extern TransactionUpdate const TransactionUpdateSignatureVerificationCompleted;      /**< Indicates signature verification completed.*/
extern TransactionUpdate const TransactionUpdateVoiceReferralStarted;                /**< Indicates voice referral started. */
extern TransactionUpdate const TransactionUpdateVoiceReferralCompleted;              /**< Indicates voice referral completed. */
extern TransactionUpdate const TransactionUpdateGratuityStarted;                     /**< Indicates gratuity started.*/
extern TransactionUpdate const TransactionUpdateGratuityCompleted;                   /**< Indicates gratuity completed.*/
extern TransactionUpdate const TransactionUpdateIdVerificationStarted;               /**< Indicates ID verification started. */
extern TransactionUpdate const TransactionUpdateIdVerificationCompleted;             /**< Indicates ID verification completed.*/
extern TransactionUpdate const TransactionUpdateForcedAcceptanceStarted;             /**< Indicates forced acceptance started.*/
extern TransactionUpdate const TransactionUpdateForcedAcceptanceCompleted;           /**< Indicates forced acceptance completed.*/
extern TransactionUpdate const TransactionUpdatePartialApprovalConfirmationStarted;  /**< Indicates partial approval confirmation started.*/
extern TransactionUpdate const TransactionUpdatePartialApprovalConfirmationCompleted;/**< Indicates partial approval confirmation completed.*/
extern TransactionUpdate const TransactionUpdateGetCardDetailsStarted;               /**< Indicates get card details started.*/
extern TransactionUpdate const TransactionUpdateGetCardDetailsCompleted;             /**< Indicates get card details completed.*/
extern TransactionUpdate const TransactionUpdateDeferredAuthorizationStarted;        /**< Indicates deferred authorization started. */
extern TransactionUpdate const TransactionUpdateDeferredAuthorizationCompleted;      /**< Indicates deferred authorization completed. */
extern TransactionUpdate const TransactionUpdatePANKeyEntryStarted;                  /**< Indicates PAN key entry has started. */
extern TransactionUpdate const TransactionUpdatePANKeyEntryCompleted;                /**< Indicates PAN key entry has completed. */
extern TransactionUpdate const TransactionUpdateSignatureCaptureStarted;             /**< Indicates signature capture started. */
extern TransactionUpdate const TransactionUpdateSignatureCaptureComplete;            /**< Indicates signature capture finished. */
