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

  func getSelf(completion: @escaping (Self) -> Void, failure: @escaping (OmniException) -> Void ) {
    request(method: "get", urlString: "/self", completion: completion, failure: failure)
  }

  var environment: Environment = .DEV
  var apiKey: String?

  func request<T>(method: String, urlString: String, body: Data? = nil, completion: @escaping (T) -> Void, failure: @escaping (OmniException) -> Void) where T: Codable {
    guard let baseUrl = environment.baseUrl() else {
      failure(OmniNetworkingException.baseUrlNotFound)
      return
    }

    print("------ HTTP REQUEST ------")
    let client = Networking(baseUrl)
    client.apiKey = apiKey

    let request = client.urlRequest(path: urlString, body: body)

    print("\(request.httpMethod) \((request.url?.absoluteString) ?? "")")

    if let body = body, let bodyString = String(data: body, encoding: .utf8) {
      print("REQUEST BODY:")
      print(bodyString)
      print()
    }

    client.dataTask(request: request, method: method) { (success, obj) in

      if let data = obj as? Data {
        if let dataString = String(data: data, encoding: .utf8) {
          print("RESPONSE:")
          print(dataString)
          print()
        }

        do {
          let jsonDecoder = JSONDecoder()
          jsonDecoder.keyDecodingStrategy = .convertFromSnakeCase
          let model = try jsonDecoder.decode(T.self, from: data)
          completion(model)
        } catch DecodingError.typeMismatch(let type, let context) {
          print(context)
        } catch let decodingError as DecodingError {
          print(decodingError)
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
