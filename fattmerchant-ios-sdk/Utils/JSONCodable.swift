public enum JSONCodable: Codable, Equatable {
  case string(String)
  case int(Int)
  case double(Double)
  case bool(Bool)
  case object([String: JSONCodable])
  case array([JSONCodable])
  case null
  
  public static func encode<T: Encodable>(_ value: T) throws -> JSONCodable {
    let data = try JSONEncoder().encode(value)
    return try JSONDecoder().decode(JSONCodable.self, from: data)
  }
  
  private var stringValue: String? {
    guard case let .string(value) = self else { return nil }
    return value
  }
  
  private var intValue: Int? {
    guard case let .int(value) = self else { return nil }
    return value
  }
  
  private var doubleValue: Double? {
    if case let .double(value) = self { return value }
    if case let .int(value) = self { return Double(value) }
    return nil
  }
  
  private var boolValue: Bool? {
    guard case let .bool(value) = self else { return nil }
    return value
  }
  
  private var objectValue: [String: JSONCodable]? {
    guard case let .object(value) = self else { return nil }
    return value
  }
  
  private var arrayValue: [JSONCodable]? {
    guard case let .array(value) = self else { return nil }
    return value
  }
  
  private var isNull: Bool {
    if case .null = self { return true }
    return false
  }
  
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
  
  public subscript(key: String) -> JSONCodable? {
    guard case let .object(dict) = self else { return nil }
    return dict[key]
  }
   
   /// Array subscript to access elements by index
  public subscript(index: Int) -> JSONCodable? {
    guard case let .array(array) = self, index >= 0, index < array.count else { return nil }
    return array[index]
  }
  
  public func string(at path: String) -> String? {
    return getValue(at: path)?.stringValue
  }
  
  public func int(at path: String) -> Int? {
    return getValue(at: path)?.intValue
  }
  
  public func double(at path: String) -> Double? {
    return getValue(at: path)?.doubleValue
  }
  
  public func bool(at path: String) -> Bool? {
    return getValue(at: path)?.boolValue
  }
  
  public func object(at path: String) -> [String: JSONCodable]? {
    return getValue(at: path)?.objectValue
  }
  
  public func array(at path: String) -> [JSONCodable]? {
    return getValue(at: path)?.arrayValue
  }
  
  public func isNull(at path: String) -> Bool {
    return getValue(at: path)?.isNull ?? false
  }
  
  private func getValue(at path: String) -> JSONCodable? {
    let components = path.split(separator: ".")
    var current: JSONCodable? = self
      
    for component in components {
      if let index = Int(component) {
        current = current?[index]
      } else {
        current = current?[String(component)]
      }
          
      if current == nil {
        return nil
      }
    }
      
    return current
  }
}
