import Foundation

/// A catalog item in the Stax platform
public struct Item: Codable, Equatable {
  public var id: String?
  public var item: String?
  public var details: String?
  public var quantity: Int
  public var price: Double
  public var isDiscount: Bool
  public var isPercentage: Bool
  public var isActive: Bool
  public var isTaxable: Bool
  public var isService: Bool
  public var isDiscountable: Bool
  public var meta: JSONValue?

  public enum CodingKeys: CodingKey {
    case id,
         item,
         details,
         quantity,
         price,
         isDiscount,
         isPercentage,
         isActive,
         isTaxable,
         isService,
         isDiscountable,
         meta
  }

  public init(
    id: String?,
    item: String?,
    details: String?,
    quantity: Int,
    price: Double,
    isActive: Bool = true,
    isTaxable: Bool = true,
    isDiscountable: Bool = true,
    isService: Bool = false,
    isDiscount: Bool = false,
    isPercentage: Bool = false
  ) {
    self.id = id
    self.item = item
    self.details = details
    self.quantity = quantity
    self.price = price
    self.isDiscount = isDiscount
    self.isPercentage = isPercentage
    self.isDiscountable = isDiscountable
    self.isActive = isActive
    self.isTaxable = isTaxable
    self.isService = isService
    
    var dict = [String: JSONValue?]()
    dict["is_percentage"] = JSONValue(self.isPercentage)
    dict["is_discountable"] = JSONValue(self.isDiscountable)
    self.meta = dict.jsonValue()
  }
}
