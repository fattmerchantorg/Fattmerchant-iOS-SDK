//
//  Endpoints.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 1/17/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

protocol StaxEndpoint {
    static func resourceEndpoint() -> String
}

extension Merchant: StaxEndpoint {
    static func resourceEndpoint() -> String {
        return "/merchant"
    }
}

extension Invoice: StaxEndpoint {
    static func resourceEndpoint() -> String {
        return "/invoice"
    }
}

extension Transaction: StaxEndpoint {
    static func resourceEndpoint() -> String {
        return "/transaction"
    }
}

extension Customer: StaxEndpoint {
    static func resourceEndpoint() -> String {
        return "/customer"
    }
}

extension PaymentMethod: StaxEndpoint {
    static func resourceEndpoint() -> String {
        return "/payment-method"
    }
}

extension MobileReaderDetails: StaxEndpoint {
    static func resourceEndpoint() -> String {
        return "/team/gateway/hardware/mobile"
    }
}
