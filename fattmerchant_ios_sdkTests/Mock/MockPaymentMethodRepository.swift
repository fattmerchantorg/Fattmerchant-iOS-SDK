//
//  MockPaymentMethodRepository.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 3/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class MockPaymentMethodRepository: PaymentMethodRepository {

  override func createTokenizedPaymentMethod(model: PaymentMethod, completion: @escaping (PaymentMethod) -> Void, error: @escaping PaymentMethodRepository.ErrorHandler) {
    let paymentMethod = model
    paymentMethod.id = "generated_id_123"
    completion(paymentMethod)
  }
}
