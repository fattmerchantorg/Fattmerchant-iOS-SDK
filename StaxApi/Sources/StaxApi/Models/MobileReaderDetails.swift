import Foundation

public class MobileReaderDetails: Codable {
  /// Holds details for AnywhereCommerce settings set in the mobile reader tab
  public var anywhereCommerce: AWCDetails?

  /// Holds details for NMI settings set in the mobile reader tab
  public var nmi: NMIDetails?
}

/// Details for AnywhereCommerce settings set in the mobile reader tab
public struct AWCDetails: Codable {
  /// AnywhereCommerce terminal id
  public var terminalId: String

  /// AnywhereCommerce secret
  public var terminalSecret: String
}

/// Details for NMI (ChipDNA) settings set in the mobile reader tab
public struct NMIDetails: Codable {
  /// NMI (ChipDNA) security key
  public var securityKey: String
}
