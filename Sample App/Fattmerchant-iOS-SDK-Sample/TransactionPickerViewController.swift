//
//  TransactionPickerViewController.swift
//  Fattmerchant-iOS-SDK-Sample
//
//  Created by Tulio Troncoso on 1/22/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import UIKit
import Fattmerchant

class TransactionPickerViewController: UITableViewController {

  var transactions: [Transaction] = []

  override func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
    let label = UILabel()
    label.insetsLayoutMarginsFromSafeArea = true
    label.text = "Choose a Transaction:"
    label.font = UIFont.preferredFont(forTextStyle: .title1)
    label.backgroundColor = UIColor.white
    label.layoutMargins = UIEdgeInsets(top: 8, left: 8, bottom: 8, right: 8)
    return label
  }

  override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
    let cell = UITableViewCell.init(style: .subtitle, reuseIdentifier: nil)
    let transaction = transactions[indexPath.row]
    cell.textLabel?.text = pretty(transaction: transaction)
    cell.detailTextLabel?.text = transaction.id
    return cell
  }

  override func numberOfSections(in tableView: UITableView) -> Int {
    return 1
  }

  override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
    return transactions.count
  }

  /// Returns a pretty string representing the transaction
  ///
  /// For example: "$5.34 charge to card ending in 7423"
  fileprivate func pretty(transaction: Transaction) -> String {
    let total = String.init(format: "%.2f", transaction.total ?? 0.0)
    let type = transaction.type ?? "<type>"
    let method = transaction.method ?? "<method>"
    let lastFour = transaction.lastFour ?? "<lastfour>"

    return "$\(total) \(type) to \(method) ending in \(lastFour)"
  }

}
