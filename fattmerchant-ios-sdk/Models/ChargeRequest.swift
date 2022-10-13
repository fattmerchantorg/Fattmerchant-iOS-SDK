//
//  ChargeRequest.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 4/17/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// A request to perform a Charge on Omni
internal struct ChargeRequest: Codable {
    /// The payment method to charge
    var paymentMethodId: String

    /// The total to charge
    var total: String

    /// True when the charge should be a pre-auth operation
    var preAuth: Bool = false

    /// Contains metadata about the charge, such as subtotal and tax
    var meta: [String: String]
}

extension Data {
    /// Initializes a `Data` with a `ChargeRequest`
    ///
    /// - Note: The need for this is primarily to convert the keys to snake case
    init?(chargeRequest: ChargeRequest) {
        let encoder = JSONEncoder()
        encoder.keyEncodingStrategy = .convertToSnakeCase
        do {
            self = try encoder.encode(chargeRequest)
        } catch {
            return nil
        }
    }
}
