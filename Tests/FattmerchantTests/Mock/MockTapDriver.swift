//
//  MockTapDriver.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 10/23/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

import Foundation

class MockTapDriver: TapDriver {
    var tapConnectionStatusDelegate: (any TapConnectionStatusDelegate)?

    func isReadyToTakePayment(completion: (Bool) -> Void) {
    }

    static var source: String = "NMI"

    var isAvailable: Bool = true
    var isRefundSupported: Bool = true
    var lastTransactionRequest: TransactionRequest?

    var mockTransactionResult = TransactionResult()

    init() {
        setupDefaultMockResult()
    }

    private func setupDefaultMockResult() {
        mockTransactionResult.success = true
        mockTransactionResult.externalId = "mock-external-id"
        mockTransactionResult.maskedPan = "4111111111111111"
        mockTransactionResult.cardType = "VISA"
        mockTransactionResult.cardExpiration = "1225"
        mockTransactionResult.authCode = "123456"
        mockTransactionResult.source = "nmi"
        mockTransactionResult.cardHolderFirstName = "Tap to Pay"
        mockTransactionResult.cardHolderLastName = "Customer"
    }

    func startPaymentSession(
        with request: TransactionRequest,
        signatureProvider: SignatureProviding?,
        transactionUpdateDelegate: TransactionUpdateDelegate?,
        userNotificationDelegate: UserNotificationDelegate?,
        completion: @escaping (TransactionResult) -> Void
    ) {
        lastTransactionRequest = request
        mockTransactionResult.request = request
        completion(mockTransactionResult)
    }

    func void(
        transactionResult: TransactionResult,
        completion: @escaping (Bool) -> Void
    ) {
        completion(true)
    }

    func capture(transaction: Transaction, completion: @escaping (Bool) -> Void)
    {
        completion(true)
    }
}
