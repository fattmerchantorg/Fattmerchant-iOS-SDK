//
//  ModelRepository+EndpointDefaultImpl.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 3/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Adds default functionality to the ModelRepository where the associated StaxModel type is also an StaxEndpoint
extension ModelRepository where StaxModel: StaxEndpoint {

    init(staxApi: StaxApi) {
        self.init(staxApi: staxApi)
    }

    func create(model: StaxModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
        let data = try? jsonEncoder().encode(model)
        staxApi.request(method: "post", urlString: StaxModel.resourceEndpoint(), body: data, completion: completion, failure: error)
    }

    func update(model: StaxModel, id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
        let data = try? jsonEncoder().encode(model)
        let endpoint = "\(StaxModel.resourceEndpoint())/\(id)"
        staxApi.request(method: "put", urlString: endpoint, body: data, completion: completion, failure: error)
    }

    func delete(model: StaxModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler) {
        //    staxApi.request(method: "delete", urlString: StaxModel.resourceEndpoint(), completion: { _ in completion() }, failure: error)
    }

    func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
        let endpoint = "\(StaxModel.resourceEndpoint())/\(id)"
        staxApi.request(method: "get", urlString: endpoint, completion: completion, failure: error)
    }

    func getList(completion: @escaping (PaginatedData<StaxModel>) -> Void, error: @escaping ErrorHandler) {
        staxApi.request(method: "get", urlString: StaxModel.resourceEndpoint(), completion: completion, failure: error)
    }

}
