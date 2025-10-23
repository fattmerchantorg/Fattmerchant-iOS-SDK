//
//  TakeTapPaymentException.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 10/22/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

/// Exception type for Tap to Pay payments
enum TakeTapPaymentException: OmniException {

    case iOSVersionNotSupported
    case invoiceIdCannotBeBlank
    case invoiceNotFound
    case couldNotCreateInvoice(detail: String?)
    case couldNotCreateCustomer(detail: String?)
    case couldNotCreatePaymentMethod(detail: String?)
    case couldNotUpdateInvoice(detail: String?)
    case couldNotCreateTransaction(detail: String?)
    case couldNotCaptureTransaction

    static var mess = "Error processing tap payment"

    var localizedDescription: String {
        switch self {
        case .iOSVersionNotSupported:
            return
                "Tap to Pay on iPhone requires iOS 17.4 or later. Please update your device to use this feature."
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
        }
    }
}
