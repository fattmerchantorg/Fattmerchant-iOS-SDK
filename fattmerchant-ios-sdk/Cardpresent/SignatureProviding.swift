//
//  StaxSignatureProviding.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 3/3/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Provides a signature to Stax when required
public protocol SignatureProviding {

    /// Called when a transaction requires a signature
    /// - Parameter completion: a block to run once the signature is complete. This should be given the signature
    func signatureRequired(completion: @escaping (String) -> Void)
}
