/// Request model for creating or updating a customer.
/// At least one of: firstname, lastname, email, or company must be provided.
struct CustomerRequest: Encodable {
  public let firstname: String?
  public let lastname: String?
  public let company: String?
  public let email: String?
  public let ccEmails: [String]?
  public let ccSms: [String]?
  public let phone: String?
  public let address1: String?
  public let address2: String?
  public let addressCity: String?
  public let addressState: String?
  public let addressZip: String?
  public let addressCountry: String?
  public let notes: String?
  public let reference: String?
  public let allowInvoiceCreditCardPayments: Bool?

  private enum CodingKeys: String, CodingKey {
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
  }

  /// Validates that the request contains at least one required field.
  /// - Throws: StaxValidationError if no required fields are provided
  public func validate() throws {
    guard firstname?.isEmpty == false ||
          lastname?.isEmpty == false ||
          email?.isEmpty == false ||
          company?.isEmpty == false else {
      throw StaxValidationError.missingRequiredFields(
        "Customer requires at least one of the following: firstname, lastname, email, or company"
      )
    }
  }

  public init(
    firstname: String? = nil,
    lastname: String? = nil,
    company: String? = nil,
    email: String? = nil,
    ccEmails: [String]? = nil,
    ccSms: [String]? = nil,
    phone: String? = nil,
    address1: String? = nil,
    address2: String? = nil,
    addressCity: String? = nil,
    addressState: String? = nil,
    addressZip: String? = nil,
    addressCountry: String? = nil,
    notes: String? = nil,
    reference: String? = nil,
    allowInvoiceCreditCardPayments: Bool? = nil
  ) {
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
  }
}
