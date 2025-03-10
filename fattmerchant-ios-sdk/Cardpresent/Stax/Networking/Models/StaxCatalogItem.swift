import Foundation

public struct StaxCatalogItem: Codable {
  public let id: String?
  public let userId: String?
  public let merchantId: String?
  public let item: String?
  public let code: String?
  public let category: String?
  public let details: String?
  public let isActive: Bool?
  public let isTaxable: Bool?
  public let isService: Bool?
  public let isDiscount: Bool?
  public let price: Double?
  public let amountInStock: Int?
  public let meta: JSONCodable?
  public let createdAt: Date?
  public let updatedAt: Date?
  
  private enum CodingKeys: String, CodingKey {
    case id
    case userId = "user_id"
    case merchantId = "merchant_id"
    case item
    case code
    case category
    case details
    case isActive = "is_active"
    case isTaxable = "is_taxable"
    case isService = "is_service"
    case isDiscount = "is_discount"
    case price
    case amountInStock = "in_stock"
    case meta
    case createdAt = "created_at"
    case updatedAt = "updated_at"
  }
  
  public init (
    id: String? = nil,
    userId: String? = nil,
    merchantId: String? = nil,
    item: String? = nil,
    code: String? = nil,
    category: String? = nil,
    details: String? = nil,
    isActive: Bool? = nil,
    isTaxable: Bool? = nil,
    isService: Bool? = nil,
    isDiscount: Bool? = nil,
    price: Double? = nil,
    amountInStock: Int? = nil,
    meta: JSONCodable? = nil,
    createdAt: Date? = nil,
    updatedAt: Date? = nil
  ) {
    self.id = id
    self.userId = userId
    self.merchantId = merchantId
    self.item = item
    self.code = code
    self.category = category
    self.details = details
    self.isActive = isActive
    self.isTaxable = isTaxable
    self.isService = isService
    self.isDiscount = isDiscount
    self.price = price
    self.amountInStock = amountInStock
    self.meta = meta
    self.createdAt = createdAt
    self.updatedAt = updatedAt
  }
  
  private init(from existing: StaxCatalogItem, changes: [String: Any]) {
    self.id = existing.id
    self.userId = existing.userId
    self.merchantId = existing.merchantId
    self.item = (changes["item"] as? String) ?? existing.item
    self.code = (changes["code"] as? String) ?? existing.code
    self.category = (changes["category"] as? String) ?? existing.category
    self.details = (changes["details"] as? String) ?? existing.details
    self.isActive = (changes["isActive"] as? Bool) ?? existing.isActive
    self.isTaxable = (changes["isTaxable"] as? Bool) ?? existing.isTaxable
    self.isService = (changes["isService"] as? Bool) ?? existing.isService
    self.isDiscount = (changes["isDiscount"] as? Bool) ?? existing.isDiscount
    self.price = (changes["price"] as? Double) ?? existing.price
    self.amountInStock = (changes["amountInStock"] as? Int) ?? existing.amountInStock
    self.meta = (changes["meta"] as? JSONCodable) ?? existing.meta
    self.createdAt = existing.createdAt
    self.updatedAt = existing.updatedAt
  }
  
  /// Creates a `StaxCatalogItem.Update` instance with mutable properties.
  /// - Returns: The copied, mutable object.
  public func updating() -> Update {
    Update(item: self)
  }
  
  /// Wrapper for handling invoice updates with explicit mutability
  @dynamicMemberLookup
  public struct Update {
    private var item: StaxCatalogItem
    private var changes: [String: Any]
      
    internal init(item: StaxCatalogItem) {
      self.item = item
      self.changes = [:]
    }

    public subscript(dynamicMember member: String) -> Any? {
      get { changes[member] ?? getValue(for: member) }
      set { changes[member] = newValue }
    }

    private func getValue(for member: String) -> Any? {
      switch member {
        case "item": return item.item
        case "code": return item.code
        case "category": return item.category
        case "details": return item.details
        case "isActive": return item.isActive
        case "isTaxable": return item.isTaxable
        case "isService": return item.isService
        case "isDiscount": return item.isDiscount
        case "price": return item.price
        case "amountInStock": return item.amountInStock
        case "meta": return item.meta
        default: return nil
      }
    }
        
    internal var modifiedFields: [String: Any] {
      changes
    }
    
    /// Converts a `StaxCatalogItem.Update` instance into a `StaxCatalogItem` instance with the pending changes.
    /// - Returns: The moedified, now immutable, `StaxCatalogItem` object.
    public func apply() -> StaxCatalogItem {
      StaxCatalogItem(from: item, changes: changes)
    }
  }
}
