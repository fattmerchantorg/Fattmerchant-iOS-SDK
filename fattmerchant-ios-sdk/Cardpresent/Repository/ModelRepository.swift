//
//  ModelRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// An object that can perform CRUD operations on a Model
protocol ModelRepository {

    /// The type of the model that this repository deals with.
    /// - Note: This must implement Model
    associatedtype StaxModel: Model

    typealias EmptyCompletionHandler = () -> Void
    typealias ErrorHandler = (StaxException) -> Void
    typealias CompletionHandler = (StaxModel) -> Void

    init(staxApi: StaxApi)

    /// An instance of StaxApi
    ///
    /// This is the object responsible for reaching out to Stax
    var staxApi: StaxApi { get set }

    /// Creates an instance of the model in Stax
    ///
    /// - Parameters:
    ///   - model: the model to be created in Stax
    ///   - completion: block to run upon completion
    ///   - error: the block to run if an error is thrown
    func create(model: StaxModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler)

    /// Updates an instance of the model in Stax
    ///
    /// - Parameters:
    ///   - model: the model to be updated in Stax
    ///   - completion: block to run upon completion
    ///   - id: the id fo the model to be updated in Stax
    ///   - error: the block to run if an error is thrown
    func update(model: StaxModel, id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler)

    /// Deletes an instance of the model in Stax
    ///
    /// - Parameters:
    ///   - model: the model to be created in Stax
    ///   - completion: block to run upon completion
    ///   - error: the block to run if an error is thrown
    func delete(model: StaxModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler)

    /// Gets a model with the given id from Stax
    ///
    /// - Parameters:
    ///   - model: the model to be created in Stax
    ///   - completion: block to run upon completion
    ///   - error: the block to run if an error is thrown
    func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler)

    /// Gets a list of the current type from stax
    ///
    /// - Parameters:
    ///   - completion: block to run upon completion
    ///   - error: the block to run if an error is thrown
    func getList(completion: @escaping (PaginatedData<StaxModel>) -> Void, error: @escaping ErrorHandler)

}

extension ModelRepository {
    internal func jsonEncoder() -> JSONEncoder {
        let encoder = JSONEncoder()
        encoder.keyEncodingStrategy = .convertToSnakeCase
        return encoder
    }
}
