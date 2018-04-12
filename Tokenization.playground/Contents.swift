//: # Fattmerchant Payment Method Tokenization
import Foundation
import Fattmerchant
import PlaygroundSupport; PlaygroundPage.current.needsIndefiniteExecution = true

//: ### Easy peasy setup
FattmerchantConfiguration.shared.webPaymentsToken = "<your webpayments token here>"

/*:
 #### ¡WARNING!
 > Please DO NOT send more than five tokenization requests per minute.
 > You will be rate limited, and that would be awful.
 */

//: ### Supa fast credit card tokenization
let card = CreditCard.testCreditCard()
FattmerchantApi().tokenize(card) { (response) in
  if case let .success(paymentMethod) = response {
    let paymentToken = paymentMethod.id
    print("I must now use \(paymentToken) to create a payment.")
  }
}

//: ### What about errors?
let failingCard = CreditCard.failingTestCreditCard()
FattmerchantApi().tokenize(failingCard) { (response) in
  switch response {
  case .success(let paymentMethod):
    print("My payment token is \(paymentMethod.id)\nI must now use this to create a payment")
  case .failure(let error):
    if case let .tokenizationError(errors) = error {
      print("Uh oh! We got errors!")
      errors.forEach { print($0) }
    }
  }
}

//: ### But I like the delegate pattern...
class MyClass: FattmerchantApiDelegate {
  func fattmerchantApi(_ fattmerchantApi: FattmerchantApi, didCreatePaymentMethod paymentMethod: PaymentMethod) {
    print("My payment token is \(paymentMethod.id)\nI must now use this to create a payment")
  }

  func fattmerchantApi(_ fattmerchantApi: FattmerchantApi, didReceiveError: Error) {
    print("😡")
  }
}

let bankAccount = BankAccount.testBankAccount()
let fattClient = FattmerchantApi()
let delegate = MyClass()
fattClient.delegate = delegate
fattClient.tokenize(bankAccount)
