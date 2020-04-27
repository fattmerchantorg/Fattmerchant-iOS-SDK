//
//  ViewController.swift
//  Fattmerchant-iOS-SDK-Sample
//
//  Created by Tulio Troncoso on 1/20/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import UIKit
import Fattmerchant

class ViewController: UIViewController {

  var omni: Omni?

  @IBOutlet weak var activityTextArea: UITextView!
  @IBOutlet weak var initializeButton: UIButton!
  @IBOutlet weak var connectReaderButton: UIButton!
  @IBOutlet weak var getReaderInfoButton: UIButton!
  @IBOutlet weak var refundPaymentButton: UIButton!
  @IBOutlet weak var totalTextInput: UITextField!

  @IBAction func onRefundPaymentButtonPress(_ sender: UIButton) {
    self.fetchTransactions()
  }

  @IBAction func onConnectReaderButtonPress(_ sender: UIButton) {
    self.searchForReaders()
  }

  @IBAction func onTakePaymentButtonPress(_ sender: UIButton) {
    self.takePayment()
  }

  @IBAction func onGetReaderInfoButtonPress(_ sender: UIButton) {
    self.getReaderInfo()
  }

  let apiKey = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJtZXJjaGFudCI6ImViNDhlZjk5LWFhNzgtNDk2ZS05YjAxLTQyMWY4ZGFmNzMyMyIsImdvZFVzZXIiOnRydWUsImJyYW5kIjoiZmF0dG1lcmNoYW50Iiwic3ViIjoiMzBjNmVlYjYtNjRiNi00N2Y2LWJjZjYtNzg3YTljNTg3OThiIiwiaXNzIjoiaHR0cDovL2FwaWRldjAxLmZhdHRsYWJzLmNvbS9hdXRoZW50aWNhdGUiLCJpYXQiOjE1ODczNTgwODUsImV4cCI6MTU4NzQ0NDQ4NSwibmJmIjoxNTg3MzU4MDg1LCJqdGkiOiJHVFprSThFeGdBeWNFY0RuIn0.q8wg7DjlidsU32d6B8qY8gKGp8n0QvRbbcPTBfatLC0"

  override func viewDidLoad() {
    super.viewDidLoad()
    initializeOmni()
  }

  fileprivate func initializeOmni() {
    // instantiate Omni and store somewhere
    omni = Omni()

    log("Attempting initalization...")

    // Initialize Omni
    omni?.initialize(params: initParams(), completion: {
      self.initializeButton.isHidden = true
      self.log("Initialized")
    }, error: { (error) in
      self.log(error)
    })

  }

  fileprivate func chooseTransaction(from transactions: [Transaction], completion: @escaping (Transaction) -> Void) {
    DispatchQueue.main.async {
      let picker = TransactionPickerViewController()
      picker.transactions = transactions
      picker.onTransactionChosen = completion
      self.present(picker, animated: true, completion: nil)
    }
  }

  fileprivate func fetchTransactions() {
    omni?.getMobileReaderTransactions(completion: { transactions in
      self.chooseTransaction(from: transactions) { chosenTransaction in
        self.refund(chosenTransaction)
      }
    }, error: { error in
      self.log(error)
    })
  }

  fileprivate func log(_ error: OmniException) {
    var errorMessage = error.message
    if let detail = error.detail {
      errorMessage += ". \(detail)"
    }
    self.log(errorMessage)
  }

  fileprivate func log(_ message: String) {
    DispatchQueue.main.async {
      let textCount: Int = ("\n \(self.timestamp()) | \(message)" + self.activityTextArea.text).count
      self.activityTextArea.insertText("\n \(self.timestamp()) | \(message)")
      if textCount > 1 {
        self.activityTextArea.scrollRangeToVisible(NSRange(location: textCount - 1, length: 1))
      }
    }
  }

  fileprivate func takePayment() {
    omni?.takeMobileReaderTransaction(request: createTransactionRequest(), completion: { _ in
      self.log("Finished transaction successfully")
    }, error: { error in
      self.log(error)
    })
  }

  fileprivate func createTransactionRequest() -> TransactionRequest {
    let request = TransactionRequest(amount: Amount(cents: 10))
    return request
  }

  fileprivate func refund(_ transaction: Transaction) {
    omni?.refundMobileReaderTransaction(transaction: transaction, refundAmount: Amount(cents: 2), completion: { _ in
      self.log("Refunded transaction successfully")
    }, error: { error in
      self.log(error)
    })
  }

  fileprivate func searchForReaders() {
    log("Searching for available reader...")
    omni?.getAvailableReaders(completion: { readers in
      guard !readers.isEmpty else {
        self.log("No readers found")
        return
      }
      self.log("Found readers:")
      for reader in readers {
        self.log(reader.name)
      }
      self.chooseReader(from: readers) { chosenReader in
        self.connectReader(reader: chosenReader, completion: { connectedReader in
          self.log("Connected reader: \(connectedReader)")
        })
      }
    }, error: { (error) in
      self.log(error)
    })
  }

  fileprivate func connectReader(reader: MobileReader, completion: @escaping (MobileReader) -> Void) {
    self.log("Trying to connect to \(reader)")
    omni?.connect(reader: reader, completion: completion) {
      self.log("Couldn't connect to \(reader)")
    }
  }

  fileprivate func getReaderInfo() {
    self.log("Trying to get info about the connected reader")
    omni?.getConnectedReader(completion: { reader in
      var a = reader
      self.log("Done")
    }, error: { error in
      self.log(error.detail ?? "huh")
    })
  }

  /// Makes the user choose which reader to connect to
  ///
  /// - Parameters:
  ///   - readers: an array of MobileReader to choose from
  ///   - completion: a block to run once the reader is chosen. Will receive a MobileReader
  fileprivate func chooseReader(from readers: [MobileReader], _ completion: @escaping (MobileReader) -> Void ) {
    // If there is only one reader, automatically choose that one
    guard readers.count > 1 else {
      completion(readers[0])
      return
    }

    let pickerController = StringPickerController(values: readers) { reader in
      if let chosenReader = reader {
        completion(chosenReader)
      }

      self.dismiss(animated: true, completion: nil)
    }
    modalPresentationStyle = .pageSheet
    present(pickerController, animated: true, completion: nil)
  }

  /// - Returns: A string for the current time in the format "1:02PM"
  fileprivate func timestamp() -> String {
    let formatter = DateFormatter()
    formatter.dateFormat = DateFormatter.dateFormat(fromTemplate: "hhmm", options: 0, locale: Locale.current)
    return formatter.string(from: Date())
  }

  fileprivate func initParams() -> Omni.InitParams {
    return Omni.InitParams(appId: "fmiossample", apiKey: apiKey, environment: Environment.DEV)
  }

}
