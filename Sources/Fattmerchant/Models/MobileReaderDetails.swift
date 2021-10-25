//
//  MobileReaderSettings.swift
//  Fattmerchant
//
//  Created by Hassan Nazari on 7/27/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Mobile reader settings defined in the mobile reader tab
class MobileReaderDetails: Model, Codable {
    public var id: String?
    /// Holds details for AnywhereCommerce settings set in the mobile reader tab
    internal var anywhereCommerce: AWCDetails?
    /// Holds details for NMI settings set in the mobile reader tab
    internal var nmi: NMIDetails?
}

/// Details for AnywhereCommerce settings set in the mobile reader tab
internal struct AWCDetails: Codable {
    /// AnywhereCommerce terminal id
    var terminalId: String
    /// AnywhereCommerce secret
    var terminalSecret: String
}

/// Details for NMI (ChipDNA) settings set in the mobile reader tab
internal struct NMIDetails: Codable {
    /// NMI (ChipDNA) security key
    var securityKey: String
}
