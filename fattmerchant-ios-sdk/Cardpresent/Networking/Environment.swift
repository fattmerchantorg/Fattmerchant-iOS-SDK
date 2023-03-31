//
//  Environment.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/20/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// The Stax environment to use
public enum Environment {
    case LIVE
    case DEV

    func baseUrlString() -> String {
        switch self {
        case .DEV:
            return "https://apidev.fattlabs.com"
        case .LIVE:
            return "https://apiprod.fattlabs.com"
        }
    }

    func baseUrl() -> URL? {
        return URL(string: baseUrlString())
    }

}
