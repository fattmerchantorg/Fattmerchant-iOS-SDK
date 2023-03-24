import Foundation

/// A bank account
public struct BankAccount: Codable {
  public var personName: String
  public var bankType: String
  public var bankHolderType: BankHolderType
  public var bankAccount: String
  public var bankRouting: String
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

  private var method = PaymentMethod.PaymentMethodType.bank

  /// Types of bank holder. Typically 'personal' or 'business'
  ///
  /// - personal: A personal bank account
  /// - business: A business bank account
  public enum BankHolderType: String, Codable {
    case personal,
         business
  }
  
  private enum CodingKeys: String, CodingKey {
    case note,
         phone,
         email,
         method,
         bankType = "bank_type",
         bankHolderType = "bank_holder_type",
         bankAccount = "bank_account",
         bankRouting = "bank_routing",
         bankName = "bank_name",
         personName = "person_name",
         addressZip = "address_zip",
         address1 = "address_1",
         address2 = "address_2",
         addressCity = "address_city",
         addressState = "address_state",
         customerId = "customer_id"
  }
  
  /// Creates a test bank account
  ///
  /// - Returns: a test bank account
  public static func testBankAccount() -> BankAccount {
    var bankAccount = BankAccount(
      personName: "Jim Parsnip",
      bankType: "savings",
      bankHolderType: .business,
      bankAccount: "9876543210",
      bankRouting: "021000021",
      addressZip: "32822"
    )

    bankAccount.address1 = "123 Orange Ave"
    bankAccount.address2 = "Unit 309"
    bankAccount.addressCity = "Orlando"
    bankAccount.addressState = "FL"
    bankAccount.phone = "3210000000"
    bankAccount.email = "fatt@merchant.com"
    bankAccount.note = "This is a test credit card"

    return bankAccount
  }

  /// Creates a test bank account that fails processing
  ///
  /// - Returns: a test bank account
  public static func failingTestBankAccount() -> BankAccount {
    var bankAccount = BankAccount(
      personName: "Jim Parsnip",
      bankType: "",
      bankHolderType: .business,
      bankAccount: "9876543210",
      bankRouting: "021000021",
      addressZip: "32822"
    )

    bankAccount.address1 = "123 Orange Ave"
    bankAccount.address2 = "Unit 309"
    bankAccount.addressCity = "Orlando"
    bankAccount.addressState = "FL"
    bankAccount.phone = "3210000000"
    bankAccount.email = "fatt@merchant.com"
    bankAccount.note = "This is a test credit card"

    return bankAccount
  }
  
  public init(
    personName: String,
    bankType: String,
    bankHolderType: BankHolderType,
    bankAccount: String,
    bankRouting: String,
    addressZip: String
  ) {
    self.personName = personName
    self.bankType = bankType
    self.bankHolderType = bankHolderType
    self.bankAccount = bankAccount
    self.bankRouting = bankRouting
    self.addressZip = addressZip
  }
}
