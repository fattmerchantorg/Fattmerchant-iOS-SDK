import Foundation

public struct StaxCreditCard: Codable {
  public var cardholder: String
  public var number: String
  public var expiry: String
  public var addressZip: String
  public var address1: String?
  public var address2: String?
  public var addressCity: String?
  public var addressState: String?
  public var note: String?
  public var phone: String?
  public var email: String?
  public var customerId: String?
  private let method: StaxPaymentMethodType = .card
  
  public init(cardholder: String, number: String, expiry: String, address: StaxAddress) throws {
    guard let zip = address.zip else { throw CreateCreditCardException.zipCodeNotValid; }

    if expiry.count != 4 { throw CreateCreditCardException.expiryNotValid; }
    if Int(expiry.prefix(2)) ?? 13 > 12 { throw CreateCreditCardException.expiryNotValid; }
    
    self.cardholder = cardholder
    self.number = number
    self.expiry = expiry
    
    self.address1 = address.line1
    self.address2 = address.line2
    self.addressCity = address.city
    self.addressState = address.state
    self.addressZip = zip  }

  private enum CodingKeys: String, CodingKey {
    case note, phone, email, method,
    cardholder = "person_name",
    number = "card_number",
    expiry = "card_exp",
    addressZip = "address_zip",
    address1 = "address_1",
    address2 = "address_2",
    addressCity = "address_city",
    addressState = "address_state",
    customerId = "customer_id"
  }
}
