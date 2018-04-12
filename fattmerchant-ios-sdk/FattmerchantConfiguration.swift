//
//  FattmerchantConfiguration.swift
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

/// Configures the FattmerchantApi
public struct FattmerchantConfiguration {

  public static var shared = FattmerchantConfiguration(hostedPaymentsId: "changeme")

  /// ID given by Fattmerchant that allows you to create tokenized payment methods
  public var webPaymentsToken: String

  /// Base URL of the Fattmerchant API
  public var baseUrl: URL = URL(string: "https://apiprod01.fattlabs.com")!

  public init(hostedPaymentsId: String) {
    self.webPaymentsToken = hostedPaymentsId
  }

}
