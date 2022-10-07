//
//  Endpoints.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 1/17/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

protocol OmniEndpoint {
    static func resourceEndpoint() -> String
}

extension Merchant: OmniEndpoint {
    static func resourceEndpoint() -> String {
        return "/merchant"
    }
}

extension Invoice: OmniEndpoint {
    static func resourceEndpoint() -> String {
        return "/invoice"
    }
}

extension Transaction: OmniEndpoint {
    static func resourceEndpoint() -> String {
        return "/transaction"
    }
}

extension Customer: OmniEndpoint {
    static func resourceEndpoint() -> String {
        return "/customer"
    }
}

extension PaymentMethod: OmniEndpoint {
    static func resourceEndpoint() -> String {
        return "/payment-method"
    }
}

extension MobileReaderDetails: OmniEndpoint {
    static func resourceEndpoint() -> String {
        return "/team/gateway/hardware/mobile"
    }
}
