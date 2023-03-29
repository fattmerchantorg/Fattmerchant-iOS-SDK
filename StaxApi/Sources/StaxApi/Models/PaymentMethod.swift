import Foundation

/// A payment method such as a credit card or a bank account
public class PaymentMethod: Codable {
  
  /// Types of payment methods. Typically, 'card' and 'bank'
  ///
  /// - card: A credit card
  /// - bank: A bank account
  public enum PaymentMethodType: String, Codable {
    case card,
         bank
  }
  
  public var id: String?
  public var method: PaymentMethodType?
  public var merchantId: String?
  public var customerId: String

  /// Whether or not Stax should tokenize this PaymentMethod
  ///
  /// - Note:
  /// If this field is true, `paymentToken` must be `null`
  var tokenize: Bool?

  /// The token that represents this payment method
  ///
  /// The only use-case for this field is storing the token within Stax. After cardpresent tokenization,
  /// we can create a PaymentMethod using this class. If we include the paymentToken, then we can later
  /// store it as an already-tokenized PaymentMethod
  ///
  /// - Important:
  ///   Stax performs transactions with this token. Therefore, it is crucial that only the actual payment
  ///   token be placed here
  ///
  /// - Note: If this field is not `null`, then `tokenize` must be `false`
  internal var paymentToken: String?
  internal var cardNumber: String?
  internal var cardCvv: String?

  /// The bank account number
  internal var bankAccount: String?

  /// The bank routing number
  internal var bankRouting: String?
  public var nickname: String?
  public var personName: String?
  public var cardType: String?
  public var cardLastFour: String?
  public var cardExp: String?
  public var bankName: String?
  public var bankType: String?
  public var bankHolderType: BankAccount.BankHolderType?

  init(customer: Customer) {
    self.customerId = customer.id ?? ""
  }

  init(card: CreditCard, customer: Customer) {
    self.customerId = customer.id ?? ""
    personName = customer.fullName()
    method = .card
    cardNumber = card.cardNumber
    cardExp = card.cardExp
  }

  init(bank: BankAccount, customer: Customer) {
    self.customerId = customer.id ?? ""
    personName = customer.fullName()
    method = .bank
    bankAccount = bank.bankAccount
    bankRouting = bank.bankRouting
    bankName = bank.bankName
    bankType = bank.bankType
  }
}
