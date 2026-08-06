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

    // `rate` may arrive as a JSON number or a numeric string; accept either.
    rate = (try? container.decodeIfPresent(Double.self, forKey: .rate))
      ?? (try? container.decodeIfPresent(String.self, forKey: .rate)).flatMap { Double($0) }
  }
}
