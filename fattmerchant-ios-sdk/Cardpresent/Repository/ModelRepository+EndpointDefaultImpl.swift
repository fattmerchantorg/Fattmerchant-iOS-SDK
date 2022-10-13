//
//  ModelRepository+EndpointDefaultImpl.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 3/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Adds default functionality to the ModelRepository where the associated OmniModel type is also an OmniEndpoint
extension ModelRepository where OmniModel: OmniEndpoint {

    init(omniApi: OmniApi) {
        self.init(omniApi: omniApi)
    }

    func create(model: OmniModel, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
        let data = try? jsonEncoder().encode(model)
        omniApi.request(method: "post", urlString: OmniModel.resourceEndpoint(), body: data, completion: completion, failure: error)
    }

    func update(model: OmniModel, id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
        let data = try? jsonEncoder().encode(model)
        let endpoint = "\(OmniModel.resourceEndpoint())/\(id)"
        omniApi.request(method: "put", urlString: endpoint, body: data, completion: completion, failure: error)
    }

    func delete(model: OmniModel, completion: @escaping EmptyCompletionHandler, error: @escaping ErrorHandler) {
        //    omniApi.request(method: "delete", urlString: OmniModel.resourceEndpoint(), completion: { _ in completion() }, failure: error)
    }

    func getById(id: String, completion: @escaping CompletionHandler, error: @escaping ErrorHandler) {
        let endpoint = "\(OmniModel.resourceEndpoint())/\(id)"
        omniApi.request(method: "get", urlString: endpoint, completion: completion, failure: error)
    }

    func getList(completion: @escaping (PaginatedData<OmniModel>) -> Void, error: @escaping ErrorHandler) {
        omniApi.request(method: "get", urlString: OmniModel.resourceEndpoint(), completion: completion, failure: error)
    }

}
