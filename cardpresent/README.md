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

![Tokenization Info](https://raw.githubusercontent.com/fattmerchantorg/Fattmerchant-iOS-SDK/master/assets/images/cardpresent-info-plist.png.png)

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


