struct StaxUser: Codable {
  let id: String?
  let name: String?
  let email: String?
  let brand: String?
  let isSystemAdmin: Bool?
  let isTeamAdmin: Bool?
  let isTeamEnabled: Bool?
  let role: String?
  let roleId: String?
  
  enum CodingKeys: String, CodingKey {
    case id, name, email, brand
    case isSystemAdmin = "system_admin"
    case isTeamAdmin = "team_admin"
    case isTeamEnabled = "team_enabled"
    case role = "team_role"
    case roleId = "role_id"
  }
}
