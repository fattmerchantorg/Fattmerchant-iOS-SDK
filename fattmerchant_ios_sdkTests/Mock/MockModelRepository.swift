//
//  MockModelRepository.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 3/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

internal var modelStore: [String: Model] = [:]

extension ModelRepository {

  func uniqueId() -> String {
    return UUID().uuidString
  }

  func create(model: StaxModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    var m = model
    let id = uniqueId()
    m.id = id
    modelStore[id] = m
    completion(m)
  }

  func update(model: StaxModel, id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    var m = model
    m.id = id
    modelStore[id] = m
    completion(m)
  }

  func delete(model: StaxModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler) {
    if let id = model.id {
      modelStore.removeValue(forKey: id)
    }
    completion()
  }

  func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    if let model = modelStore[id] as? Self.StaxModel {
      completion(model)
    } else {
      error(StaxApi.StaxNetworkingException.unknown("Model not found"))
    }
  }

  func getList(completion: @escaping (PaginatedData<StaxModel>) -> Void, error: @escaping ErrorHandler) {
    fatalError("Need to implement this")
  }
}
