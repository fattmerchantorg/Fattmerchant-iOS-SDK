import Foundation

/// Represents a invoice in the Stax API.
public struct StaxInvoice: Codable {
  /// Unique identifier for the invoice
  public var id: String?
  public var userId: String?
  public var customerId: String?
  public var merchantId: String?
  public var paymentMethodId: String?
  public var scheduleId: String?
  public var balanceDue: Double?
  public var isMerchantPresent: Bool?
  public var isWebpayment: Bool?
  public var hasPaymentAttemptFailed: Bool?
  public var paymentAttemptMessage: String?
  public var status: StaxInvoiceStatus?
  public var total: Double?
  public var totalPaid: Double?
  public var url: String?
  public var meta: JSONCodable?
  public var dueAt: Date?
  public var sentAt: Date?
  public var paidAt: Date?
  public var viewedAt: Date?
  public var createdAt: Date?
  public var updatedAt: Date?
  
  private enum CodingKeys: String, CodingKey {
    case id
    case userId = "user_id"
    case customerId = "customer_id"
    case merchantId = "merchant_id"
    case paymentMethodId = "payment_method_id"
    case scheduleId = "schedule_id"
    case balanceDue = "balance_due"
    case isMerchantPresent = "is_merchant_present"
    case isWebpayment = "is_webpayment"
    case hasPaymentAttemptFailed = "payment_attempt_failed"
    case paymentAttemptMessage = "payment_attempt_message"
    case status
    case total
    case totalPaid = "total_paid"
    case url
    case meta
    case dueAt = "due_at"
    case sentAt = "sent_at"
    case paidAt = "paid_at"
    case viewedAt = "viewed_at"
    case createdAt = "created_at"
    case updatedAt = "updated_at"
  }
  
  public init(
    id: String? = nil,
    userId: String? = nil,
    customerId: String? = nil,
    merchantId: String? = nil,
    paymentMethodId: String? = nil,
    scheduleId: String? = nil,
    balanceDue: Double? = nil,
    isMerchantPresent: Bool? = nil,
    isWebpayment: Bool? = nil,
    hasPaymentAttemptFailed: Bool? = nil,
    paymentAttemptMessage: String? = nil,
    status: StaxInvoiceStatus? = nil,
    total: Double? = nil,
    totalPaid: Double? = nil,
    url: String? = nil,
    meta: JSONCodable? = nil,
    dueAt: Date? = nil,
    sentAt: Date? = nil,
    paidAt: Date? = nil,
    viewedAt: Date? = nil,
    createdAt: Date? = nil,
    updatedAt: Date? = nil
  ) {
    self.id = id
    self.userId = userId
    self.customerId = customerId
    self.merchantId = merchantId
    self.paymentMethodId = paymentMethodId
    self.scheduleId = scheduleId
    self.balanceDue = balanceDue
    self.isMerchantPresent = isMerchantPresent
    self.isWebpayment = isWebpayment
    self.hasPaymentAttemptFailed = hasPaymentAttemptFailed
    self.paymentAttemptMessage = paymentAttemptMessage
    self.status = status
    self.total = total
    self.totalPaid = totalPaid
    self.url = url
    self.meta = meta
    self.dueAt = dueAt
    self.sentAt = sentAt
    self.paidAt = paidAt
    self.viewedAt = viewedAt
    self.createdAt = createdAt
    self.updatedAt = updatedAt
  }
  
  private init(from existing: StaxInvoice, changes: [String: Any]) {
    self.id = existing.id
    self.userId = (changes["userId"] as? String) ?? existing.userId
    self.customerId = (changes["customerId"] as? String) ?? existing.customerId
    self.merchantId = (changes["merchantId"] as? String) ?? existing.merchantId
    self.paymentMethodId = (changes["paymentMethodId"] as? String) ?? existing.paymentMethodId
    self.scheduleId = (changes["scheduleId"] as? String) ?? existing.scheduleId
    self.balanceDue = (changes["balanceDue"] as? Double) ?? existing.balanceDue
    self.isMerchantPresent = (changes["isMerchantPresent"] as? Bool) ?? existing.isMerchantPresent
    self.isWebpayment = (changes["isWebpayment"] as? Bool) ?? existing.isWebpayment
    self.hasPaymentAttemptFailed = (changes["hasPaymentAttemptFailed"] as? Bool) ?? existing.hasPaymentAttemptFailed
    self.paymentAttemptMessage = (changes["paymentAttemptMessage"] as? String) ?? existing.paymentAttemptMessage
    self.status = (changes["status"] as? StaxInvoiceStatus) ?? existing.status
    self.total = (changes["total"] as? Double) ?? existing.total
    self.totalPaid = (changes["totalPaid"] as? Double) ?? existing.totalPaid
    self.url = (changes["url"] as? String) ?? existing.url
    self.meta = (changes["meta"] as? JSONCodable) ?? existing.meta
    self.dueAt = (changes["dueAt"] as? Date) ?? existing.dueAt
    self.sentAt = existing.sentAt
    self.paidAt = existing.paidAt
    self.viewedAt = existing.viewedAt
    self.createdAt = existing.createdAt
    self.updatedAt = existing.updatedAt
  }
  
  /// Creates a `StaxInvoice.Update` instance with mutable properties.
  /// - Returns: The copied, mutable object.
  public func updating() -> Update {
    Update(invoice: self)
  }
  
  /// Wrapper for handling invoice updates with explicit mutability
  @dynamicMemberLookup
  public struct Update {
    private var invoice: StaxInvoice
    private var changes: [String: Any]
      
    internal init(invoice: StaxInvoice) {
      self.invoice = invoice
      self.changes = [:]
    }

    public subscript(dynamicMember member: String) -> Any? {
      get { changes[member] ?? getValue(for: member) }
      set { changes[member] = newValue }
    }

    private func getValue(for member: String) -> Any? {
      switch member {
        case "userId": return invoice.userId
        case "customerId": return invoice.customerId
        case "merchantId": return invoice.merchantId
        case "paymentMethodId": return invoice.paymentMethodId
        case "scheduleId": return invoice.scheduleId
        case "balanceDue": return invoice.balanceDue
        case "isMerchantPresent": return invoice.isMerchantPresent
        case "isWebpayment": return invoice.isWebpayment
        case "hasPaymentAttemptFailed": return invoice.hasPaymentAttemptFailed
        case "status": return invoice.status
        case "total": return invoice.total
        case "totalPaid": return invoice.totalPaid
        case "url": return invoice.url
        case "meta": return invoice.meta
        case "dueAt": return invoice.dueAt
        default: return nil
      }
    }
        
    internal var modifiedFields: [String: Any] {
      changes
    }
    
    /// Converts a `StaxInvoice.Update` instance into a `StaxInvoice` instance with the pending changes.
    /// - Returns: The moedified, now immutable, `StaxInvoice` object.
    public func apply() -> StaxInvoice {
      StaxInvoice(from: invoice, changes: changes)
    }
  }
}
