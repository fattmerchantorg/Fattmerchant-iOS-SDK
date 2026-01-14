public enum StaxInvoiceStatus: String, CaseIterable, Codable {
  case void = "VOID"
  case deleted = "DELETED"
  case draft = "DRAFT"
  case sent = "SENT"
  case viewed = "VIEWED"
  case paid = "PAID"
  case partial = "PARTIAL"
  case attempted = "ATTEMPTED"
}
