import Foundation

public class PaginatedResponse<T : Codable>: Codable {
  var data: [T]?
  var total: Int?
}
