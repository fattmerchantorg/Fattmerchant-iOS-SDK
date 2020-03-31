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

  override func getSelf(completion: @escaping (OmniSelf) -> Void, failure: @escaping (OmniException) -> Void ) {

    if let omniSelf = omniSelf {
      completion(omniSelf)
    } else {
      failure(OmniNetworkingException.unknown("Could not get self"))
    }

  }

}
