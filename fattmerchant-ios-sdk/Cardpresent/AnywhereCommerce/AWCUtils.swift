//
//  AWCUtils.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 5/4/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import AnyPay

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
      self.amount = Amount(amount)
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
    if let cardHolderName = anyPayTransaction.cardHolderName{
      var name = cardHolderName.split(separator: " ")
      if name.count > 0 {
        self.cardHolderFirstName = String(name.removeFirst())

        if name.count > 1 {
          self.cardHolderLastName = name.joined(separator: " ")
        }
      }
    }
    self.success = anyPayTransaction.status == .APPROVED
    self.transactionType = TransactionType(anyPayTransaction.transactionType).rawValue
  }
}

extension Transaction {
  func awcExternalId() -> String? {
    let g = ANPGatewayResponse(response: meta?["awcTransactionId"])
    return meta?["awcTransactionId"]
  }
}

#endif
