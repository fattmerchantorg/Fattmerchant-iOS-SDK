//
//  WebpaymentsTokenize.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 4/2/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

enum TokenizePaymentMethodException: OmniException {
  static var mess: String = "Could not tokenize payment method"

  case tokenizationError
  case couldNotParsePaymentMethodError
  case merchantMissingHostedPaymentsToken
  case noCreditCardOrBankAccountSupplied
}

/// Tokenizes a payment method
class TokenizePaymentMethod {

  typealias Exception = TokenizePaymentMethodException

  /// Responsible for communicating with Omni
  var omniApi: OmniApi

  /// The Merchant that will be associated with the tokenized payment method
  var merchant: Merchant

  /// The card to tokenize. If supplied, bankAccount must be nil
  var creditCard: CreditCard?

  /// The bank account to tokenize. If supplied, creditCard must be nil
  var bankAccount: BankAccount?

  var customerRepository: CustomerRepository
  var paymentMethodRepository: PaymentMethodRepository

  /// Initializes a TokenizePaymentMethod instance with OmniApi and Merchant
  init(omniApi: OmniApi, merchant: Merchant) {
    self.omniApi = omniApi
    self.merchant = merchant
    customerRepository = CustomerRepository(omniApi: omniApi)
    paymentMethodRepository = PaymentMethodRepository(omniApi: omniApi)
  }

  func start(completion: @escaping (PaymentMethod) -> Void, failure: @escaping (OmniException) -> Void) {

    guard let customerName = creditCard?.personName ?? bankAccount?.personName else {
      return failure(CreateCustomerException.customerNameNotSupplied)
    }

    createCustomer(customerName) { (customer, error) in
      guard error == nil, let customer = customer else {
        return failure(error ?? CreateCustomerException.unknown)
      }

      var paymentMethod: PaymentMethod

      if let card = self.creditCard {
        paymentMethod = PaymentMethod(card: card, customer: customer)
      } else if let bank = self.bankAccount {
        paymentMethod = PaymentMethod(bank: bank, customer: customer)
      } else {
        return failure(TokenizePaymentMethodException.noCreditCardOrBankAccountSupplied)
      }

      self.paymentMethodRepository.create(model: paymentMethod, completion: completion, error: failure)
    }
  }

  private func createCustomer(_ card: CreditCard, completion: @escaping (Customer?, OmniException?) -> Void) {
    createCustomer(card.personName, completion: completion)
  }

  private func createCustomer(_ bankAccount: BankAccount, completion: @escaping (Customer?, OmniException?) -> Void) {
    createCustomer(bankAccount.personName, completion: completion)
  }

  private func createCustomer(_ name: String, completion: @escaping (Customer?, OmniException?) -> Void) {
    let customerToCreate = Customer(fullName: name)
    customerRepository.create(model: customerToCreate) { (createdCustomer) in
      completion(createdCustomer, nil)
    } error: { (error) in
      completion(nil, error)
    }
  }

}
