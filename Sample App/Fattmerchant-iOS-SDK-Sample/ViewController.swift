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

  var omni: Omni? = nil

  @IBOutlet weak var activityTextArea: UITextView!
  @IBOutlet weak var initializeButton: UIButton!
  @IBOutlet weak var connectReaderButton: UIButton!
  @IBOutlet weak var refundPaymentButton: UIButton!
  @IBOutlet weak var totalTextInput: UITextField!

  @IBAction func onRefundPaymentButtonPress(_ sender: UIButton) {
    self.refundPayment()
  }

  @IBAction func onConnectReaderButtonPress(_ sender: UIButton) {
    self.searchForReaders()
  }

  @IBAction func onTakePaymentButtonPress(_ sender: UIButton) {
    self.takePayment()
  }

  let apiKey = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJtZXJjaGFudCI6ImE2MWQ3OGNjLWNkZTktNDRhYy04YTE4LTMwYzM5YmUwNTg3OSIsImdvZFVzZXIiOnRydWUsImJyYW5kIjoiZmF0dG1lcmNoYW50Iiwic3ViIjoiMzBjNmVlYjYtNjRiNi00N2Y2LWJjZjYtNzg3YTljNTg3OThiIiwiaXNzIjoiaHR0cDovL2FwaWRldjAxLmZhdHRsYWJzLmNvbS9hdXRoZW50aWNhdGUiLCJpYXQiOjE1Nzk3OTMzMzQsImV4cCI6MTU3OTg3OTczNCwibmJmIjoxNTc5NzkzMzM0LCJqdGkiOiJxZUpNdFM0Uk5EYUc2UmEwIn0.QbvtLHPgu-oaBXOtbDjvhEPuRGz8ecsx7ZHjSpZTZk4"

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
    }) { (error) in
      self.log(error)
    }

  }

  fileprivate func chooseTransaction(from transactions: [Transaction], completion: (Transaction?) -> Void) {

    DispatchQueue.main.async {
      let picker = TransactionPickerViewController()
      picker.transactions = transactions
      self.present(picker, animated: true, completion: nil)
    }
  }

  fileprivate func fetchTransactions() {
    omni?.getMobileReaderTransactions(completion: { transactions in
      self.chooseTransaction(from: transactions) { chosenTransaction in
        self.refundPayment()
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
      self.activityTextArea.insertText("\n \(self.timestamp()) | \(message)")
    }
  }

  fileprivate func takePayment() {
    omni?.takeMobileReaderTransaction(request: createTransactionRequest(), completion: { completedTransaction in
      self.log("Finished transaction successfully")
    }) { error in
      self.log(error)
    }
  }

  fileprivate func createTransactionRequest() -> TransactionRequest {
    let request = TransactionRequest(amount: Amount(cents: 1))
    return request
  }

  fileprivate func refundPayment() {
    fetchTransactions()
  }

  fileprivate func searchForReaders() {
    log("Attempting to connect reader...")

    omni?.getAvailableReaders(completion: { readers in

      // Make sure we have at least one reader available
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
          self.log("Connected reader: \(connectedReader.name)")
        })
      }

    }) { (error) in
      self.log(error)
    }
  }

  fileprivate func connectReader(reader: MobileReader, completion: @escaping (MobileReader) -> Void) {
    omni?.connect(reader: reader, completion: completion) {
      self.log("Couldn't connect reader")
    }
  }

  /// Makes the user choose which reader to connect to
  ///
  /// - Parameters:
  ///   - readers: an array of MobileReader to choose from
  ///   - completion: a block to run once the reader is chosen. Will receive a MobileReader
  fileprivate func chooseReader(from readers: [MobileReader], _ completion: (MobileReader) -> Void ) {
    completion(readers.first!)
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
