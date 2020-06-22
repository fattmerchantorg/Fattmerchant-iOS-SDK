//
//  OmniApi.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/10/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Responsible for communicating with Omni
class MockOmniApi: OmniApi {

  var omniSelf: OmniSelf?

  struct Stub {
    enum Response {
      case success(_ value: Any)
      case failure(_ value: OmniException)
    }

    let method: String
    let url: String
    let body: Data?
    let completionResponse: Response
  }

  /// This block runs before the mock omni api has executed one of the completion blocks, and is a good opportunity
  /// to, for example, verify that the method has been called at all.
  ///
  /// All the arguments of the request are passed in here, so you can verify that a request is being attempted and
  /// with which params
  ///
  /// Return true in the block if you want the request to proceed, false otherwise.
  typealias Spy = (_ method: String, _ urlString: String, _ body: Data?) -> Bool

  fileprivate var stubs: [Stub] = []
  fileprivate var spies: [Spy] = []

  func stub(_ method: String, _ url: String, body: Data?, response: Stub.Response) {
    stubs.append(Stub(method: method, url: url, body: body, completionResponse: response))
  }

  func spy(_ spy: @escaping Spy) {
    spies.append(spy)
  }

  override func getSelf(completion: @escaping (OmniSelf) -> Void, failure: @escaping (OmniException) -> Void ) {
    if let omniSelf = omniSelf {
      completion(omniSelf)
    } else {
      failure(OmniNetworkingException.unknown("Could not get self"))
    }
  }

  override func request<T>(method: String, urlString: String, body: Data? = nil, completion: @escaping (T) -> Void, failure: @escaping (OmniException) -> Void) where T : Decodable, T : Encodable {

    // See if any of the spies want to prevent further execution
    for spy in spies {
      if spy(method, urlString, body) == true {
        return
      }
    }

    if let stub = stubs.first(where: {
      $0.method == method
        && $0.url == urlString
        && $0.body == body
    }) {
      switch stub.completionResponse {
      case .success(let value):
        completion(value as! T)
      case .failure(let value):
        failure(value)
      }
    } else {
      super.request(method: method, urlString: urlString, body: body, completion: completion, failure: failure)
    }
  }

}
