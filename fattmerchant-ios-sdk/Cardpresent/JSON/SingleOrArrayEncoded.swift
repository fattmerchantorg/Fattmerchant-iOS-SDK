//
//  SingleOrArrayEncoded.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 8/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public protocol SingleOrArrayEncoded: Codable {
  associatedtype SingleOrArrayCodingKey: CodingKey
}

extension KeyedDecodingContainer {
  func decode<Decodable: SingleOrArrayEncoded>(key: Key) throws -> [Decodable] {
    let nestedContainer = try self.nestedContainer(
      keyedBy: Decodable.SingleOrArrayCodingKey.self,
      forKey: key
    )

    let key = nestedContainer.allKeys.first!

    do {
      return try nestedContainer.decode([Decodable].self, forKey: key)
    } catch {
      return try [nestedContainer.decode(Decodable.self, forKey: key)]
    }
  }
}
