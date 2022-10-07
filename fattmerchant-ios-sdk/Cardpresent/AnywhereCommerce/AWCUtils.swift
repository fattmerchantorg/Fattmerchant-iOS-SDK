//
//  AWCUtils.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 5/4/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//
import AnyPay
import Foundation

#if targetEnvironment(simulator)

#else

extension MobileReader {

    static func from(btDevice: ANPBluetoothDevice) -> MobileReader {
        guard let bbposDevice = btDevice as? ANPBBPOSDevice else {
            fatalError()
        }

        return from(anyPayCardReader: bbposDevice)
    }

    static func from(anyPayCardReader: AnyPayCardReader) -> MobileReader {
        return MobileReader(name: anyPayCardReader.name(),
                            firmwareVersion: anyPayCardReader.firmwareVersion,
                            model: (anyPayCardReader as ANPPeripheralDevice).productID,
                            serialNumber: anyPayCardReader.serialNumber)
    }

}

extension TransactionUpdate {
    /// Makes an Omni TransactionUpdate from a ChipDna TransactionUpdate string
    init?(anpMeaningfulMessage: ANPMeaningfulMessage) {
        guard let message = anpMeaningfulMessage.message else {
            return nil
        }

        if message == "SWIPE OR INSERT OR TAP" {
            self = .PromptInsertSwipeTap
        } else if message == "SWIPE OR INSERT" {
            self = .PromptInsertSwipeCard
        } else if message == "Processing" {
            self = .Authorizing
        } else if message == "Remove Card" {
            self = .PromptRemoveCard
        } else if message == "Insert Card" {
            self = .PromptInsertCard
        } else {
            return nil
        }
    }
}

extension Amount {
    init(_ anyPayAmount: ANPAmount) {
        self.cents = anyPayAmount.decimalValue.multiplying(by: 100).intValue
    }
}

extension TransactionType {
    init(_ anyPayTransactionType: ANPTransactionType) {
        switch anyPayTransactionType {
        case .SALE:
            self = .sale

        case .REFUND:
            self = .refund

        default:
            self = .sale
        }
    }
}

extension TransactionResult {
    init(_ anyPayTransaction: AnyPayTransaction) {
        if anyPayTransaction.status == .DECLINED {
            // Add the responseText, since this usually contains info about why the transaction was declined
            if let responseText = anyPayTransaction.responseText {
                self.message = responseText
            }
        }

        self.source = AWCDriver.source

        if let amount = anyPayTransaction.approvedAmount {
            // AWC seems to return a negative approvedAmount for REFUNDS, so we have to take the absolute value
            // This is because this amount gets assigned to the "total_refunded" field in Omni sees the negative
            // and that value is not negative
            if anyPayTransaction.transactionType == .REFUND {
                let amt = Amount(amount)
                let correctedAmount = Amount(cents: abs(amt.cents))
                self.amount = correctedAmount
            } else {
                self.amount = Amount(amount)
            }
        }
        if let refTransactionId = anyPayTransaction.refTransactionID {
            self.externalId = refTransactionId
        }
        if let maskedPan = anyPayTransaction.maskedPAN {
            self.maskedPan = maskedPan
        }
        if let cardType = anyPayTransaction.cardType {
            self.cardType = cardType
        }
        if let gatewayResponse = anyPayTransaction.gatewayResponse, let gatewayJsonString = gatewayResponse.toJSONString() {
            self.gatewayResponse = gatewayJsonString
        }
        if let approvalCode = anyPayTransaction.approvalCode {
            self.authCode = approvalCode
        }
        if let cardHolderName = anyPayTransaction.cardHolderName, !cardHolderName.isEmpty {
            var name = cardHolderName.split(separator: " ")
            if !name.isEmpty {
                self.cardHolderFirstName = String(name.removeFirst())
                self.cardHolderLastName = name.joined(separator: " ")
            }
        }
        self.success = anyPayTransaction.status == .APPROVED
        self.transactionType = TransactionType(anyPayTransaction.transactionType).rawValue

        self.transactionSource = nil
    }
}

extension Transaction {
    func awcExternalId() -> String? {
        _ = ANPGatewayResponse(response: meta?["awcTransactionId"])
        return meta?["awcTransactionId"]
    }
}

#endif
