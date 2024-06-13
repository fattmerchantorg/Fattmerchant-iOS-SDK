import Foundation

/// Details for NMI (ChipDNA) settings set in the mobile reader tab
internal struct NMIDetails: Codable {
    /// NMI (ChipDNA) security key
    var securityKey: String
}
