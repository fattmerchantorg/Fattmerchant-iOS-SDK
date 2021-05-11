# Fattmerchant iOS SDK

The Fattmerchant iOS SDK provides a simple way to accept a payment on your iOS app by providing tokenization of payment methods. By using these tokens instead of card and bank information, you no longer have to worry about sending sensitive card information to your server.

* [Requirements](#requirements)
* [Installation](#installation)
* [Getting Started](#getting-started)
* [Testing](#testing)

# iOS Swiped Payments Guide

## Mobile Reader Payments

Supercharge your mobile app by quickly adding mobile reader payments using the Omni Mobile SDK. These payments will create invoices, customers, and transaction objects in the Stax platform. You can also choose to have the payment method stored within Stax so you can use it from the Stax API.

### How it works

1. You'll first need to create an ephemeral key to initialize the `Omni` object.
2. Then you'll create a `TransactionRequest` that holds all necessary data to take a payment.
3. Finally, you'll ask `Omni` to take the payment by calling the `takeMobileReaderPayment()` method, passing in the `TransactionRequest` and a block to run once the payment is complete.

***

## Requirements

* Xcode 8+
* iOS 9+
* Ephemeral Stax API key

***

# Installation

Use CocoaPods to install the Fattmerchant iOS SDK.

1. Install [CocoaPods](https://guides.cocoapods.org/using/getting-started.html){:target="_blank" rel="noreferrer"}
2. Add `pod 'Fattmerchant'` to your `Podfile`
3. Run `pod install`

***

# Getting Started

## Setup Info.plist

In order to build and run with the Cardpresent functionality, you must include the following in your project's `Info.plist`

* **NSBluetoothAlwaysUsageDescription**: Provide a value here to let your users know why Bluetooth access is required

## Initialize

Create an instance of `InitParams`

```swift
var initParams = Omni.InitParams(appId: "fmiossample", apiKey: apiKey, environment: Environment.DEV)
```

Pass the initParams to `Omni.initialize(...)`, along with a completion lambda and an error lambda

```swift
omni = Omni()

log("Attempting initalization...")

// Initialize Omni
omni?.initialize(params: initParams, completion: {
  // Initialized!
}) { (error) in

}
```

***

# Connect a Mobile Reader

In order to connect a mobile reader, you must first search for a list of available readers

```swift
omni.getAvailableReaders { readers ->

}
```

Once you have the list of available ones, you can choose which one you'd like to connect

```swift
omni?.getAvailableReaders(completion: { readers in
  guard !readers.isEmpty else {
    self.log("No readers found")
    return
  }

  var chosenReader = ... // Choose a reader

  omni.connect(reader: chosenReader, completion: { connectedReader in
    self.log("Connected reader: \(connectedReader)")
  }) { (error) in
    // Something went wrong
  }
}) {
  self.log("Couldn't connect to the mobile reader")
}
```

***

# Take a Payment

To take a payment, simply create a `TransactionRequest` and pass it along to `omni.takeMobileReaderTransaction(...)`

```swift
// Create an Amount
let amount = Amount(cents: 50)

// Create the TransactionRequest
let request = TransactionRequest(amount: amount)

// Take the payment
omni.takeMobileReaderTransaction(request, { completedTransaction in
    // Payment successful!
}) {
    // Error
}
```

By default, the PaymentMethod used in the Transaction is tokenized for reuse. This allows the PaymentMethod to be used from the Stax Virtual Terminal and via the Stax API. To opt-out of tokenization, you can set the `tokenize` field of `TransactionRequest` to `false`

```swift
// Create a TransactionRequest with no tokenization
let request = TransactionRequest(amount: amount, tokenize: false)
```

***

# Refund a Payment

You can use the [Stax API]({{ site.api_ref_url }}#reference/0/transactions){:target="_blank" rel="noreferrer"} to do so.
Once you get the transaction, you can use the `refundMobileReaderTransaction` method to attempt the refund.

```swift
// Attain a transaction
var transaction = Transaction()

// Perform refund
omni.refundMobileReaderTransaction(transaction: transaction, completion: { (refundedTransaction) in
  // Refund successful!
}, error: { error in
  // Error
})
```

