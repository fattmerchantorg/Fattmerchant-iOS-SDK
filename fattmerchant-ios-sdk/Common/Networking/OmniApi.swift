//
//  OmniApi.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/10/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class OmniApi {

  enum Environment {
    case LIVE
    case DEV

    func baseUrl() -> String {
      switch self {
      case .DEV:
      return "https://apiprod.fattlabs.com/"
      case .LIVE:
        return "https://apidev.fattlabs.com/"
      }
    }

  }

}
