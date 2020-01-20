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

  var omni: Omni!

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

  let apiKey =
    "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJtZXJjaGFudCI6ImViNDhlZjk5LWFhNzgtNDk2ZS05YjAxLTQyMWY4ZGFmNzMy"
  + "MyIsImdvZFVzZXIiOnRydWUsInN1YiI6IjMwYzZlZWI2LTY0YjYtNDdmNi1iY2Y2LTc4N2E5YzU4Nzk4YiIsImlzcyI6Imh0dHA6L"
  + "y9hcGlkZXYwMS5mYXR0bGFicy5jb20vYXV0aGVudGljYXRlIiwiaWF0IjoxNTc"
  + "5NTA0MzcwLCJleHAiOjE1Nzk1OTA3NzAsIm5iZiI6MTU3OTUwNDM3MCwianRpIjoiNEEwTjlOM25ob0hpTm1BS"
  + "iJ9.6YCODhA-ogmRhiAWeRhXvgm0WmxL_0iVnKpwJ--E0_s"

  override func viewDidLoad() {
    super.viewDidLoad()
    initializeOmni()
  }

  fileprivate func initializeOmni() {
    omni = Omni()
    log("Attempting initalization...")
    omni.initialize(params: initParams(), completion: {
      self.initializeButton.isHidden = true
      self.log("Initialized")
    }) { (error) in
      self.log(error.message)
    }
  }

  fileprivate func log(_ message: String) {
    DispatchQueue.main.async {
      self.activityTextArea.insertText("\n \(self.timestamp()) | \(message)")
    }
  }

  fileprivate func takePayment() {
    omni.takeMobileReaderTransaction(request: createTransactionRequest(), completion: { completedTransaction in
      self.log("Finished transaction successfully")
    }) { error in
      self.log(error.message)
    }
  }

  fileprivate func createTransactionRequest() -> TransactionRequest {
    let request = TransactionRequest(amount: Amount(cents: 1))
    return request
  }

  fileprivate func refundPayment() {

  }

  fileprivate func searchForReaders() {
    log("Attempting to connect reader...")
    omni.getAvailableReaders(completion: { readers in

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
      self.log(error.message)
    }
  }

  fileprivate func connectReader(reader: MobileReader, completion: @escaping (MobileReader) -> Void) {
    omni.connect(reader: reader, completion: completion) {
      self.log("Couldn't connect reader")
    }
  }

  fileprivate func chooseReader(from readers: [MobileReader], _ completion: (MobileReader) -> Void ) {
    completion(readers.first!)
  }

  fileprivate func timestamp() -> String {
    let formatter = DateFormatter()
    formatter.dateFormat = DateFormatter.dateFormat(fromTemplate: "hhmm", options: 0, locale: Locale.current)
    return formatter.string(from: Date())
  }

  fileprivate func initParams() -> Omni.InitParams {
    return Omni.InitParams(appId: "fmiossample", apiKey: apiKey, environment: Environment.DEV)
  }

}
