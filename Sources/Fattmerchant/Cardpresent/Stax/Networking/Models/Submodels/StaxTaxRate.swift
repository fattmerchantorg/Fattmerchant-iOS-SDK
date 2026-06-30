struct StaxTaxRate: Codable {
  let id: String?
  let name: String?
  let rate: Double?
  let isDefault: Bool?

  enum CodingKeys: String, CodingKey {
    case id, name, rate
    case isDefault = "is_default"
  }

  init(from decoder: Decoder) throws {
    let container = try decoder.container(keyedBy: CodingKeys.self)
    id = try container.decodeIfPresent(String.self, forKey: .id)
    name = try container.decodeIfPresent(String.self, forKey: .name)
    isDefault = try container.decodeIfPresent(Bool.self, forKey: .isDefault)

    if let doubleRate = try? container.decodeIfPresent(Double.self, forKey: .rate) {
      rate = doubleRate
    } else if let stringRate = try? container.decodeIfPresent(String.self, forKey: .rate) {
      rate = Double(stringRate)
    } else {
      rate = nil
    }
  }
}
