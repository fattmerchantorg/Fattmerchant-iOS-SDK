//
//  MobileReader.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public class MobileReader: CustomStringConvertible {
  public var name: String

  init(name: String) {
    self.name = name
  }

  public var description: String { return self.name }
}
