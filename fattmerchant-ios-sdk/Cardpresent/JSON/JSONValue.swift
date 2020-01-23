//
//  JSONValue.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/14/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public enum JSONValue: Codable, Equatable {

  public enum JSONValueError: Error {
    case InitializationError(message: String)
  }

  case string(String)
  case int(Int)
  case double(Double)
  case bool(Bool)
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
    } else if let value = from as? [Codable] {
      if let jsonValueArray = value.map({ JSONValue($0) }).filter({ $0 != nil }) as? [JSONValue] {
        jsonValue = .array(jsonValueArray)
      }
    } else if let value = from as? [String: Codable] {
      let jsonDictionary = value.mapValues({ JSONValue($0) })
      jsonValue = .object(jsonDictionary)
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
    } else if let value = try? container.decode([String: JSONValue?].self) {
      self = .object(value)
    } else if let value = try? container.decode([JSONValue].self) {
      self = .array(value)
    } else {
      throw DecodingError.typeMismatch(JSONValue.self, DecodingError.Context(codingPath: container.codingPath, debugDescription: "Not valid JSON"))
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
    case .object(let value):
      try container.encode(value)
    case .array(let value):
      try container.encode(value)
    }
  }

}

extension Dictionary where Value: Codable, Key == String {
  func jsonValue() -> JSONValue? {
    return JSONValue(self)
  }
}
