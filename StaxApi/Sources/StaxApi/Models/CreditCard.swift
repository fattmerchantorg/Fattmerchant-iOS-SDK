import Foundation

/// A credit card
public struct CreditCard: Codable {
  public var personName: String
  public var cardNumber: String
  public var cardExp: String
  public var addressZip: String
  public var address1: String?
  public var address2: String?
  public var addressCity: String?
  public var addressState: String?
  public var note: String?
  public var phone: String?
  public var email: String?
  public var customerId: String?
  
  private var method: String = "card"
  
  private enum CodingKeys: String, CodingKey {
      case note,
           phone,
           email,
           method,
           personName = "person_name",
           cardNumber = "card_number",
           cardExp = "card_exp",
           addressZip = "address_zip",
           address1 = "address_1",
           address2 = "address_2",
           addressCity = "address_city",
           addressState = "address_state",
           customerId = "customer_id"
  }

  /// Creates a test credit card
  ///
  /// - Returns: a test credit card
  public static func testCreditCard() -> CreditCard {
    var creditCard = CreditCard(
      personName: "Joan Parsnip",
      cardNumber: "4111111111111111",
      cardExp: "1230",
      addressZip: "32822"
    )

    creditCard.address1 = "123 Orange Ave"
    creditCard.address2 = "Unit 309"
    creditCard.addressCity = "Orlando"
    creditCard.addressState = "FL"
    creditCard.phone = "3210000000"
    creditCard.email = "fatt@merchant.com"
    creditCard.note = "This is a test credit card"

    return creditCard
  }

  /// Creates a test credit card that fails processing
  ///
  /// - Returns: a test credit card
  public static func failingTestCreditCard() -> CreditCard {
      var creditCard = CreditCard(
        personName: "Joan Parsnip",
        cardNumber: "4111111111111111",
        cardExp: "",
        addressZip: "32822"
      )

      creditCard.address1 = "123 Orange Ave"
      creditCard.address2 = "Unit 309"
      creditCard.addressCity = "Orlando"
      creditCard.addressState = "FL"
      creditCard.phone = "3210000000"
      creditCard.email = "fatt@merchant.com"
      creditCard.note = "This is a test credit card"

      return creditCard
  }
  
  public init(personName: String, cardNumber: String, cardExp: String, addressZip: String) {
    self.personName = personName
    self.cardNumber = cardNumber
    self.cardExp = cardExp
    self.addressZip = addressZip
  }
}
