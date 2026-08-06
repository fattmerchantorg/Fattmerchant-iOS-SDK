/// A bank account
public struct StaxBankAccount: Codable {
  public var accountHolderName: String
  public var bankType: StaxBankAccountType
  public var bankHolderType: StaxBankHolderType
  public var accountNumber: String
  public var routingNumber: String
  public var addressZip: String
  public var bankName: String?
  public var address1: String?
  public var address2: String?
  public var addressCity: String?
  public var addressState: String?
  public var note: String?
  public var phone: String?
  public var email: String?
  public var customerId: String?
  public let method: StaxPaymentMethodType = .bank

  public init(
    accountHolderName: String,
    bankType: StaxBankAccountType,
    bankHolderType: StaxBankHolderType,
    accountNumber: String,
    routingNumber: String,
    address: StaxAddress
  ) throws {
    guard let zip = address.zip else {
      throw CreateBankAccountException.zipCodeNotValid
    }
    
    self.accountHolderName = accountHolderName
    self.bankType = bankType
    self.bankHolderType = bankHolderType
    self.accountNumber = accountNumber
    self.routingNumber = routingNumber
    
    self.address1 = address.line1
    self.address2 = address.line2
    self.addressCity = address.city
    self.addressState = address.state
    self.addressZip = zip
  }

  private enum CodingKeys: String, CodingKey {
    case note
    case phone
    case email
    case method
    case bankType = "bank_type"
    case bankHolderType = "bank_holder_type"
    case accountNumber = "bank_account"
    case routingNumber = "bank_routing"
    case bankName = "bank_name"
    case accountHolderName = "person_name"
    case addressZip = "address_zip"
    case address1 = "address_1"
    case address2 = "address_2"
    case addressCity = "address_city"
    case addressState = "address_state"
    case customerId = "customer_id"
  }
}
