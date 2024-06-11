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
    /// Holds details for NMI settings set in the mobile reader tab
    internal var nmi: NMIDetails?
}
