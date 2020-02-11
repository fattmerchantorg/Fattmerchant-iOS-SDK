//
//  Networking.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 4/5/18.
//  Copyright © 2018 Fattmerchant, inc. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

import Foundation

internal class Networking {

  var baseUrl: URL
  var apiKey: String?

  init(_ baseUrl: URL) {
    self.baseUrl = baseUrl
  }

  func dataTask(request: NSMutableURLRequest, method: String, completion: @escaping (Bool, Any?) -> Void) {
    request.httpMethod = method

    let session = URLSession(configuration: .default)

    session.dataTask(with: request as URLRequest) { (data, _, _) in
      completion(data != nil, data)
    }.resume()
  }

  func get(request: NSMutableURLRequest, completion: @escaping (Bool, Any?) -> Void) {
    dataTask(request: request, method: "GET", completion: completion)
  }

  func get(_ path: String, completion: @escaping (Bool, Any?) -> Void) {
    dataTask(request: urlRequest(path: path), method: "GET", completion: completion)
  }

  func post(_ path: String, body: Data? = nil, completion: @escaping (Bool, Any?) -> Void) {
    let request = urlRequest(path: path, body: body)
    dataTask(request: request, method: "POST", completion: completion)
  }

  func put(_ path: String, body: Data? = nil, completion: @escaping (Bool, Any?) -> Void) {
    let request = urlRequest(path: path, body: body)
    dataTask(request: request, method: "PUT", completion: completion)
  }

  func urlRequest(path: String, body: Data? = nil) -> NSMutableURLRequest {
    // Set url
    let url = baseUrl.appendingPathComponent(path)
    let request = NSMutableURLRequest(url: url)

    // Set headers
    request.addValue("application/json", forHTTPHeaderField: "Content-Type")
    request.addValue("application/json", forHTTPHeaderField: "Accept")

    if let token = apiKey {
      request.addValue("Bearer \(token)", forHTTPHeaderField: "Authorization")
    }

    // Set body
    request.httpBody = body

    return request
  }

}
