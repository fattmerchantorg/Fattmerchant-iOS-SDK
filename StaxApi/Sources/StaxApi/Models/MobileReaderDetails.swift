import Foundation

class MobileReaderDetails: Codable {
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
