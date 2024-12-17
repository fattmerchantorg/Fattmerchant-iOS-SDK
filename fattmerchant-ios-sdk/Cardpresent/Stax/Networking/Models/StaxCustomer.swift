/// Represents a customer in the Stax API.
/// At least one of: firstname, lastname, email, or company must be provided.
public struct StaxCustomer: Codable {
  /// Unique identifier for the customer
  public let id: String
  
  /// Customer's first name - required if no lastname, email, or company
  public let firstname: String?
  
  /// Customer's last name - required if no firstname, email, or company
  public let lastname: String?
  
  /// Customer's company name - required if no firstname, lastname, or email
  public let company: String?
  
  /// Customer's email address - required if no firstname, lastname, or company
  public let email: String?
  
  /// Additional email addresses to CC on communications
  public let ccEmails: [String]?
  
  /// Additional phone numbers to CC on SMS communications
  public let ccSms: [String]?
  
  /// Customer's primary phone number
  public let phone: String?
  
  /// First line of customer's address
  public let address1: String?
  
  /// Second line of customer's address
  public let address2: String?
  
  /// City of customer's address
  public let addressCity: String?
  
  /// State/province of customer's address
  public let addressState: String?
  
  /// Postal code of customer's address
  public let addressZip: String?
  
  /// Country of customer's address
  public let addressCountry: String?
  
  /// Additional notes about the customer
  public let notes: String?
  
  /// Customer reference number or identifier
  public let reference: String?
  
  /// Whether the customer can pay invoices via credit card
  public let allowInvoiceCreditCardPayments: Bool?
  
  /// Whether the customer has a complete address
  public let hasAddress: Bool?
  
  /// Reference to parent customer if this customer was merged
  public let parentMerge: String?
  
  /// References to child customers that were merged into this one
  public let childMerges: [String]?
  
  /// Timestamp when the customer was created
  public let createdAt: Date?
  
  /// Timestamp when the customer was last updated
  public let updatedAt: Date?
  
  private enum CodingKeys: String, CodingKey {
    case id
    case firstname
    case lastname
    case company
    case email
    case ccEmails = "cc_emails"
    case ccSms = "cc_sms"
    case phone
    case address1 = "address_1"
    case address2 = "address_2"
    case addressCity = "address_city"
    case addressState = "address_state"
    case addressZip = "address_zip"
    case addressCountry = "address_country"
    case notes
    case reference
    case allowInvoiceCreditCardPayments = "allow_invoice_credit_card_payments"
    case hasAddress = "has_address"
    case parentMerge = "parent_merge"
    case childMerges = "child_merges"
    case createdAt = "created_at"
    case updatedAt = "updated_at"
  }
}
