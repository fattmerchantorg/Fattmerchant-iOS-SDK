//
//  String+Blank.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 8/1/22.
//  Copyright © 2022 Fattmerchant. All rights reserved.
//

import Foundation

extension String {

    /// Checks to see if the receiver contains non-whitespace characters
    /// - Returns: true if the string is only made of whitespace
    func isBlank() -> Bool {
        let trimmed = trimmingCharacters(in: CharacterSet.whitespacesAndNewlines)
        return trimmed.isEmpty
    }
}
