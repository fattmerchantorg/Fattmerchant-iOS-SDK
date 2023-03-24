import Foundation

/// A Transaction in the Stax Platform
public class Transaction: Codable {
  public init() {}

  public var id: String?
  public var success: Bool?
  public var total: Double?
  public var totalRefunded: Double?
  public var type: String?
  public var customerId: String?
  public var invoiceId: String?
  public var isVoided: Bool?
  public var lastFour: String?
  public var merchantId: String?
  public var message: String?
  public var userId: String?
  public var paymentMethodId: String?
  public var receiptEmailAt: String?
  public var preAuth: Bool?
  public var scheduleId: String?
  public var createdAt: String?
  public var meta: JSONValue?
  public var method: String?
  public var source: String?
  
  internal var token: String?

  var authId: String?
  var gateway: String?
  var gatewayId: String?
  var gatewayName: String?
  var isCaptured: Int?
  var isManual: Bool?
  var isMerchantPresent: Bool?
  var isRefundable: Bool?
  var issuerAuthCode: String?
  var isVoidable: Bool?
  var parentAuth: String?
  var receiptSmsAt: String?
  var referenceId: String?
  var settledAt: String?
  var sourceIp: String?
  var response: JSONValue?
  var updatedAt: String?
  var channel: String? = "ios"
  var childCaptures: [Transaction]?

  public func getLineItems() -> [Item]? {
    guard let dict = meta?.toDictionary() else {
      return nil
    }
    
    return dict["lineItems"] as? [Item]
  }
}
