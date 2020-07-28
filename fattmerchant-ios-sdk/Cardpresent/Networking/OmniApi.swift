//
//  OmniApi.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/10/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Responsible for communicating with Omni
class OmniApi {

  private let debug = false

  enum OmniNetworkingException: OmniException {
    case baseUrlNotFound
    case couldNotParseResponse(String?)
    case unknown(String?)

    static var mess: String = "Omni Networking Exception"

    var detail: String? {
      switch self {
      case .baseUrlNotFound:
        return "Base Url Not Found"
      case .couldNotParseResponse(let cause):
        return cause
      case .unknown(let cause):
        return cause
      }
    }
  }

  /// A Json encoder that should be used for encoding data to send to the Omni API
  func jsonEncoder() -> JSONEncoder {
    let encoder = JSONEncoder()
    encoder.keyEncodingStrategy = .convertToSnakeCase
    return encoder
  }

  /// A Json decoder that should be used for encoding data to send to the Omni API
  func jsonDecoder() -> JSONDecoder {
    let decoder = JSONDecoder()
    decoder.keyDecodingStrategy = .convertFromSnakeCase
    return decoder
  }

  private func log(_ thing: Any) {
    if debug {
      print(thing)
    }
  }

  func getSelf(completion: @escaping (OmniSelf) -> Void, failure: @escaping (OmniException) -> Void ) {
    request(method: "get", urlString: "/self", completion: completion, failure: failure)
  }
  func getMobileReaderSettings(completion: @escaping (MobileReaderDetails) -> Void, failure: @escaping (OmniException) -> Void) {
    request(method: "get", urlString: MobileReaderDetails.resourceEndpoint(), completion: completion, failure: failure)
  }

  /// Posts a void-or-refund to Omni
  /// - Parameters:
  ///   - transactionId: the id of the transaction to void or refund
  ///   - total: the amount, in dollars, to void or refund
  ///   - completion: a block to call once finished. Receives the refund or void transaction
  ///   - error: a block to call if an error is thrown
  func postVoidOrRefund(transactionId: String,
                        total: String? = nil,
                        completion: @escaping (Transaction) -> Void,
                        error: @escaping (OmniException) -> Void) {
    var body: Data?
    if let total = total {
      body = try? jsonEncoder().encode(["total": total])
    }
    request(method: "post",
            urlString: "/transaction/\(transactionId)/void-or-refund",
            body: body,
            completion: completion,
            failure: error)
  }

  var environment: Environment = .DEV
  var apiKey: String?

  func request<T>(method: String, urlString: String, body: Data? = nil, completion: @escaping (T) -> Void, failure: @escaping (OmniException) -> Void) where T: Codable {
    guard let baseUrl = environment.baseUrl() else {
      failure(OmniNetworkingException.baseUrlNotFound)
      return
    }

    log("------ HTTP REQUEST ------")
    let client = Networking(baseUrl)
    client.apiKey = apiKey

    let request = client.urlRequest(path: urlString, body: body)

    log("\(request.httpMethod) \((request.url?.absoluteString) ?? "")")

    if let body = body, let bodyString = String(data: body, encoding: .utf8) {
      log("REQUEST BODY:")
      log(bodyString)
      log("")
    }

    client.dataTask(request: request, method: method) { (_, obj) in

      if let data = obj as? Data {
        if let dataString = String(data: data, encoding: .utf8) {
          self.log("RESPONSE:")
          self.log(dataString)
          self.log("")
        }

        do {
          let jsonDecoder = JSONDecoder()
          jsonDecoder.keyDecodingStrategy = .convertFromSnakeCase
          let model = try jsonDecoder.decode(T.self, from: data)
          completion(model)
        } catch {
          var error: OmniException = OmniNetworkingException.couldNotParseResponse(nil)

          // When the API throws an error it, returns json in the following structure
          // {
          //  "method": [
          //    "The selected method is invalid."
          //  ]
          // }
          if let json = try? JSONSerialization.jsonObject(with: data, options: []) {

            switch json {
            case let map as [String: [String]]:
              let errorStrings = map.values.flatMap {$0}
              error = OmniNetworkingException.unknown(errorStrings.first ?? nil)

            case let arr as [String]:
              error = OmniNetworkingException.unknown(arr.first ?? nil)

            default: break
            }

          }

          failure(error)
        }

      }
    }

  }

}
