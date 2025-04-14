import Foundation
import Fattmerchant
import SwiftUI

@Observable
class OmniInitializer: TransactionUpdateDelegate, UserNotificationDelegate, MobileReaderConnectionStatusDelegate, UsbAccessoryDelegate {
    static let shared = OmniInitializer()
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

    // Now we define initializeOmni as a method that launches a Task
      public func initializeOmni() {
    
          LogMessagesManager.shared.log("Attempting initialization...")

          // Check if already initializing to avoid multiple initializations
          guard !isInitializing else { return }
          
          // Mark the process as initializing
          isInitializing = true
          
          // Launch a new Task to run the async initialization
          Task {
              do {
                  // Await Omni initialization inside the Task
                  try await withCheckedThrowingContinuation { continuation in
                      omni.initialize(params: self.initParams(), completion: {
                          self.isInitializing = false
                          LogMessagesManager.shared.log("Omni Initialized")
                          continuation.resume()  // Successfully finished, resume continuation
                      }, error: { error in
                          self.isInitializing = false
                          LogMessagesManager.shared.log("Error: \(error.localizedDescription)")
                          continuation.resume(throwing: error)  // Handle error and resume continuation
                      })
                  }
              } catch {
                  // Handle error during initialization if needed
                  LogMessagesManager.shared.log("Initialization failed: \(error.localizedDescription)")
              }
          }
      }

    public func initializeEphemeralOmni() throws {
        LogMessagesManager.shared.log("Attempting to initialize Omni with Ephemeral Token...")

        // Ensure we are not already initializing
        guard !isInitializing else { return }
        
        isInitializing = true

        // Launch a Task to handle the async operation
        Task {
            do {
                guard let url = URL(string: "https://apiprod.fattlabs.com/ephemeral") else {
                    throw URLError(.badURL)  // Handle invalid URL
                }
                
                var request = URLRequest(url: url)
                request.httpMethod = "GET"
                request.setValue("application/json", forHTTPHeaderField: "Accept")
                request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
                request.timeoutInterval = 5
                
                // Perform the async URLSession call
                let (data, _) = try await URLSession.shared.data(for: request)
                
                // Parse the response
                guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let token = json["token"] as? String else {
                    throw URLError(.cannotParseResponse)
                }
                
                // Initialize Omni with the token
                try await withCheckedThrowingContinuation { continuation in
                    self.omni.initialize(params: self.initParams(token: token), completion: {
                        LogMessagesManager.shared.log("Omni Initialized with Ephemeral Token")
                        self.isInitializing = false
                        continuation.resume()  // Complete the async flow successfully
                    }, error: { error in
                        LogMessagesManager.shared.log("Error: \(error.message)")
                        self.isInitializing = false
                        continuation.resume(throwing: error)  // Handle error and resume
                    })
                }
            } catch {
                LogMessagesManager.shared.log("Initialization failed: \(error.localizedDescription)")
                self.isInitializing = false  // Mark as finished even if failed
            }
        }
    }


    private func initParams(token: String? = nil) -> Omni.InitParams {
           return Omni.InitParams(appId: "fmiossample", apiKey: token ?? apiKey, environment: Environment.LIVE)
       }

    // Function to search for available readers and connect to the first one
       func searchForReaders() {
           guard omni.isInitialized else {
               LogMessagesManager.shared.log("Omni is not initialized yet.")
               return
           }

           LogMessagesManager.shared.log("Searching for available readers...")

           // Wrapping the completion handler in a Task
           Task {
               omni.getAvailableReaders(completion: { readers in
                   if readers.isEmpty {
                       LogMessagesManager.shared.log("No readers found")
                   } else {
                       LogMessagesManager.shared.log("Found readers:")
                       for reader in readers {
                           LogMessagesManager.shared.log(reader.name) // Log each reader's name
                       }
                       
                       // Connect to the first reader if available
                       if let firstReader = readers.first {
                           self.connectReader(reader: firstReader)
                       }
                   }
               }, error: { error in
                   LogMessagesManager.shared.log("Error: \(error.localizedDescription)")
               })
           }
       }
       
       // Function to connect to a chosen reader
       func connectReader(reader: MobileReader) {
           isConnecting = true
           LogMessagesManager.shared.log("Trying to connect to \(reader.name)")

           // Wrap the completion-based connect method in a task
           Task {
               omni.connect(reader: reader, completion: { connectedReader in
                   self.connectedReader = connectedReader
                   self.isConnecting = false
                   LogMessagesManager.shared.log("Connected reader: \(connectedReader.name)")
               }, error: { error in
                   LogMessagesManager.shared.log("Couldn't connect to \(reader.name): \(error.localizedDescription)")
                   self.isConnecting = false
               })
           }
       }

    func disconnectReader() {
        Task {
            // Attempt to get the connected reader asynchronously using completion handlers
            omni.getConnectedReader(completion: { reader in
                guard let reader = reader else {
                    LogMessagesManager.shared.log("No connected reader found")
                    return
                }
                
                // Attempt to disconnect asynchronously using completion handlers
                self.omni.disconnect(reader: reader, completion: { success in
                    LogMessagesManager.shared.log(success ? "Successfully disconnected reader" : "Could not disconnect reader")
                }, error: { error in
                    LogMessagesManager.shared.log(error.detail ?? "Error disconnecting")
                })
            }, error: { error in
                LogMessagesManager.shared.log(error.detail ?? "Error getting connected reader")
            })
        }
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
            // Return the error to the caller
            error(err)
        })
    }

    // Number Formatter to handle parsing the amount input
       fileprivate func numberFormatter() -> NumberFormatter {
           let formatter = NumberFormatter()
           formatter.numberStyle = .currency
           formatter.currencySymbol = "$"  // Adjust according to your locale, e.g., if you use other currencies
           return formatter
       }

    // Function to get the amount from a text field (or other sources)
        fileprivate func getAmount(from textInput: String?) -> Amount {
            guard
                let text = textInput,  // Use the text from the text input (passed in as a parameter)
                let number = numberFormatter().number(from: text)
            else {
                return Amount(cents: 0)  // Default to 1 cent if invalid or no value is provided
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
            // Access the error message from the error
            let errorMessage = err.detail ?? err.message
            // Pass the error message to the SwiftUI error closure
            error(errorMessage)
        })
    }
    

       fileprivate func createTransactionRequest(amountText: String) -> TransactionRequest {
           let amount = getAmount(from: amountText)
           let request = TransactionRequest(amount: amount)
           return request
       }

      
    func takePayment(amountText: String, preauth: Bool = false) {
        Task {
            var req = createTransactionRequest(amountText: amountText)
            req.preauth = preauth

            do {
                let transaction = try await withCheckedThrowingContinuation { continuation in
                    omni.takeMobileReaderTransaction(
                        request: req,
                        completion: { transaction in
                            continuation.resume(returning: transaction)
                        },
                        error: { error in
                            continuation.resume(throwing: error)
                        }
                    )
                }

                log("Finished transaction successfully")
                if preauth {
                    lastPreauthTransaction = transaction
                }
            } catch {
                log("Transaction error: \(error.localizedDescription)")
            }
        }
    }
    
    func captureLastTransaction() {
        Task {
            guard let transaction = lastPreauthTransaction else {
                return log("No preauth transactions to capture")
            }

            guard let id = transaction.id else {
                return log("Transaction ID is missing")
            }

            let amount = transaction.total.map { Amount(dollars: $0) }

            do {
                _ = try await withCheckedThrowingContinuation { continuation in
                    omni.capturePreauthTransaction(
                        transactionId: id,
                        amount: amount,
                        completion: { transaction in
                            continuation.resume(returning: transaction)
                        },
                        error: { error in
                            continuation.resume(throwing: error)
                        }
                    )
                }

                log("Captured transaction successfully")
            } catch {
                log("Capture error: \(error)")
            }
        }
    }

   
    func voidLastTransaction() {
        Task {
            guard let lastTransaction = lastPreauthTransaction else {
                log("No preauth transactions to void")
                return
            }
            
            guard let id = lastTransaction.id else {
                log("Transaction ID is missing")
                return
            }
            
            do {
                try await withCheckedThrowingContinuation { continuation in
                    omni.voidTransaction(
                        transactionId: id,
                        completion: { _ in
                            continuation.resume()
                        },
                        error: { error in
                            continuation.resume(throwing: error)
                        }
                    )
                }
                log("Voided transaction successfully")
            } catch {
                log("Error voiding transaction: \(error.localizedDescription)")
            }
        }
    }
    
    func cancelTransaction() {
        Task {
            do {
                try await withCheckedThrowingContinuation { continuation in
                    omni.cancelMobileReaderTransaction(
                        completion: { _ in
                            continuation.resume()
                        },
                        error: { error in
                            continuation.resume(throwing: error)
                        }
                    )
                }
                log("Transaction cancelled")
            } catch {
                log("Error cancelling transaction: \(error.localizedDescription)")
            }
        }
    }
    
    func tokenizeCard() async throws {
        log("Attempting CNP Tokenization")
        
        var card = CreditCard.testCreditCard()
        card.personName = "Test Creditcard"

        let paymentMethod = try await withCheckedThrowingContinuation { continuation in
            omni.tokenize(card, { paymentMethod in
                continuation.resume(returning: paymentMethod)
            }, error: { err in
                continuation.resume(throwing: err)
            })
        }

        log("Created PaymentMethod Successfully")
        log("PaymentMethod:\nid: \(paymentMethod.id ?? "nil")\ncustomerId: \(paymentMethod.customerId)\nmethod: \(paymentMethod.method?.rawValue ?? "nil")")
    }
    
    func tokenizeBankAccount() async throws {
        log("Attempting CNP Tokenization")

        let bank = BankAccount.testBankAccount()

        let paymentMethod = try await withCheckedThrowingContinuation { continuation in
            omni.tokenize(bank, { paymentMethod in
                continuation.resume(returning: paymentMethod)
            }, error: { err in
                continuation.resume(throwing: err)
            })
        }

        log("Created PaymentMethod Successfully")
        log("PaymentMethod:\nid: \(paymentMethod.id ?? "nil")\ncustomerId: \(paymentMethod.customerId)\nmethod: \(paymentMethod.method?.rawValue ?? "nil")")
    }
    
    func payWithCard(amountText: String) async throws {
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
        
        _ = try await withCheckedThrowingContinuation { continuation in
            omni.pay(transactionRequest: transactionRequest, completion: { completedTransaction in
                continuation.resume(returning: completedTransaction)
            }, error: { error in
                continuation.resume(throwing: error)
            })
        }
        
        log("Finished transaction successfully")
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
        LogMessagesManager.shared.log("Transaction Update: \(transactionUpdate)")
    }

    func onUserNotification(userNotification: UserNotification) {
        LogMessagesManager.shared.log("User Notification: \(userNotification)")
    }

    func onUsbAccessoryConnected() {
        LogMessagesManager.shared.log("USB Accessory Connected")
    }

    func onUsbAccessoryDisconnected() {
        LogMessagesManager.shared.log("USB Accessory Disconnected")
    }
}

