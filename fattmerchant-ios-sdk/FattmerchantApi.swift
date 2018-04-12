//
//  FattmerchantApi.swift
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

/// Receives events from the FattmerchantApi
public protocol FattmerchantApiDelegate: class {

  /// Called when the FattmerchantApi successfully creates a payment method
  ///
  /// - Parameters:
  ///   - fattmerchantApi: the fattmerchantApi that created the event
  ///   - paymentMethod: the PaymentMethod that was created
  func fattmerchantApi(_ fattmerchantApi: FattmerchantApi, didCreatePaymentMethod paymentMethod: PaymentMethod)

  /// Called when the FattmerchantApi fails at creating a payment method
  ///
  /// - Parameters:
  ///   - fattmerchantApi: the fattmerchantApi that created the event
  ///   - didReceiveError: the error that was thrown
  func fattmerchantApi(_ fattmerchantApi: FattmerchantApi, didReceiveError: Error)
}

/// Object that communicates with the Fattmerchant API
public class FattmerchantApi {

  public enum Error: Swift.Error {
    case tokenizationError([String])
    case unknownError()
  }

  public enum Response {
    case success(paymentMethod: PaymentMethod)
    case failure(errors: Error)
  }

  /// An object used to configure the FattmerchantApi
  public var configuration: FattmerchantConfiguration

  public weak var delegate: FattmerchantApiDelegate?

  /// Initializes FattmerchantApi with the given configuration
  ///
  /// - Parameter configuration: object that configures the FattmerchantApi
  public init(_ configuration: FattmerchantConfiguration) {
    self.configuration = configuration
  }

  /// Initializes FattmerchantApi with the default configuration
  public init() {
    self.configuration = FattmerchantConfiguration.shared
  }

  /// Creates a PaymentMethod out of a CreditCard, then calls the completion block
  ///
  /// - Note: Use the PaymentMethod's id to create payments in Fattmerchant
  ///
  /// - Parameters:
  ///   - card: CreditCard to be tokenized
  ///   - completion: block to run upon completion
  public func tokenize(_ card: CreditCard, _ completion: @escaping (Response) -> Void) {
    tokenize(card, completion: completion)
  }

  /// Creates a PaymentMethod out of a CreditCard, then notifies the FattmerchantApi delegate
  ///
  /// - Parameter card: CreditCard to be tokenized
  public func tokenize(_ card: CreditCard) {
    tokenize(card) { (response) in
      switch response {
      case .success(let paymentMethod):
        self.delegate?.fattmerchantApi(self, didCreatePaymentMethod: paymentMethod)
      case .failure(let error):
        self.delegate?.fattmerchantApi(self, didReceiveError: error)
        break
      }
    }
  }

  /// Creates a PaymentMethod out of a BankAccount, then calls the completion block
  ///
  /// - Note: You want to use the PaymentMethod's id to create payments in Fattmerchant
  ///
  /// - Parameters:
  ///   - bankAccount: BankAccount to be tokenized
  ///   - completion: block to run upon completion
  public func tokenize(_ bankAccount: BankAccount, _ completion: @escaping (Response) -> Void) {
    tokenize(bankAccount, completion: completion)
  }

  /// Creates a PaymentMethod out of a BankAccount, then notifies the FattmerchantApi delegate
  ///
  /// - Parameter bankAccount: BankAccount to be tokenized
  public func tokenize(_ bankAccount: BankAccount) {
    tokenize(bankAccount) { (response) in
      switch response {
      case .success(let paymentMethod):
        self.delegate?.fattmerchantApi(self, didCreatePaymentMethod: paymentMethod)
      case .failure(let error):
        self.delegate?.fattmerchantApi(self, didReceiveError: error)
        break
      }
    }
  }

  private func tokenize<T: Codable>(_ codablePaymentMethod: T, completion: @escaping (Response) -> Void) {
    guard let paymentData = try? JSONEncoder().encode(codablePaymentMethod)  else { return }

    let path = "/webpayment/\(configuration.webPaymentsToken)/tokenize"

    Networking(configuration.baseUrl).post(path, body: paymentData) { (success, obj) in

    if let data = obj as? Data {
      do {
        let paymentMethod = try JSONDecoder().decode(PaymentMethod.self, from: data)
        completion(.success(paymentMethod: paymentMethod))
      } catch {
        var error: Error = .unknownError()

        // When the API is unable to tokenize a payment method because of an error in the json
        // content, e.g. invalid payment method type, it returns json in the following structure
        // {
        //  "method": [
        //    "The selected method is invalid."
        //  ]
        // }
        if let json = try? JSONSerialization.jsonObject(with: data, options: []) {

          switch json {
          case let map as [String: [String]]:
            let errorStrings = map.values.flatMap {$0}
            error = FattmerchantApi.Error.tokenizationError(errorStrings)
            break

          case let arr as [String]:
            error = FattmerchantApi.Error.tokenizationError(arr)
            break

          default: break
          }

        }

        completion(.failure(errors: error))
      }

    }

    }
  }
}
