//
//  ViewController.swift
//  Fattmerchant-iOS-SDK-Sample
//
//  Created by Tulio Troncoso on 1/20/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import UIKit
import Fattmerchant

class ViewController: UIViewController, TransactionUpdateDelegate, MobileReaderConnectionStatusDelegate, UserNotificationDelegate {

  var omni: Omni?
  var lastPreauthTransaction: Transaction? = nil

  @IBOutlet weak var activityTextArea: UITextView!
  @IBOutlet weak var initializeButton: UIButton!
  @IBOutlet weak var connectReaderButton: UIButton!
  @IBOutlet weak var disconnectReaderButton: UIButton!
  @IBOutlet weak var getReaderInfoButton: UIButton!
  @IBOutlet weak var refundPaymentButton: UIButton!
  @IBOutlet weak var totalTextInput: UITextField!

  @IBAction func onRefundPaymentButtonPress(_ sender: UIButton) {
    self.fetchTransactions()
  }

  @IBAction func onConnectReaderButtonPress(_ sender: UIButton) {
    #if targetEnvironment(simulator)
    let alertController = UIAlertController(title: "Number of readers wanted", message: "", preferredStyle: UIAlertController.Style.alert)
    alertController.addTextField { (textField : UITextField!) -> Void in
      textField.placeholder = "Enter number of readers needed"
    }
    let saveAction = UIAlertAction(title: "Save", style: UIAlertAction.Style.default, handler: { alert -> Void in
      let firstTextField = alertController.textFields![0] as UITextField
      let numberWanted: Int? = Int(firstTextField.text ?? "")
      self.searchForReaders(numberOfReadersNeeded: numberWanted ?? 0)
    })
    let cancelAction = UIAlertAction(title: "Cancel", style: UIAlertAction.Style.default, handler: {
      (action : UIAlertAction!) -> Void in })
    alertController.addAction(saveAction)
    alertController.addAction(cancelAction)

    self.present(alertController, animated: true, completion: nil)
    #else
    self.searchForReaders()
    #endif
  }

  @IBAction func onDisconnectButtonPress(_ sender: Any) {
    self.disconnectReader()
  }

  @IBAction func onTakePaymentButtonPress(_ sender: UIButton) {
    self.takePayment()
  }

  @IBAction func onPreauthPaymentButtonPress(_ sender: UIButton) {
    self.takePayment(preauth: true)
  }

  @IBAction func onCaptureLastTransactionButtonPress(_ sender: UIButton) {
    self.captureLastTransaction()
  }

  @IBAction func onVoidLastTransactionButtonPress(_ sender: UIButton) {
    self.voidLastTransaction()
  }

  @IBAction func onGetReaderInfoButtonPress(_ sender: UIButton) {
    self.getReaderInfo()
  }

  @IBAction func onCancelTransactionButtonPress(_ sender: UIButton) {
    self.cancelTransaction()
  }

  let apiKey = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJtZXJjaGFudCI6IjQ2NTQ0MTg5LWZmNTItNGQ1Zi05NTMyLTU0ZDA2MmM4Mzk5OCIsImdvZFVzZXIiOmZhbHNlLCJicmFuZCI6ImZhdHRtZXJjaGFudCIsInN1YiI6IjIxODNhODQ1LWMxMjAtNGZkYi04Mzc4LThlZjBkYzhhYjkzYSIsImlzcyI6Imh0dHA6Ly9hcGlkZXYuZmF0dGxhYnMuY29tL2F1dGhlbnRpY2F0ZSIsImlhdCI6MTY1NDI1MzAwNywiZXhwIjoxNjU0MzM5NDA3LCJuYmYiOjE2NTQyNTMwMDcsImp0aSI6ImE2VDM2TEZSUURNT0hzRTUifQ.qgPu-gG_bqj6r3mrPCUCkFimfvXjYfg0pRaHlSXoq9o"

  override func viewDidLoad() {
    super.viewDidLoad()
    initializeOmni()
    totalTextInput.delegate = self

    let tap = UITapGestureRecognizer(target: self.totalTextInput, action: #selector(UIView.endEditing(_:)))
    tap.cancelsTouchesInView = false
    self.view.addGestureRecognizer(tap)
  }

  fileprivate func initializeOmni() {
    // instantiate Omni and store somewhere
    omni = Omni()
    omni?.signatureProvider = SignatureViewController()
    omni?.transactionUpdateDelegate = self
    omni?.userNotificationDelegate = self
    omni?.mobileReaderConnectionUpdateDelegate = self

    log("Attempting initalization...")

    // Initialize Omni
    omni?.initialize(params: initParams(), completion: {
      self.initializeButton.isHidden = true
      self.log("Initialized")
    }, error: { (error) in
      if let initialized = self.omni?.isInitialized, initialized {
        self.initializeButton.isHidden = true
        self.log("Initialized")
      }
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

  fileprivate func takePayment(preauth: Bool = false) {
    var req = createTransactionRequest()
    req.preauth = preauth
    omni?.takeMobileReaderTransaction(request: req, completion: { transaction in
      self.log("Finished transaction successfully")

      if preauth {
        self.lastPreauthTransaction = transaction
      }
    }, error: { error in
      self.log(error)
    })
  }

  fileprivate func captureLastTransaction() {
    guard let id = lastPreauthTransaction?.id else {
      return self.log("No preauth transactions to capture")
    }

    var amount: Amount?

    if let total = lastPreauthTransaction?.total,
       getAmount().dollarsString() != Amount(dollars: total).dollarsString() {
      amount = getAmount()
    }

    omni?.capturePreauthTransaction(transactionId: id, amount: amount, completion: { transaction in
      self.log("Captured transaction successfully")
    }, error: { error in
      self.log(error)
    })
  }

  fileprivate func voidLastTransaction() {
    guard let id = lastPreauthTransaction?.id else {
      return self.log("No preauth transactions to capture")
    }

    omni?.voidTransaction(transactionId: id, completion: { transaction in
      self.log("Voided transaction successfully")
    }, error: { error in
      self.log(error)
    })
  }

  fileprivate func cancelTransaction() {
    omni?.cancelMobileReaderTransaction(completion: { success in
      self.log("Transaction cancelled")
    }, error: { error in
      self.log(error)
    })
  }

  fileprivate func getAmount() -> Amount {
    guard
      let text = totalTextInput.text ?? totalTextInput.placeholder,
      let number = numberFormatter().number(from: text)
      else {
        return Amount(cents: 2)
    }

    return Amount(dollars: number.doubleValue)
  }

  @IBAction fileprivate func payWithCard() {
    self.log("Attempting CNP Transaction")
    let card = CreditCard(personName: "Test Payment", cardNumber: "4111111111111111", cardExp: "0224", addressZip: "32812")
    let transactionRequest = TransactionRequest(amount: getAmount(), card: card)
    omni?.pay(transactionRequest: transactionRequest, completion: { completedTransaction in
      self.log("Finished transaction successfully")
    }, error: { error in
      self.log(error)
    })
  }

  @IBAction fileprivate func payWithBankAccount() {
    fatalError("This feature isn't available yet")
  }

  @IBAction fileprivate func tokenizeCard() {
    self.log("Attempting CNP Tokenization")
    var card = CreditCard.testCreditCard()
    card.personName = "Test Creditcard"
    omni?.tokenize(card, { (paymentMethod) in
      self.log("Created PaymentMethod Successfully")
      self.log(paymentMethod)
    }, error: { (err) in
      self.log(err)
    })
  }

  @IBAction fileprivate func tokenizeBankAccount() {
    self.log("Attempting CNP Tokenization")
    let bank = BankAccount.testBankAccount()
    omni?.tokenize(bank, { (paymentMethod) in
      self.log("Created PaymentMethod Successfully")
      self.log(paymentMethod)
    }, error: { (err) in
      self.log(err)
    })
  }

  fileprivate func createTransactionRequest() -> TransactionRequest {
    var request = TransactionRequest(amount: getAmount())
    return request
  }

  fileprivate func refund(_ transaction: Transaction) {
    omni?.refundMobileReaderTransaction(transaction: transaction, refundAmount: getAmount(), completion: { _ in
      self.log("Refunded transaction successfully")
    }, error: { error in
      self.log(error)
    })
  }

  fileprivate func searchForReaders(numberOfReadersNeeded: Int? = 0) {
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
    }, args: ["readersNeeded": numberOfReadersNeeded])
  }

  fileprivate func connectReader(reader: MobileReader, completion: @escaping (MobileReader) -> Void) {
    self.log("Trying to connect to \(reader)")
    omni?.connect(reader: reader, completion: completion) {
      self.log("Couldn't connect to \(reader)")
    }
  }

  func mobileReaderConnectionStatusUpdate(status: MobileReaderConnectionStatus) {
    self.log("Mobile reader \(status.rawValue)")
  }

  fileprivate func disconnectReader() {
    omni?.getConnectedReader(completion: { reader in
      guard let reader = reader else { return }
      self.omni?.disconnect(reader: reader, completion: { success in
        self.log(success ? "Successfully disconnected reader" : "Could not disconnect reader")
      }, error: { error in
        self.log(error.detail ?? "error disconnecting")
      })
    }, error: { error in
      self.log(error.detail ?? "error getting connected reader")
    })
  }

  fileprivate func getReaderInfo() {
    self.log("Trying to get info about the connected reader")
    omni?.getConnectedReader(completion: { reader in
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

  func onTransactionUpdate(transactionUpdate: TransactionUpdate) {
    self.log(transactionUpdate)
  }

  func onUserNotification(userNotification: UserNotification) {
    self.log(userNotification)
  }

  // MARK: Logging
  fileprivate func log(_ message: String) {
    DispatchQueue.main.async {
      let textCount: Int = ("\n \(self.timestamp()) | \(message)" + self.activityTextArea.text).count
      self.activityTextArea.insertText("\n \(self.timestamp()) | \(message)")
      if textCount > 1 {
        self.activityTextArea.scrollRangeToVisible(NSRange(location: textCount - 1, length: 1))
      }
    }
  }


  fileprivate func log(_ transactionUpdate: TransactionUpdate) {
    var message = "[\(transactionUpdate.value)]"
    if let userFriendlyMessage = transactionUpdate.userFriendlyMessage {
      message += " | \(userFriendlyMessage)"
    }
    self.log(message)
  }

  fileprivate func log(_ userNotification: UserNotification) {
    var message = "[\(userNotification.value)]"
    if let userFriendlyMessage = userNotification.userFriendlyMessage {
      message += " | \(userFriendlyMessage)"
    }
    self.log(message)
  }

  fileprivate func log(_ error: OmniException) {
    var errorMessage = error.message
    if let detail = error.detail {
      errorMessage += ". \(detail)"
    }
    self.log(errorMessage)
  }

  fileprivate func log(_ paymentMethod: PaymentMethod) {
    var message = "PaymentMethod: "
    message += "\n\t id: \(paymentMethod.id ?? "")"
    message += "\n\t customerId: \(paymentMethod.customerId)"
    message += "\n\t method: \(paymentMethod.method?.rawValue ?? "")"
    log(message)
  }
}

extension UIViewController: UITextFieldDelegate {

  fileprivate func numberFormatter() -> NumberFormatter {
    let formatter = NumberFormatter()
    formatter.numberStyle = .currency
    formatter.locale = .current
    return formatter
  }

  public func textFieldDidEndEditing(_ textField: UITextField) {
    guard let text = textField.text else {
      return
    }

    let numberString = text.filter { $0 != "$" }
    let number = Double(numberString)
    textField.text = numberFormatter().string(for: number)
  }

}
