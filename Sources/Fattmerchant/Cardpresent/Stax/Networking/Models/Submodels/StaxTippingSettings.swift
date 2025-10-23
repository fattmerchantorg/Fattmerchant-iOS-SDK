struct StaxTippingSettings: Codable {
  let enabled: Bool?
  let amounts: [Double]?
  let percentages: [Double]?
  let defaultOption: String?
  
  enum CodingKeys: String, CodingKey {
    case enabled, amounts, percentages
    case defaultOption = "default_option"
  }
}
