import Foundation

public indirect enum JSONValue: Codable, Equatable {

    public enum JSONValueError: Error {
        case InitializationError(message: String)
    }

    case string(String)
    case int(Int)
    case double(Double)
    case bool(Bool)
    case item(Item)
    case object([String: JSONValue?])
    case array([JSONValue])

    public init?(_ from: Codable) {
        var jsonValue: JSONValue?

        if let value = from as? String {
            jsonValue = .string(value)
        } else if let value = from as? Int {
            jsonValue = .int(value)
        } else if let value = from as? Double {
            jsonValue = .double(value)
        } else if let value = from as? Bool {
            jsonValue = .bool(value)
        } else if let value = from as? Item {
            jsonValue = .item(value)
        } else if let value = from as? [Codable] {
            if let jsonValueArray = value.map({ JSONValue($0) }).filter({ $0 != nil }) as? [JSONValue] {
                jsonValue = .array(jsonValueArray)
            }
        } else if let value = from as? [String: Codable] {
            let jsonDictionary = value.mapValues({ JSONValue($0) })
            jsonValue = .object(jsonDictionary)
        } else if let value = from as? JSONValue {
            if case let JSONValue.array(from) = from {
                jsonValue = .array(from as [JSONValue])
            } else {
                jsonValue = value
            }
        }

        if jsonValue != nil {
            self = jsonValue!
        } else {
            return nil
        }
    }

    subscript(key: String) -> JSONValue? {
        if case let JSONValue.object(dict) = self, dict.keys.contains(key) {
            return dict[key] ?? nil
        }
        return nil
    }

    subscript(key: String) -> Int? {
        if case let JSONValue.object(dict) = self, dict.keys.contains(key) {
            guard
                let val = dict[key],
                val != nil,
                case let JSONValue.int(i) = val!
            else {
                return nil
            }

            return i
        }
        return nil
    }

    subscript(key: String) -> Double? {
        if case let JSONValue.object(dict) = self, dict.keys.contains(key) {
            guard
                let val = dict[key],
                val != nil,
                case let JSONValue.double(i) = val!
            else {
                return nil
            }
            return i
        }
        return nil
    }

    subscript(key: String) -> String? {
        if case let JSONValue.object(dict) = self, dict.keys.contains(key) {
            guard
                let val = dict[key],
                val != nil,
                case let JSONValue.string(i) = val!
            else {
                return nil
            }

            return i
        }
        return nil
    }

    subscript<T>(key: String) -> T? {
        if case let JSONValue.object(dict) = self, dict.keys.contains(key) {
            return (dict[key] as? T) ?? nil
        }
        return nil
    }

    public init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        if let value = try? container.decode(String.self) {
            self = .string(value)
        } else if let value = try? container.decode(Int.self) {
            self = .int(value)
        } else if let value = try? container.decode(Double.self) {
            self = .double(value)
        } else if let value = try? container.decode(Bool.self) {
            self = .bool(value)
        } else if let value = try? container.decode(Item.self) {
            self = .item(value)
        } else if let value = try? container.decode([String: JSONValue?].self) {
            self = .object(value)
        } else if let value = try? container.decode([JSONValue].self) {
            self = .array(value)
        } else {
            throw DecodingError.typeMismatch(
                JSONValue.self,
                DecodingError.Context(
                    codingPath: container.codingPath,
                    debugDescription: "Not valid JSON"
                )
            )
        }
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()

        switch self {
        case .string(let value):
            try container.encode(value)
        case .int(let value):
            try container.encode(value)
        case .double(let value):
            try container.encode(value)
        case .bool(let value):
            try container.encode(value)
        case .item(let value):
            try container.encode(value)
        case .object(let value):
            try container.encode(value)
        case .array(let value):
            try container.encode(value)
        }
    }
}

extension JSONValue {
    func toDictionary() -> [String: Any?] {
        var dict = [String: Any?]()
        if case let JSONValue.object(jsonDict) = self {
            for key in jsonDict.keys {
                switch jsonDict[key] {
                case .array(let value):
                    if let array = extractElements(from: value) {
                        dict[key] = array
                    }
                case .string(let value):
                    dict[key] = value
                case .bool(let value):
                    dict[key] = value
                case .double(let value):
                    dict[key] = value
                case .int(let value):
                    dict[key] = value
                case .item(let value):
                    dict[key] = value
                default:
                    continue
                }
            }
        }
        return dict
    }

    func extractElements(from array: [JSONValue]) -> [Any]? {
        var extractedArray = [Any]()
        for element in array {
            switch element {
            case .string(let string):
                extractedArray.append(string)
            case .bool(let bool):
                extractedArray.append(bool)
            case .double(let double):
                extractedArray.append(double)
            case .int(let int):
                extractedArray.append(int)
            case .item(let item):
                extractedArray.append(item)
            default:
                continue
            }
        }
        return extractedArray
    }
}

extension Dictionary where Value: Codable, Key == String {
    func jsonValue() -> JSONValue? {
        return JSONValue(self)
    }
}
