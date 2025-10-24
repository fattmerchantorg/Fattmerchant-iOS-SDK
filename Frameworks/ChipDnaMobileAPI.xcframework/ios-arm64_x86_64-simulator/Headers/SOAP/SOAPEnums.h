/**
 * @author Creditcall Ltd
 * @copyright Copyright Creditcall Limited 2010 - 2017.
 * @file SOAPEnums.h
 */

#import <Foundation/Foundation.h>

/**
 * @typedef CardSchemeID
 *
 * The possible values returned for CardSchemeID.
 */
typedef enum
{
    CardSchemeIDVisa                    = 1,    /**< (1)     Visa */
    CardSchemeIDMasterCard              = 2,    /**< (2)     MasterCard */
    CardSchemeIDMaestro                 = 3,    /**< (3)     Maestro */
    CardSchemeIDAmex                    = 4,    /**< (4)     Amex */
    CardSchemeIDJCB                     = 5,    /**< (5)     JCB */
    CardSchemeIDDiners                  = 6,    /**< (6)     Diners */
    CardSchemeIDDiscover                = 7,    /**< (7)     Discover */
    CardSchemeIDCash                    = 9401, /**< (9401)  Cash */
    CardSchemeIDCheque                  = 9402, /**< (9402)  Cheque */
} CardSchemeID;

/**
 * @typedef SettlementStateID
 *
 * The possible values returned for SettlementStateID.
 */
typedef enum
{
    SettlementStateIDNotSettledUnknown = 0,    /**< (0) Not Settled / Unknown */
    SettlementStateIDSettled           = 1,    /**< (1) Settled */
} SettlementStateID;

/**
 * @typedef StateID
 *
 * The possible values returned for StateID.
 */
typedef enum
{
    StateIDUncommitted                        = 0,    /**< (0)    Uncommitted */
    StateIDPartialCommit                      = 1,    /**< (1)    Partial Commit */
    StateIDCommitted                          = 2,    /**< (2)    Committed */
    StateIDRolledBack                         = 3,    /**< (3)    Rolled Back */
    StateIDVoided                             = 4,    /**< (4)    Voided */
    StateIDConfirmedManHold                   = 5,    /**< (5)    Confirmed / Man Hold */
    StateIDPotentialDuplicateTransactionHeld  = 6,    /**< (6)    Potential Duplicate / Transaction Held */
} StateID;

/**
 * @typedef CardTypeID
 *
 * The possible values returned for CardTypeID.
 */
typedef enum
{
    CardTypeIDUnknown                = 0,   /**< (0)   Unknown */
    CardTypeIDCredit                 = 1,   /**< (1)   Credit */
    CardTypeIDCreditRestricted       = 2,   /**< (2)   Credit (Restricted) */
    CardTypeIDDebit                  = 10,  /**< (10)  Debit */
    CardTypeIDDebitRestricted        = 11,  /**< (11)  Debit (Restricted) */
    CardTypeIDPurchasing             = 100, /**< (100) Purchasing */
    CardTypeIDCorporate              = 110, /**< (110) Corporate */
    CardTypeIDCorporatePurchasing    = 111, /**< (111) Corporate Purchasing */
    CardTypeIDFleet                  = 120, /**< (120) Fleet */
    CardTypeIDCommercial             = 130, /**< (130) Commercial */
    CardTypeIDConsumer               = 140, /**< (140) Consumer */
    CardTypeIDBusinessCredit         = 150, /**< (150) Business Credit */
    CardTypeIDBusinessDebit          = 151, /**< (151) Business Debit */
    CardTypeIDElectronic             = 200, /**< (200) Electronic */
    CardTypeIDElectronicBusinessCard = 201, /**< (201) Electronic Business Card */
    CardTypeIDStoredValue            = 300, /**< (300) Stored Value */
} CardTypeID;

/**
 * @typedef ResultID
 *
 * The possible values returned for ResultID.
 */
typedef enum
{
    ResultIDApproved                = 0,  /**< (0)  Approved */
    ResultIDDeclined                = 1,  /**< (1)  Declined */
    ResultIDVoiceReferral           = 2,  /**< (2)  Voice Referral  */
    ResultIDPartialApproval         = 10, /**< (10)  Partial Approval  */
} ResultID;

/**
 * @typedef TransactionTypeID
 *
 * The possible values returned for TransactionTypeID.
 */
typedef enum
{
    TransactionTypeIDAuthorisationCredit         = 0,     /**< (0)     Authorisation [Credit] */
    TransactionTypeIDRefundDebit                 = 1,     /**< (1)     Refund [Debit] */
    TransactionTypeIDPreAuthNeither              = 3,     /**< (3)     PreAuth [Neither] */
    TransactionTypeIDOfflineAuthorisationCredit  = 4,     /**< (4)     Offline Authorisation [Credit]  */
    TransactionTypeIDVoiceReferralCredit         = 5,     /**< (5)     Voice Referral [Credit] */
    TransactionTypeIDStandaloneRefundDebit       = 6,     /**< (6)     Standalone Refund [Debit] */
    TransactionTypeIDOfflineRefundDebit          = 7,     /**< (7)     Offline Refund [Debit] */
} TransactionTypeID;
