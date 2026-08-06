//
//  MockTapDriver.swift
//  fattmerchant-ios-sdk
//
//  Copyright © 2026 Fattmerchant. All rights reserved.
//

import Foundation

/// Compile-only mock for `TapDriver`, used on simulator builds so the SDK
/// continues to compile when ProximityReader is unavailable. The iOS
/// simulator cannot execute Tap to Pay on iPhone, so these methods are
/// never actually invoked — they exist only to satisfy the protocol.
@available(iOS 17.4, *)
class MockTapDriver: TapDriver {

    static var source: String = "MOCKTAPSOURCE"

    weak var tapConnectionStatusDelegate: TapConnectionStatusDelegate?

    func isReadyToTakePayment(completion: (Bool) -> Void) {
        completion(true)
    }

    func connectToTap(completion: @escaping (Bool, OmniException?) -> Void) {
        completion(true, nil)
    }

    func performTransaction(
        with request: TransactionRequest,
        signatureProvider: SignatureProviding?,
        transactionUpdateDelegate: TransactionUpdateDelegate?,
        userNotificationDelegate: UserNotificationDelegate?,
        completion: @escaping (TransactionResult) -> Void
    ) {
        fatalError("MockTapDriver.performTransaction is not supported on simulator")
    }
}
