//
//  AWCUtils.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 5/4/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import AnyPay

extension MobileReader {

  static func from(btDevice: ANPBluetoothDevice) -> MobileReader {
    return MobileReader(name: (btDevice as? ANPBBPOSDevice)?.name() ?? "Name not found")
  }

  static func from(anyPayCardReader: AnyPayCardReader) -> MobileReader {
    return MobileReader(name: anyPayCardReader.serialNumber,
                        serialNumber: anyPayCardReader.serialNumber)
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
    self.amount = Amount(anyPayTransaction.approvedAmount)
    self.source = AWCDriver.source
    self.cardType = anyPayTransaction.cardType
    self.externalId = anyPayTransaction.externalID
    self.maskedPan = anyPayTransaction.cardNumber
    self.transactionType = TransactionType(anyPayTransaction.transactionType).rawValue
    self.success = anyPayTransaction.status == .APPROVED
    self.gatewayResponse = anyPayTransaction.gatewayResponse.toJSONString()
    self.authCode = anyPayTransaction.approvalCode

    var name = anyPayTransaction.cardHolderName.split(separator: " ")
    self.cardHolderFirstName = String(name.removeFirst())
    self.cardHolderLastName = name.joined(separator: " ")
  }
}

extension Transaction {
  func awcExternalId() -> String? {
    let g = ANPGatewayResponse(response: meta?["awcTransactionId"])
    return meta?["awcTransactionId"]
  }
}
