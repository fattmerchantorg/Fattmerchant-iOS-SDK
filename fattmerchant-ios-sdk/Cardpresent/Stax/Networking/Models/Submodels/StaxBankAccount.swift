public struct StaxBankAccount: Codable {
  public let name: String
  public let number: String
  public let expiry: String
  public let cvv: String?
  public let address: StaxAddress?
  
  public init(
    name: String,
    number: String,
    expiry: String,
    cvv: String? = nil,
    address: StaxAddress? = nil
  ) {
    self.name = name
    self.number = number
    self.expiry = expiry
    self.cvv = cvv
    self.address = address
  }
}
