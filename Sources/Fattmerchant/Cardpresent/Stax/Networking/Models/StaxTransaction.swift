import Foundation

/// Represents a transaction in the Stax API.
public struct StaxTransaction: Codable {
  /// Unique identifier for the transaction
  public var id: String?

  /// ID of the associated invoice
  public var invoiceId: String?

  /// Reference ID for the transaction
  public var referenceId: String?

  /// ID of the recurring transaction if part of a recurring series
  public var recurringTransactionId: String?

  /// Authorization ID for the transaction
  public var authId: String?

  /// Type of transaction
  public var type: TransactionType?

  /// Source of the transaction
  public var source: String?

  /// IP address of the transaction source
  public var sourceIp: String?

  /// Whether merchant was present during the transaction
  public var isMerchantPresent: Bool?

  /// ID of the merchant associated with the transaction
  public var merchantId: String?

  /// ID of the user who processed the transaction
  public var userId: String?

  /// ID of the customer associated with the transaction
  public var customerId: String?

  /// ID of the payment method used
  public var paymentMethodId: String?

  /// Whether the transaction was manually entered
  public var isManual: Bool?

  /// Spreedly payment token
  public var spreedlyToken: String?

  /// Response from Spreedly service
  public var spreedlyResponse: String?

  /// Whether the transaction was successful
  public var success: Bool?

  /// Message detailing transaction status or errors
  public var message: String?

  /// Additional metadata about the transaction
  public var meta: JSONCodable?

  /// Total amount of the transaction
  public var total: Double?

  /// Payment method used
  public var method: String?

  /// Whether this was a pre-authorization
  public var preAuth: Bool?

  /// Whether the transaction has been captured
  public var isCaptured: Int?

  /// Last four digits of the payment method
  public var lastFour: String?

  /// Interchange code for the transaction
  public var interchangeCode: String?

  /// Interchange fee amount
  public var interchangeFee: Double?

  /// ID of the batch containing this transaction
  public var batchId: String?

  /// When the transaction was added to a batch
  public var batchedAt: Date?

  /// EMV (chip card) response data
  public var emvResponse: String?

  /// Address Verification System response
  public var avsResponse: String?

  /// Card Verification Value response
  public var cvvResponse: String?

  /// Point of Sale entry mode
  public var posEntry: String?

  /// Salesperson who processed the transaction
  public var posSalesperson: String?

  /// When the receipt was sent via email
  public var receiptEmailAt: Date?

  /// When the receipt was sent via SMS
  public var receiptSmsAt: Date?

  /// When the transaction was settled
  public var settledAt: Date?

  /// When the transaction was created
  public var createdAt: Date?

  /// When the transaction was last updated
  public var updatedAt: Date?

  /// ID of the payment gateway used
  public var gatewayId: String?

  /// Issuer authorization code
  public var issuerAuthCode: String?

  /// Channel through which the transaction was processed
  public var channel: String?

  /// Currency used for the transaction
  public var currency: Currency?
  
  /// Child transactions
  public var childCaptures: [StaxTransaction]?
  
  /// `true` if this object is able to be refunded.
  public var isRefundable: Bool?

  /// `true` if this object is able to be voided.
  public var isVoidable: Bool?
  
  /// The response from the gateway
  public var response: JSONCodable?
  
  /// The transaction 'token'. This is technically the id of the transaction in a foreign platform.
  /// The Stax API expects this value when we create a transaction, but we will likely never receive it.
  internal var token: String?
    
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
    case childCaptures = "child_captures"
    case isRefundable = "is_refundable"
    case isVoidable = "is_voidable"
    case response
    case token
  }
    
  /// Creates a new `StaxTransaction` instance
  public init(
    id: String? = nil,
    invoiceId: String? = nil,
    referenceId: String? = nil,
    recurringTransactionId: String? = nil,
    authId: String? = nil,
    type: TransactionType? = nil,
    source: String? = nil,
    sourceIp: String? = nil,
    isMerchantPresent: Bool? = nil,
    merchantId: String? = nil,
    userId: String? = nil,
    customerId: String? = nil,
    paymentMethodId: String? = nil,
    isManual: Bool? = nil,
    spreedlyToken: String? = nil,
    spreedlyResponse: String? = nil,
    success: Bool? = nil,
    message: String? = nil,
    meta: JSONCodable? = nil,
    total: Double? = nil,
    method: String? = nil,
    preAuth: Bool? = nil,
    isCaptured: Int? = nil,
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
    currency: Currency? = nil,
    childCaptures: [StaxTransaction]? = nil,
    isRefundable: Bool? = nil,
    isVoidable: Bool? = nil,
    response: JSONCodable? = nil,
    token: String? = nil
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
    self.childCaptures = childCaptures
    self.response = response
    self.token = token
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
