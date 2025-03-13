import Foundation

struct StaxMerchant: Codable {
  let id: String?
  let mid: String?
  let status: String?
  let companyName: String?
  let displayName: String?
  let options: StaxMerchantOptions?
  
  enum CodingKeys: String, CodingKey {
    case id, mid, status
    case companyName = "company_name"
    case displayName = "display_name"
    case options
  }
}
