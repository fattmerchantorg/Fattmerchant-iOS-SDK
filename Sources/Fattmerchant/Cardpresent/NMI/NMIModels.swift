import Foundation

// MARK: - Processor Models

/// Request model for adding a processor or value-added service to NMI
public struct NMIAddProcessorRequest: Codable {
  /// The merchant ID for whom the processor/service is being added
  let merchantId: String
  
  /// The service ID (e.g., "taptombl", "fdmehvm", etc.)
  let serviceId: String
  
  /// The condition of the service (e.g., "offered", "active")
  let condition: String?
  
  /// The status of the service (e.g., "active", "inactive")
  let status: String?
  
  /// The processor type/name (e.g., "authorize.net", "stripe", etc.) - used for traditional processors
  let processorType: String?
  
  /// The processor's login/username - used for traditional processors
  let login: String?
  
  /// The processor's password/API key - used for traditional processors
  let password: String?
  
  /// The processor's transaction key - used for traditional processors
  let transactionKey: String?
  
  /// Additional processor-specific settings
  let settings: [String: String]?
  
  enum CodingKeys: String, CodingKey {
    case merchantId
    case serviceId
    case condition
    case status
    case processorType
    case login
    case password
    case transactionKey
    case settings
  }
  
  public init(
    merchantId: String,
    serviceId: String,
    condition: String? = nil,
    status: String? = nil,
    processorType: String? = nil,
    login: String? = nil,
    password: String? = nil,
    transactionKey: String? = nil,
    settings: [String: String]? = nil
  ) {
    self.merchantId = merchantId
    self.serviceId = serviceId
    self.condition = condition
    self.status = status
    self.processorType = processorType
    self.login = login
    self.password = password
    self.transactionKey = transactionKey
    self.settings = settings
  }
}

/// Response model for processor operations
public struct NMIProcessorResponse: Codable {
  /// The processor ID
  let processorId: String?
  
  /// Response status (e.g., "success", "error")
  let status: String
  
  /// Response message
  let message: String?
  
  /// Additional response data
  let data: [String: String]?
  
  enum CodingKeys: String, CodingKey {
    case processorId = "processor_id"
    case status
    case message
    case data
  }
}

// MARK: - Transaction Query Models

/// Response model for transaction queries
public struct NMITransactionQueryResponse: Codable {
  /// Transaction ID
  let transactionId: String?
  
  /// Credit card expiration date
  let ccExpiration: String?
  
  /// Transaction amount
  let amount: String?
  
  /// Transaction status
  let status: String?
  
  /// Additional transaction details
  let details: [String: String]?
  
  enum CodingKeys: String, CodingKey {
    case transactionId = "transaction_id"
    case ccExpiration = "cc_exp"
    case amount
    case status
    case details
  }
}

// MARK: - Get Processors Report Models

/// Request to get merchant processors and value-added services details
public struct NMIGetProcessorsReportRequest: Codable {
  /// Number of max results in response. Must be in range from 1 to 1000
  let maxResults: String
  /// Array of merchant IDs (optional)
  let merchantIds: [String]?
  /// Start point for the responses (optional)
  let offset: String?
  
  enum CodingKeys: String, CodingKey {
    case maxResults
    case merchantIds
    case offset
  }
  
  public init(maxResults: String, merchantIds: [String]? = nil, offset: String? = nil) {
    self.maxResults = maxResults
    self.merchantIds = merchantIds
    self.offset = offset
  }
}

/// Response from get processors report endpoint
public struct NMIProcessorsReportResponse: Codable {
  /// The type of object that was returned
  let objectType: String?
  /// The report's ID
  let reportId: Int?
  /// The offset for paginated results
  let offset: Int?
  /// The maximum number of results
  let maxResults: Int?
  /// The total number of results
  let totalResults: Int?
  /// Indicates if there are more results to be fetched
  let hasMore: Bool?
  /// Links for navigating the paginated results
  let links: PaginationLinks?
  /// Array of processor/service objects
  let results: [ProcessorService]?
  
  public struct PaginationLinks: Codable {
    public let first: String?
    public let last: String?
    public let next: String?
    public let previous: String?
  }
  
  public struct ProcessorService: Codable {
    public let objectType: String?
    public let id: Int?
    public let processorType: String?
    public let processorId: String?
    public let processorName: String?
    public let serviceId: String?
    public let merchantId: String?
    public let mcc: String?
    public let accountClassification: String?
    public let maxTicketAmount: String?
    public let maxMonthlyVolume: String?
    public let applyToAllCardType: Bool?
    public let enableDuplicateChecking: Bool?
    public let allowMerchantOverride: Bool?
    public let freeTrialEndDate: String?
    public let freeTrialDaysLeft: String?
    public let duplicateTime: String?
    public let status: String?
    public let condition: String?
    public let created: String?
    public let updated: String?
    public let supportsReroutedRefunds: Bool?
    public let shouldDisableFields: Bool?
    public let disabled: Bool?
    public let refundProcessor: String?
    public let billingType: String?
    public let avsCvvPreCheck: String?
    public let skipAvsCvv: Bool?
    public let paymentTypes: [String]?
    public let currencies: [String]?
    public let settlementTime: String?
    public let settlementTimezone: String?
    public let descriptor: Descriptor?
    public let requiredFields: RequiredFields?
    public let extraDetails: [String: AnyCodable]?
    public let processorFields: [String: AnyCodable]?
    public let faceConfiguration: [String: AnyCodable]?
    public let disabledFields: DisabledFields?
    
    public struct Descriptor: Codable {
      public let descriptor: String?
      public let phone: String?
      public let city: String?
      public let state: String?
      public let url: String?
    }
    
    public struct RequiredFields: Codable {
      public let name: Bool?
      public let company: Bool?
      public let address: Bool?
      public let city: Bool?
      public let state: Bool?
      public let zip: Bool?
      public let country: Bool?
      public let phone: Bool?
      public let email: Bool?
      public let driversLicense: Bool?
      public let driversLicenseState: Bool?
      public let driversLicenseDob: Bool?
      public let ssn: Bool?
      public let cvv: String?
      public let fax: Bool?
      public let url: Bool?
      public let orderId: Bool?
      public let orderDescription: Bool?
      public let poNumber: Bool?
      public let shipping: Bool?
      public let tax: Bool?
      public let shippingName: Bool?
      public let shippingAddress: Bool?
      public let shippingCity: Bool?
      public let shippingState: Bool?
      public let shippingZip: Bool?
      public let shippingCountry: Bool?
      public let mdf1: Bool?
      public let mdf2: Bool?
      public let mdf3: Bool?
      public let mdf4: Bool?
      public let mdf5: Bool?
      public let mdf6: Bool?
      public let mdf7: Bool?
      public let mdf8: Bool?
      public let mdf9: Bool?
      public let mdf10: Bool?
      public let mdf11: Bool?
      public let mdf12: Bool?
      public let mdf13: Bool?
      public let mdf14: Bool?
      public let mdf15: Bool?
      public let mdf16: Bool?
      public let mdf17: Bool?
      public let mdf18: Bool?
      public let mdf19: Bool?
      public let mdf20: Bool?
    }
    
    public struct DisabledFields: Codable {
      public let processorFields: [String: AnyCodable]?
    }
  }
}

/// Helper type to handle dynamic JSON values in processor responses
public enum AnyCodable: Codable {
  case string(String)
  case int(Int)
  case double(Double)
  case bool(Bool)
  case null
  case array([AnyCodable])
  case dictionary([String: AnyCodable])
  
  public init(from decoder: Decoder) throws {
    let container = try decoder.singleValueContainer()
    
    if container.decodeNil() {
      self = .null
    } else if let value = try? container.decode(Bool.self) {
      self = .bool(value)
    } else if let value = try? container.decode(Int.self) {
      self = .int(value)
    } else if let value = try? container.decode(Double.self) {
      self = .double(value)
    } else if let value = try? container.decode(String.self) {
      self = .string(value)
    } else if let value = try? container.decode([AnyCodable].self) {
      self = .array(value)
    } else if let value = try? container.decode([String: AnyCodable].self) {
      self = .dictionary(value)
    } else {
      throw DecodingError.dataCorruptedError(
        in: container,
        debugDescription: "AnyCodable value cannot be decoded"
      )
    }
  }
  
  public func encode(to encoder: Encoder) throws {
    var container = encoder.singleValueContainer()
    
    switch self {
    case .string(let value):
      try container.encode(value)
    case .int(let value):
      try container.encode(value)
    case .double(let value):
      try container.encode(value)
    case .bool(let value):
      try container.encode(value)
    case .null:
      try container.encodeNil()
    case .array(let value):
      try container.encode(value)
    case .dictionary(let value):
      try container.encode(value)
    }
  }
}
