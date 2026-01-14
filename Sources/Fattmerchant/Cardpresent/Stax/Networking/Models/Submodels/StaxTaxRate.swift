struct StaxTaxRate: Codable {
  let id: String?
  let name: String?
  let rate: Double?
  let isDefault: Bool?
  
  enum CodingKeys: String, CodingKey {
    case id, name, rate
    case isDefault = "is_default"
  }
}
