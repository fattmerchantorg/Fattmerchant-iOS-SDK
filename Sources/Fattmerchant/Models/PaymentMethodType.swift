//
//  PaymentMethodType.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 4/6/18.
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

/// Types of payment methods. Typically, 'card' and 'bank'
///
/// - card: A credit card
/// - bank: A bank account
public enum PaymentMethodType: String, Codable {
  case card, bank
}
