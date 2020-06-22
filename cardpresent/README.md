# Omni Cardpresent



## <a name="getting-started">Getting Started</a>

In order to take mobile reader payments, you will need the following:

* **Ephemeral Omni API Key**: Using your Omni API key, you will need to create an ephemeral token. The cardpresent codebase does not store your token, so you'll need to pass one in every time you create the `Omni` object. [Here](https://fattmerchant.docs.apiary.io/#reference/0/authentication-tokens/generate-an-ephemeral-token) is a helpful guide showing you how to create an ephemeral token
* **App Name**: A name for your app!
* **Mobile Reader**: A Fattmerchant-provided mobile reader

## Setup Info.plist
In order to build and run with the Cardpresent functionality, you must include the following items in your project's `Info.plist`

* **NSBluetoothAlwaysUsageDescription**: Provide a value here to let your users know why Bluetooth access is required
* **UISupportedExternalAccessoryProtocols**: Include and array of the following items here. These are the MobileReaders that the Omni Cardpresent SDK supports
	* com.bbpost.bt.wisepad
	* com.miura.shuttle
	* com.datecs.pinpad

![Tokenization Info](https://raw.githubusercontent.com/fattmerchantorg/Fattmerchant-iOS-SDK/master/assets/images/cardpresent-info-plist.png)

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

## Pairing the Reader
**Miura M010**

1. Turn the Miura M010 mobile reader on and wait until the display shows "MIURA SYSTEMS" along with a bluetooth indicator.
2. Press and hold the bluetooth indicator button until it flashes rapidly (this lets you know it is in discovery mode)
3. Find the Miura on your iOS device's list of bluetooth devices and pair it

**BBPOS Chipper 2XBT**

The BBPOS must not be paired prior to the connect step. All you have to do here is turn it on. 

> Pairing the BBPOS with the iOS Device will prevent the SDK from being able to connect to it


## Connect a Mobile Reader
> Before connecting to a Miura reader, the reader must be paired to the iOS device within the Settings app


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

## Taking a Payment
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

## Subcribing to Transaction Updates
To receive Transaction updates, register a `TransactionUpdateDelegate` on the Omni object. This object will receive transaction updates such as:

- `PromptSwipeCard` - The mobile reader is waiting for a card to be swiped
- `CardSwiped` - A card was swiped on the mobile reader
- `Authorizing` - The payment is being authorized
- `PromptProvideSignature` - The payment requires a signature

These will be instances of `TransactionUpdate`, and will each have a `value` and a `userFriendlyMessage`. The `value` is a key you can use to identify the event, and the `userFriendlyMessage` is a string that can be shown to an end user, should you choose to.

```swift
// Register to listen to the transaction events
omni.transactionUpdateDelegate = self

func onTransactionUpdate(transactionUpdate: TransactionUpdate) {
	print(transactionUpdate.userFriendlyMessage)
}

// Begin the transaction
omni.takeMobileReaderTransaction(...) 
```

## Providing a Signature
Should a transaction require a signature, one can be provided by registering a `SignatureProviding` on the Omni object. This object will be required to implement a method called 

```swift
/// Called when a transaction requires a signature
/// - Parameter completion: a block to run once the signature is complete. This should be given the signature
func signatureRequired(completion: @escaping (String) -> Void)
```

You can then pass a base64 encoded string representation of the signature and pass it to the completion block.

```swift
omni.signatureProvider = self

override func signatureRequired(completion: @escaping (String) -> Void) {
   var base64EncodedSignature = // ...
   completion(base64EncodedSignature)
}

```


## Refunding a Payment
. You can use the [Omni API](https://fattmerchant.docs.apiary.io/#reference/0/transactions) to do so. 
Once you get the transaction, you can use the `refundMobileReaderTransaction` method to attempt the refund.

> At this time, you may only refund transactions that were performed on the same device that performed the original transaction 


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


