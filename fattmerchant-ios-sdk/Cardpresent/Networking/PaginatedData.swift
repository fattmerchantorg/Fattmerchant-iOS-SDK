//
//  PaginatedData.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 1/23/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

class PaginatedData<T: Codable>: Codable {
    var data: [T]?
}
