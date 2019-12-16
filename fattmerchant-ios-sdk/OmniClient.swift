//
//  OmniClient.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 12/9/19.
//  Copyright © 2019 Fattmerchant. All rights reserved.
//

import Foundation
import shared

public class Omni {

  public init() {}

  public func connectMobileReader() {
    CoroutineTesting().testmebro { (str) in
      print(str)
    }
  }

  public func plzwork(c: @escaping (String) -> Void) {
    CoroutineTesting().testmebro(completion: c)
  }
}
