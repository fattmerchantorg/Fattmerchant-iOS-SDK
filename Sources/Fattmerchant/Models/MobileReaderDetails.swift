import Foundation

/// Mobile reader settings defined in the mobile reader tab
class MobileReaderDetails: Codable {
  internal var nmi: NMIDetails?
}

/// Details for NMI (ChipDNA) settings set in the mobile reader tab
internal struct NMIDetails: Codable {
  /// NMI API key for authenticated requests
  var apiKey: String
  
  /// NMI (ChipDNA) security key
  var securityKey: String
  
  private enum CodingKeys: String, CodingKey {
    case apiKey = "api_key"
    case securityKey = "security_key"
  }
}
