import Foundation

public struct StaxPaymentMethod: Codable {

  public let id: String?
  public var customerId: String?
  public var merchantId: String?
  public var userId: String?
  public var nickname: String?
  public var isDefault: Int?
  public var method: StaxPaymentMethodType?
  public var meta: JSONCodable?
  public var binType: String?
  public var personName: String?
  public var cardType: String?
  public var cardNumber: String?
  public var cardLastFour: String?
  public var cardExpiry: String?
  public var bankName: String?
  public var bankAccount: String?
  public var bankRouting: String?
  public var bankType: StaxBankAccountType?
  public var bankHolderType: StaxBankHolderType?
  public var address1: String?
  public var address2: String?
  public var addressCity: String?
  public var addressState: String?
  public var addressZip: String?
  public var addressCountry: String?
  
  private enum CodingKeys: String, CodingKey {
    case id
    case customerId = "customer_id"
    case merchantId = "merchant_id"
    case userId = "user_id"
    case nickname
    case isDefault = "is_default"
    case method
    case meta
    case binType = "bin_type"
    case personName = "person_name"
    case cardType = "card_type"
    case cardNumber = "card_number"
    case cardLastFour = "card_last_four"
    case cardExpiry = "card_exp"
    case bankName = "bank_note"
    case bankAccount = "bank_account"
    case bankRouting = "bank_routing"
    case bankHolderType = "bank_holder_type"
    case bankType = "bank_type"
    case address1 = "address_1"
    case address2 = "address_2"
    case addressCity = "address_city"
    case addressState = "address_state"
    case addressZip = "address_zip"
    case addressCountry = "address_country"
  }
  
  public init(
    id: String? = nil,
    customerId: String? = nil,
    merchantId: String? = nil,
    userId: String? = nil,
    nickname: String? = nil,
    isDefault: Int? = nil,
    method: StaxPaymentMethodType? = nil,
    meta: JSONCodable? = nil,
    binType: String? = nil,
    personName: String? = nil,
    cardType: String? = nil,
    cardNumber: String? = nil,
    cardLastFour: String? = nil,
    cardExpiry: String? = nil,
    bankName: String? = nil,
    bankAccount: String? = nil,
    bankRouting: String? = nil,
    bankType: StaxBankAccountType? = nil,
    bankHolderType: StaxBankHolderType? = nil,
    address1: String? = nil,
    address2: String? = nil,
    addressCity: String? = nil,
    addressState: String? = nil,
    addressZip: String? = nil,
    addressCountry: String? = nil
  ) {
    self.id = id
    self.customerId = customerId
    self.merchantId = merchantId
    self.userId = userId
    self.nickname = nickname
    self.isDefault = isDefault
    self.method = method
    self.meta = meta
    self.binType = binType
    self.personName = personName
    self.cardType = cardType
    self.cardNumber = cardNumber
    self.cardLastFour = cardLastFour
    self.cardExpiry = cardExpiry
    self.bankName = bankName
    self.bankAccount = bankAccount
    self.bankRouting = bankRouting
    self.bankType = bankType
    self.bankHolderType = bankHolderType
    self.address1 = address1
    self.address2 = address2
    self.addressCity = addressCity
    self.addressState = addressState
    self.addressZip = addressZip
    self.addressCountry = addressCountry
  }
  
  /// Creates a `StaxPaymentMethod` instance from an existing `StaxPaymentMethod` and mutable properties.
  /// - Parameter from: The existing `StaxPaymentMethod` object to use as a base.
  /// - Parameter changes: A `[String:Any]` of changes to apply to the `StaxPaymentMethod`.
  /// - Returns: The copied, mutable object.
  private init(from existing: StaxPaymentMethod, changes: [String: Any]) {
    self.id = existing.id
    self.customerId = existing.customerId
    self.merchantId = existing.merchantId
    self.userId = existing.userId
    self.nickname = existing.nickname
    self.isDefault = (changes["isDefault"] as? Int) ?? existing.isDefault
    self.method = existing.method
    self.meta = (changes["meta"] as? JSONCodable) ?? existing.meta
    self.binType = existing.binType
    self.personName = (changes["personName"] as? String) ?? existing.personName
    self.cardType = (changes["cardType"] as? String) ?? existing.cardType
    self.cardLastFour = (changes["cardLastFour"] as? String) ?? existing.cardLastFour
    self.cardExpiry = (changes["cardExpiry"] as? String) ?? existing.cardExpiry
    self.bankName = (changes["bankName"] as? String) ?? existing.bankName
    self.bankType = (changes["bankType"] as? StaxBankAccountType) ?? existing.bankType
    self.bankHolderType = (changes["bankHolderType"] as? StaxBankHolderType) ?? existing.bankHolderType
    self.address1 = (changes["address1"] as? String) ?? existing.address1
    self.address2 = (changes["address2"] as? String) ?? existing.address2
    self.addressCity = (changes["addressCity"] as? String) ?? existing.addressCity
    self.addressState = (changes["addressState"] as? String) ?? existing.addressState
    self.addressZip = (changes["addressZip"] as? String) ?? existing.addressZip
    self.addressCountry = (changes["addressCountry"] as? String) ?? existing.addressCountry
    self.nickname = (changes["nickname"] as? String) ?? existing.nickname
  }
  
  /// Creates a `StaxPaymentMethod.Update` instance with mutable properties.
  /// - Returns: The copied, mutable object.
  public func updating() -> Update {
    Update(paymentMethod: self)
  }
  
  /// Wrapper for handling customer updates with explicit mutability
  @dynamicMemberLookup
  public struct Update {
    private var paymentMethod: StaxPaymentMethod
    private var changes: [String: Any]
    
    internal init(paymentMethod: StaxPaymentMethod) {
      self.paymentMethod = paymentMethod
      self.changes = [:]
    }
    
    public subscript(dynamicMember member: String) -> Any? {
      get { changes[member] ?? getValue(for: member) }
      set { changes[member] = newValue }
    }
    
    private func getValue(for member: String) -> Any? {
      switch member {
        case "isDefault": return paymentMethod.isDefault
        case "meta": return paymentMethod.meta
        case "personName": return paymentMethod.personName
        case "cardType": return paymentMethod.cardType
        case "cardLastFour": return paymentMethod.cardLastFour
        case "cardExpiry": return paymentMethod.cardExpiry
        case "bankName": return paymentMethod.bankName
        case "bankType": return paymentMethod.bankType
        case "bankHolderType": return paymentMethod.bankHolderType
        case "address1": return paymentMethod.address1
        case "address2": return paymentMethod.address2
        case "addressCity": return paymentMethod.addressCity
        case "addressState": return paymentMethod.addressState
        case "addressZip": return paymentMethod.addressZip
        case "addressCountry": return paymentMethod.addressCountry
        case "nickname": return paymentMethod.nickname
        default: return nil
      }
    }
    
    internal var modifiedFields: [String: Any] {
      changes
    }
    
    /// Converts a `StaxPaymentMethod.Update` instance into a `StaxPaymentMethod` instance with the pending changes.
    /// - Returns: The moedified, now immutable, `StaxPaymentMethod` object.
    public func apply() -> StaxPaymentMethod {
      StaxPaymentMethod(from: paymentMethod, changes: changes)
    }
  }
}
