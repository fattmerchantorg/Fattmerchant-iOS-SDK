import Foundation

/// Represents a customer in the Stax API.
/// At least one of: firstname, lastname, email, or company must be provided.
public struct StaxCustomer: Codable {
  /// Unique identifier for the customer
  public let id: String?
  
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

  
  public var name: String {
    return "\(firstname ?? "") \(lastname ?? "")".trimmingCharacters(in: .whitespacesAndNewlines)
  }
  
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
  }
  
  public static func from(name: String) -> StaxCustomer {
    var first = ""
    var last = ""
    var names = name.split(separator: " ")
    if !names.isEmpty {
      first = String(names.removeFirst())
      last = names.joined(separator: " ")
    }
    
    return StaxCustomer(firstname: first, lastname: last)
  }
  
  /// Creates a `StaxCustomer` instance. All of the fields are optional, but you may encounter
  /// validation errors upon saving with the API. Check the documentation for the latest info on the
  /// minimum required fields for saving a Customer with the Stax API.
  /// - Parameter firstname: The customer's first name.
  /// - Parameter lastname: The customer's last name.
  /// - Parameter company: The company the customer is associated with.
  /// - Parameter email: The primary email associated with the customer.
  /// - Parameter ccEmails: An array of secondary emails associated with the customer.
  /// - Parameter phone: The primary phone associated with the customer.
  /// - Parameter ccSms: An array of secondary phone numbers associated with the customer.
  /// - Parameter address1: The first line of the address associated with the customer.
  /// - Parameter address2: The second line of the address associated with the customer.
  /// - Parameter addressCity: The city of the address associated with the customer.
  /// - Parameter addressState: The state of the address associated with the customer.
  /// - Parameter addressZip: The zip code associated with the customer.
  /// - Parameter addressCountry: The country associated with the customer.
  /// - Parameter notes: Any additional notes about the customer.
  /// - Parameter reference: A reference number associated with the customer.
  /// - Parameter allowInvoiceCreditCardPayments: Whether or not to allow invoices to be sent to this customer.
  /// - Parameter hasAddress: An optional parameter for whether addresses are included in transactions for this customer.
  /// - Parameter parentMerge: The ID of the parent customer this customer has been merged with.
  /// - Parameter childMerge: The children IDs of customers that have merged into this customer.
  /// - Returns: The immutable `StaxCustomer` instance. Use `.updating()` to create a mutable instance.
  public init(
    firstname: String? = nil,
    lastname: String? = nil,
    company: String? = nil,
    email: String? = nil,
    ccEmails: [String]? = nil,
    phone: String? = nil,
    ccSms: [String]? = nil,
    address1: String? = nil,
    address2: String? = nil,
    addressCity: String? = nil,
    addressState: String? = nil,
    addressZip: String? = nil,
    addressCountry: String? = nil,
    notes: String? = nil,
    reference: String? = nil,
    allowInvoiceCreditCardPayments: Bool? = nil,
    hasAddress: Bool? = nil,
    parentMerge: String? = nil,
    childMerges: [String]? = nil
  ) {
    self.id = nil
    self.firstname = firstname
    self.lastname = lastname
    self.company = company
    self.email = email
    self.ccEmails = ccEmails
    self.ccSms = ccSms
    self.phone = phone
    self.address1 = address1
    self.address2 = address2
    self.addressCity = addressCity
    self.addressState = addressState
    self.addressZip = addressZip
    self.addressCountry = addressCountry
    self.notes = notes
    self.reference = reference
    self.allowInvoiceCreditCardPayments = allowInvoiceCreditCardPayments
    self.hasAddress = hasAddress
    self.parentMerge = parentMerge
    self.childMerges = childMerges
  }
  
  /// Creates a `StaxCustomer` instance from an existing customer and mutable properties.
  /// - Parameter from: The existing `StaxCustomer` object to use as a base.
  /// - Parameter changes: An array of changes to apply to the `StaxCustomer`.
  /// - Returns: The copied, mutable object.
  private init(from existing: StaxCustomer, changes: [String: Any]) {
    self.id = existing.id
    self.firstname = (changes["firstname"] as? String) ?? existing.firstname
    self.lastname = (changes["lastname"] as? String) ?? existing.lastname
    self.company = (changes["company"] as? String) ?? existing.company
    self.email = (changes["email"] as? String) ?? existing.email
    self.ccEmails = (changes["ccEmails"] as? [String]) ?? existing.ccEmails
    self.ccSms = (changes["ccSms"] as? [String]) ?? existing.ccSms
    self.phone = (changes["phone"] as? String) ?? existing.phone
    self.address1 = (changes["address1"] as? String) ?? existing.address1
    self.address2 = (changes["address2"] as? String) ?? existing.address2
    self.addressCity = (changes["addressCity"] as? String) ?? existing.addressCity
    self.addressState = (changes["addressState"] as? String) ?? existing.addressState
    self.addressZip = (changes["addressZip"] as? String) ?? existing.addressZip
    self.addressCountry = (changes["addressCountry"] as? String) ?? existing.addressCountry
    self.notes = (changes["notes"] as? String) ?? existing.notes
    self.reference = (changes["reference"] as? String) ?? existing.reference
    self.allowInvoiceCreditCardPayments = (changes["allowInvoiceCreditCardPayments"] as? Bool) ?? existing.allowInvoiceCreditCardPayments
    self.hasAddress = existing.hasAddress
    self.parentMerge = existing.parentMerge
    self.childMerges = existing.childMerges
  }
  
  /// Creates a `StaxCustomer.Update` instance with mutable properties.
  /// - Returns: The copied, mutable object.
  public func updating() -> Update {
    Update(customer: self)
  }
  
  /// Wrapper for handling customer updates with explicit mutability
  @dynamicMemberLookup
  public struct Update {
    private var customer: StaxCustomer
    private var changes: [String: Any]
      
    internal init(customer: StaxCustomer) {
      self.customer = customer
      self.changes = [:]
    }

    public subscript(dynamicMember member: String) -> Any? {
      get { changes[member] ?? getValue(for: member) }
      set { changes[member] = newValue }
    }
        
    private func getValue(for member: String) -> Any? {
      switch member {
        case "firstname": return customer.firstname
        case "lastname": return customer.lastname
        case "company": return customer.company
        case "email": return customer.email
        case "ccEmails": return customer.ccEmails
        case "ccSms": return customer.ccSms
        case "phone": return customer.phone
        case "address1": return customer.address1
        case "address2": return customer.address2
        case "addressCity": return customer.addressCity
        case "addressState": return customer.addressState
        case "addressZip": return customer.addressZip
        case "addressCountry": return customer.addressCountry
        case "notes": return customer.notes
        case "reference": return customer.reference
        case "allowInvoiceCreditCardPayments": return customer.allowInvoiceCreditCardPayments
        default: return nil
      }
    }
        
    internal var modifiedFields: [String: Any] {
      changes
    }
    
    /// Converts a `StaxCustomer.Update` instance into a `StaxCustomer` instance with the pending changes.
    /// - Returns: The moedified, now immutable, `StaxCustomer` object.
    public func apply() -> StaxCustomer {
      StaxCustomer(from: customer, changes: changes)
    }
  }
}
