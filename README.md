# Stax iOS SDK

The Stax iOS SDK provides a simple way to accept a payment on your iOS app by providing tokenization of payment methods. By using these tokens instead of card and bank information, you no longer have to worry about sending sensitive card information to your server.

* [Requirements](#requirements)
* [Installation](#installation)
* [Getting Started](#getting-started)
* [Testing](#testing)

# iOS Swiped Payments Guide

## Mobile Reader Payments

Supercharge your mobile app by quickly adding mobile reader payments using the Stax iOS SDK. These payments will create invoices, customers, and transaction objects in the Stax platform. You can also choose to have the payment method stored within Stax so you can use it from the Stax API.

### How it works

1. You'll first need to create an ephemeral key to initialize the `Omni` object.
2. Then you'll create a `TransactionRequest` that holds all necessary data to take a payment.
3. Finally, you'll ask `Omni` to take the payment by calling the `takeMobileReaderPayment()` method, passing in the `TransactionRequest` and a block to run once the payment is complete.

***

## Requirements

* Xcode 16.0+
* iOS 15+
* Stax API key

***

# Installation

## Swift Package Manager

1. In Xcode, go to **File** → **Add Package Dependencies**
2. Enter the repository URL: `https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK.git`
3. Choose the version rule (recommended: **Up to Next Major**)
4. Click **Add Package**

Alternatively, add it to your `Package.swift` file:

```swift
dependencies: [
    .package(url: "https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK.git", from: "2.4.3")
]
```

## CocoaPods

Use CocoaPods to install the Stax iOS SDK.

1. Install [CocoaPods](https://guides.cocoapods.org/using/getting-started.html){:target="_blank" rel="noreferrer"}
2. Add `pod 'Fattmerchant'` to your `Podfile`
3. Run `pod install`

#### Note

The `Fattmerchant` pod is both the name of the package and the import. The Stax iOS SDK and the `Fattmerchant` package are one and the same.

***

# Getting Started

## Setup Info.plist

In order to build and run with the Cardpresent functionality, you must include the following in your project's `Info.plist`

* **NSBluetoothAlwaysUsageDescription**: Provide a value here to let your users know why Bluetooth access is required

## IDTech Device Support

If you plan to use IDTech mobile readers (VP3350), you **must** manually add the `IDTech.bundle` to your app's bundle resources. See [IDTECH_SETUP.md](IDTECH_SETUP.md) for detailed instructions.

> ⚠️ **Important**: Failing to add the bundle will result in `IdTechBundleRequired` errors when connecting to IDTech devices.

## CloudCommerce Support

CloudCommerce is a framework that provides transaction processing capabilities for **Tap to Pay on iPhone** functionality. It also includes advanced features like SOAP-based transaction history, receipt management, and extended transaction details. 

> ⚠️ **Important**: CloudCommerce.xcframework is stored in this repository using **Git LFS** (Large File Storage) and is **not distributed** via CocoaPods or Swift Package Manager.

### When to Add CloudCommerce

You **must** add CloudCommerce to your project if you:
- Plan to use **Tap to Pay on iPhone** (iOS 17.4+)

You may also want to add it if you need:
- SOAP transaction history retrieval
- Extended transaction details and statistics
- Advanced receipt management features

### How to Obtain CloudCommerce

**If you have Git LFS enabled**: The framework will be downloaded automatically when you clone the repository.

**If you don't have Git LFS enabled**: You'll see a pointer file instead of the actual framework. To get the framework:

```bash
# Install Git LFS (if not already installed)
brew install git-lfs  # macOS

# Enable Git LFS in this repository
cd /path/to/Fattmerchant-iOS-SDK
git lfs install

# Pull the actual framework files
git lfs pull
```

Alternatively, contact Stax support at **techteam@fattmerchant.com** to obtain the framework directly.

### How to Add CloudCommerce to Your Project

#### Swift Package Manager

1. Ensure you have the framework (see above for Git LFS instructions)
2. Drag `OptionalFrameworks/CloudCommerce.xcframework` into your Xcode project
3. In your target's **General** tab, add `CloudCommerce.xcframework` to **Frameworks, Libraries, and Embedded Content**
4. Set the embed option to **Embed & Sign**

#### CocoaPods

If using CocoaPods, you'll need to manually add the framework:

1. Ensure you have the framework (see above for Git LFS instructions)
2. Copy `OptionalFrameworks/CloudCommerce.xcframework` to your project's frameworks folder
3. Add the following to your Podfile:

```ruby
post_install do |installer|
  installer.pods_project.targets.each do |target|
    target.build_configurations.each do |config|
      config.build_settings['FRAMEWORK_SEARCH_PATHS'] ||= ['$(inherited)']
      config.build_settings['FRAMEWORK_SEARCH_PATHS'] << '$(PROJECT_DIR)/Frameworks'
    end
  end
end
```

4. Ensure the framework is added to your target's **Frameworks, Libraries, and Embedded Content**

> ⚠️ **Important**: CloudCommerce is **required** for Tap to Pay on iPhone functionality. It is optional for mobile reader payments and basic payment processing.

## Initialize

Create an instance of `InitParams`

```swift
var initParams = Omni.InitParams(appId: "com.example.app", apiKey: apiKey, environment: Environment.LIVE)
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

Once you have the list of available readers, you can choose which one you'd like to connect to.

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

# Tap to Pay on iPhone

Use Tap to Pay on iPhone to accept contactless payments directly on supported iPhone devices without any additional hardware.

## Requirements

* iPhone XS or later
* iOS 17.4+
* Stax account with Tap to Pay enabled
* **CloudCommerce.xcframework** - See [CloudCommerce Support](#cloudcommerce-support) section above for installation instructions

## How it works

Tap to Pay uses the same `TransactionRequest` model as mobile reader payments, but uses the iPhone's built-in NFC reader to accept contactless cards and digital wallets.

## Taking a Tap to Pay Transaction

```swift
// Create an Amount
let amount = Amount(cents: 1050) // $10.50

// Create the TransactionRequest
let request = TransactionRequest(amount: amount)

// Take a Tap to Pay transaction
if #available(iOS 17.4, *) {
    omni.takeTapTransaction(with: request) { transaction in
        print("Tap payment successful!")
        print("Transaction ID: \(transaction.id)")
        print("Amount: \(transaction.total)")
        
        // Handle successful payment
        // Update your UI, print receipt, etc.
    } error: { error in
        print("Tap payment failed: \(error)")
        
        // Handle error
        // Show error message to user
    }
} else {
    print("Tap to Pay requires iOS 17.4 or later")
}
```

## Complete Example

```swift
import Fattmerchant

class PaymentViewController: UIViewController {
    var omni: Omni?
    
    @IBAction func processTapPayment(_ sender: UIButton) {
        guard #available(iOS 17.4, *) else {
            showAlert(title: "Not Supported", message: "Tap to Pay requires iOS 17.4 or later")
            return
        }
        
        // Create amount for $25.00
        let amount = Amount(cents: 2500)
        
        // Create transaction request
        let request = TransactionRequest(amount: amount)
        
        // Optional: Add metadata
        request.meta = ["order_id": "12345", "customer_name": "John Doe"]
        
        // Show processing UI
        showProcessingUI()
        
        // Process the payment
        omni?.takeTapTransaction(with: request, completion: { [weak self] transaction in
            guard let self = self else { return }
            
            self.hideProcessingUI()
            
            // Payment succeeded
            self.showSuccessUI(transaction: transaction)
            
            print("✅ Payment successful!")
            print("Transaction ID: \(transaction.id)")
            print("Amount: \(transaction.total)")
            print("Last 4: \(transaction.lastFour ?? "N/A")")
            
        }, error: { [weak self] error in
            guard let self = self else { return }
            
            self.hideProcessingUI()
            
            // Payment failed
            self.showErrorUI(error: error)
            
            print("❌ Payment failed: \(error.localizedDescription)")
        })
    }
    
    private func showProcessingUI() {
        // Show loading indicator and "Hold card near iPhone" message
    }
    
    private func hideProcessingUI() {
        // Hide loading indicator
    }
    
    private func showSuccessUI(transaction: StaxTransaction) {
        // Show success checkmark, amount, etc.
    }
    
    private func showErrorUI(error: OmniException) {
        // Show error message
    }
    
    private func showAlert(title: String, message: String) {
        let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default))
        present(alert, animated: true)
    }
}
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

## API Docs

For more information on how to use the Stax iOS SDK, visit our [API documentation site](https://api-docs.staxpayments.com)
