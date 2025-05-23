struct StaxMerchantOptions: Codable {
  let taxRates: [StaxTaxRate]?
  let surcharge: StaxSurcharge?
  let emvUser: String?
  let emvPassword: String?
  let emvTerminalId: String?
  let emvTerminalSecret: String?
  let tipping: StaxTippingSettings?
  
  enum CodingKeys: String, CodingKey {
    case taxRates = "tax_rates"
    case surcharge
    case emvUser = "emv_user"
    case emvPassword = "emv_password"
    case emvTerminalId = "emv_terminal_id"
    case emvTerminalSecret = "emv_terminal_secret"
    case tipping
  }
}
