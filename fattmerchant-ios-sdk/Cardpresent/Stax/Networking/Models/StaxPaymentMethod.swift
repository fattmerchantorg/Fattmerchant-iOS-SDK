import Foundation

public struct StaxPaymentMethod: Codable {

  public let id: String?
  public let customerId: String?
  public let merchantId: String?
  public let userId: String?
  public let nickname: String?
  public let isDefault: Int?
  public let method: StaxPaymentMethodType?
  public let meta: JSONCodable?
  public let binType: String?
  public let personName: String?
  public let cardType: String?
  public let cardLastFour: String?
  public let cardExpiry: String?
  public let bankName: String?
  public let bankType: StaxBankAccountType?
  public let bankHolderType: StaxBankHolderType?
  public let address1: String?
  public let address2: String?
  public let addressCity: String?
  public let addressState: String?
  public let addressZip: String?
  public let addressCountry: String?
  public let purgedAt: Date?
  public let createdAt: Date?
  public let updatedAt: Date?
  
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
    case cardLastFour = "card_last_four"
    case cardExpiry = "card_exp"
    case bankName = "bank_note"
    case bankHolderType = "bank_holder_type"
    case bankType = "bank_type"
    case address1 = "address_1"
    case address2 = "address_2"
    case addressCity = "address_city"
    case addressState = "address_state"
    case addressZip = "address_zip"
    case addressCountry = "address_country"
    case purgedAt = "purged_at"
    case createdAt = "created_at"
    case updatedAt = "updated_at"
  }
  
  public init(
    id: String?,
    customerId: String?,
    merchantId: String?,
    userId: String?,
    nickname: String?,
    isDefault: Int?,
    method: StaxPaymentMethodType?,
    meta: JSONCodable?,
    binType: String?,
    personName: String?,
    cardType: String?,
    cardLastFour: String?,
    cardExpiry: String?,
    bankName: String?,
    bankType: StaxBankAccountType?,
    bankHolderType: StaxBankHolderType?,
    address1: String?,
    address2: String?,
    addressCity: String?,
    addressState: String?,
    addressZip: String?,
    addressCountry: String?,
    purgedAt: Date?,
    createdAt: Date?,
    updatedAt: Date?
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
    self.cardLastFour = cardLastFour
    self.cardExpiry = cardExpiry
    self.bankName = bankName
    self.bankType = bankType
    self.bankHolderType = bankHolderType
    self.address1 = address1
    self.address2 = address2
    self.addressCity = addressCity
    self.addressState = addressState
    self.addressZip = addressZip
    self.addressCountry = addressCountry
    self.purgedAt = purgedAt
    self.createdAt = createdAt
    self.updatedAt = updatedAt
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
    self.address2 = (changes["address2"] as? String) ?? existing.address1
    self.addressCity = (changes["addressCity"] as? String) ?? existing.addressCity
    self.addressState = (changes["addressState"] as? String) ?? existing.addressState
    self.addressZip = (changes["addressZip"] as? String) ?? existing.addressZip
    self.addressCountry = (changes["addressCountry"] as? String) ?? existing.addressCountry
    self.purgedAt = existing.purgedAt
    self.createdAt = existing.createdAt
    self.updatedAt = existing.updatedAt
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
