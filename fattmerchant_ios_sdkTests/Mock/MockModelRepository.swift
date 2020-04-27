//
//  MockModelRepository.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 3/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

extension ModelRepository {

  func create(model: OmniModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    var m = model
    m.id = "generated_id_123"
    completion(m)
  }

  func update(model: OmniModel, id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    var m = model
    m.id = "generated_id_123"
    completion(m)
  }

  func delete(model: OmniModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler) {
    completion()
  }

  func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    fatalError("Need to implement this")
  }

  func getList(completion: @escaping (PaginatedData<OmniModel>) -> Void, error: @escaping ErrorHandler) {
    fatalError("Need to implement this")
  }
}
