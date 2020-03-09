//
//  SignatureViewController.swift
//  Fattmerchant-iOS-SDK-Sample
//
//  Created by Tulio Troncoso on 3/6/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import UIKit
import Fattmerchant

class SignatureViewController: SignatureProviding {

  func signatureRequired(completion: @escaping (String) -> Void) {
    DispatchQueue.main.async {
      sleep(60)
      completion("signaturedata")
    }
  }

}
