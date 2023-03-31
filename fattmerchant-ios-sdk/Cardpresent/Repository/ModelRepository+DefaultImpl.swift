//
//  ModelRepository+DefaultImpl.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 3/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

extension ModelRepository {
    func create(model: StaxModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {}
    func update(model: StaxModel, id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {}
    func delete(model: StaxModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler) {}
    func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {}
    func getList(completion: @escaping (PaginatedData<StaxModel>) -> Void, error: @escaping ErrorHandler) {}
}
