import Foundation

/// Represents a transaction in the Stax API.
public struct StaxTransaction: Codable {
  /// Unique identifier for the transaction
  public let id: String?

  /// ID of the associated invoice
  public let invoiceId: String?

  /// Reference ID for the transaction
  public let referenceId: String?

  /// ID of the recurring transaction if part of a recurring series
  public let recurringTransactionId: String?

  /// Authorization ID for the transaction
  public let authId: String?

  /// Type of transaction
  public let type: TransactionType?

  /// Source of the transaction
  public let source: String?

  /// IP address of the transaction source
  public let sourceIp: String?

  /// Whether merchant was present during the transaction
  public let isMerchantPresent: Bool?

  /// ID of the merchant associated with the transaction
  public let merchantId: String?

  /// ID of the user who processed the transaction
  public let userId: String?

  /// ID of the customer associated with the transaction
  public let customerId: String?

  /// ID of the payment method used
  public let paymentMethodId: String?

  /// Whether the transaction was manually entered
  public let isManual: Bool?

  /// Spreedly payment token
  public let spreedlyToken: String?

  /// Response from Spreedly service
  public let spreedlyResponse: String?

  /// Whether the transaction was successful
  public let success: Bool?

  /// Message detailing transaction status or errors
  public let message: String?

  /// Additional metadata about the transaction
  public let meta: String?

  /// Total amount of the transaction
  public let total: Double?

  /// Payment method used
  public let method: String?

  /// Whether this was a pre-authorization
  public let preAuth: Bool?

  /// Whether the transaction has been captured
  public let isCaptured: Bool?

  /// Last four digits of the payment method
  public let lastFour: String?

  /// Interchange code for the transaction
  public let interchangeCode: String?

  /// Interchange fee amount
  public let interchangeFee: Double?

  /// ID of the batch containing this transaction
  public let batchId: String?

  /// When the transaction was added to a batch
  public let batchedAt: Date?

  /// EMV (chip card) response data
  public let emvResponse: String?

  /// Address Verification System response
  public let avsResponse: String?

  /// Card Verification Value response
  public let cvvResponse: String?

  /// Point of Sale entry mode
  public let posEntry: String?

  /// Salesperson who processed the transaction
  public let posSalesperson: String?

  /// When the receipt was sent via email
  public let receiptEmailAt: Date?

  /// When the receipt was sent via SMS
  public let receiptSmsAt: Date?

  /// When the transaction was settled
  public let settledAt: Date?

  /// When the transaction was created
  public let createdAt: Date?

  /// When the transaction was last updated
  public let updatedAt: Date?

  /// ID of the payment gateway used
  public let gatewayId: String?

  /// Issuer authorization code
  public let issuerAuthCode: String?

  /// Channel through which the transaction was processed
  public let channel: String?

  /// Currency used for the transaction
  public let currency: Currency?
    
  /// Types of transactions supported by the Stax API
  public enum TransactionType: String, Codable {
    case charge
    case void
    case refund
    case capture
    case preAuth = "pre_auth"
    case credit
    case empty = ""
  }
    
  /// Currencies supported by the Stax API
  public enum Currency: String, Codable {
    case usd = "USD"
    case cad = "CAD"
    case mxn = "MXN"
    case eur = "EUR"
    case gbp = "GBP"
  }
    
  private enum CodingKeys: String, CodingKey {
    case id
    case invoiceId = "invoice_id"
    case referenceId = "reference_id"
    case recurringTransactionId = "recurring_transaction_id"
    case authId = "auth_id"
    case type
    case source
    case sourceIp = "source_ip"
    case isMerchantPresent = "is_merchant_present"
    case merchantId = "merchant_id"
    case userId = "user_id"
    case customerId = "customer_id"
    case paymentMethodId = "payment_method_id"
    case isManual = "is_manual"
    case spreedlyToken = "spreedly_token"
    case spreedlyResponse = "spreedly_response"
    case success
    case message
    case meta
    case total
    case method
    case preAuth = "pre_auth"
    case isCaptured = "is_captured"
    case lastFour = "last_four"
    case interchangeCode = "interchange_code"
    case interchangeFee = "interchange_fee"
    case batchId = "batch_id"
    case batchedAt = "batched_at"
    case emvResponse = "emv_response"
    case avsResponse = "avs_response"
    case cvvResponse = "cvv_response"
    case posEntry = "pos_entry"
    case posSalesperson = "pos_salesperson"
    case receiptEmailAt = "receipt_email_at"
    case receiptSmsAt = "receipt_sms_at"
    case settledAt = "settled_at"
    case createdAt = "created_at"
    case updatedAt = "updated_at"
    case gatewayId = "gateway_id"
    case issuerAuthCode = "issuer_auth_code"
    case channel
    case currency
  }
    
  /// Creates a new `StaxTransaction` instance
  public init(
    id: String?,
    invoiceId: String?,
    referenceId: String?,
    recurringTransactionId: String?,
    authId: String? = nil,
    type: TransactionType?,
    source: String? = nil,
    sourceIp: String? = nil,
    isMerchantPresent: Bool? = nil,
    merchantId: String?,
    userId: String?,
    customerId: String?,
    paymentMethodId: String? = nil,
    isManual: Bool? = nil,
    spreedlyToken: String?,
    spreedlyResponse: String?,
    success: Bool?,
    message: String? = nil,
    meta: String?,
    total: Double?,
    method: String?,
    preAuth: Bool?,
    isCaptured: Bool?,
    lastFour: String? = nil,
    interchangeCode: String? = nil,
    interchangeFee: Double? = nil,
    batchId: String? = nil,
    batchedAt: Date? = nil,
    emvResponse: String? = nil,
    avsResponse: String? = nil,
    cvvResponse: String? = nil,
    posEntry: String? = nil,
    posSalesperson: String? = nil,
    receiptEmailAt: Date? = nil,
    receiptSmsAt: Date? = nil,
    settledAt: Date? = nil,
    createdAt: Date? = nil,
    updatedAt: Date? = nil,
    gatewayId: String? = nil,
    issuerAuthCode: String? = nil,
    channel: String? = nil,
    currency: Currency? = nil
  ) {
    self.id = id
    self.invoiceId = invoiceId
    self.referenceId = referenceId
    self.authId = authId
    self.source = source
    self.sourceIp = sourceIp
    self.isMerchantPresent = isMerchantPresent
    self.merchantId = merchantId
    self.userId = userId
    self.customerId = customerId
    self.paymentMethodId = paymentMethodId
    self.isManual = isManual
    self.spreedlyToken = spreedlyToken
    self.spreedlyResponse = spreedlyResponse
    self.success = success
    self.message = message
    self.meta = meta
    self.total = total
    self.lastFour = lastFour
    self.interchangeCode = interchangeCode
    self.interchangeFee = interchangeFee
    self.batchId = batchId
    self.batchedAt = batchedAt
    self.emvResponse = emvResponse
    self.avsResponse = avsResponse
    self.cvvResponse = cvvResponse
    self.posEntry = posEntry
    self.posSalesperson = posSalesperson
    self.receiptEmailAt = receiptEmailAt
    self.receiptSmsAt = receiptSmsAt
    self.settledAt = settledAt
    self.createdAt = createdAt
    self.updatedAt = updatedAt
    self.gatewayId = gatewayId
    self.issuerAuthCode = issuerAuthCode
    self.channel = channel
    self.currency = currency
    self.recurringTransactionId = recurringTransactionId
    self.type = type
    self.method = method
    self.preAuth = preAuth
    self.isCaptured = isCaptured
  }
}

// MARK: - Date Handling Extension

extension StaxTransaction {
  /// Custom decoder to handle various date formats from the API
  public static func dateDecodingStrategy() -> JSONDecoder.DateDecodingStrategy {
    return .custom { decoder -> Date in
      let container = try decoder.singleValueContainer()
      
      // Try to decode as a Date string
      let dateString = try container.decode(String.self)
      
      // Try different date formats
      let formats = [
        "yyyy-MM-dd'T'HH:mm:ss.SSSZ",  // ISO8601 with milliseconds
        "yyyy-MM-dd'T'HH:mm:ssZ",      // ISO8601 without milliseconds
        "yyyy-MM-dd HH:mm:ss"          // Simple date format
      ]
      
      let dateFormatters = formats.map { format -> DateFormatter in
        let formatter = DateFormatter()
        formatter.dateFormat = format
        return formatter
      }
      
      for formatter in dateFormatters {
        if let date = formatter.date(from: dateString) {
          return date
        }
      }
      
      throw DecodingError.dataCorruptedError(
        in: container,
        debugDescription: "Cannot decode date: \(dateString)"
      )
    }
  }
}
