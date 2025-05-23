public struct StaxAddress: Codable {
  public let line1: String?
  public let line2: String?
  public let city: String?
  public let state: String?
  public let zip: String?
  public let country: String?
  
  public init(
    line1: String? = nil,
    line2: String? = nil,
    city: String? = nil,
    state: String? = nil,
    zip: String? = nil,
    country: String? = nil
  ) {
    self.line1 = line1
    self.line2 = line2
    self.city = city
    self.state = state
    self.zip = zip
    self.country = country
  }
}
