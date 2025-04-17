import Foundation
import Fattmerchant
import SwiftUI

@Observable
class StaxService: TransactionUpdateDelegate, UserNotificationDelegate, MobileReaderConnectionStatusDelegate, UsbAccessoryDelegate {
    static let shared = StaxService()
    private var omni: Omni
    private let apiKey: String = ""
    var isOmniInitialized: Bool = false
    var availableReaders: [MobileReader] = []
    var connectedReader: MobileReader? = nil
    var isConnecting = false
    var logMessages: [String] = []
    private var isInitializing: Bool = false
    var transactions: [Fattmerchant.Transaction] = []
    var lastPreauthTransaction: Fattmerchant.Transaction? = nil
    
   private init() {
       omni = Omni()
       omni.transactionUpdateDelegate = self
       omni.userNotificationDelegate = self
       omni.mobileReaderConnectionUpdateDelegate = self
       omni.usbAccessoryDelegate = self
       
       if (omni.isInitialized == false) {
           initializeOmni()
       }
    }
    public func initializeOmni() {
           log("Attempting initialization...")

           // Check if already initializing to avoid multiple initializations
           guard !isInitializing else { return }
           
           // Mark the process as initializing
           isInitializing = true

           // Perform Omni initialization
           omni.initialize(params: self.initParams(), completion: {
               self.isInitializing = false
               self.log("Omni Initialized")
           }, error: { error in
               self.isInitializing = false
               self.log("Error: \(error.localizedDescription)")
           })
       }

    public func initializeEphemeralOmni() {
        log("Attempting to initialize Omni with Ephemeral Token...")

        // Prevent duplicate initializations
        guard !isInitializing else { return }

        isInitializing = true

        guard let url = URL(string: "https://apiprod.fattlabs.com/ephemeral") else {
            log("Invalid URL for ephemeral token request")
            isInitializing = false
            return
        }

        var request = URLRequest(url: url)
        request.httpMethod = "GET"
        request.setValue("application/json", forHTTPHeaderField: "Accept")
        request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
        request.timeoutInterval = 5

        URLSession.shared.dataTask(with: request) { data, response, error in
            if let error = error {
                self.log("Network error: \(error.localizedDescription)")
                self.isInitializing = false
                return
            }

            guard let data = data else {
                self.log("No data received")
                self.isInitializing = false
                return
            }

            do {
                guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let token = json["token"] as? String else {
                    self.log("Failed to parse ephemeral token from response")
                    self.isInitializing = false
                    return
                }

                self.omni.initialize(params: self.initParams(token: token), completion: {
                    self.log("Omni Initialized with Ephemeral Token")
                    self.isInitializing = false
                }, error: { error in
                    self.log("Omni Initialization Error: \(error.message)")
                    self.isInitializing = false
                })

            } catch {
                self.log("JSON parse error: \(error.localizedDescription)")
                self.isInitializing = false
            }
        }.resume()
    }


    private func initParams(token: String? = nil) -> Omni.InitParams {
           return Omni.InitParams(appId: "fmiossample", apiKey: token ?? apiKey, environment: Environment.LIVE)
       }

    // Search for available readers and connect to the first one
    func searchForReaders() {
        guard omni.isInitialized else {
            log("Omni is not initialized yet.")
            return
        }

        log("Searching for available readers...")

        omni.getAvailableReaders(completion: { readers in
            if readers.isEmpty {
                self.log("No readers found")
            } else {
                self.log("Found readers:")
                for reader in readers {
                    self.log(reader.name)
                }
                
                // Connect to the first reader if available
                if let firstReader = readers.first {
                    self.connectReader(reader: firstReader)
                }
            }
        }, error: { error in
            self.log("Error: \(error.localizedDescription)")
        })
    }
       
    // Connect to a chosen reader
    func connectReader(reader: MobileReader) {
            isConnecting = true
            log("Trying to connect to \(reader.name)")

            omni.connect(reader: reader, completion: { connectedReader in
                self.connectedReader = connectedReader
                self.isConnecting = false
                self.log("Connected reader: \(connectedReader.name)")
            }, error: { error in
                self.log("Couldn't connect to \(reader.name): \(error.localizedDescription)")
                self.isConnecting = false
            })
        }

    func disconnectReader() {
            omni.getConnectedReader(completion: { reader in
                guard let reader = reader else {
                    self.log("No connected reader found")
                    return
                }
                
                self.omni.disconnect(reader: reader, completion: { success in
                    self.log(success ? "Successfully disconnected reader" : "Could not disconnect reader")
                }, error: { error in
                    self.log(error.detail ?? "Error disconnecting")
                })
            }, error: { error in
                self.log(error.detail ?? "Error getting connected reader")
            })
        }

    // Function to get information about the connected reader
    func getReaderInfo() {
        log("Trying to get info about the connected reader")
        omni.getConnectedReader(completion: { reader in
            self.log("Done")
        }, error: { error in
            self.log(error.detail ?? "Huh")
        })
    }
    
    // Fetch transactions from Omni SDK
    func fetchTransactions(completion: @escaping ([Fattmerchant.Transaction]) -> Void, error: @escaping (Error) -> Void) {
        omni.getMobileReaderTransactions(completion: { transactions in
            // Return the transactions to the caller
            completion(transactions)
        }, error: { err in
            error(err)
        })
    }

    // Number Formatter to handle parsing the amount input
       fileprivate func numberFormatter() -> NumberFormatter {
           let formatter = NumberFormatter()
           formatter.numberStyle = .currency
           formatter.currencySymbol = "$"
           return formatter
       }

    // Get the amount from a text field
        fileprivate func getAmount(from textInput: String?) -> Amount {
            guard
                let text = textInput,  // Use the text from the text input (passed in as a parameter)
                let number = numberFormatter().number(from: text)
            else {
                return Amount(cents: 0)
            }

            return Amount(dollars: number.doubleValue)
        }
        
    // Refund the selected transaction with an amount
    func refund(transaction: Fattmerchant.Transaction, totalTextInput: String?, completion: @escaping () -> Void, error: @escaping (String) -> Void) {
        let amount = getAmount(from: totalTextInput)  // Get the amount from the text input

        omni.refundMobileReaderTransaction(transaction: transaction, refundAmount: amount, completion: { _ in
            // Refund successful
            completion()
        }, error: { err in
            let errorMessage = err.detail ?? err.message
            error(errorMessage)
        })
    }
    

    fileprivate func createTransactionRequest(amountText: String) -> TransactionRequest {
           let amount = getAmount(from: amountText)
           let request = TransactionRequest(amount: amount)
           return request
    }

      
    func takePayment(amountText: String, preauth: Bool = false) {
            var req = createTransactionRequest(amountText: amountText)
            req.preauth = preauth

            omni.takeMobileReaderTransaction(request: req, completion: { transaction in
                self.log("Finished transaction successfully")
                if preauth {
                    self.lastPreauthTransaction = transaction
                }
            }, error: { error in
                self.log("Transaction error: \(error.localizedDescription)")
            })
        }
    
    func captureLastTransaction() {
           guard let transaction = lastPreauthTransaction else {
               return log("No preauth transactions to capture")
           }

           guard let id = transaction.id else {
               return log("Transaction ID is missing")
           }

           let amount = transaction.total.map { Amount(dollars: $0) }

           omni.capturePreauthTransaction(
               transactionId: id,
               amount: amount,
               completion: { transaction in
                   self.log("Captured transaction successfully")
               },
               error: { error in
                   self.log("Capture error: \(error.detail ?? "Unknown error")")
               }
           )
       }

    func voidLastTransaction() {
        guard let lastTransaction = lastPreauthTransaction else {
            log("No preauth transactions to void")
            return
        }
        
        guard let id = lastTransaction.id else {
            log("Transaction ID is missing")
            return
        }
        
        omni.voidTransaction(
            transactionId: id,
            completion: { _ in
                self.log("Voided transaction successfully")
            },
            error: { error in
                self.log("Error voiding transaction: \(error.localizedDescription)")
            }
        )
    }
    
    func cancelTransaction() {
        omni.cancelMobileReaderTransaction(
            completion: { _ in
                self.log("Transaction cancelled")
            },
            error: { error in
                self.log("Error cancelling transaction: \(error.localizedDescription)")
            }
        )
    }
    
    func tokenizeCard() {
        log("Attempting CNP Tokenization")
        
        var card = CreditCard.testCreditCard()
        card.personName = "Test Creditcard"
        
        omni.tokenize(card, { paymentMethod in
            self.log("Created PaymentMethod Successfully")
            self.log("""
            PaymentMethod:
            id: \(paymentMethod.id ?? "nil")
            customerId: \(paymentMethod.customerId)
            method: \(paymentMethod.method?.rawValue ?? "nil")
            """)
        }, error: { err in
            self.log("Error tokenizing card: \(err.localizedDescription)")
        })
    }

    func tokenizeBankAccount() {
        log("Attempting CNP Tokenization")

        let bank = BankAccount.testBankAccount()

        omni.tokenize(bank, { paymentMethod in
            self.log("Created PaymentMethod Successfully")
            self.log("""
            PaymentMethod:
            id: \(paymentMethod.id ?? "nil")
            customerId: \(paymentMethod.customerId)
            method: \(paymentMethod.method?.rawValue ?? "nil")
            """)
        }, error: { err in
            self.log("Error tokenizing bank account: \(err.localizedDescription)")
        })
    }


    func payWithCard(amountText: String) {
        log("Attempting CNP Transaction")

        let card = CreditCard(
            personName: "Test Payment",
            cardNumber: "4111111111111111",
            cardExp: "0228",
            addressZip: "32812"
        )

        let transactionRequest = TransactionRequest(
            amount: getAmount(from: amountText),
            card: card
        )

        omni.pay(transactionRequest: transactionRequest, completion: { completedTransaction in
            self.log("Finished transaction successfully")
        }, error: { error in
            self.log("Error during payment: \(error.localizedDescription)")
        })
    }
    
    func payWithBankAccount() {
        log("This feature isn't available yet")
    }
    
    // Function to log messages (will be displayed in the UI)
    func log(_ message: String) {
        LogMessagesManager.shared.log(message)
        logMessages.append(message)  // Add the message to the UI log
    }

    // MobileReaderConnectionStatusDelegate method
    func mobileReaderConnectionStatusUpdate(status: MobileReaderConnectionStatus) {
        log("Mobile reader \(status.rawValue)")
    }

    // Delegate methods for other updates
    func onTransactionUpdate(transactionUpdate: TransactionUpdate) {
      log("Transaction Update: \(transactionUpdate)")
    }

    func onUserNotification(userNotification: UserNotification) {
        log("User Notification: \(userNotification)")
    }

    func onUsbAccessoryConnected() {
        log("USB Accessory Connected")
    }

    func onUsbAccessoryDisconnected() {
       log("USB Accessory Disconnected")
    }
}

