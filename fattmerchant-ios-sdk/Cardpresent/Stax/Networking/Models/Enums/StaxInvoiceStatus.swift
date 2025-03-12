public enum StaxInvoiceStatus: String, CaseIterable, Codable {
  case void = "void"
  case deleted = "deleted"
  case draft = "draft"
  case sent = "sent"
  case viewed = "viewed"
  case paid = "paid"
  case partial = "partial"
  case attempted = "attempted"
}
