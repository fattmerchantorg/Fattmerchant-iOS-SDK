//
//  Meta.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 8/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public struct Meta<T: SingleOrArrayEncoded>: Codable {
  /// Array or Single Object response
  let meta: [T]
}

extension Meta {
  public init(from decoder: Decoder) throws {
    do {
      if let container = try? decoder.container(keyedBy: CodingKeys.self) {
        meta = try container.decode(key: .meta)
      } else {
        meta = [T]()
      }
    } catch {
      print(error.localizedDescription)
      meta = [T]()
    }
  }
}

public struct TransactionMeta: Codable {
  public var nmiUserRef: String?
  public var cardEaseReference: String?
  public var nmiTransactionId: String?
  public var awcTransactionId: String?
  public var gatewayResponse: String?
  public var lineItems: [CatalogItem]?
}

extension TransactionMeta: SingleOrArrayEncoded {
  public enum SingleOrArrayCodingKey: CodingKey {
    case meta
  }
}
