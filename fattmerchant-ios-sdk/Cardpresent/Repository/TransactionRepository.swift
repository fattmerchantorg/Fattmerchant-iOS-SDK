//
//  TransactionRepository.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/15/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class TransactionRepository: ModelRepository {
    typealias StaxModel = Transaction
    var staxApi: StaxApi

    required init(staxApi: StaxApi) {
        self.staxApi = staxApi
    }
}
