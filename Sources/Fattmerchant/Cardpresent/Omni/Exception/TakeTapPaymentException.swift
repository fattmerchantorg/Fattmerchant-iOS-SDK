//
//  TakeTapPaymentException.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 10/22/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

/// Exception type for Tap to Pay payments
@available(iOS 17.4, *)
enum TakeTapPaymentException: OmniException {

    case invoiceIdCannotBeBlank
    case invoiceNotFound
    case couldNotCreateInvoice(detail: String?)
    case couldNotCreateCustomer(detail: String?)
    case couldNotCreatePaymentMethod(detail: String?)
    case couldNotUpdateInvoice(detail: String?)
    case couldNotCreateTransaction(detail: String?)
    case couldNotCaptureTransaction
    
    // Start Transaction errors
    case transactionPOINotConnected
    case transactionPOIInvalid
    case autoConfirmRequired
    case tipAmountInvalid
    case tipAmountNotAllowed
    case merchantTippingNotSupported
    
    // Transaction Finished event errors
    case tapToMobileTransactionTerminated
    case tapToMobileSessionClosed

    static var mess = "Error processing tap payment"

    var localizedDescription: String {
        switch self {
        case .invoiceIdCannotBeBlank:
            return "Invoice ID cannot be blank"
        case .invoiceNotFound:
            return "Invoice not found"
        case .couldNotCreateInvoice(let detail):
            return "Could not create invoice: \(detail ?? "")"
        case .couldNotCreateCustomer(let detail):
            return "Could not create customer: \(detail ?? "")"
        case .couldNotCreatePaymentMethod(let detail):
            return "Could not create payment method: \(detail ?? "")"
        case .couldNotUpdateInvoice(let detail):
            return "Could not update invoice: \(detail ?? "")"
        case .couldNotCreateTransaction(let detail):
            return "Could not create transaction: \(detail ?? "")"
        case .couldNotCaptureTransaction:
            return "Could not capture transaction"
        case .transactionPOINotConnected:
            return "Tap to Pay is not connected"
        case .transactionPOIInvalid:
            return "Invalid transaction point of interaction"
        case .autoConfirmRequired:
            return "Auto-confirmation is required for this transaction"
        case .tipAmountInvalid:
            return "Invalid tip amount format"
        case .tipAmountNotAllowed:
            return "Merchant tipping is not supported for this device"
        case .merchantTippingNotSupported:
            return "Tipping is not supported with the configured processor"
        case .tapToMobileTransactionTerminated:
            return "Transaction was terminated by Tap to Pay"
        case .tapToMobileSessionClosed:
            return "Tap to Pay session is no longer available"
        }
    }
}
