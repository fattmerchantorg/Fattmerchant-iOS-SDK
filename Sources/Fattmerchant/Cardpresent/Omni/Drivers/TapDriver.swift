//
//  TapDriver.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 10/22/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

import Foundation

/// Protocol defining the interface for Tap to Pay services
@available(iOS 17.4, *)
protocol TapDriver {
    
    static var source: String { get }

    func void(
        transactionResult: TransactionResult,
        completion: @escaping (Bool) -> Void
    )
    
    var tapConnectionStatusDelegate: TapConnectionStatusDelegate? { get set }

    func isReadyToTakePayment(completion: (Bool) -> Void)
    
    func connectToTap(completion: @escaping (Bool, OmniException?) -> Void)
    
    func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, completion: @escaping (TransactionResult) -> Void)

    func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, userNotificationDelegate: UserNotificationDelegate?, completion: @escaping (TransactionResult) -> Void)

    func capture(transaction: Transaction, completion: @escaping (Bool) -> Void)

    func capture(_ transaction: StaxTransaction, completion: @escaping (Bool) -> Void)
}

@available(iOS 17.4, *)
extension TapDriver {
    
    func connectToTap(completion: @escaping (Bool, OmniException?) -> Void) {
        connectToTap(completion: completion)
    }
    
    func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, completion: @escaping (TransactionResult) -> Void) {
      performTransaction(with: request,
                         signatureProvider: signatureProvider,
                         transactionUpdateDelegate: transactionUpdateDelegate,
                         userNotificationDelegate: nil,
                         completion: completion)
    }
    
    func performTransaction(with request: TransactionRequest, signatureProvider: SignatureProviding?, transactionUpdateDelegate: TransactionUpdateDelegate?, userNotificationDelegate: UserNotificationDelegate?, completion: @escaping (TransactionResult) -> Void) {
      performTransaction(with: request,
                         signatureProvider: signatureProvider,
                         transactionUpdateDelegate: transactionUpdateDelegate,
                         userNotificationDelegate: userNotificationDelegate,
                         completion: completion)
    }

    func capture(transaction: Transaction, completion: @escaping (Bool) -> Void)
    {
        print("TapDriver#capture not implemented")
        completion(true)
    }

    func capture(
        _ transaction: StaxTransaction,
        completion: @escaping (Bool) -> Void
    ) {
        print("TapDriver#capture not implemented")
        completion(true)
    }

    func void(
        transactionResult: TransactionResult,
        completion: @escaping (Bool) -> Void
    ) {
        print("TapDriver#void not implemented")
        completion(true)
    }

}
