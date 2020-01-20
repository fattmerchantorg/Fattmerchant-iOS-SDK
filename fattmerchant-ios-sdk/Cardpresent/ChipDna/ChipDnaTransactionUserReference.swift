//
//  ChipDnaTransactionUserReference.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 1/17/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation


/// Generates a user reference for chipDNA transactions
///
/// - Returns: a String containing the generated user reference
func generateChipDnaTransactionUserReference() -> String {
  let dateFormatter = DateFormatter()
  dateFormatter.locale = Locale(identifier: "en_US")
  dateFormatter.setLocalizedDateFormatFromTemplate("yy-MM-dd-HH.mm.ss")

  let dateString = dateFormatter.string(from: Date())
  return "CDM-\(dateString)"
}
