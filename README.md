# Fattmerchant iOS SDK

The Fattmerchant iOS SDK provides a simple way to accept a payment on your iOS app by providing tokenization of payment methods. By using these tokens instead of card and bank information, you no longer have to worry about sending sensitive card information to your server.

* [Requirements](#requirements)
* [Installation](#installation)
* [Getting Started](#getting-started)
* [Testing](#testing)
* [Taking a payment with a mobile reader](https://fattmerchantorg.github.io/Fattmerchant-iOS-SDK/cardpresent)

![Tokenization Info](https://raw.githubusercontent.com/fattmerchantorg/Fattmerchant-iOS-SDK/master/assets/images/tokenization-info.png)

## Try it out! Please!
The Fattmerchant iOS SDK comes with a Swift Playground. All you need to get started is your `web payments token`, provided to you by your trusty Fattmerchant Account Manager. You can find the playground, called `Tokenization.Playground` in the project root 🤓

## <a name="requirements">Requirements</a>

* Xcode 8
* iOS 9

## <a name="installation">Installation</a>
Use CocoaPods  to install the Fattmerchant iOS SDK.

#### CocoaPods
1. Install [CocoaPods](https://guides.cocoapods.org/using/getting-started.html)
2. Add `pod 'Fattmerchant'` to your `Podfile`
3. Run `pod install`

## <a name="getting-started">Getting Started</a>
To accept a payment, you'll need to collect information from the customer, tokenize it, and send the token to your server. Your server will then be responsible for using the Fattmerchant API to run the transaction.

#### Setup

You'll first need to setup the `FattmerchantApi` for usage.  All you have to do here is set the `webPaymentsToken` field on the shared `FattmerchantConfiguration`. `FattmerchantApi` will then use that configuration by default.

```swift
import UIKit
import Fattmerchant

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

  func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
    FattmerchantConfiguration.shared.webPaymentsToken = "mywebpaymentstoken"
    // ...
    return true
  }
}
```

Alternatively, you may create a configuration object and pass it to the new `FattmerchantApi` instance as you need it.

```swift
let configuration = FattmerchantConfiguration(webPaymentsToken: "mywebpaymentstoken")
let fattClient = FattmerchantApi(configuration: configuration)
```

#### Collect payment information
You first want to collect credit card information and populate a `CreditCard` or a `BankAccount` object.

```swift
let card = CreditCard(personName: "Joan Parsnip",
                      cardNumber: "4111111111111111",
                      cardExp: "1220",
                      addressZip: "32814")


// Or for a bank account...
let bankAccount = BankAccount(routingNumber: "021000021",
                              accountNumber: "38294738291937485",
                              bankHolderType: .personal,
                              accountType: .checking)
```

#### Associate a Customer
If you want to associate a Customer with the new PaymentMethod, all you have to do is pass is set the `customerId` on the `CreditCard` or `BankAccount`

```swift
let card = CreditCard(personName: "Joan Parsnip",
                      cardNumber: "4111111111111111",
                      cardExp: "1220",
                      addressZip: "32814",
                      customerId: "7404cae1-86ba-408c-bb43-8c5cacfdcaab")
```

#### Get a payment method token
Once you have a `CreditCard` object, call the `tokenize(:)` method on  `FattmerchantAPI` object and pass a block to run once tokenization is complete.

```swift
let fattClient = FattmerchantApi(webPaymentsToken: "mywebpaymentstoken")
fattClient.tokenize(card) { (response) in
  if case let .success(paymentMethod) = response {
    let paymentToken = paymentMethod.id
    print("I must now use \(paymentToken) to create a payment.")
  }
  // Success! You can now run a transaction with Fattmerchant using paymentToken as the PaymentMethod
}
```

Or you can set a delegate to be notified.

```swift
class MyClass: FattmerchantApiDelegate {

  func gottaHaveThatFunc() {
    // ...
    let fattClient = FattmerchantApi(webPaymentsToken: "mywebpaymentstoken")
    fattClient.delegate = self
    fattClient.tokenize(card)
  }

  func fattmerchantApi(_ fattmerchantApi: FattmerchantApi, didCreatePaymentMethod paymentMethod: PaymentMethod) {
    let paymentToken = paymentMethod.token
    // You can now run a transaction with Fattmerchant using paymentToken as the PaymentMethod
  }
  
  func fattmerchantApi(_ fattmerchantApi: FattmerchantApi, didReceiveError: Error) {
    if case let .tokenizationError(errors) = error {
      print("Uh oh! 😡 We got errors!")
      errors.forEach { print($0) }
    }
  }
}
```


#### Using the token
Now that you have the token representing the payment method, you can send it to your server to run a payment with it. You have to setup a way for your backend to accept the token and create a transaction with it.

## <a name="testing">Testing</a>
If you'd like to try tokenization without real payment information, you can use the `CreditCard.testCreditCard()` or `BankAccount.testBankAccount()` methods to get a test credit card or bank account.

```swift
let creditCard = CreditCard.testCreditCard()

let bankAccount = BankAccount.testBankAccount()
```

If you want to test failures, you can use the following methods

```swift
let failingCreditCard = CreditCard.failingTestCreditCard()

let failingBankAccount = BankAccount.failingTestBankAccount()
```

Or you can create the `CreditCard` or `BankAccount` object with the following testing payment information:

#### Credit card numbers

| Card Type | Good Card | Bad Card |
|---------|--------------------|-----------|
|VISA|4111111111111111|4012888888881881|
|Mastercard|5555555555554444|5105105105105100|
|Amex|378282246310005|371449635398431|
|Discover|6011111111111117|6011000990139424|
|JCB|3569990010030400|3528327757705979|
|Diners Club|30569309025904|30207712915383|

> Use any CVV number for the above

#### Bank routing & account numbers

* Routing: 021000021
* Account: 9876543210

To test failing bank accounts, use the given routing number and any other account number
