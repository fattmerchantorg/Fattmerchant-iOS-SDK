public struct StaxCreditCard: Codable {
  public let cardholderName: String
  public let number: String
  public let expiry: String
  public let cvv: String?
  public let address: StaxAddress?
  
  public init(
    cardholderName: String,
    number: String,
    expiry: String,
    cvv: String? = nil,
    address: StaxAddress? = nil
  ) {
    self.cardholderName = cardholderName
    self.number = number
    self.expiry = expiry
    self.cvv = cvv
    self.address = address
  }
}
