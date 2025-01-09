public enum StaxInvoiceStatus: String, Codable {
  case void = "VOID"
  case deleted = "DELETED"
  case draft = "DRAFT"
  case sent = "SENT"
  case viewed = "VIEWED"
  case paid = "PAID"
  case partial = "PARTIALLY APPLIED"
  case attempted = "ATTEMPTED"
}
