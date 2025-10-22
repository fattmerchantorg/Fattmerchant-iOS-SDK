//
//  TapDriver.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 10/22/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

import Foundation

/// Protocol defining the interface for Tap to Pay services
protocol TapDriver {
    /// Whether Tap to Pay is available on this device
    var isAvailable: Bool { get }
    
    /// Whether refunds are supported for Tap to Pay transactions
    var isRefundSupported: Bool { get }
    
    /// Starts a payment session using Tap to Pay
    func startPaymentSession(
        with request: TransactionRequest,
        signatureProvider: SignatureProviding?,
        transactionUpdateDelegate: TransactionUpdateDelegate?,
        userNotificationDelegate: UserNotificationDelegate?,
        completion: @escaping (TransactionResult) -> Void
    )
    
    /// Voids a transaction
    func void(transactionResult: TransactionResult, completion: @escaping (Bool) -> Void)
    
    /// Captures a transaction
    func capture(transaction: Transaction, completion: @escaping (Bool) -> Void)
}
