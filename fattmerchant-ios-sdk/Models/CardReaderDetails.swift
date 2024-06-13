/// Card reader settings defined in the mobile reader tab in Connect Lite
class CardReaderSettings: Codable {
  public var id: String?

  /// Holds details for NMI settings set in the mobile reader tab
  internal var nmi: NMIDetails?
}

extension CardReaderSettings: OmniEndpoint {
  static func resourceEndpoint() -> String {
    return "/team/gateway/hardware/mobile"
  }
}
