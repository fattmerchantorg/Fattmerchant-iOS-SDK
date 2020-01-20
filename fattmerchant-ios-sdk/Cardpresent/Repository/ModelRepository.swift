//
//  ModelRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

protocol ModelRepository {

  /// The type of the model that this repository deals with.
  /// - Note: This must implement Model
  associatedtype OmniModel: Model

  typealias EmptyCompletionHandler = () -> Void
  typealias ErrorHandler = (OmniException) -> Void
  typealias CompletionHandler = (OmniModel) -> Void

  init(omniApi: OmniApi)

  /// An instance of OmniApi
  ///
  /// This is the object responsible for reaching out to Omni
  var omniApi: OmniApi { get set }

  /// Creates an instance of the model in Omni
  ///
  /// - Parameters:
  ///   - model: the model to be created in Omni
  ///   - completion: block to run upon completion
  ///   - error: the block to run if an error is thrown
  func create(model: OmniModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler)

  /// Updates an instance of the model in Omni
  ///
  /// - Parameters:
  ///   - model: the model to be updated in Omni
  ///   - completion: block to run upon completion
  ///   - error: the block to run if an error is thrown
  func update(model: OmniModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler)

  /// Deletes an instance of the model in Omni
  ///
  /// - Parameters:
  ///   - model: the model to be created in Omni
  ///   - completion: block to run upon completion
  ///   - error: the block to run if an error is thrown
  func delete(model: OmniModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler)

  /// Gets a model with the given id from Omni
  ///
  /// - Parameters:
  ///   - model: the model to be created in Omni
  ///   - completion: block to run upon completion
  ///   - error: the block to run if an error is thrown
  func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler)

  /// Gets a list of the current type from omni
  ///
  /// - Parameters:
  ///   - completion: block to run upon completion
  ///   - error: the block to run if an error is thrown
  func getList(completion: @escaping CompletionHandler, error: @escaping ErrorHandler)

}

extension ModelRepository {
  func create(model: OmniModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {}
  func update(model: OmniModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {}
  func delete(model: OmniModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler) {}
  func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {}
  func getList(completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {}
}

extension ModelRepository where OmniModel: OmniEndpoint {

  init(omniApi: OmniApi) {
    self.init(omniApi: omniApi)
  }

  func create(model: OmniModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    let data = try? JSONEncoder().encode(model)
    omniApi.request(method: "post", urlString: OmniModel.resourceEndpoint(), body: data, completion: completion, failure: error)
  }

  func update(model: OmniModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    let data = try? JSONEncoder().encode(model)
    omniApi.request(method: "put", urlString: OmniModel.resourceEndpoint(), body: data, completion: completion, failure: error)
  }

  func delete(model: OmniModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler) {
//    omniApi.request(method: "delete", urlString: OmniModel.resourceEndpoint(), completion: { _ in completion() }, failure: error)
  }

  func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    let endpoint = "\(OmniModel.resourceEndpoint())/\(id)"
    omniApi.request(method: "get", urlString: endpoint, completion: completion, failure: error)
  }

  func getList(completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
    omniApi.request(method: "get", urlString: OmniModel.resourceEndpoint(), completion: completion, failure: error)
  }

}
