//
//  StaxApi.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/10/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Responsible for communicating with Stax
class MockStaxApi: StaxApi {

  var staxSelf: StaxSelf?

  struct Stub {
    enum Response {
      case success(_ value: Any)
      case failure(_ value: StaxException)
    }

    let method: String
    let url: String
    let body: Data?
    let completionResponse: Response
  }

  /// This block runs before the mock stax api has executed one of the completion blocks, and is a good opportunity
  /// to, for example, verify that the method has been called at all.
  ///
  /// All the arguments of the request are passed in here, so you can verify that a request is being attempted and
  /// with which params
  ///
  /// Return true in the block if you want the request to proceed, false otherwise.
  typealias Spy = (request: (method: String, urlString: String, body: Data?), shouldProceed: () -> Bool)

  fileprivate var stubs: [Stub] = []
  fileprivate var expectedRequests: [Spy] = []

  func stub(_ method: String, _ url: String, body: Data?, response: Stub.Response) {
    stubs.append(Stub(method: method, url: url, body: body, completionResponse: response))
  }

  func expectRequest(method: String, urlString: String, body: Data? = nil, shouldProceed: @escaping () -> Bool) {
    let spy = ((method, urlString, body), shouldProceed)
    expectedRequests.append(spy)
  }

  override func getSelf(completion: @escaping (StaxSelf) -> Void, failure: @escaping (StaxException) -> Void ) {
    if let staxSelf = staxSelf {
      completion(staxSelf)
    } else {
      failure(StaxNetworkingException.unknown("Could not get self"))
    }
  }

  override func request<T>(method: String, urlString: String, body: Data? = nil, completion: @escaping (T) -> Void, failure: @escaping (StaxException) -> Void) where T : Decodable, T : Encodable {

    // See if any of the spies want to prevent further execution
    for request in expectedRequests {
      if request.request.method == method && request.request.urlString == urlString && request.request.body == body {
        if request.shouldProceed() == false {
          return
        }
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
