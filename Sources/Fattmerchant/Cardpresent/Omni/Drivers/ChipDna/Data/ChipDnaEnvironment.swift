#if !targetEnvironment(simulator)
//
//  ChipDnaEnvironment.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 11/7/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

public enum ChipDnaEnvironment {
  case LIVE
  case TEST

  func baseUrlString() -> String {
    switch self {
    case .TEST:
      return CCValueEnvironmentTest
    case .LIVE:
      return CCValueEnvironmentLive
    }
  }
}
#endif
