//
//  OmniException.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/13/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// An error in the Omni platform
public protocol OmniException: Error {
  static var mess: String { get }
  var message: String { get }
  var detail: String? { get }
}

extension OmniException {
  var message: String {
    return Self.mess
  }

  var detail: String? {
    return String(describing: self)
  }
}
