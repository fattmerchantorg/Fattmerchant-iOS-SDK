//
//  Omni.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/16/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

public class Omni {

  private var transactionRepository = TransactionRepository()
  private var invoiceRepository = InvoiceRepository()
  private var customerRepository = CustomerRepository()
  private var paymentMethodRepository = PaymentMethodRepository()
  private var mobileReaderDriverRepository = MobileReaderDriverRepository()
  private var omniApi = OmniApi()

  public struct InitParams {
    var appId: String
    var apiKey: String

    public init(appId: String, apiKey: String) {
      self.appId = appId
      self.apiKey = apiKey
    }
  }

  public init() { }

  public func initialize(params: InitParams, completion: @escaping () -> Void, error: @escaping (OmniException) -> Void) {
    omniApi = OmniApi()
    omniApi.apiKey = params.apiKey
    omniApi.environment = OmniApi.Environment.DEV //TODO: Need to pass in environment in InitParams

    // Verify the apikey corresponds to a real merchant
    omniApi.getSelf(completion: { myself in

      guard let merchant = myself.merchant else {
        fatalError("Couldn't get merchant from getself")
      }

      let args: [String: Any] = [
        "appId": params.appId,
        "merchant": merchant
      ]

      print("Initializing drivers")
      InitializeDrivers(mobileReaderDriverRepository: self.mobileReaderDriverRepository, args: args).start(completion: { _ in
        print("Completed initilization")
        completion()
      }, failure: error)
    }, failure: error)

  }

}
