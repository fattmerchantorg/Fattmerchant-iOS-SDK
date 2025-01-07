public enum JSONCodable: Codable {
  case string(String)
  case int(Int)
  case double(Double)
  case bool(Bool)
  case object([String: JSONCodable])
  case array([JSONCodable])
  case null
  
  public init(from decoder: Decoder) throws {
    let container = try decoder.singleValueContainer()
    if let string = try? container.decode(String.self) {
      self = .string(string)
    } else if let int = try? container.decode(Int.self) {
      self = .int(int)
    } else if let double = try? container.decode(Double.self) {
      self = .double(double)
    } else if let bool = try? container.decode(Bool.self) {
      self = .bool(bool)
    } else if let object = try? container.decode([String: JSONCodable].self) {
      self = .object(object)
    } else if let array = try? container.decode([JSONCodable].self) {
      self = .array(array)
    } else if container.decodeNil() {
      self = .null
    } else {
      throw DecodingError.typeMismatch(JSONCodable.self, DecodingError.Context(codingPath: decoder.codingPath, debugDescription: "Unknown JSON value"))
    }
  }
  
  public func encode(to encoder: Encoder) throws {
    var container = encoder.singleValueContainer()
    switch self {
      case .string(let string): try container.encode(string)
      case .int(let int): try container.encode(int)
      case .double(let double): try container.encode(double)
      case .bool(let bool): try container.encode(bool)
      case .object(let object): try container.encode(object)
      case .array(let array): try container.encode(array)
      case .null: try container.encodeNil()
    }
  }
}
